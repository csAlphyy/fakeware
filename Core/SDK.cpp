#include "SDK.h"
#include "Utilities.h"

MsgFn g_Msg;
WarningFn g_Warning;
IVEngineClient* g_EngineClient = nullptr;
IBaseClientDLL* g_CHLClient = nullptr;
IClientEntityList* g_EntityList = nullptr;
CGlobalVarsBase* g_GlobalVars = nullptr;
IEngineTrace* g_EngineTrace = nullptr;
ICvar* g_CVar = nullptr;
IPanel* g_VGuiPanel = nullptr;
IClientMode* g_ClientMode = nullptr;
IVDebugOverlay* g_DebugOverlay = nullptr;
ISurface* g_VGuiSurface = nullptr;
CInput* g_Input = nullptr;
IVModelInfoClient* g_ModelInfo = nullptr;
IVModelRender* g_ModelRender = nullptr;
IVRenderView* g_RenderView = nullptr;
IMaterialSystem* g_MatSystem = nullptr;
IGameEventManager2* g_GameEvents = nullptr;
IMoveHelper* g_MoveHelper = nullptr;
IPrediction* g_Prediction = nullptr;
CGameMovement* g_GameMovement = nullptr;
IEngineSound* g_EngineSound = nullptr;
CGlowObjectManager* g_GlowObjManager = nullptr;
IViewRender* g_ViewRender = nullptr;
IDirect3DDevice9* g_D3DDevice9 = nullptr;
CClientState* g_ClientState = nullptr;
IPhysicsSurfaceProps* g_PhysSurface = nullptr;
ILocalize* g_Localize = nullptr;
C_LocalPlayer g_LocalPlayer;
bool Die = false;

namespace Interfaces
{
	CreateInterfaceFn GetModuleFactory(HMODULE Module)
	{
		return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(Module, "CreateInterface"));
	}

	template<typename T>
	T* GetInterface(CreateInterfaceFn Interface, const char* szVersion)
	{
		auto result = reinterpret_cast<T*>(Interface(szVersion, nullptr));

		if (!result)
		{
			throw std::runtime_error(std::string("[GetInterface] Failed to GetOffset interface: ") + szVersion);
		}

		return result;
	}

	template<typename T>
	inline T GetExport(const char *dll, const char *name)
	{
		return (T) ((void *(*)(const char *, const char *))GetProcAddress(GetModuleHandleA(dll), name));
	}

	void Initialize()
	{
		g_Msg = GetExport<MsgFn>("tier0.dll", "Msg");
		g_Warning = GetExport<WarningFn>("tier0.dll", "Warning");

		auto EngineFactory = GetModuleFactory(GetModuleHandleW(L"engine.dll"));
		auto ClientFactory = GetModuleFactory(GetModuleHandleW(L"client.dll"));
		auto ValveStdFactory = GetModuleFactory(GetModuleHandleW(L"vstdlib.dll"));
		auto VguiFactory = GetModuleFactory(GetModuleHandleW(L"vguimatsurface.dll"));
		auto Vgui2Factory = GetModuleFactory(GetModuleHandleW(L"vgui2.dll"));
		auto MatSysFactory = GetModuleFactory(GetModuleHandleW(L"materialsystem.dll"));
		auto DataCacheFactory = GetModuleFactory(GetModuleHandleW(L"datacache.dll"));
		auto VphysicsFactory = GetModuleFactory(GetModuleHandleW(L"vphysics.dll"));
		auto LocalizeFactory = GetModuleFactory(GetModuleHandleW(L"localize.dll"));

		g_CHLClient = GetInterface<IBaseClientDLL>(ClientFactory, "VClient018");
		g_EntityList = GetInterface<IClientEntityList>(ClientFactory, "VClientEntityList003");
		g_Prediction = GetInterface<IPrediction>(ClientFactory, "VClientPrediction001");
		g_GameMovement = GetInterface<CGameMovement>(ClientFactory, "GameMovement001");
		g_EngineClient = GetInterface<IVEngineClient>(EngineFactory, "VEngineClient014");
		g_ModelInfo = GetInterface<IVModelInfoClient>(EngineFactory, "VModelInfoClient004");
		g_ModelRender = GetInterface<IVModelRender>(EngineFactory, "VEngineModel016");
		g_RenderView = GetInterface<IVRenderView>(EngineFactory, "VEngineRenderView014");
		g_EngineTrace = GetInterface<IEngineTrace>(EngineFactory, "EngineTraceClient004");
		g_DebugOverlay = GetInterface<IVDebugOverlay>(EngineFactory, "VDebugOverlay004");
		g_GameEvents = GetInterface<IGameEventManager2>(EngineFactory, "GAMEEVENTSMANAGER002");
		g_EngineSound = GetInterface<IEngineSound>(EngineFactory, "IEngineSoundClient003");
		g_MatSystem = GetInterface<IMaterialSystem>(MatSysFactory, "VMaterialSystem080");
		g_CVar = GetInterface<ICvar>(ValveStdFactory, "VEngineCvar007");
		g_VGuiPanel = GetInterface<IPanel>(Vgui2Factory, "VGUI_Panel009");
		g_VGuiSurface = GetInterface<ISurface>(VguiFactory, "VGUI_Surface031");
		g_PhysSurface = GetInterface<IPhysicsSurfaceProps>(VphysicsFactory, "VPhysicsSurfaceProps001");
		g_Localize = GetInterface<ILocalize>(LocalizeFactory, "Localize_001");

		auto Client = GetModuleHandleW(L"client.dll");
		auto Engine = GetModuleHandleW(L"engine.dll");
		auto DX9API = GetModuleHandleW(L"shaderapidx9.dll");

		g_GlobalVars = **(CGlobalVarsBase***)(Utilities::PatternScan(Client, "A1 ? ? ? ? 5E 8B 40 10") + 1);
		g_ClientMode = *(IClientMode**)(Utilities::PatternScan(Client, "A1 ? ? ? ? 8B 80 ? ? ? ? 5D") + 1);
		g_Input = *(CInput**)(Utilities::PatternScan(Client, "B9 ? ? ? ? 8B 40 38 FF D0 84 C0 0F 85") + 1);
		g_MoveHelper = **(IMoveHelper***)(Utilities::PatternScan(Client, "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01") + 2);
		g_GlowObjManager = *(CGlowObjectManager**)(Utilities::PatternScan(Client, "0F 11 05 ? ? ? ? 83 C8 01") + 3);
		g_ViewRender = *(IViewRender**)(Utilities::PatternScan(Client, "A1 ? ? ? ? B9 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? FF 10") + 1);
		g_D3DDevice9 = **(IDirect3DDevice9***)(Utilities::PatternScan(DX9API, "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
		g_ClientState = **(CClientState***)(Utilities::PatternScan(Engine, "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);
		g_LocalPlayer = *(C_LocalPlayer*)(Utilities::PatternScan(Client, "8B 0D ? ? ? ? 83 FF FF 74 07") + 2);
	}
}
