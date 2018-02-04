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
		Visuals::Get().PaintTraverse();
		grenade_prediction::Get().Paint();


		Menu::Get().PaintTraverse();	
	}
}
