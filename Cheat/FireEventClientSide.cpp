#include "../Core/Hooks.h"

//--------------------------------------------------------------------------------
bool __stdcall Hooks::hkFireEventClientSide(IGameEvent* event)
{
	if (!strcmp(event->GetName(), "player_death") && g_EngineClient->GetPlayerForUserID(event->GetInt("attacker")) == g_EngineClient->GetLocalPlayer())
	{
		if (auto icon_override = Skinchanger::Get().GetIconOverride(event->GetString("weapon")))
		{
			event->SetString("weapon", icon_override);
			g_VGuiSurface->PlaySound_("commander\\commander_comment_18.wav");
		}
	}

	static auto ofunc = GameEventManagerHook.get_original<FireEventClientSide>(Index::FireEventClientSide);
	return ofunc(g_GameEvents, event);
}
