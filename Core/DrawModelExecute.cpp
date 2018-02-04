#include "Hooks.h"

void __stdcall Hooks::hkDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &info, matrix3x4_t* matrix)
{
	if (!g_EngineClient->IsTakingScreenshot())
	{
		Visuals::Get().DrawModelExecute(ctx, state, info, matrix);
	}

	static auto ofunc = MDLRenderHook.get_original<DrawModelExecute>(Index::DrawModelExecute);
	ofunc(g_ModelRender, ctx, state, info, matrix);

	g_ModelRender->ForcedMaterialOverride(nullptr);
}
