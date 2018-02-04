#pragma once
#include "vfunc_hook.h"
#include "RecvProxyHook.h"
#include "SDK.h"
#include "../SDK/csgostructs.h"
#include "../Cheat/Cheats.h"
#include "options.h"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_internal.h"
#include "../ImGui/imgui_impl_dx9.h"

#include <d3d9.h>

namespace Index
{
	constexpr auto EndScene = 42;
	constexpr auto Reset = 16;
	constexpr auto PaintTraverse = 41;
	constexpr auto CreateMove = 21;
	constexpr auto PlaySound = 82;
	constexpr auto FrameStageNotify = 36;
	constexpr auto DrawModelExecute = 21;
	constexpr auto FireEventClientSide = 9;
	constexpr auto LockCursor = 67;
	constexpr auto GetViewmodelFOV = 35;
}
#include "../grenadeprediction.h"

namespace Hooks
{
	void Initialize();
	void Kill();

	extern vfunc_hook HLClientHook;
	extern vfunc_hook EngineHook;
	extern vfunc_hook Direct3DHook;
	extern vfunc_hook VGUIPanelHook;
	extern vfunc_hook VGUISurfaceHook;
	extern vfunc_hook MDLRenderHook;
	extern vfunc_hook ClientModeHook;
	extern vfunc_hook GameEventManagerHook;
	extern vfunc_hook MatSystemHook;
	//extern RecvPropHook* SequenceHook;

	using EndScene = long(__stdcall *)(IDirect3DDevice9*);
	using Reset = long(__stdcall *)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	using CreateMove = void(__thiscall*)(IBaseClientDLL*, int, float, bool);
	using PaintTraverse = void(__thiscall*)(IPanel*, vgui::VPANEL, bool, bool);
	using PlaySound = void(__thiscall*)(ISurface*, const char* name);
	using DrawModelExecute = void(__thiscall*)(IVModelRender*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
	using FireEvent = bool(__thiscall*)(IGameEventManager2*, IGameEvent* pEvent);
	using FrameStageNotify = void(__thiscall*)(IBaseClientDLL*, ClientFrameStage_t);
	using OverrideView_t = void(__thiscall*)(IClientMode*, CViewSetup*);
	using FireEventClientSide = bool(__thiscall*)(IGameEventManager2*, IGameEvent*);
	using LockCursor = void(__thiscall*)();
	using GetViewmodelFOV = float(__thiscall*)();
	using IsConnected_t = bool(__thiscall*)(IVEngineClient*);

	void __cdecl hkSequenceProxyFn(const CRecvProxyData* proxy_data_const, void* entity, void* output);
	long __stdcall hkEndScene(IDirect3DDevice9* device);
	void __stdcall OverrideView(CViewSetup* setup);
	long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
	void __stdcall hkCreateMove(int sequence, float frametime, bool active, bool& bSendPacket);
	void __stdcall hkCreateMove_Proxy(int sequence, float frametime, bool active);
	void __stdcall hkPaintTraverse(vgui::VPANEL panel, bool forceRepaint, bool allowForce);
	void __stdcall hkPlaySound(const char* name);
	void __stdcall hkDrawModelExecute(IMatRenderContext * ctx, const DrawModelState_t &state, const ModelRenderInfo_t &info, matrix3x4_t *matrix);
	void __stdcall hkFrameStageNotify(ClientFrameStage_t stage);
	bool __stdcall hkFireEventClientSide(IGameEvent* event);
	void __stdcall hkLockCursor();
	float __stdcall hkGetViewmodelFOV();
	bool __stdcall IsConnected();
}