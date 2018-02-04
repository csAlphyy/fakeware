#include "Hooks.h"
#include "../Menu/Menu.h"

void __stdcall Hooks::hkPaintTraverse(vgui::VPANEL panel, bool forceRepaint, bool allowForce)
{
	static auto panelId = vgui::VPANEL{ 0 };
	static auto ofunc = VGUIPanelHook.get_original<PaintTraverse>(Index::PaintTraverse);
	const auto panelName = g_VGuiPanel->GetName(panel);

	ofunc(g_VGuiPanel, panel, forceRepaint, allowForce);

	if (g_EngineClient->IsTakingScreenshot()) return;

	if (!panelId)
	{
		if (!strcmp(panelName, "FocusOverlayPanel"))
		{
			panelId = panel;
		}
	}
	else if (panelId == panel)
	{
	//	Visuals::Get().PaintTraverse();
		for (int i = 0; i < g_EntityList->GetHighestEntityIndex(); i++) {
			C_BasePlayer* entity = C_BasePlayer::GetPlayerByIndex(i);

			if (!entity) continue;
			if (entity->IsDormant() || entity->m_iHealth() <= 0) continue;

			RECT bbox = getPlayerBox(entity);

			g_VGuiSurface->DrawSetColor(0, 0, 0, 255);
			g_VGuiSurface->DrawOutlinedRect(bbox.left - 1, bbox.top - 1, bbox.right + 1, bbox.bottom + 1);
			g_VGuiSurface->DrawOutlinedRect(bbox.left + 1, bbox.top + 1, bbox.right - 1, bbox.bottom - 1);

			g_VGuiSurface->DrawSetColor(255, 255, 255, 255);
			g_VGuiSurface->DrawOutlinedRect(bbox.left, bbox.top, bbox.right, bbox.bottom);
		}
		grenade_prediction::Get().Paint();
		/*if (g_LocalPlayer && g_LocalPlayer->IsAlive()) {
			float damage;

			int xs, ys;
			g_EngineClient->GetScreenSize(xs, ys);
			xs /= 2; ys /= 2;

			g_VGuiSurface->DrawSetColor(trace_autowallable(damage) ? Color(130, 241, 13) : Color(255, 102, 102));
			g_VGuiSurface->DrawOutlinedCircle(xs - 2, ys - 2, 5, 5);
		}*/


		Menu::Get().PaintTraverse();	
	}
}
