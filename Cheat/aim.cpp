#include "aim.h"
#include "../Core/InputSys.h"
#include "../Core/options.h"
#include "../Core/SDK.h"
#include "../Core/math.h"
#include "../Core/Utilities.h"
#include "Cheats.h"
static int custom_delay = 0;
#define TICK_INTERVAL			(g_GlobalVars->interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
legitbot::legitbot()
{
	best_target = -1;

	view_angle = QAngle(0.0f, 0.0f, 0.0f);
	aim_angle = QAngle(0.0f, 0.0f, 0.0f);
	delta_angle = QAngle(0.0f, 0.0f, 0.0f);
	final_angle = QAngle(0.0f, 0.0f, 0.0f);

	hitbox_position = Vector(0.0f, 0.0f, 0.0f);

	aim_key = 0;
	aim_smooth = 1;
	aim_fov = 0;
	randomized_smooth = 0;
	recoil_min = 0;
	recoil_max = 0;
	randomized_angle = 0;
	shoot = false;
}

void legitbot::on_create_move(CUserCmd *cmd, C_BasePlayer* local, C_BaseCombatWeapon* weapon)
{
	do_aimbot(local, weapon, cmd);
}

void legitbot::do_aimbot(C_BasePlayer *local, C_BaseCombatWeapon *weapon, CUserCmd *cmd)
{
	if (!local)
		return;

	if (!weapon)
		return;

	if (!g_Options.enablemain && weapon->IsRifle() | !g_Options.enablepistol && weapon->IsPistol())
		return;

	if (!local->IsAlive())
		return;

	if (!weapon->HasBullets())
		return;

	weapon_settings(weapon);

	if (!PressedKeys[aim_key])
		return;

	best_target = get_target(local, weapon, cmd, hitbox_position);

	if (best_target == -1)
		return;

	C_BasePlayer* entity = (C_BasePlayer*)g_EntityList->GetClientEntity(best_target);
	if (!entity)
		return;

	if (math::get_distance(local->GetEyePos(), hitbox_position) > 8192.0f)
		return;

	math::compute_angle(local->GetEyePos(), hitbox_position, aim_angle);
	math::normalize_angles(aim_angle);

	aim_angle -= get_randomized_recoil(local);
	aim_angle += get_randomized_angles(local);

	math::normalize_angles(view_angle);

	delta_angle = view_angle - aim_angle;
	math::normalize_angles(delta_angle);

	float randomSmoothing = 1.0f;

	if (randomized_smooth > 1.0f)
		randomSmoothing = Utilities::random_number_range(randomized_smooth / 10.0f, 1.0f);

	final_angle = view_angle - delta_angle / aim_smooth * randomSmoothing;
	math::normalize_angles(final_angle);

	if (!math::sanitize_angles(final_angle))
		return;

	cmd->viewangles = final_angle;
	g_EngineClient->SetViewAngles(cmd->viewangles);
}

void legitbot::weapon_settings(C_BaseCombatWeapon* weapon)
{
	if (!weapon)
		return;


	if (weapon->IsPistol())
	{
		aim_key = g_Options.PistolKey;
		aim_smooth = g_Options.PistolSmooth;
		aim_fov = g_Options.Pistolfov;
		randomized_smooth = 1;
		recoil_min = g_Options.pistol_recoil_min;
		recoil_max = g_Options.pistol_recoil_max;
		randomized_angle = 1;
	}
	else if (weapon->IsRifle())
	{
		aim_key = g_Options.MainKey;
		aim_smooth = g_Options.MainSmooth;
		aim_fov = g_Options.Mainfov;
		randomized_smooth = 1;
		recoil_min = g_Options.main_recoil_min;
		recoil_max = g_Options.main_recoil_max;
		randomized_angle = 1;
	}
}

QAngle legitbot::get_randomized_recoil(C_BasePlayer *local)
{
	QAngle compensatedAngles = (local->m_aimPunchAngle() * 2.0f) * (Utilities::random_number_range(recoil_min, recoil_max) / 100.0f);
	math::normalize_angles(compensatedAngles);

	return (local->m_iShotsFired() > 1 ? compensatedAngles : QAngle(0.0f, 0.0f, 0.0f));
}

QAngle legitbot::get_randomized_angles(C_BasePlayer *local)
{
	QAngle randomizedValue = QAngle(0.0f, 0.0f, 0.0f);

	float randomRate = Utilities::random_number_range(-randomized_angle, randomized_angle);
	float randomDeviation = Utilities::random_number_range(-randomized_angle, randomized_angle);

	switch (rand() % 2)
	{
	case 0:
		randomizedValue.pitch = (randomRate * cos(randomDeviation));
		randomizedValue.yaw = (randomRate * cos(randomDeviation));
		randomizedValue.roll = (randomRate * cos(randomDeviation));
		break;
	case 1:
		randomizedValue.pitch = (randomRate * sin(randomDeviation));
		randomizedValue.yaw = (randomRate * sin(randomDeviation));
		randomizedValue.roll = (randomRate * sin(randomDeviation));
		break;
	}

	math::normalize_angles(randomizedValue);

	return (local->m_iShotsFired() > 1 ? randomizedValue : QAngle(0.0f, 0.0f, 0.0f));
}

bool legitbot::get_hitbox(C_BasePlayer *local, C_BasePlayer *entity, Vector &destination)
{
	int bestHitbox = -1;
	float best_fov = aim_fov;

	int hitboxspots;

	switch (g_Options.hitbox)
	{
	case 0:
		break;
	case 1:
		hitboxspots = HITBOX_HEAD;
		break;
	case 2:
		hitboxspots = HITBOX_NECK;
		break;
	case 3:
		hitboxspots = HITBOX_CHEST;
		break;
	case 4:
		hitboxspots = HITBOX_STOMACH;
		break;
	case 5:
		hitboxspots = HITBOX_NECK, HITBOX_HEAD, HITBOX_CHEST, HITBOX_STOMACH;
		break;
	}

	std::vector<int> hitboxes = { hitboxspots };
	for (auto hitbox : hitboxes)
	{
		Vector temp;
		if (!entity->GetHitboxPos(hitbox, temp))
			continue;

		float fov = math::get_fov(view_angle, math::compute_angle(local->GetEyePos(), temp));
		if (fov < best_fov)
		{
			best_fov = fov;
			bestHitbox = hitbox;
		}
	}

	if (bestHitbox != -1)
	{
		if (!entity->GetHitboxPos(bestHitbox, destination))
			return true;
	}

	return false;
}

int legitbot::get_target(C_BasePlayer *local, C_BaseCombatWeapon *weapon, CUserCmd *cmd, Vector &destination)
{
	int best_target = -1;
	float best_fov = FLT_MAX;

	g_EngineClient->GetViewAngles(view_angle);

	for (int i = 1; i <= g_GlobalVars->maxClients; i++)
	{
		C_BasePlayer *entity = (C_BasePlayer*)g_EntityList->GetClientEntity(i);
		if (!entity
			|| entity == local
			|| entity->IsDormant()
			|| entity->m_lifeState() != LIFE_ALIVE
			|| entity->m_bGunGameImmunity()
			|| entity->GetClientClass()->m_ClassInfo != CCSPlayer
			|| !(entity->m_fFlags() & FL_ONGROUND))
			continue;

		if (true)
		{
			if (entity->m_iTeamNum() == local->m_iTeamNum())
				continue;
		}

		Vector hitbox;
		if (get_hitbox(local, entity, hitbox))
			continue;

		float fov = math::get_fov(view_angle + (local->m_aimPunchAngle() * 2.0f), math::compute_angle(local->GetEyePos(), hitbox));
		if (fov < best_fov && fov < aim_fov)
		{
			if (local->CanSeePlayer(entity, hitbox))
			{
				best_fov = fov;
				destination = hitbox;
				best_target = i;
			}
		}
	}

	return best_target;
}