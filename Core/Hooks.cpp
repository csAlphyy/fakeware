#include "Hooks.h"
#include "Utilities.h"
#include "InputSys.h"
#include "Font.h"
#include "math.h"
#include "../Menu/Menu.h"
#include <codecvt>
#include <string>
#include "../skins.h"

void Marquee(std::string& clantag)
{
	std::string temp = clantag;
	clantag.erase(0, 1);
	clantag += temp[0];
}

RecvVarProxyFn originialsequencehook;

struct KnifeInfo_t {
	int definition_index;
	std::string display_name;
	std::string entity_name;
	std::string model_name;
	std::string killicon_name;

	KnifeInfo_t() {
		definition_index = 0;
		display_name = entity_name = model_name = killicon_name = "";
	}
	KnifeInfo_t(int definition, std::string display, std::string entity, std::string model, std::string killicon) {
		definition_index = definition;
		display_name = display;
		entity_name = entity;
		model_name = model;
		killicon_name = killicon;
	}
};

//default, bayonet, flip, gut, karambit, m9, huntsman, bowie, butterfly, falchion, daggers

enum KNIVES : int {
	DEFAULT_T = -1,
	DEFAULT_CT = 0,
	BAYONET = 1,
	FLIP,
	GUT,
	KARAMBIT,
	M9BAYONET,
	HUNTSMAN,
	BOWIE,
	BUTTERFLY,
	FALCHION,
	SHADOWDAGGERS
};

std::unordered_map<KNIVES, KnifeInfo_t> knivesInfo = {
	{ DEFAULT_CT, KnifeInfo_t(WEAPON_KNIFE, "Knife (Counter-Terrorists)", "WEAPON_KNIFE", "models/weapons/v_knife_default_ct.mdl", "knife_default_ct") },
{ DEFAULT_T, KnifeInfo_t(WEAPON_KNIFE_T,"Knife (Terrorists)", "WEAPON_KNIFE_T", "models/weapons/v_knife_default_t.mdl", "knife_t") },
{ BAYONET, KnifeInfo_t(WEAPON_KNIFE_BAYONET, "Bayonet", "WEAPON_KNIFE_BAYONET", "models/weapons/v_knife_bayonet.mdl", "bayonet") },
{ FLIP,KnifeInfo_t(WEAPON_KNIFE_FLIP, "Flip Knife", "WEAPON_KNIFE_FLIP", "models/weapons/v_knife_flip.mdl", "knife_flip") },
{ GUT, KnifeInfo_t(WEAPON_KNIFE_GUT, "Gut Knife", "WEAPON_KNIFE_GUT", "models/weapons/v_knife_gut.mdl", "knife_gut") },
{ KARAMBIT, KnifeInfo_t(WEAPON_KNIFE_KARAMBIT, "Karambit", "WEAPON_KNIFE_KARAMBIT", "models/weapons/v_knife_karam.mdl", "knife_karambit") },
{ M9BAYONET, KnifeInfo_t(WEAPON_KNIFE_M9_BAYONET,  "M9 Bayonet", "WEAPON_KNIFE_M9_BAYONET", "models/weapons/v_knife_m9_bay.mdl", "knife_m9_bayonet") },
{ HUNTSMAN, KnifeInfo_t(WEAPON_KNIFE_TACTICAL, "Huntsman Knife", "WEAPON_KNIFE_TACTICAL", "models/weapons/v_knife_tactical.mdl", "knife_tactical") },
{ BOWIE, KnifeInfo_t(WEAPON_KNIFE_SURVIVAL_BOWIE,  "Bowie Knife", "WEAPON_KNIFE_SURVIVAL_BOWIE", "models/weapons/v_knife_survival_bowie.mdl", "knife_survival_bowie") },
{ BUTTERFLY, KnifeInfo_t(WEAPON_KNIFE_BUTTERFLY, "Butterfly Knife", "WEAPON_KNIFE_BUTTERFLY", "models/weapons/v_knife_butterfly.mdl", "knife_butterfly") },
{ FALCHION, KnifeInfo_t(WEAPON_KNIFE_FALCHION, "Falchion Knife", "WEAPON_KNIFE_FALCHION", "models/weapons/v_knife_falchion_advanced.mdl", "knife_falchion") },
{ SHADOWDAGGERS, KnifeInfo_t(WEAPON_KNIFE_PUSH, "Shadow Daggers", "WEAPON_KNIFE_PUSH", "models/weapons/v_knife_push.mdl", "knife_push") }
};

void ChangeSkins(C_BasePlayer* local) {
	if (!local || Die)
		return;


	auto weapons = local->m_hMyWeapons();
	for (auto i = 0; weapons[i]; i++) {
		C_BaseAttributableItem* weapon = (C_BaseAttributableItem*)g_EntityList->GetClientEntityFromHandle(weapons[i]);
		if (!weapon)
			continue;

		int definition_index = weapon->m_Item().m_iItemDefinitionIndex();
		if (definition_index == 0)
			continue;


		weapon->m_nFallbackPaintKit() = k_skins[g_Options.skins[definition_index].weapon_skin_id].id;
	//	weapon->m_nFallbackPaintKit() = g_Options.skins[definition_index].weapon_skin_id;
		weapon->m_nFallbackSeed() = g_Options.skins[definition_index].weapon_seed;
		weapon->m_nFallbackStatTrak() = -1;
		weapon->m_flFallbackWear() = g_Options.skins[definition_index].weapon_wear;

		weapon->m_iItemIDHigh() = -1;
		weapon->m_iAccountID() = local->GetPlayerInfo().xuid_low;
	}
}



void ChangeKnife(C_BasePlayer* local, std::unordered_map<int, SkinInfo_t> skinsInfo, KNIVES type) {
	if (!local)
		return;

	auto weapons = local->m_hMyWeapons();
	for (auto i = 0; weapons[i]; i++) {
		C_BaseAttributableItem* weapon = (C_BaseAttributableItem*)g_EntityList->GetClientEntityFromHandle(weapons[i]);
		if (!weapon)
			continue;

		int definition_index = weapon->m_Item().m_iItemDefinitionIndex();
		if ((definition_index == WEAPON_KNIFE || definition_index == WEAPON_KNIFE_T) ||
			(definition_index >= WEAPON_KNIFE_BAYONET && definition_index <= WEAPON_KNIFE_PUSH)) {

			weapon->m_nModelIndex() = g_ModelInfo->GetModelIndex(knivesInfo[type].model_name.c_str());
			weapon->m_Item().m_iItemDefinitionIndex() = knivesInfo[type].definition_index;

			int definition_index = weapon->m_Item().m_iItemDefinitionIndex();
			if (definition_index == 0)
				continue;

			if (skinsInfo[definition_index].weapon_skin_id <= 0)
				continue;

			weapon->m_nFallbackPaintKit() = skinsInfo[definition_index].weapon_skin_id;
			weapon->m_nFallbackSeed() = skinsInfo[definition_index].weapon_seed;
			weapon->m_nFallbackStatTrak() = skinsInfo[definition_index].weapon_stat_trak;
			weapon->m_flFallbackWear() = skinsInfo[definition_index].weapon_wear;

			weapon->m_iItemIDHigh() = -1;
			weapon->m_iAccountID() = local->GetPlayerInfo().xuid_low;

		}
	}
}

#include "../skins.h"

auto RandomSequence = [](int low, int high) -> int {
	return rand() % (high - low + 1) + low;
};
const static std::unordered_map<std::string, int(*)(int)> animation_fix_map{
	{ "models/weapons/v_knife_butterfly.mdl", [](int sequence) -> int {
	switch (sequence) {
	case SEQUENCE_DEFAULT_DRAW:
		return RandomSequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
	case SEQUENCE_DEFAULT_LOOKAT01:
		return RandomSequence(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
	default:
		return sequence + 1;
	}
} },
{ "models/weapons/v_knife_falchion_advanced.mdl", [](int sequence) -> int {
	switch (sequence) {
	case SEQUENCE_DEFAULT_IDLE2:
		return SEQUENCE_FALCHION_IDLE1;
	case SEQUENCE_DEFAULT_HEAVY_MISS1:
		return RandomSequence(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
	case SEQUENCE_DEFAULT_LOOKAT01:
		return RandomSequence(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
	case SEQUENCE_DEFAULT_DRAW:
	case SEQUENCE_DEFAULT_IDLE1:
		return sequence;
	default:
		return sequence - 1;
	}
} },
{ "models/weapons/v_knife_push.mdl", [](int sequence) -> int {
	switch (sequence) {
	case SEQUENCE_DEFAULT_IDLE2:
		return SEQUENCE_DAGGERS_IDLE1;
	case SEQUENCE_DEFAULT_LIGHT_MISS1:
	case SEQUENCE_DEFAULT_LIGHT_MISS2:
		return RandomSequence(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
	case SEQUENCE_DEFAULT_HEAVY_MISS1:
		return RandomSequence(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
	case SEQUENCE_DEFAULT_HEAVY_HIT1:
	case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
	case SEQUENCE_DEFAULT_LOOKAT01:
		return sequence + 3;
	case SEQUENCE_DEFAULT_DRAW:
	case SEQUENCE_DEFAULT_IDLE1:
		return sequence;
	default:
		return sequence + 2;
	}
} },
{ "models/weapons/v_knife_survival_bowie.mdl", [](int sequence) -> int {
	switch (sequence) {
	case SEQUENCE_DEFAULT_DRAW:
	case SEQUENCE_DEFAULT_IDLE1:
		return sequence;
	case SEQUENCE_DEFAULT_IDLE2:
		return SEQUENCE_BOWIE_IDLE1;
	default:
		return sequence - 1;
	}
} }
};

class RecvPropHook {
private:
	RecvProp * target_prop;
	RecvVarProxyFn original_proxy_fn;
public:
	void setup(RecvProp* target_property);
	void unhook();
	const RecvVarProxyFn get_original();
	void set_proxy_fn(RecvVarProxyFn user_proxy_fn);
};

void RecvPropHook::setup(RecvProp* target_property) {
	this->target_prop = target_property;
	this->original_proxy_fn = target_property->m_ProxyFn;
}
void RecvPropHook::unhook() {
	target_prop->m_ProxyFn = this->original_proxy_fn;
}
const RecvVarProxyFn RecvPropHook::get_original() {
	return this->original_proxy_fn;
}
void RecvPropHook::set_proxy_fn(RecvVarProxyFn user_proxy_fn) {
	target_prop->m_ProxyFn = user_proxy_fn;
}

RecvPropHook viewmodel_hook;
RecvPropHook sequence_hook;

void RecvProxyViewModel(CRecvProxyData *pData, void *pStruct, void *pOut) {
	int default_t = g_ModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");
	int default_ct = g_ModelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
	int indexBayonet = g_ModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
	int indexButterfly = g_ModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
	int indexFlip = g_ModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
	int indexGut = g_ModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
	int indexKarambit = g_ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
	int indexM9 = g_ModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
	int indexHuntsman = g_ModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
	int indexFalchion = g_ModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
	int indexSDaggers = g_ModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
	int indexBowie = g_ModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
	int indexGunGame = g_ModelInfo->GetModelIndex("models/weapons/v_knife_gg.mdl");

	C_BasePlayer* pLocal = C_BasePlayer::GetPlayerByIndex(g_EngineClient->GetLocalPlayer());

	if (pLocal && pLocal->IsAlive() && (
		pData->m_Value.m_Int == default_t ||
		pData->m_Value.m_Int == default_ct ||
		pData->m_Value.m_Int == indexBayonet ||
		pData->m_Value.m_Int == indexFlip ||
		pData->m_Value.m_Int == indexGunGame ||
		pData->m_Value.m_Int == indexGut ||
		pData->m_Value.m_Int == indexKarambit ||
		pData->m_Value.m_Int == indexM9 ||
		pData->m_Value.m_Int == indexHuntsman ||
		pData->m_Value.m_Int == indexBowie ||
		pData->m_Value.m_Int == indexButterfly ||
		pData->m_Value.m_Int == indexFalchion ||
		pData->m_Value.m_Int == indexSDaggers)) {
		//default, bayonet, flip, gut, karambit, m9, huntsman, bowie, butterfly, falchion, daggers
		switch (0) {
		case 1:
			pData->m_Value.m_Int = indexBayonet;
			break;
		case 2:
			pData->m_Value.m_Int = indexFlip;
			break;
		case 3:
			pData->m_Value.m_Int = indexGut;
			break;
		case 4:
			pData->m_Value.m_Int = indexKarambit;
			break;
		case 5:
			pData->m_Value.m_Int = indexM9;
			break;
		case 6:
			pData->m_Value.m_Int = indexHuntsman;
			break;
		case 7:
			pData->m_Value.m_Int = indexBowie;
			break;
		case 8:
			pData->m_Value.m_Int = indexButterfly;
			break;
		case 9:
			pData->m_Value.m_Int = indexFalchion;
			break;
		case 10:
			pData->m_Value.m_Int = indexSDaggers;
			break;
		}
	}

	viewmodel_hook.get_original()(pData, pStruct, pOut);
}
/*uint8_t* m_present;

typedef HRESULT(_stdcall *Present_T)(void*, const RECT*, RECT*, HWND, RGNDATA*);
Present_T oPresent;
HRESULT _stdcall hkPresent(LPDIRECT3DDEVICE9 pDevice, RECT* pSourceRect, RECT* pDestRect, HWND hDestWindowOverride, RGNDATA* pDirtyRegion)
{
	static bool mouse_enabled = false;
	pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);

	ImGuiStyle &style = ImGui::GetStyle();

	return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}*/


using do_post_screen_space_effects_t = bool(__thiscall*)(void*, CViewSetup*);
bool _fastcall hkDoPostScreenSpaceEffects(void* ecx, void* edx, CViewSetup* pSetup)
{
	static auto ofunc = Hooks::ClientModeHook.get_original<do_post_screen_space_effects_t>(44);

	IMaterial *pMatGlowColor = g_MatSystem->FindMaterial("", TEXTURE_GROUP_OTHER, true);
	g_ModelRender->ForcedMaterialOverride(pMatGlowColor);

	if (g_Options.glow && g_GlowObjManager && g_EngineClient->IsConnected())
	{
		auto local = g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer());
		if (local)
		{
			for (int i = 0; i < g_GlowObjManager->m_GlowObjectDefinitions_t.Count(); ++i)
			{
				if (g_GlowObjManager->m_GlowObjectDefinitions_t[i].IsUnused() || !g_GlowObjManager->m_GlowObjectDefinitions_t[i].getEnt())
					continue;

				CGlowObjectManager::GlowObjectDefinition_tt* glowEnt = &g_GlowObjManager->m_GlowObjectDefinitions_t[i];
				C_BasePlayer * pEntity;
			
				for (int i = 1; i <= g_GlobalVars->maxClients; i++)
				{
					auto pEntity = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(i));

					if (pEntity) {
						if (pEntity->IsPlayer())
							glowEnt->set(1.0f, 1.0f, 1.0f, 1.0f);
					}
						
				}
			}
		}
	}
	return ofunc(ecx, pSetup);
}


void RecvProxyModelSequence(const CRecvProxyData *pDataConst, void *pStruct, void *pOut) {
	if (!g_LocalPlayer || !g_LocalPlayer->IsAlive())
		return sequence_hook.get_original()(pDataConst, pStruct, pOut);

	// Remove the constness from the proxy data allowing us to make changes.
	auto proxy_data = const_cast<CRecvProxyData*>(pDataConst);

	auto pViewModel = static_cast<C_BaseViewModel*>(pStruct);

	if (pViewModel && pViewModel->m_hOwner() && pViewModel->m_hOwner().IsValid())
	{
		auto pOwner = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntityFromHandle(pViewModel->m_hOwner()));

		if (pOwner == g_LocalPlayer)
		{
			// Get the filename of the current view model.
			auto knife_model = g_ModelInfo->GetModel(pViewModel->m_nModelIndex());
			auto model_name = g_ModelInfo->GetModelName(knife_model);

			if (animation_fix_map.count(model_name))
			{
				proxy_data->m_Value.m_Int = animation_fix_map.at(model_name)(proxy_data->m_Value.m_Int);
				//pViewModel->SendViewModelMatchingSequence(animation_fix_map.at(model_name)(proxy_data->m_Value.m_Int));
			}
		}
	}

	sequence_hook.get_original()(pDataConst, pStruct, pOut);
}



namespace Hooks
{
	vfunc_hook HLClientHook;
	vfunc_hook EngineHook;
	vfunc_hook Direct3DHook;
	vfunc_hook VGUIPanelHook;
	vfunc_hook VGUISurfaceHook;
	vfunc_hook MDLRenderHook;
	vfunc_hook ClientModeHook;
	vfunc_hook RenderViewHook;
	vfunc_hook GameEventManagerHook;
	vfunc_hook MatSystemHook;
	//RecvPropHook* SequenceHook;

	void Initialize()
	{
		std::ofstream("csgo\\materials\\simple_regular.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
		std::ofstream("csgo\\materials\\simple_ignorez.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
		std::ofstream("csgo\\materials\\simple_flat.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
		std::ofstream("csgo\\materials\\simple_flat_ignorez.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";

		// Set up virtual function hooks
		HLClientHook.setup(g_CHLClient);
		EngineHook.setup(g_EngineClient);
		Direct3DHook.setup(g_D3DDevice9);
		VGUIPanelHook.setup(g_VGuiPanel);
		VGUISurfaceHook.setup(g_VGuiSurface);
		MDLRenderHook.setup(g_ModelRender);
		ClientModeHook.setup(g_ClientMode);
		GameEventManagerHook.setup(g_GameEvents);
		MatSystemHook.setup(g_MatSystem);

		// Hook functions

		initialize();
		HLClientHook.hook_index(Index::FrameStageNotify, hkFrameStageNotify);
		HLClientHook.hook_index(Index::CreateMove, hkCreateMove_Proxy);

		EngineHook.hook_index(27, IsConnected);

		Direct3DHook.hook_index(Index::EndScene, hkEndScene);
		Direct3DHook.hook_index(Index::Reset, hkReset);

		VGUIPanelHook.hook_index(Index::PaintTraverse, hkPaintTraverse);

		VGUISurfaceHook.hook_index(Index::PlaySound, hkPlaySound);
		//VGUISurfaceHook.hook_index(Index::LockCursor, hkLockCursor);

		MDLRenderHook.hook_index(Index::DrawModelExecute, hkDrawModelExecute);

		ClientModeHook.hook_index(Index::GetViewmodelFOV, hkGetViewmodelFOV);
		ClientModeHook.hook_index(18, OverrideView);
		ClientModeHook.hook_index(44, hkDoPostScreenSpaceEffects);

		GameEventManagerHook.hook_index(Index::FireEventClientSide, hkFireEventClientSide);

		auto SequenceProp = C_BaseViewModel::GetSequenceProp();
		viewmodel_hook.setup(NetvarSys::Get().GetNetvarProp("DT_BaseViewModel", "m_nModelIndex"));
		sequence_hook.setup(NetvarSys::Get().GetNetvarProp("DT_BaseViewModel", "m_nSequence"));
		viewmodel_hook.set_proxy_fn(reinterpret_cast<RecvVarProxyFn>(RecvProxyViewModel));
		sequence_hook.set_proxy_fn(reinterpret_cast<RecvVarProxyFn>(RecvProxyModelSequence));

	//	oPresent = **reinterpret_cast<Present_T**>(m_present);

	//	**reinterpret_cast<void***>(m_present) = reinterpret_cast<void*>(&hkPresent);
	//	m_present = Utilities::PatternScan(GetModuleHandleW(L"gameoverlayrenderer.dll"), "FF 15 ? ? ? ? 8B F8 85 DB 74 1F") + 0x2;//big ( large ) obs bypass


		g_VGuiSurface->PlaySound_("ui\\mm_success_lets_roll.wav");

		Font::Get().Create();
		g_ClientState->ForceFullUpdate();
		Skinchanger::Get().Dump();
		
	}



	void Kill()
	{
		// Kill all hooks
		static ConVar* cl_mouseenable = g_CVar->FindVar("cl_mouseenable"); cl_mouseenable->SetValue(1);
		Direct3DHook.unhook_all();
		HLClientHook.unhook_all();
		EngineHook.unhook_all();	
		VGUIPanelHook.unhook_all();
		VGUISurfaceHook.unhook_all();
		MDLRenderHook.unhook_all();
		ClientModeHook.unhook_all();
		GameEventManagerHook.unhook_all();
		viewmodel_hook.set_proxy_fn(viewmodel_hook.get_original());
		sequence_hook.set_proxy_fn(sequence_hook.get_original());

	}

	void __stdcall OverrideView(CViewSetup* setup)
	{
		if (g_EngineClient->IsInGame()) {
		
			if (!g_LocalPlayer->m_bIsScoped())
				setup->fov = 100.f;
			grenade_prediction::Get().View(setup);
		}
		reinterpret_cast<OverrideView_t>(ClientModeHook.get_original<OverrideView_t>(18))(g_ClientMode, setup);		
	}

	Menu *menu;
	#include "../Menu/Menu.h"

	bool __stdcall IsConnected() {

		auto ofunc = EngineHook.get_original<IsConnected_t>(27);
		
		static void* unk = Utilities::PatternScan(GetModuleHandleA("client.dll"), "75 04 B0 01 5F") - 2;

		if (_ReturnAddress() == unk && g_Options.MISC_ForceInventoryOpen) {
			return false;
		}

		return ofunc(g_EngineClient);
	}

	//--------------------------------------------------------------------------------
	long __stdcall hkEndScene(IDirect3DDevice9* device)
	{
		static bool once = false;
		if (!once)
		{
			ImGui_ImplDX9_Init(FindWindow("Valve001", NULL), device);
			once = true;
		}
		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::GetIO().MouseDrawCursor = g_Options.menu_open;

		if (g_Options.menu_open) 
		{
			ImGui_ImplDX9_NewFrame();
			
			Init(); //calls the function where i draw the menu, i can draw it here but i dont want to.

			ImGui::Render();
		}
		
		

		if (g_Options.MISC_RankReveal && InputSys::Get().IsKeyDown(VK_TAB))
			Utilities::RevealRanks();

		auto ofunc = Direct3DHook.get_original<EndScene>(Index::EndScene);
		return ofunc(device);
	}

	//--------------------------------------------------------------------------------
	long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		auto ofunc = Direct3DHook.get_original<Reset>(Index::Reset);

		
		auto hr = ofunc(device, pPresentationParameters);

		if (hr >= 0)
		{
			Font::Get().Create();
		}

		return hr;
	}

	//--------------------------------------------------------------------------------
	void __stdcall hkPlaySound(const char* name)
	{
		static auto ofunc = VGUISurfaceHook.get_original<PlaySound>(Index::PlaySound);
		ofunc(g_VGuiSurface, name);

		if (g_Options.MISC_AutoAccept)
		{
			// Auto Accept
			if (strstr(name, "!UI/competitive_accept_beep.wav"))
			{
				static auto fnAccept = (void(*)())Utilities::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 83 BE");

				fnAccept();

				//This will flash the CSGO window on the taskbar
				//so we know a game was found (you cant hear the beep sometimes cause it auto-accepts too fast)
				FLASHWINFO fi;
				fi.cbSize = sizeof(FLASHWINFO);
				fi.hwnd = InputSys::Get().GetMainWindow();
				fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
				fi.uCount = 0;
				fi.dwTimeout = 0;
				FlashWindowEx(&fi);
			}
		}
	}

	//--------------------------------------------------------------------------------

#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

	#include <TlHelp32.h>
	#include <Psapi.h>

	DWORD Pattern(std::string moduleName, std::string pattern) {

		const char* pat = pattern.c_str();
		DWORD firstMatch = 0;
		DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
		MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
		DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;

		for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++) {
			if (!*pat)
				return firstMatch;

			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat)) {

				if (!firstMatch)
					firstMatch = pCur;

				if (!pat[2])
					return firstMatch;

				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
					pat += 3;

				else
					pat += 2;
			}

			else {
				pat = pattern.c_str();
				firstMatch = 0;
			}
		}
		return NULL;
	}



	void Spoof(C_BaseEntity* pLocal, int Coin, int Music, int Level, int CommendsLeader, int CommendsTeacher, int CommendsFriendly)
	{
		if (!pLocal)
			return;
		static DWORD DT_CSPlayerResource = NULL;

		if (DT_CSPlayerResource == NULL)
			DT_CSPlayerResource = Pattern("client.dll", "8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7") + 2;

		if (!DT_CSPlayerResource)
			return;



		DWORD ptrResource = **(DWORD**)DT_CSPlayerResource;
		DWORD m_nActiveCoinRank = (DWORD)ptrResource + 0x4838 + (pLocal->EntIndex() * 4);
		DWORD m_nMusicID = (DWORD)ptrResource + 0x493C + (pLocal->EntIndex() * 4);
		DWORD m_nPersonaDataPublicLevel = (DWORD)ptrResource + 0x4AD0 + (pLocal->EntIndex() * 4);
		DWORD m_nPersonaDataPublicCommendsLeader = (DWORD)ptrResource + 0x4BD4 + (pLocal->EntIndex() * 4);
		DWORD m_nPersonaDataPublicCommendsTeacher = (DWORD)ptrResource + 0x4CD8 + (pLocal->EntIndex() * 4);
		DWORD m_nPersonaDataPublicCommendsFriendly = (DWORD)ptrResource + 0x4DDC + (pLocal->EntIndex() * 4);

		if (Coin > 0) *(PINT)((DWORD)m_nActiveCoinRank) = Coin;
		if (Music > 0) *(PINT)((DWORD)m_nMusicID) = Music;
		if (Level > 0) *(PINT)((DWORD)m_nPersonaDataPublicLevel) = Level;
		if (CommendsLeader > 0) *(PINT)((DWORD)m_nPersonaDataPublicCommendsLeader) = CommendsLeader;
		if (CommendsTeacher > 0) *(PINT)((DWORD)m_nPersonaDataPublicCommendsTeacher) = CommendsTeacher;
		if (CommendsFriendly > 0) *(PINT)((DWORD)m_nPersonaDataPublicCommendsFriendly) = CommendsFriendly;
	}

	void __stdcall hkFrameStageNotify(ClientFrameStage_t stage)
	{
		C_BasePlayer* me2 = C_BasePlayer::GetPlayerByIndex(g_EngineClient->GetLocalPlayer());
		static bool ins = false;
		if (!ins && GetAsyncKeyState(VK_INSERT)) {
			ins = true; g_Options.menu_open = !g_Options.menu_open;
			static ConVar* cl_mouseenable = g_CVar->FindVar("cl_mouseenable"); cl_mouseenable->SetValue(!g_Options.menu_open);
		}
		if (ins && !GetAsyncKeyState(VK_INSERT)) {
			ins = false;
		}
		if (!g_EngineClient->IsInGame() || !g_EngineClient->IsConnected())
			return;

		if (me2)
		{
			Spoof(me2, 916, 0, 30, 4343, 3434, 3434);
		}
		switch (stage)
		{
		case FRAME_UNDEFINED:
			break;
		case FRAME_START:
			

			
			break;
		case FRAME_NET_UPDATE_START:
			break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_START:
		{
			C_BasePlayer* me = C_BasePlayer::GetPlayerByIndex(g_EngineClient->GetLocalPlayer());
			if (!me) break;

		/*	using convert_type = std::codecvt_utf8<wchar_t>;
			std::wstring_convert<convert_type, wchar_t> converter;
			std::string converted_str = converter.to_bytes(g_Options.skins_knifeseed);
			std::unordered_map<int, SkinInfo_t> skinsInfo;

			skinsInfo[knivesInfo[(KNIVES)g_Options.skins_knifemodel].definition_index] = SkinInfo_t(knives_skins[g_Options.skins_knifeskins], (g_Options.skins_knifeseed));


			*/
			ChangeSkins(me);
		//	ChangeKnife(me, skinsInfo, (KNIVES)g_Options.skins_knifemodel);

			Spoof(me, 0, 0, 30, 4343, 3434, 3434);
		}
		break;
		case FRAME_NET_UPDATE_POSTDATAUPDATE_END:
			break;
		case FRAME_NET_UPDATE_END:
			break;
		case FRAME_RENDER_START:

			break;
		case FRAME_RENDER_END:
			break;
		}

		static auto ofunc = HLClientHook.get_original<FrameStageNotify>(Index::FrameStageNotify);
		ofunc(g_CHLClient, stage);
	}

	//--------------------------------------------------------------------------------
	void __stdcall hkLockCursor()
	{
		static auto ofunc = VGUISurfaceHook.get_original<LockCursor>(Index::LockCursor);

		if (Menu::Get().IsVisible()) g_VGuiSurface->UnlockCursor();
		else ofunc();
	}

	//--------------------------------------------------------------------------------
	float __stdcall hkGetViewmodelFOV()
	{
		static auto ofunc = ClientModeHook.get_original<GetViewmodelFOV>(Index::GetViewmodelFOV);
		if (g_EngineClient->IsTakingScreenshot()) return ofunc();
		else return ofunc() + g_Options.MISC_ViewmodelFOV;
	}
}
