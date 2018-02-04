#include "Visuals.h"
#include "../Core/Math.h"
#include "../Core/Draw.h"
#include "../Core/Font.h"
#include "../Core/options.h"

#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include <iterator>
#include <vector>

void Visuals::PaintTraverse()
{
	Overlay();

	if (g_LocalPlayer && g_EngineClient->IsInGame())
	{
		SniperCrosshair();
		Radar();
		NightMode();

		if (g_Options.VIS_ESP)
		{
			for (int i = 1; i <= g_GlobalVars->maxClients; i++)
			{
				auto pEntity = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(i));
				if (!pEntity) continue;
				if (pEntity == g_LocalPlayer) continue;
				if (pEntity->m_iTeamNum() == g_LocalPlayer->m_iTeamNum()) continue;
				if (pEntity->IsDormant() || !pEntity->IsAlive()) continue;

				Vector max = pEntity->GetCollideable()->OBBMaxs();
				Vector top3D, pos3D;
				Vector2D top, pos;
				pos3D = pEntity->m_vecOrigin();
				top3D = pEntity->m_fFlags() & FL_DUCKING ? pos3D + Vector(0, 0, 56.f) : pos3D + Vector(0, 0, 72.f);

				if (!Math::WorldToScreen(pos3D, pos) || !Math::WorldToScreen(top3D, top))
					continue;

				float height = (pos.y - top.y);
				float width = height / 4.f;

				Color clr = Color(g_Options.BOX[0], g_Options.BOX[1], g_Options.BOX[2]);
				Color clr1 = Color(g_Options.BOXFill[0], g_Options.BOXFill[1], g_Options.BOXFill[2], g_Options.fillalpha[0]);
				Color clr2 = Color(g_Options.skelclr[0], g_Options.skelclr[1], g_Options.skelclr[2]);
				Color clr3 = Color(g_Options.snapclr[0], g_Options.snapclr[1], g_Options.snapclr[2]);


				if (g_Options.VIS_ESP_DeadCheck)
				{
					/*if dead check*/

					if (!g_LocalPlayer->IsAlive())
					{
						if (g_Options.VIS_ESP_Box) ESP_Box(top.x, top.y, width, height, pEntity, clr, clr1);				
						if (g_Options.VIS_ESP_Name) ESP_Name(top.x, top.y, width, height, pEntity);
						if (g_Options.VIS_ESP_Health) DrawHealth(pos, top, pEntity->m_iHealth());
						if (g_Options.VIS_ESP_Weapon) ESP_Weapon(top.x, top.y, width, height, pEntity);

						if (g_Options.VIS_ESP_Bone) ESP_Bone(pEntity, clr2);
						if (g_Options.VIS_ESP_Snaplines) ESP_Snapline(pEntity, clr3);
					}				
				}

				if (!g_Options.VIS_ESP_DeadCheck)
				{
					/*typical drawing, no need to check anything*/
					if (g_Options.VIS_ESP_Box) ESP_Box(top.x, top.y, width, height, pEntity, clr, clr1);
					if (g_Options.VIS_ESP_Name) ESP_Name(top.x, top.y, width, height, pEntity);
					if (g_Options.VIS_ESP_Health) DrawHealth(pos, top, pEntity->m_iHealth());
					if (g_Options.VIS_ESP_Weapon) ESP_Weapon(top.x, top.y, width, height, pEntity);

					if (g_Options.VIS_ESP_Bone) ESP_Bone(pEntity, clr2);
					if (g_Options.VIS_ESP_Snaplines) ESP_Snapline(pEntity, clr3);
				}

			}
		}
	}
}
#include "../Core/Hooks.h"

void Visuals::DrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &info, matrix3x4_t* matrix)
{
	const auto model = info.pModel;

	auto texturedColor = g_MatSystem->FindMaterial("simple_regular", TEXTURE_GROUP_MODEL); //textured color chams
	auto flatColor = g_MatSystem->FindMaterial("simple_flat", TEXTURE_GROUP_MODEL); //flat color chams
	if (strstr(model->szName, "models/player") != nullptr)
	{
		auto pEntity = C_BasePlayer::GetPlayerByIndex(info.entity_index);

		if (pEntity
			&& !pEntity->IsDormant()
			&& pEntity->IsAlive()
			&& pEntity->m_iTeamNum() != g_LocalPlayer->m_iTeamNum())
		{

			if (g_Options.VIS_ChamsToggle)
			{
				switch (g_Options.MISC_ChamsType)
				{
				case 0: {
					texturedColor->ColorModulate(g_Options.CHAMS[0], g_Options.CHAMS[1], g_Options.CHAMS[2]);
					texturedColor->AlphaModulate(1.f);
					g_ModelRender->ForcedMaterialOverride(texturedColor);
				}
				break;
				case 1: {
					flatColor->ColorModulate(g_Options.CHAMS[0], g_Options.CHAMS[1], g_Options.CHAMS[2]);
					flatColor->AlphaModulate(1.f);
					g_ModelRender->ForcedMaterialOverride(flatColor);
				}
				break;
				case 2: {
					float hp = pEntity->m_iHealth();
					texturedColor->ColorModulate(1.f - float(hp / 100), float(hp / 100), 0.f);
					texturedColor->AlphaModulate(1.f);
					g_ModelRender->ForcedMaterialOverride(texturedColor);
				}
				break;
				}
			}

		}
	} 
	else 
	{

	}	
}

void Visuals::Overlay()
{

}

void Visuals::SniperCrosshair()
{
	if (!g_Options.VIS_SniperCH)
		return;

	auto pWeapon = g_LocalPlayer->m_hActiveWeapon().Get();
	if (!pWeapon) return;
	if (!pWeapon->IsSniper()) return;

	int ScreenWidth, ScreenHeight;
	g_EngineClient->GetScreenSize(ScreenWidth, ScreenHeight);

	auto Center = Vector2D(ScreenWidth / 2, ScreenHeight / 2);

	Draw::FilledCircle(Center, 10, 1.5f, Color::White);
	Draw::Circle(Center, 10, 1.5f, Color::Black);
}

void Visuals::ESP_Box(int x, int y, int w, int h, C_BasePlayer* pEntity, Color clr, Color clr1)
{
	
	g_VGuiSurface->DrawSetColor(clr1);
	g_VGuiSurface->DrawFilledRectFade(x - w, y, x + w, y + h, 20, 100, false);
	g_VGuiSurface->DrawSetColor(clr);
	g_VGuiSurface->DrawOutlinedRect(int(x - w), int(y), int(x + w), int(y + h));
	g_VGuiSurface->DrawSetColor(Color::Black);
	g_VGuiSurface->DrawOutlinedRect(int(x - w - 1), int(y - 1), int(x + w + 1), int(y + h + 1));
	g_VGuiSurface->DrawOutlinedRect(int(x - w + 1), int(y + 1), int(x + w - 1), int(y + h - 1));
}

void Visuals::ESP_Name(int x, int y, int w, int h, C_BasePlayer* pEntity)
{
	wchar_t buf[80];

	player_info_t info = pEntity->GetPlayerInfo();

	if (MultiByteToWideChar(CP_UTF8, 0, info.szName, -1, buf, 80) > 0)
		Draw::Text(x, y - 7, buf, Font::Get().ESP, Color::White, true);
}

void Visuals::ESP_Weapon(int x, int y, int w, int h, C_BasePlayer* pEntity)
{
	wchar_t buf[80];

	auto weapon = pEntity->m_hActiveWeapon().Get();

	if (!weapon) return;

	if (MultiByteToWideChar(CP_UTF8, 0, weapon->GetCSWeaponData()->szWeaponName + 7, -1, buf, 80) > 0)
		Draw::Text(x + w + 2, y + -3, buf, Font::Get().ESP, Color::White);	
}

void Visuals::DrawHealth(C_BasePlayer* entity, Visuals::ESPBox size)
{
	int health = entity->m_iHealth();
	int HP = health;
	if (HP > 100)
		HP = 100;
	int hp = health;
	float r = float(255 - health * 2.55);
	float g = float(health * 2.55);
	hp = (size.h - ((size.h * hp) / 100));

	Draw::Outline(int(size.x - 4), int(size.y + hp), (int)2, int(size.h - hp + 1), Color((int)r, (int)g, 0));
	Draw::Outline(int(size.x - 5), int(size.y - 1), (int)3, int(size.h + 2), Color(0, 0, 0, 150));
}

void Visuals::DrawHealth(Vector2D bot, Vector2D top, float health)
{
	float h = (bot.y - top.y);
	float offset = (h / 4.f) + 5;
	float w = h / 64.f;

	UINT hp = UINT(h - (UINT)((h * health) / 100)); // Percentage

	int Red = int(255 - (health*2.55));
	int Green = int(health*2.55);

	Draw::DrawOutlinedRect(int((top.x - offset) - 1), int(top.y - 1), 3, int(h + 2), Color::Black);

	Draw::Line(int((top.x - offset)), int(top.y + hp), int((top.x - offset)), int(top.y + h), Color(Red, Green, 0, 255));
}

void Visuals::ESP_Bone(C_BasePlayer* pEntity, Color clr)
{
	studiohdr_t* hdr = g_ModelInfo->GetStudioModel(pEntity->GetModel());

	if (hdr)
	{
		static matrix3x4_t matrix[128];
		if (pEntity->SetupBones(matrix, 128, 0x100, pEntity->m_flSimulationTime()))
		{
			for (int j = 0; j < hdr->numbones; j++)
			{
				mstudiobone_t* bone = hdr->GetBone(j);

				if (bone && bone->flags & 0x100 && bone->parent != -1)
				{
					auto child_bone = Vector(matrix[j][0][3], matrix[j][1][3], matrix[j][2][3]);
					auto parent_bone = Vector(matrix[bone->parent][0][3], matrix[bone->parent][1][3], matrix[bone->parent][2][3]);

					Vector2D child_screen, parent_screen;

					if (Math::WorldToScreen(child_bone, child_screen) && Math::WorldToScreen(parent_bone, parent_screen))
						Draw::Line(child_screen, parent_screen, clr);
				}
			}
		}
	}
}

bool done = false;

void Visuals::NightMode()
{
	if (g_Options.night)
	{
		if (!done)
		{


			static auto sv_skyname = g_CVar->FindVar("sv_skyname");
			static auto r_DrawSpecificStaticProp = g_CVar->FindVar("r_DrawSpecificStaticProp");
			r_DrawSpecificStaticProp->SetValue(1);
			sv_skyname->SetValue("sky_csgo_night02");

			for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i))
			{
				IMaterial *pMaterial = g_MatSystem->GetMaterial(i);

				if (!pMaterial)
					continue;

				const char* group = pMaterial->GetTextureGroupName();
				const char* name = pMaterial->GetName();

				if (strstr(group, "World textures"))
				{
					pMaterial->ColorModulate(0.10, 0.10, 0.10);
				}
				if (strstr(group, "StaticProp"))
				{
					pMaterial->ColorModulate(0.30, 0.30, 0.30);
				}
				if (strstr(name, "models/props/de_dust/palace_bigdome"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
				}
				if (strstr(name, "models/props/de_dust/palace_pillars"))
				{
					pMaterial->ColorModulate(0.30, 0.30, 0.30);
				}

				if (strstr(group, "Particle textures"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
				}
				done = true;
			}

		}
	}
	else
	{
		if (done)
		{
			for (MaterialHandle_t i = g_MatSystem->FirstMaterial(); i != g_MatSystem->InvalidMaterial(); i = g_MatSystem->NextMaterial(i))
			{
				IMaterial *pMaterial = g_MatSystem->GetMaterial(i);

				if (!pMaterial)
					continue;

				const char* group = pMaterial->GetTextureGroupName();
				const char* name = pMaterial->GetName();

				if (strstr(group, "World textures"))
				{

					pMaterial->ColorModulate(1, 1, 1);
				}
				if (strstr(group, "StaticProp"))
				{

					pMaterial->ColorModulate(1, 1, 1);
				}
				if (strstr(name, "models/props/de_dust/palace_bigdome"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
				}
				if (strstr(name, "models/props/de_dust/palace_pillars"))
				{

					pMaterial->ColorModulate(1, 1, 1);
				}
				if (strstr(group, "Particle textures"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
				}
			}
			done = false;
		}
	}
}

void Visuals::ESP_Snapline(C_BasePlayer * pEntity, Color clr)
{
	Vector max = pEntity->GetCollideable()->OBBMaxs();
	Vector2D pos;

	if (!Math::WorldToScreen(pEntity->m_vecOrigin(), pos))
		return;

	int wide, tall;
	g_VGuiSurface->GetScreenSize(wide, tall);

	auto color = g_LocalPlayer->CanSeePlayer(pEntity, HITBOX_HEAD) ? clr : clr;

	Draw::Line(wide / 2, tall, pos.x, pos.y, color);
}

void Visuals::Radar()
{
	if (!g_Options.VIS_ESP_Radar)
		return;

	for (int i = 1; i <= g_GlobalVars->maxClients; i++)
	{
		if (i == g_EngineClient->GetLocalPlayer())
			continue;

		auto pPlayer = reinterpret_cast< C_BasePlayer* >(g_EntityList->GetClientEntity(i));
		if (!pPlayer)
			continue;

		pPlayer->m_bSpotted() = true;
	}
}

