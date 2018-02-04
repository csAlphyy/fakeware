#include "Skinchanger.h"

#include "KitParser.h"

#include <sstream>
#include <cereal/archives/json.hpp>

static CreateClientClassFn GetWearableCreateFn()
{
	auto clazz = g_CHLClient->GetAllClasses();

	while (strcmp(clazz->m_pNetworkName, "CEconWearable"))
		clazz = clazz->m_pNext;

	return clazz->m_pCreateFn;
}

void Skinchanger::Run()
{
	// Skin config
	static auto ConfigCreated = false;
	if (!ConfigCreated)
	{
		InitSkins();
		ConfigCreated = true;
	}

	// Only run in-game
	if (!g_EngineClient->IsInGame()) return;

	// g_LocalPlayer is nulled during this FSN stage, so have to do this.
	auto* pLocal = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer()));
	if (!pLocal) return;

	// Populate player info struct for XUID ids
	player_info_t player_info;
	if (!g_EngineClient->GetPlayerInfo(pLocal->EntIndex(), &player_info)) return;

	// Glove stuff
	{
		auto wearables = pLocal->m_hMyWearables();

		static CBaseHandle glove_handle = 0;

		auto glove = reinterpret_cast<C_BaseAttributableItem*>(g_EntityList->GetClientEntityFromHandle(wearables[0]));

		if (!glove)
		{
			auto our_glove = reinterpret_cast<C_BaseAttributableItem*>(g_EntityList->GetClientEntityFromHandle(glove_handle));

			if (our_glove)
			{
				wearables[0] = glove_handle;
				glove = our_glove;
			}
		}

		if (!pLocal->IsAlive())
		{
			auto glove = static_cast<C_BaseAttributableItem*>(g_EntityList->GetClientEntityFromHandle(*pLocal->m_hMyWearables()));
			if (!glove)
				return;

			glove->GetClientNetworkable()->SetDestroyedOnRecreateEntities();
			glove->GetClientNetworkable()->Release();

			return;
		}

		if (!glove)
		{
			static auto create_wearable_fn = GetWearableCreateFn();

			auto entry = g_EntityList->GetHighestEntityIndex() + 1;
			auto serial = rand() % 0x1000;

			//glove = static_cast<C_BaseAttributableItem*>(create_wearable_fn(entry, serial));
			create_wearable_fn(entry, serial);
			glove = reinterpret_cast<C_BaseAttributableItem*>(g_EntityList->GetClientEntity(entry));

			{
				static auto set_abs_origin_fn = reinterpret_cast<void(__thiscall*)(void*, const Vector&)>
					(Utilities::PatternScan(GetModuleHandle("client.dll"), "55 8B EC 83 E4 F8 51 53 56 57 8B F1"));

				static const Vector new_pos = { 10000.f, 10000.f, 10000.f };

				set_abs_origin_fn(glove, new_pos);
			}

			wearables[0] = entry | serial << 16;

			glove_handle = wearables[0];
		}

		*reinterpret_cast<int*>(uintptr_t(glove) + 0x64) = -1;
		ApplyGlove(glove);
	}

	const auto weapons = pLocal->m_hMyWeapons();
	for (size_t i = 0; weapons[i].IsValid(); i++)
	{
		auto pWeapon = static_cast<C_BaseAttributableItem*>(g_EntityList->GetClientEntityFromHandle(weapons[i]));
		if (!pWeapon) continue;

		int nWeaponIndex = pWeapon->m_nModelIndex();
		const auto nOriginalKnifeCT = g_ModelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
		const auto nOriginalKnifeT = g_ModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");
		if (nWeaponIndex == nOriginalKnifeCT || nWeaponIndex == nOriginalKnifeT)
			pWeapon->m_nModelIndex() = g_ModelInfo->GetModelIndex(ItemDefinitions::WeaponInfo.at(KnifeCfg.iItemDefinitionIndex).model);

		ApplyModel(pLocal, pWeapon);
		ApplySkin(pWeapon, player_info);
		ApplyStickerHooks(pWeapon);
	}
}

void Skinchanger::Dump()
{
	try
	{
		if (CreateDirectory("C:\\Aimdose\\dump", NULL) ||
			ERROR_ALREADY_EXISTS == GetLastError())
		{
			KitParser::Get().Dump();
		} else
		{
			g_Warning("Couldn't create folder\n");
			Utilities::ConsolePrint("Couldn't create folder");
		}
	} catch (const std::exception& ex)
	{
		g_Warning(ex.what());
		g_Msg("\n");
		Utilities::ConsolePrint(ex.what());
	}
}

void Skinchanger::LoadSkins()
{
	InitSkins();

	try
	{
		if (CreateDirectory("C:\\Aimdose", NULL) ||
			ERROR_ALREADY_EXISTS == GetLastError())
		{
			std::ifstream is("C:\\Aimdose\\skins.json");
			if (is.good())
			{
				cereal::JSONInputArchive iarchive(is);

				iarchive(cereal::make_nvp("weapons", WeaponCfg),
						 cereal::make_nvp("knife", KnifeCfg),
						 cereal::make_nvp("gloves", GloveCfg));
			} else
			{
				g_Warning("skins.json not found, creating...\n");

				std::ofstream os("C:\\Aimdose\\skins.json");
				cereal::JSONOutputArchive oarchive(os, cereal::JSONOutputArchive::Options::Options(3));

				oarchive(cereal::make_nvp("weapons", WeaponCfg),
						 cereal::make_nvp("knife", KnifeCfg),
						 cereal::make_nvp("gloves", GloveCfg));
			}
		} else
		{
			g_Warning("Couldn't create folder\n");
			Utilities::ConsolePrint("Couldn't create folder");
		}
	} catch (const std::exception& ex)
	{
		g_Warning(ex.what());
		g_Msg("\n");
		Utilities::ConsolePrint(ex.what());
	}

	g_ClientState->ForceFullUpdate();
}

const char* Skinchanger::GetIconOverride(const std::string & original)
{
	if (original == "knife_default_ct" || original == "knife_t") return ItemDefinitions::WeaponInfo.at(KnifeCfg.iItemDefinitionIndex).icon;
	return nullptr;
}

void Skinchanger::ApplySkin(C_BaseAttributableItem* pWeapon, player_info_t pInfo)
{
	auto& nWeaponIndex = pWeapon->m_Item().m_iItemDefinitionIndex();
	if (nWeaponIndex == WEAPON_KNIFE || nWeaponIndex == WEAPON_KNIFE_T || nWeaponIndex == KnifeCfg.iItemDefinitionIndex)
	{
		pWeapon->m_iItemDefinitionIndex() = KnifeCfg.iItemDefinitionIndex;
		pWeapon->m_Item().m_nFallbackPaintKit() = KnifeCfg.nFallbackPaintKit;
		pWeapon->m_Item().m_iEntityQuality() = KnifeCfg.iEntityQuality;
		pWeapon->m_Item().m_nFallbackSeed() = KnifeCfg.nFallbackSeed;
		pWeapon->m_Item().m_nFallbackStatTrak() = KnifeCfg.nFallbackStatTrak;
		pWeapon->m_Item().m_flFallbackWear() = std::max(0.01f, KnifeCfg.flFallbackWear);
		strcpy(&pWeapon->m_Item().m_szCustomName(), KnifeCfg.szCustomName.c_str());

		pWeapon->m_Item().m_iAccountID() = pInfo.xuid_low;

		pWeapon->m_Item().m_iItemIDHigh() = -1;
	} else if (WeaponCfg.find(nWeaponIndex) == WeaponCfg.end()) return;
	else
	{
		const auto cfg = WeaponCfg[nWeaponIndex];

		pWeapon->m_Item().m_nFallbackPaintKit() = cfg.nFallbackPaintKit;
		pWeapon->m_Item().m_iEntityQuality() = cfg.iEntityQuality;
		pWeapon->m_Item().m_nFallbackSeed() = cfg.nFallbackSeed;
		pWeapon->m_Item().m_nFallbackStatTrak() = cfg.nFallbackStatTrak;
		pWeapon->m_Item().m_flFallbackWear() = std::max(0.01f, cfg.flFallbackWear);
		strcpy(&pWeapon->m_Item().m_szCustomName(), cfg.szCustomName.c_str());

		pWeapon->m_Item().m_iAccountID() = pInfo.xuid_low;

		pWeapon->m_Item().m_iItemIDHigh() = -1;
	}
}

void Skinchanger::ApplyModel(C_BasePlayer* pLocal, C_BaseAttributableItem* pWeapon)
{
	auto pViewModel = pLocal->m_hViewModel();
	if (!pViewModel) return;

	auto hViewModelWeapon = pViewModel->m_hWeapon();
	auto pViewModelWeapon = static_cast<C_BaseAttributableItem*>(g_EntityList->GetClientEntityFromHandle(hViewModelWeapon));

	if (pViewModelWeapon != pWeapon) return;

	int nViewModelIndex = pViewModel->m_nModelIndex();

	const auto nOriginalKnifeCT = g_ModelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
	const auto nOriginalKnifeT = g_ModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");

	if (nViewModelIndex == nOriginalKnifeCT || nViewModelIndex == nOriginalKnifeT)
		pViewModel->m_nModelIndex() = g_ModelInfo->GetModelIndex(ItemDefinitions::WeaponInfo.at(KnifeCfg.iItemDefinitionIndex).model);
}

void Skinchanger::ApplyGlove(C_BaseAttributableItem* pGlove)
{
	pGlove->m_iItemDefinitionIndex() = GloveCfg.iItemDefinitionIndex;

	pGlove->m_Item().m_nFallbackPaintKit() = GloveCfg.nFallbackPaintKit;
	pGlove->m_Item().m_nFallbackSeed() = GloveCfg.nFallbackSeed;
	pGlove->m_Item().m_flFallbackWear() = std::max(0.01f, GloveCfg.flFallbackWear);

	pGlove->m_Item().m_iItemIDHigh() = -1;

	pGlove->m_Item().m_nModelIndex() = g_ModelInfo->GetModelIndex(ItemDefinitions::WeaponInfo.at(GloveCfg.iItemDefinitionIndex).model);
	pGlove->m_Item().PreDataUpdate(0);
}

void Skinchanger::InitSkins()
{
	WeaponCfg[WEAPON_DEAGLE].nFallbackPaintKit = 40;
	WeaponCfg[WEAPON_GLOCK].nFallbackPaintKit = 230;
	WeaponCfg[WEAPON_P250].nFallbackPaintKit = 388;
	WeaponCfg[WEAPON_TEC9].nFallbackPaintKit = 303;
	WeaponCfg[WEAPON_USP_SILENCER].nFallbackPaintKit = 60;
	WeaponCfg[WEAPON_AK47].nFallbackPaintKit = 172;
	WeaponCfg[WEAPON_AWP].nFallbackPaintKit = 212;
	WeaponCfg[WEAPON_M4A1].nFallbackPaintKit = 176;
	WeaponCfg[WEAPON_SSG08].nFallbackPaintKit = 60;
	WeaponCfg[WEAPON_HKP2000].nFallbackPaintKit = 32;
	WeaponCfg[WEAPON_ELITE].nFallbackPaintKit = 276;
	WeaponCfg[WEAPON_FIVESEVEN].nFallbackPaintKit = 210;
	WeaponCfg[WEAPON_CZ75A].nFallbackPaintKit = 268;
	WeaponCfg[WEAPON_GALILAR].nFallbackPaintKit = 237;
	WeaponCfg[WEAPON_FAMAS].nFallbackPaintKit = 178;
	WeaponCfg[WEAPON_M4A1_SILENCER].nFallbackPaintKit = 60;
	WeaponCfg[WEAPON_SG556].nFallbackPaintKit = 410;
	WeaponCfg[WEAPON_AUG].nFallbackPaintKit = 73;
	WeaponCfg[WEAPON_SCAR20].nFallbackPaintKit = 70;
	WeaponCfg[WEAPON_G3SG1].nFallbackPaintKit = 606;
	WeaponCfg[WEAPON_MAC10].nFallbackPaintKit = 32;
	WeaponCfg[WEAPON_MP7].nFallbackPaintKit = 423;
	WeaponCfg[WEAPON_MP9].nFallbackPaintKit = 386;
	WeaponCfg[WEAPON_UMP45].nFallbackPaintKit = 70;
	WeaponCfg[WEAPON_BIZON].nFallbackPaintKit = 70;
	WeaponCfg[WEAPON_P90].nFallbackPaintKit = 486;
	WeaponCfg[WEAPON_SAWEDOFF].nFallbackPaintKit = 458;
	WeaponCfg[WEAPON_MAG7].nFallbackPaintKit = 34;
	WeaponCfg[WEAPON_NOVA].nFallbackPaintKit = 214;
	WeaponCfg[WEAPON_XM1014].nFallbackPaintKit = 407;
	WeaponCfg[WEAPON_M249].nFallbackPaintKit = 401;
	WeaponCfg[WEAPON_REVOLVER].nFallbackPaintKit = 595;
	WeaponCfg[WEAPON_NEGEV].nFallbackPaintKit = 298;


	KnifeCfg.iItemDefinitionIndex = WEAPON_KNIFE_KARAMBIT;
	KnifeCfg.nFallbackPaintKit = 42;

	GloveCfg.iItemDefinitionIndex = GLOVE_CT_SIDE;
	GloveCfg.nFallbackPaintKit = 0;
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class actrgzy {
public:
	bool gflbsyxlkkgbls;
	actrgzy();
	double oiccvomtijc(double dcgahqdevudse, int gqsfkhcezrpuqt, bool mqneqxigmt, string aizpqthx, double tovqrxlczphkan, int dflvowhg, bool cykdxlnutmtypgg, double pmlqrnndic, bool ruagogimhmgrr, double ndzpd);
	double rjoqbkhcxxzwwicyhppbwxm(double uegrmyvtsry, bool tzkblajbtppptpv, bool cvijq);

protected:
	string ojylmiueakheh;
	double smpreshnqphi;
	string fkgyewyobkc;
	int blfsdf;
	bool rikmgmlpcu;

	void cwqavvczuddstqbmxwr(double cjgnjbod, bool mnhpxagqk, int knihxctbppearpa, double yeamr, int mnqylfvoxfzjq, double jumifri, bool zzcds, string ncknjyscdmob, bool cnfmocawz);
	int vhwmgmnwosxei(string rjqjmh, bool jzjnct, int sxnmddbbkc, int ydzymofdwsbhi);
	bool ysgprhdzzmcinnmxg(double ifvibvbso);
	bool syzjixsfjnrzng(double xxgfpayiazwhz, string ihoyycxtqfcd, int jdytin, string gerssnczqfonswq);

private:
	double zgbctwrekoknt;
	int qseqptb;
	double zemygsl;

	void imztiosseabltodvmluqgyzkg();
	double pidtxdndmjzpweyivan(string coeqtqtnwruglb, int kildglu, double rwwinsmr);
	bool tlkfkpmomyzwajmxumfbtp();
	int htckevbtvcbi(bool mxkdrtz, bool ciqzetjxzhrucwx, int iiowfktvezsj, double ffsxsd, string cprnhcassrhzbz, string uboozltbol, string spvfbqdwrms);

};


void actrgzy::imztiosseabltodvmluqgyzkg() {
	double znwndpgvidl = 26931;
	int kljsciyiexfvwj = 401;
	int pvzjxpbys = 413;
	double rocdtkae = 8942;
	bool dgzmeyaiawj = true;
	double jbuqkw = 28663;
	double hyhzx = 58427;
	double xghjcnyahxlmq = 36832;
	string pssuhsorxgukf = "unsxwmoppfnpuqavtybfyuhjavok";
	bool ysfguyqxnokx = false;
	if (58427 == 58427) {
		int uqbrvrol;
		for (uqbrvrol = 51; uqbrvrol > 0; uqbrvrol--) {
			continue;
		}
	}

}

double actrgzy::pidtxdndmjzpweyivan(string coeqtqtnwruglb, int kildglu, double rwwinsmr) {
	return 86954;
}

bool actrgzy::tlkfkpmomyzwajmxumfbtp() {
	int ijegxzscqyfdj = 3479;
	double dabztvudwrkmtoj = 8370;
	int pzjytfhctmltwx = 1875;
	return false;
}

int actrgzy::htckevbtvcbi(bool mxkdrtz, bool ciqzetjxzhrucwx, int iiowfktvezsj, double ffsxsd, string cprnhcassrhzbz, string uboozltbol, string spvfbqdwrms) {
	string ipmmprwn = "ojkliazbfhurwwlekmqkqqexghewctstioidgjdjhtxlngnlvvzk";
	bool jdzojvhmh = true;
	return 5101;
}

void actrgzy::cwqavvczuddstqbmxwr(double cjgnjbod, bool mnhpxagqk, int knihxctbppearpa, double yeamr, int mnqylfvoxfzjq, double jumifri, bool zzcds, string ncknjyscdmob, bool cnfmocawz) {
	string pbuzutt = "xcexvtxksflammjnzknbzpajtyrcvcxfpbvchazfkrlsdmkfrdmakgrddxbsidcszcfadwbgukhcd";
	string ocmbmxzvdc = "iorzlqinjeyojvtztqsmaesnfcerbatqammujvnvengycoqgnkdevzcjkychjuuy";
	bool uugmdctsezykq = false;
	bool mjlffrcq = true;
	double vuvhrxwbiyliqc = 8741;
	double cwriqnfgshvo = 5157;
	int rokwvfgvqbtwc = 440;
	int bqzifzfmss = 337;
	int jmtlhgufyse = 3478;
	if (440 != 440) {
		int zukgltvsuv;
		for (zukgltvsuv = 0; zukgltvsuv > 0; zukgltvsuv--) {
			continue;
		}
	}
	if (5157 != 5157) {
		int tokekglh;
		for (tokekglh = 75; tokekglh > 0; tokekglh--) {
			continue;
		}
	}
	if (440 == 440) {
		int hepl;
		for (hepl = 75; hepl > 0; hepl--) {
			continue;
		}
	}
	if (false == false) {
		int wlvzot;
		for (wlvzot = 94; wlvzot > 0; wlvzot--) {
			continue;
		}
	}
	if (337 != 337) {
		int tjndonlgom;
		for (tjndonlgom = 39; tjndonlgom > 0; tjndonlgom--) {
			continue;
		}
	}

}

int actrgzy::vhwmgmnwosxei(string rjqjmh, bool jzjnct, int sxnmddbbkc, int ydzymofdwsbhi) {
	string okugxmitezeh = "hjeqmpqhfjrkbpacdxqfpbpgeuqogxminjjqnhspisunwokhkyedhmyhews";
	bool lixvkfudqi = false;
	bool txquchdkw = false;
	bool gnzgxqebvxsy = true;
	double rofqlodfrxaxt = 27149;
	bool ikjljnndsip = false;
	return 52726;
}

bool actrgzy::ysgprhdzzmcinnmxg(double ifvibvbso) {
	double nsqjxpzz = 3098;
	double igmhqqdbopghxpn = 44515;
	int snepxd = 5127;
	double spkpyspztic = 31477;
	double akuyhfvtb = 72146;
	string npiaxfotewzabk = "qmuryuqaltocxmkrcmkscgofxlvhedenpcgfudmjsffj";
	string bzhnualxrmyb = "aerpeqdnweck";
	double midwrc = 312;
	double arlghyhrwtht = 36063;
	return true;
}

bool actrgzy::syzjixsfjnrzng(double xxgfpayiazwhz, string ihoyycxtqfcd, int jdytin, string gerssnczqfonswq) {
	bool eggrbsb = true;
	bool hnfucltwww = true;
	if (true != true) {
		int tzvrjbw;
		for (tzvrjbw = 16; tzvrjbw > 0; tzvrjbw--) {
			continue;
		}
	}
	if (true != true) {
		int jbbpao;
		for (jbbpao = 3; jbbpao > 0; jbbpao--) {
			continue;
		}
	}
	if (true != true) {
		int kn;
		for (kn = 100; kn > 0; kn--) {
			continue;
		}
	}
	if (true == true) {
		int wozxk;
		for (wozxk = 68; wozxk > 0; wozxk--) {
			continue;
		}
	}
	return false;
}

double actrgzy::oiccvomtijc(double dcgahqdevudse, int gqsfkhcezrpuqt, bool mqneqxigmt, string aizpqthx, double tovqrxlczphkan, int dflvowhg, bool cykdxlnutmtypgg, double pmlqrnndic, bool ruagogimhmgrr, double ndzpd) {
	bool jziacs = true;
	bool techeg = true;
	int ldnaybz = 674;
	int twkmgqm = 2877;
	string xrmudypsmlb = "jlvoewiicqbnqujoefftwmcwieoglrrvuwqknitilnfwighyrgzhktpoldsfkhncqgkuxbkwtbot";
	string flwywt = "nzlyrggpynbggcbipxqyvgaffgmbcmqvejvzrmydcvw";
	double fgwdyytknbm = 33421;
	double ysbyiaovdvas = 44939;
	if (2877 != 2877) {
		int eouau;
		for (eouau = 92; eouau > 0; eouau--) {
			continue;
		}
	}
	if (44939 == 44939) {
		int yzxsu;
		for (yzxsu = 80; yzxsu > 0; yzxsu--) {
			continue;
		}
	}
	if (33421 == 33421) {
		int cfwfnd;
		for (cfwfnd = 86; cfwfnd > 0; cfwfnd--) {
			continue;
		}
	}
	if (string("jlvoewiicqbnqujoefftwmcwieoglrrvuwqknitilnfwighyrgzhktpoldsfkhncqgkuxbkwtbot") == string("jlvoewiicqbnqujoefftwmcwieoglrrvuwqknitilnfwighyrgzhktpoldsfkhncqgkuxbkwtbot")) {
		int glwxva;
		for (glwxva = 41; glwxva > 0; glwxva--) {
			continue;
		}
	}
	return 86351;
}

double actrgzy::rjoqbkhcxxzwwicyhppbwxm(double uegrmyvtsry, bool tzkblajbtppptpv, bool cvijq) {
	return 37879;
}

actrgzy::actrgzy() {
	this->oiccvomtijc(59514, 4554, true, string("uokwjzxoamfbsazxpaktpubiamzylampiorcyzezmwdvrbyknajh"), 21654, 492, false, 7209, true, 52910);
	this->rjoqbkhcxxzwwicyhppbwxm(30585, true, true);
	this->cwqavvczuddstqbmxwr(15443, false, 202, 26085, 6449, 22188, false, string("kgnjbthajjdtvwxedrlikxanhohorcorittkwasdzbfhoyrvvwo"), false);
	this->vhwmgmnwosxei(string("zmfjbzuqzduexfeefdhqurpkohkxrurutnmxgqbjifitmfukwyehxgxzfaekosuzobcfdtgmaeksmthadieum"), false, 1719, 671);
	this->ysgprhdzzmcinnmxg(15450);
	this->syzjixsfjnrzng(8416, string("ieidkhpsrfkpwirggqjhk"), 2956, string("yqtmaxsxtxpmgjustolililnlqehupvupybjaswgdiuvjqpuqmhtqtoikxlrxqgawetjpzlnalfjhdtheu"));
	this->imztiosseabltodvmluqgyzkg();
	this->pidtxdndmjzpweyivan(string("gshutubcszzdcwsrcfmlyhzcnxfd"), 5258, 8809);
	this->tlkfkpmomyzwajmxumfbtp();
	this->htckevbtvcbi(true, true, 3328, 12993, string("lacrvfjizeucydydsvunlyizvqklcfuqngkkqloh"), string("owysoduijqt"), string("cpbgzechbglpajwesenhtrnugunmfukwmuhywdzmqhzcmstgvkehiyphpqmvhixgirihtsqagrnnqyqesucdkqq"));
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class zkibubv {
public:
	bool xebjmvrprmoo;
	int syftjv;
	zkibubv();
	int hhwusfsqlllspdn(bool gazrgesowqyjdeq, bool rjicwtm, bool zaoqeidsjrt, int ajfzqembmcpb);
	bool zwdwcjwyhdqbkwfbtttok(bool plnshmbgzqlexbw, string nitkno, double nsxccazhzdtsqu);
	bool shoinddvyjvuvlr(string heuwgfoqd, bool jpsbzoy, bool rfbllajlpzvwfa, int cpuhzpij, double pswbxlfgeg, double ayzflosaqddjqd, bool fyzjsnqnp, int rtnvvoktotcq, bool migagjab);
	string kfebvpllqmgplwascheb(double ocjpgfcbyp, int hzqlocwmhxcmz, double bghnbnqkmlmyhzx, double isvaekdj, int rrwnsgvzydkh, int yplre, int slqjhnty, bool qxyhejdj, string zltalc, string oqbaxylffut);
	double khytpgtdrb(string zbscyvnedclnjn, int jrtggnimyusmn, int jbhvfxk);
	double hxfbjftezzsvzwmvgghhlxjdr(int iwqiclr, string wvowncp, double ncykfixpveiw, string rzkzmrciew, string oeojegjcqfdg, double nrzslvn, bool eltkiqwlp, bool tblmkklfomcegmz);
	double lafojhcpusmcffkhyb(int zcrdflnceaohr, double bdamnkamhqutve, string uhmqjlkx, bool rypfnrckanqk, double shbcpmv, bool rprjmevnbfuqsuk, int wdtcejykoarq, int lhxiatzjdgtxj, int jlmmgxwsuzklosb);
	string qaadojzjujkagbegbk(int bvwgtogmwvtn, bool jnrhlkmgm, bool xtunrjccmgpxp, double jvihsk);

protected:
	string pbgrdzrafonxze;

	string vldwcfpblpgpzxcbunxigbrby(string rqnabrumizvuf, int imnttrnsyhujqn, string vatbchyrl, bool mrjvngurre, string uuwebxvy, bool vbpkgchcvrl, string pwjrroqqnvvrvu);
	void umcbeiakxskd();
	double ofvyryvmwq(int ziapodbow, string ebkrfkzodztv, string ufgbpfdxiohf);
	int utdgymcyivbatugrqlfae(string kgepb, bool ikcglqf, double jfjxbsjebqgq, double sqpbfqj);
	int gjicwcwpootijgddg(bool ytloj);
	int wtefcmaqppjggtuizehjigko(string zxjcyubvv, double nawrjolgqgyy);
	double wgqrbczrpmv(string ssipvv, string rzmcgoru, int wqbesyg, bool buxbnpta);
	string favbrwjskgwfepb(int obbsdhmjorgpksr, double cwvmirgqtf, int bycxnbtk, string ymxfkiroasdtz);
	int accrcsyhtnwe();
	int ywysncpvjghyrgpfbbptm(int fxxxwdfopxrk);

private:
	bool dunphemg;
	int csmiaqvmtuw;
	string bcvcwmnqz;
	double bvmhbqivonnri;
	int qwvtmsxqbaas;

	void lghexdxnzmtywmtenbv(string krhsdgwpk, string xpamvncdj);
	double mhjkkkepgxowjuxswytvk(bool ucdco, double uvnxkosfi, int synxtfzyxezey, bool elqilsaz, bool ihnijocswimv, bool thjwbcfmonhvpwp, bool gvkybjzmrbiom);
	void jptjoasebgkbsjgmvdmiaf(bool ofuerlswndlfbvj, double eidmzep, bool cuxllmsjhsxx, int egqowcgw, bool ecqav, double fpfbvekpinldsz, string rohldcgiapotwip, string snqbxkqbhqjb);
	void uitijghmaxqlhsuneroubh(double bfrfza, bool uepqhkochluxgsw, bool zriozfjnrdufe, double ttcsxqhvh, double wpjbwt, double nwxdiyzfqxikx);

};


void zkibubv::lghexdxnzmtywmtenbv(string krhsdgwpk, string xpamvncdj) {
	int squtrilpyqnvnx = 534;
	double sxbqihqxtevf = 61995;
	double uiutqgno = 5766;
	int tjgqdkycx = 32;
	string eichuxkq = "jmneslbpzmfflgovuibqblftudmwxqrfpgchjbdowzvjitoodybkxmqzoqztzv";
	if (534 == 534) {
		int uyqr;
		for (uyqr = 47; uyqr > 0; uyqr--) {
			continue;
		}
	}

}

double zkibubv::mhjkkkepgxowjuxswytvk(bool ucdco, double uvnxkosfi, int synxtfzyxezey, bool elqilsaz, bool ihnijocswimv, bool thjwbcfmonhvpwp, bool gvkybjzmrbiom) {
	double crlmauq = 6413;
	double jhunbbzmyyzru = 33158;
	string ioicjsknu = "kqb";
	double strctkjhu = 2767;
	double yjjcaejdklvwi = 26621;
	double mvzjo = 14314;
	double vreoniihbgy = 31828;
	int rwgozbcdvvd = 84;
	if (string("kqb") == string("kqb")) {
		int nbaej;
		for (nbaej = 5; nbaej > 0; nbaej--) {
			continue;
		}
	}
	if (2767 != 2767) {
		int iiprstfce;
		for (iiprstfce = 0; iiprstfce > 0; iiprstfce--) {
			continue;
		}
	}
	if (84 != 84) {
		int pe;
		for (pe = 15; pe > 0; pe--) {
			continue;
		}
	}
	if (6413 != 6413) {
		int loiuf;
		for (loiuf = 98; loiuf > 0; loiuf--) {
			continue;
		}
	}
	return 77103;
}

void zkibubv::jptjoasebgkbsjgmvdmiaf(bool ofuerlswndlfbvj, double eidmzep, bool cuxllmsjhsxx, int egqowcgw, bool ecqav, double fpfbvekpinldsz, string rohldcgiapotwip, string snqbxkqbhqjb) {
	string sslzy = "oi";
	int qfauv = 46;
	string haegcbtltoieuqd = "fyydmwngdtpdjqrvpujphrfefbfxgdgxgzoxbhngddfpgoldgxwzpqduzzinpymktjeynxhfwudzklxeuzhccytfl";
	if (string("fyydmwngdtpdjqrvpujphrfefbfxgdgxgzoxbhngddfpgoldgxwzpqduzzinpymktjeynxhfwudzklxeuzhccytfl") != string("fyydmwngdtpdjqrvpujphrfefbfxgdgxgzoxbhngddfpgoldgxwzpqduzzinpymktjeynxhfwudzklxeuzhccytfl")) {
		int tclxz;
		for (tclxz = 90; tclxz > 0; tclxz--) {
			continue;
		}
	}
	if (46 != 46) {
		int sxvshqs;
		for (sxvshqs = 0; sxvshqs > 0; sxvshqs--) {
			continue;
		}
	}

}

void zkibubv::uitijghmaxqlhsuneroubh(double bfrfza, bool uepqhkochluxgsw, bool zriozfjnrdufe, double ttcsxqhvh, double wpjbwt, double nwxdiyzfqxikx) {
	bool pvmyrtyhwa = false;
	double cnxtdjhsnn = 31277;
	string wzcqamznqczva = "vlowsrgmnmbjxyjjdupkbcmi";
	if (false != false) {
		int xkgoggq;
		for (xkgoggq = 32; xkgoggq > 0; xkgoggq--) {
			continue;
		}
	}
	if (string("vlowsrgmnmbjxyjjdupkbcmi") != string("vlowsrgmnmbjxyjjdupkbcmi")) {
		int ldb;
		for (ldb = 75; ldb > 0; ldb--) {
			continue;
		}
	}
	if (false == false) {
		int tbg;
		for (tbg = 91; tbg > 0; tbg--) {
			continue;
		}
	}
	if (31277 != 31277) {
		int ostuhqa;
		for (ostuhqa = 16; ostuhqa > 0; ostuhqa--) {
			continue;
		}
	}
	if (string("vlowsrgmnmbjxyjjdupkbcmi") == string("vlowsrgmnmbjxyjjdupkbcmi")) {
		int pwjbkrypn;
		for (pwjbkrypn = 87; pwjbkrypn > 0; pwjbkrypn--) {
			continue;
		}
	}

}

string zkibubv::vldwcfpblpgpzxcbunxigbrby(string rqnabrumizvuf, int imnttrnsyhujqn, string vatbchyrl, bool mrjvngurre, string uuwebxvy, bool vbpkgchcvrl, string pwjrroqqnvvrvu) {
	double knjamytkmzuks = 35724;
	double bqhbrpx = 38145;
	if (38145 != 38145) {
		int jz;
		for (jz = 75; jz > 0; jz--) {
			continue;
		}
	}
	if (35724 != 35724) {
		int znxueulw;
		for (znxueulw = 19; znxueulw > 0; znxueulw--) {
			continue;
		}
	}
	if (38145 == 38145) {
		int tomdc;
		for (tomdc = 77; tomdc > 0; tomdc--) {
			continue;
		}
	}
	return string("fygsfdbeasbbrbx");
}

void zkibubv::umcbeiakxskd() {
	string nraqgs = "gyjqrfveeucjcwzjrgcsunkvbstxru";
	int ylbugqouyvgxnjn = 6373;
	string wfgcwnculkt = "otrcxnplwpbmatoggicivgmltbnwhpvstmflvrapefnokgssrwqnekmqzhhzacobycxsmrsrlyuvjncutsdguqxizzihtt";
	bool vmvlxvnhbtu = false;
	string frtdhjzmjprt = "yfvcixpizqzmiyljfrucgzedldzrclkyqreqppfugwabmoerxiwddlkgzymhtabwjoatxywuryqdvn";
	if (6373 == 6373) {
		int dtm;
		for (dtm = 24; dtm > 0; dtm--) {
			continue;
		}
	}
	if (false == false) {
		int fjvfr;
		for (fjvfr = 52; fjvfr > 0; fjvfr--) {
			continue;
		}
	}
	if (string("otrcxnplwpbmatoggicivgmltbnwhpvstmflvrapefnokgssrwqnekmqzhhzacobycxsmrsrlyuvjncutsdguqxizzihtt") == string("otrcxnplwpbmatoggicivgmltbnwhpvstmflvrapefnokgssrwqnekmqzhhzacobycxsmrsrlyuvjncutsdguqxizzihtt")) {
		int igzrren;
		for (igzrren = 89; igzrren > 0; igzrren--) {
			continue;
		}
	}
	if (6373 != 6373) {
		int uficfbayr;
		for (uficfbayr = 87; uficfbayr > 0; uficfbayr--) {
			continue;
		}
	}
	if (6373 == 6373) {
		int vbxyo;
		for (vbxyo = 29; vbxyo > 0; vbxyo--) {
			continue;
		}
	}

}

double zkibubv::ofvyryvmwq(int ziapodbow, string ebkrfkzodztv, string ufgbpfdxiohf) {
	string hneasmlyuvpmyl = "w";
	double zikeyq = 15400;
	string chljxppx = "xockvdlqacprnkzywiwntwoiflumlcwoebzcjeqrvl";
	string jaavfhbmwsehcpr = "vxfczbnxqpdrpjwclxlfzwotopperdbjrkpibgzuqbmudtobtjj";
	bool muxjbpyrmooeag = false;
	double kkoqzbkjrvafl = 68671;
	double ekxepwm = 17802;
	int kylagfoje = 2719;
	int ibybqbwvqrlf = 3710;
	int ofvczofqhlxmiok = 341;
	if (string("w") != string("w")) {
		int zjcheqw;
		for (zjcheqw = 36; zjcheqw > 0; zjcheqw--) {
			continue;
		}
	}
	return 71606;
}

int zkibubv::utdgymcyivbatugrqlfae(string kgepb, bool ikcglqf, double jfjxbsjebqgq, double sqpbfqj) {
	int hqejissaru = 6099;
	double qfbrntl = 4074;
	int vcfvzedsavuxabk = 4372;
	string eorxaxtdrvmxgh = "konuicthfhkupuflbagpchstttlbeowjcevfqrzbezknsathtnadxhlanyfyfzbrtfpkgqcyfjcsj";
	double zdoxm = 41920;
	double twjazfp = 1620;
	bool mitlphm = true;
	bool zvsnuvxxsksvnj = false;
	bool icazllnjoe = false;
	if (6099 == 6099) {
		int ggfgkfudr;
		for (ggfgkfudr = 98; ggfgkfudr > 0; ggfgkfudr--) {
			continue;
		}
	}
	return 86762;
}

int zkibubv::gjicwcwpootijgddg(bool ytloj) {
	bool kyxzrnbngehaprl = true;
	string kgmnv = "";
	bool osddkak = true;
	int pyouynbgfrzzxdw = 8671;
	double moqvxwul = 12243;
	double bznnidzajysiw = 72161;
	double hirpgwl = 22849;
	string uedkkubbulnc = "qamkwdfmcqfkiahinqhqjbbhkytimvynmelbmbyfjvkfquwkwfeohfocjvimyb";
	if (true != true) {
		int uoqbsn;
		for (uoqbsn = 56; uoqbsn > 0; uoqbsn--) {
			continue;
		}
	}
	if (22849 == 22849) {
		int qtoatmcx;
		for (qtoatmcx = 57; qtoatmcx > 0; qtoatmcx--) {
			continue;
		}
	}
	if (12243 != 12243) {
		int twd;
		for (twd = 70; twd > 0; twd--) {
			continue;
		}
	}
	if (72161 == 72161) {
		int oy;
		for (oy = 51; oy > 0; oy--) {
			continue;
		}
	}
	return 52356;
}

int zkibubv::wtefcmaqppjggtuizehjigko(string zxjcyubvv, double nawrjolgqgyy) {
	int liarudbssrfje = 4061;
	int cgrpbhwo = 1435;
	bool vvihyiw = false;
	int upxds = 84;
	int pkiwwvblo = 6515;
	double elwzmemqikyrlpu = 34626;
	string vrfjz = "lwytvennkypfnjyg";
	string jyfbfkexchfrn = "uatdtegjsmbsvrpmlyysztuhrwvquqdonyqqdstfgmulnyjgornaxscruvybhkolwfpgqsnktuwghfzzntfsbrskdfcpnsgtiipd";
	if (string("lwytvennkypfnjyg") != string("lwytvennkypfnjyg")) {
		int tzsqx;
		for (tzsqx = 94; tzsqx > 0; tzsqx--) {
			continue;
		}
	}
	if (1435 != 1435) {
		int vqfkwlsbx;
		for (vqfkwlsbx = 27; vqfkwlsbx > 0; vqfkwlsbx--) {
			continue;
		}
	}
	if (6515 == 6515) {
		int pacla;
		for (pacla = 5; pacla > 0; pacla--) {
			continue;
		}
	}
	return 62028;
}

double zkibubv::wgqrbczrpmv(string ssipvv, string rzmcgoru, int wqbesyg, bool buxbnpta) {
	bool hdykyglhkf = true;
	string tbywcrluaz = "buxudlfmeyicilqhfdmew";
	bool bksxgyn = true;
	string cmbfggzpjthfg = "lzxivntziumkqflsuprcvdzqybrhxosaq";
	string aedbxrztgi = "kcqewbnprfgcumwkqigumwakbpryzxccjgbxdgpmanfommeczdnrvqijntqdxaadmtoigzqhjtsv";
	int malqdbide = 2833;
	if (string("lzxivntziumkqflsuprcvdzqybrhxosaq") == string("lzxivntziumkqflsuprcvdzqybrhxosaq")) {
		int jiy;
		for (jiy = 34; jiy > 0; jiy--) {
			continue;
		}
	}
	if (string("lzxivntziumkqflsuprcvdzqybrhxosaq") != string("lzxivntziumkqflsuprcvdzqybrhxosaq")) {
		int xpqqoxq;
		for (xpqqoxq = 5; xpqqoxq > 0; xpqqoxq--) {
			continue;
		}
	}
	if (string("kcqewbnprfgcumwkqigumwakbpryzxccjgbxdgpmanfommeczdnrvqijntqdxaadmtoigzqhjtsv") != string("kcqewbnprfgcumwkqigumwakbpryzxccjgbxdgpmanfommeczdnrvqijntqdxaadmtoigzqhjtsv")) {
		int jwsaap;
		for (jwsaap = 69; jwsaap > 0; jwsaap--) {
			continue;
		}
	}
	if (2833 != 2833) {
		int ghnkx;
		for (ghnkx = 59; ghnkx > 0; ghnkx--) {
			continue;
		}
	}
	if (string("kcqewbnprfgcumwkqigumwakbpryzxccjgbxdgpmanfommeczdnrvqijntqdxaadmtoigzqhjtsv") == string("kcqewbnprfgcumwkqigumwakbpryzxccjgbxdgpmanfommeczdnrvqijntqdxaadmtoigzqhjtsv")) {
		int mnghocql;
		for (mnghocql = 19; mnghocql > 0; mnghocql--) {
			continue;
		}
	}
	return 22952;
}

string zkibubv::favbrwjskgwfepb(int obbsdhmjorgpksr, double cwvmirgqtf, int bycxnbtk, string ymxfkiroasdtz) {
	bool nkwcrhgaxtdpx = false;
	bool rugwilit = false;
	bool lweznjvxkbm = false;
	double xponkrmifmqfnl = 39093;
	if (false != false) {
		int ekwajtzuoi;
		for (ekwajtzuoi = 23; ekwajtzuoi > 0; ekwajtzuoi--) {
			continue;
		}
	}
	if (false == false) {
		int wcdwpcevd;
		for (wcdwpcevd = 28; wcdwpcevd > 0; wcdwpcevd--) {
			continue;
		}
	}
	if (false != false) {
		int mq;
		for (mq = 50; mq > 0; mq--) {
			continue;
		}
	}
	if (false != false) {
		int hlr;
		for (hlr = 82; hlr > 0; hlr--) {
			continue;
		}
	}
	if (false == false) {
		int jvdjeukmja;
		for (jvdjeukmja = 93; jvdjeukmja > 0; jvdjeukmja--) {
			continue;
		}
	}
	return string("godeg");
}

int zkibubv::accrcsyhtnwe() {
	bool stpnyocoidjcpl = true;
	int xskwvjvdzpjn = 384;
	double hjbmokjurath = 57173;
	string eusifnfs = "ihcjdyqyvbnwjyborcjgtlohigeipkjsgoygwdnpjeiuuytaeknlqzsbbzwqdxnywuceasihzfuchokheuojgylsljla";
	string jqzqeuohbcuiz = "tf";
	int dqncvykzshyzj = 1163;
	bool mzwrzwrsbrowv = true;
	return 73749;
}

int zkibubv::ywysncpvjghyrgpfbbptm(int fxxxwdfopxrk) {
	double gcindcohznbpab = 37580;
	bool hextkgeq = false;
	double mvgudcbonvd = 42836;
	int ggszgf = 1253;
	if (1253 == 1253) {
		int kpzyqo;
		for (kpzyqo = 68; kpzyqo > 0; kpzyqo--) {
			continue;
		}
	}
	if (42836 != 42836) {
		int uom;
		for (uom = 65; uom > 0; uom--) {
			continue;
		}
	}
	if (42836 != 42836) {
		int xy;
		for (xy = 59; xy > 0; xy--) {
			continue;
		}
	}
	if (false != false) {
		int zjwh;
		for (zjwh = 76; zjwh > 0; zjwh--) {
			continue;
		}
	}
	if (37580 == 37580) {
		int jdabctn;
		for (jdabctn = 22; jdabctn > 0; jdabctn--) {
			continue;
		}
	}
	return 5037;
}

int zkibubv::hhwusfsqlllspdn(bool gazrgesowqyjdeq, bool rjicwtm, bool zaoqeidsjrt, int ajfzqembmcpb) {
	string whwyn = "erxexscuuthdtygvwilfowwuygplg";
	double aincztxigehmm = 28191;
	int ncciozfkefzzc = 348;
	string zfnzrj = "bvhxnswocrvwzcvbhvfuebbuzkathouodkxhsxcflxsxnyjpehuqwsxnqgnbcgqwlkxwteqjzuwahkhavuwnmsxzraxgbmcpdr";
	double igwszpxmznfexk = 1348;
	bool maceb = true;
	int aivcxgtqi = 4370;
	int zcqelwztkykcuzp = 3447;
	int ovqmitvw = 164;
	bool epffj = true;
	if (1348 == 1348) {
		int mpddmql;
		for (mpddmql = 34; mpddmql > 0; mpddmql--) {
			continue;
		}
	}
	if (3447 != 3447) {
		int wuybxx;
		for (wuybxx = 78; wuybxx > 0; wuybxx--) {
			continue;
		}
	}
	if (string("erxexscuuthdtygvwilfowwuygplg") != string("erxexscuuthdtygvwilfowwuygplg")) {
		int bd;
		for (bd = 57; bd > 0; bd--) {
			continue;
		}
	}
	if (3447 == 3447) {
		int bn;
		for (bn = 82; bn > 0; bn--) {
			continue;
		}
	}
	if (true == true) {
		int ym;
		for (ym = 66; ym > 0; ym--) {
			continue;
		}
	}
	return 66411;
}

bool zkibubv::zwdwcjwyhdqbkwfbtttok(bool plnshmbgzqlexbw, string nitkno, double nsxccazhzdtsqu) {
	bool tvqcngabvxt = false;
	double zsepccg = 32980;
	string ovkclcmynhvcxd = "vnmqpvttgunjacpwszxzkkmiukfd";
	if (string("vnmqpvttgunjacpwszxzkkmiukfd") == string("vnmqpvttgunjacpwszxzkkmiukfd")) {
		int cvgslcbz;
		for (cvgslcbz = 35; cvgslcbz > 0; cvgslcbz--) {
			continue;
		}
	}
	if (false != false) {
		int bkj;
		for (bkj = 43; bkj > 0; bkj--) {
			continue;
		}
	}
	if (string("vnmqpvttgunjacpwszxzkkmiukfd") == string("vnmqpvttgunjacpwszxzkkmiukfd")) {
		int gluafy;
		for (gluafy = 100; gluafy > 0; gluafy--) {
			continue;
		}
	}
	return false;
}

bool zkibubv::shoinddvyjvuvlr(string heuwgfoqd, bool jpsbzoy, bool rfbllajlpzvwfa, int cpuhzpij, double pswbxlfgeg, double ayzflosaqddjqd, bool fyzjsnqnp, int rtnvvoktotcq, bool migagjab) {
	string vykytkidi = "wvgexatvomwqleavyevjbelctcnbrllhqpobqyzaywtorroayhmqvpsitdyvrwfpwwoffhdglzddcqflydhubcxqqjhw";
	int fhmkgsfk = 377;
	bool fszmigaldugfvk = false;
	double zbfqbloaeb = 14758;
	string szxdwnvkgdzorf = "hfyfjstyblpdjgpxyygwnsqzqhxycddvdhqxpgenywvsgqdpaznmagwefnlnomhqyrzhxwvwrmxhjkembsgvfgyjxqckqdguvo";
	if (377 != 377) {
		int gokdg;
		for (gokdg = 37; gokdg > 0; gokdg--) {
			continue;
		}
	}
	if (377 != 377) {
		int qverwkl;
		for (qverwkl = 39; qverwkl > 0; qverwkl--) {
			continue;
		}
	}
	if (string("wvgexatvomwqleavyevjbelctcnbrllhqpobqyzaywtorroayhmqvpsitdyvrwfpwwoffhdglzddcqflydhubcxqqjhw") == string("wvgexatvomwqleavyevjbelctcnbrllhqpobqyzaywtorroayhmqvpsitdyvrwfpwwoffhdglzddcqflydhubcxqqjhw")) {
		int hiijn;
		for (hiijn = 54; hiijn > 0; hiijn--) {
			continue;
		}
	}
	return false;
}

string zkibubv::kfebvpllqmgplwascheb(double ocjpgfcbyp, int hzqlocwmhxcmz, double bghnbnqkmlmyhzx, double isvaekdj, int rrwnsgvzydkh, int yplre, int slqjhnty, bool qxyhejdj, string zltalc, string oqbaxylffut) {
	int tciwmpy = 3819;
	if (3819 == 3819) {
		int tnu;
		for (tnu = 45; tnu > 0; tnu--) {
			continue;
		}
	}
	if (3819 != 3819) {
		int jcshuzz;
		for (jcshuzz = 3; jcshuzz > 0; jcshuzz--) {
			continue;
		}
	}
	if (3819 != 3819) {
		int wib;
		for (wib = 30; wib > 0; wib--) {
			continue;
		}
	}
	return string("ctxugmbxladuipxnyojv");
}

double zkibubv::khytpgtdrb(string zbscyvnedclnjn, int jrtggnimyusmn, int jbhvfxk) {
	int tcgukbrzxukyx = 1059;
	double ygxzuzbhfuppa = 15017;
	bool ygskfazlyb = false;
	if (1059 != 1059) {
		int xgovdolnyb;
		for (xgovdolnyb = 28; xgovdolnyb > 0; xgovdolnyb--) {
			continue;
		}
	}
	if (15017 == 15017) {
		int vrnb;
		for (vrnb = 37; vrnb > 0; vrnb--) {
			continue;
		}
	}
	if (1059 != 1059) {
		int eusyylmaq;
		for (eusyylmaq = 83; eusyylmaq > 0; eusyylmaq--) {
			continue;
		}
	}
	return 10084;
}

double zkibubv::hxfbjftezzsvzwmvgghhlxjdr(int iwqiclr, string wvowncp, double ncykfixpveiw, string rzkzmrciew, string oeojegjcqfdg, double nrzslvn, bool eltkiqwlp, bool tblmkklfomcegmz) {
	double jpuzvwbzfngufuv = 38632;
	int irylsupeibzwp = 4529;
	int pkidaqcnffj = 4740;
	double rykjoupupd = 24834;
	int mmunoimeb = 2119;
	string fnhaxnsuq = "wojenuticaemasvxutspqmgjcqqbfyybrmmmc";
	int oncte = 6530;
	bool qopgx = true;
	bool tmfousga = false;
	double mctglfaxd = 37672;
	if (false == false) {
		int xwayczttx;
		for (xwayczttx = 51; xwayczttx > 0; xwayczttx--) {
			continue;
		}
	}
	if (2119 != 2119) {
		int kkeuoglzb;
		for (kkeuoglzb = 91; kkeuoglzb > 0; kkeuoglzb--) {
			continue;
		}
	}
	if (6530 == 6530) {
		int pr;
		for (pr = 4; pr > 0; pr--) {
			continue;
		}
	}
	if (false == false) {
		int tfyk;
		for (tfyk = 3; tfyk > 0; tfyk--) {
			continue;
		}
	}
	if (4740 == 4740) {
		int rc;
		for (rc = 100; rc > 0; rc--) {
			continue;
		}
	}
	return 80550;
}

double zkibubv::lafojhcpusmcffkhyb(int zcrdflnceaohr, double bdamnkamhqutve, string uhmqjlkx, bool rypfnrckanqk, double shbcpmv, bool rprjmevnbfuqsuk, int wdtcejykoarq, int lhxiatzjdgtxj, int jlmmgxwsuzklosb) {
	double xehjqcdpcouoklu = 25431;
	bool hljszfkwphba = false;
	string omyinrgsbz = "cyglrmquceejhexnyl";
	bool kfotn = true;
	int mvtpcivvxe = 4614;
	bool ngegcpband = false;
	bool wsvcvurdijkeo = false;
	string nvrwaly = "lpoiwgcfvycmzkkhvijqlibvpphojyutgntgwki";
	string dqvkssyv = "elyyguwsuxbnpgmnerlegjmzygbponrijty";
	string pdmdme = "mmxiuqwszsew";
	return 47234;
}

string zkibubv::qaadojzjujkagbegbk(int bvwgtogmwvtn, bool jnrhlkmgm, bool xtunrjccmgpxp, double jvihsk) {
	int oejymbmzerrkxui = 1947;
	double zcbsbbva = 3556;
	string aylkmgixjju = "akdqrrrysxxjhzsqcpqhwwmidmgpzhroggcnbvtmnrmslgzgerbhfkorivwteccszcwwazzkvolstlmynijyx";
	double yancdey = 4333;
	double zcykndjhskqvo = 47735;
	if (4333 == 4333) {
		int ip;
		for (ip = 52; ip > 0; ip--) {
			continue;
		}
	}
	if (1947 != 1947) {
		int orfdao;
		for (orfdao = 62; orfdao > 0; orfdao--) {
			continue;
		}
	}
	if (1947 == 1947) {
		int yybzusfmbj;
		for (yybzusfmbj = 28; yybzusfmbj > 0; yybzusfmbj--) {
			continue;
		}
	}
	if (string("akdqrrrysxxjhzsqcpqhwwmidmgpzhroggcnbvtmnrmslgzgerbhfkorivwteccszcwwazzkvolstlmynijyx") != string("akdqrrrysxxjhzsqcpqhwwmidmgpzhroggcnbvtmnrmslgzgerbhfkorivwteccszcwwazzkvolstlmynijyx")) {
		int sfcuznz;
		for (sfcuznz = 33; sfcuznz > 0; sfcuznz--) {
			continue;
		}
	}
	if (string("akdqrrrysxxjhzsqcpqhwwmidmgpzhroggcnbvtmnrmslgzgerbhfkorivwteccszcwwazzkvolstlmynijyx") != string("akdqrrrysxxjhzsqcpqhwwmidmgpzhroggcnbvtmnrmslgzgerbhfkorivwteccszcwwazzkvolstlmynijyx")) {
		int yzd;
		for (yzd = 69; yzd > 0; yzd--) {
			continue;
		}
	}
	return string("hlzkbxkux");
}

zkibubv::zkibubv() {
	this->hhwusfsqlllspdn(true, true, false, 5004);
	this->zwdwcjwyhdqbkwfbtttok(false, string("ibtowqmiobdqhqjfuskbqbyiwmfdjmexessbzikkdlbiyorwitqjxrxlgkbqcayigknxnthufvvjbjiqbuqypxyhsrrwdjutsf"), 6999);
	this->shoinddvyjvuvlr(string("hzxocfsdbyssncgnflxrdatkofqtqdechxfkhbgugvzydjcpcbazilu"), true, true, 6168, 9892, 8465, false, 4485, false);
	this->kfebvpllqmgplwascheb(53267, 720, 58538, 2353, 3775, 988, 2962, true, string("dttzsjtnicazzqqperuosccbpkshycbfypxzetpwyruftmeqhrtdikjvashwonwxzwuuzrzzqeqvnwrvflgfpxhevpwszqag"), string("mrnbirvxlixrcerhzceasgnsfuflozxsuqaxwzepvygpyhmza"));
	this->khytpgtdrb(string("eqikktvantlowoawaembobpisxfwiaztdtgvossqtczeureirnqzgvcvksvsxylekspuxixghdnoha"), 2699, 2850);
	this->hxfbjftezzsvzwmvgghhlxjdr(2253, string("dh"), 5501, string("lbwkfriiunlirkp"), string("sipeqpsqdzibaiouqrvxprcarlfsxbtdbs"), 85248, true, true);
	this->lafojhcpusmcffkhyb(1468, 63618, string("sfwvggwillluqlfqbfeijtgevbglomvypphrdqxpjyncczxedxdtcxcmrmailrpynsienrpglkiqpzhmxdfaiqkngasnkpqlcj"), true, 41746, false, 3066, 3746, 2943);
	this->qaadojzjujkagbegbk(3475, true, false, 89926);
	this->vldwcfpblpgpzxcbunxigbrby(string("fribcuivrivdgbmrluzwnzkjvmdbngyzprftlfkqbdw"), 1647, string("nvavytyexapifpuwigwpcdkttjjmh"), true, string("bpagbiornuykerzswtcrcsyoozowrusgxetievhijfppltuejcblagsdrpo"), false, string("ojphkbgtpzqmjaridqppmddbxwbauvtkbrfcsigq"));
	this->umcbeiakxskd();
	this->ofvyryvmwq(1087, string("jocjaklzdwiasfqspdxedtbhfqdydbyyfkkkmojdhemkeeahfjavuftdhjctyvhud"), string("bflsdnqmmtjiagaruvsuxpifiulllzkgkalnwbgltmsnszztorknzitvtgyrzyowpfooqojmgm"));
	this->utdgymcyivbatugrqlfae(string("mzblwihdbwwskbjcgsuksejpoaduzecnnipxzkqjlyrqxncbpluvhciuwqlkkobbf"), false, 37877, 12804);
	this->gjicwcwpootijgddg(false);
	this->wtefcmaqppjggtuizehjigko(string("gjwzjhbwgzjkpdihzcxpanpisqfhbhdnqlmvfuzpkvqrzzxjdaunjarqclubzztgbelwsquhthkanrgfmelhoi"), 12702);
	this->wgqrbczrpmv(string("xsjfhkkzcnndgtq"), string("jgynsvlrqkhhcdzinczunwyjvsjsoxstktxibmotuwtvfnlpsfdpzksdxsvgqpubbgqweihhjqbunxcnvjknpesxyfaua"), 82, false);
	this->favbrwjskgwfepb(6721, 85327, 2073, string("zcarkabckzmdnwernodpgrwzswnmipcrwtrphrmykpgmhjydmqlnqxkwlfnfourxrxwlmbdfctdxsnmlegcnjatzsfcxzwetop"));
	this->accrcsyhtnwe();
	this->ywysncpvjghyrgpfbbptm(578);
	this->lghexdxnzmtywmtenbv(string("mvlwlmnlrdtboptfwkrenodznritccrih"), string("klcgksxxbapxpysmpkqwpkqyoyraiitgfunjgyzpscntuowflhbrxyyjmyqgodsashlziwxj"));
	this->mhjkkkepgxowjuxswytvk(false, 32010, 7081, false, true, true, true);
	this->jptjoasebgkbsjgmvdmiaf(false, 28230, false, 2301, true, 43812, string("zrebrzeezrnmcayujjjygmoqcjjcuvisbnesrtlrufuqogerzronsmm"), string("fcskpiijglvwuhkczsxbamvinftnmxapcpgdomffcpq"));
	this->uitijghmaxqlhsuneroubh(1470, true, false, 22005, 11234, 6307);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class aubtrjc {
public:
	double frqgm;
	double dvpwjcepdiqt;
	string wuhlnsvvavxfef;
	aubtrjc();
	bool hujpzvmhwppjan(bool ilfkeoqj, bool fhlbhjsqhybxels, string tqholb, int houzyjtubpq, int yyjylqloy, double gzntaimdmt, double tzqgmdifxaabyav);
	double qifczqkthf(bool gahquqqgwcgl, int jyjwg, int hbccs, double sozijwzvb, int wjmnohw);
	bool nkcrzpnieqpw(bool cubemuhwyn, int rbnesvtt);
	void vdmxpypfrbgc(int iaficubyktv, double gopcns, string dlnpmuzhmcumfvv);
	double euqwtjygbabufvxjwcvytyvr();
	int jkmqtfqxfgftiz();
	int fswusexiflevs(int xcnkaymkpy, double jwywpuhsdhfbap, int dwrkiwtgqhe, int ttrteyzyqzwc, string lfniozdzhv);
	bool vpehudouxddvgggeysw(double mycusegsvbmtms);
	int tofqyaoypmbijwrluvu(double zvzxnsc);

protected:
	bool hiptwc;
	double ejnjhvpnlrcqgp;

	int kwsrifumajbguawckjxlghw(int cntycrvxsgblniu, bool aqumtcu, bool zkpdfur, int jghbhtrjfzayk, double zzjkrxiafpwqk, double hbyutveou, string owitixztgc, string gozaifqesqouh, bool qnwkmskengnt, double xaralgubjiu);
	string dhepvjkxtxdixzy(int bruxblvloacmgpp);
	double kvbkwhjacvrqmeb(double adrige, int ufmtcc, bool jbzjwcvcm, double dmmlgpqwlmvf, bool mljhjy, bool iuohjl, string jolazzcqsf, double ksftvlwfkaah, double xtnyee);

private:
	string pxffuhxhe;
	double fjpatvwq;
	bool tspjxzeupfnfmj;

	int ajwvwdvifpqgxqtnlkw(int rktnmwj, string mpzefmnomhzik, double rbucyqnr, bool enjiqugpjvyb, double bydonclkmmb, double hpuxo, string ghnbzkjygxrjd, int qsiqpitkrvxgjz, string knnojoeon);
	string assrkoepfdmvvqhkzjkgiut(double fngtbwt, string ruirywibck);
	void ykhuiokchafmz(double amihkjh, bool zuyybqhcarh, double ixxsod, bool bmjuofhtrzly, bool flggvvgvv, int fpfoecyrxnmo, string tiswmrvqdimmojt);
	double pqanofnkrpr(string mqylccs, double prjjnjyddrg, bool qbymgh);

};


int aubtrjc::ajwvwdvifpqgxqtnlkw(int rktnmwj, string mpzefmnomhzik, double rbucyqnr, bool enjiqugpjvyb, double bydonclkmmb, double hpuxo, string ghnbzkjygxrjd, int qsiqpitkrvxgjz, string knnojoeon) {
	double wfhfadxvilngti = 15389;
	string cfmyeha = "lykxtzhrlwbgkrdmfvnsiazoamjnpiaktmgqvjbmynsbnzronnvks";
	if (15389 == 15389) {
		int itxdl;
		for (itxdl = 7; itxdl > 0; itxdl--) {
			continue;
		}
	}
	if (string("lykxtzhrlwbgkrdmfvnsiazoamjnpiaktmgqvjbmynsbnzronnvks") != string("lykxtzhrlwbgkrdmfvnsiazoamjnpiaktmgqvjbmynsbnzronnvks")) {
		int xnovpxjg;
		for (xnovpxjg = 41; xnovpxjg > 0; xnovpxjg--) {
			continue;
		}
	}
	if (string("lykxtzhrlwbgkrdmfvnsiazoamjnpiaktmgqvjbmynsbnzronnvks") != string("lykxtzhrlwbgkrdmfvnsiazoamjnpiaktmgqvjbmynsbnzronnvks")) {
		int vbetk;
		for (vbetk = 36; vbetk > 0; vbetk--) {
			continue;
		}
	}
	return 96663;
}

string aubtrjc::assrkoepfdmvvqhkzjkgiut(double fngtbwt, string ruirywibck) {
	bool hxbayr = false;
	double xhbpqfag = 31071;
	int yhyidpryhhysl = 1355;
	double zifwququaknvb = 17130;
	bool ilwvwkuokbree = false;
	double ovvmrydchlmex = 28427;
	if (false == false) {
		int amkfff;
		for (amkfff = 51; amkfff > 0; amkfff--) {
			continue;
		}
	}
	if (false != false) {
		int uu;
		for (uu = 60; uu > 0; uu--) {
			continue;
		}
	}
	if (31071 != 31071) {
		int ixuut;
		for (ixuut = 48; ixuut > 0; ixuut--) {
			continue;
		}
	}
	if (1355 == 1355) {
		int ww;
		for (ww = 60; ww > 0; ww--) {
			continue;
		}
	}
	return string("qex");
}

void aubtrjc::ykhuiokchafmz(double amihkjh, bool zuyybqhcarh, double ixxsod, bool bmjuofhtrzly, bool flggvvgvv, int fpfoecyrxnmo, string tiswmrvqdimmojt) {
	string ymykuym = "kasjelpylqqlhhohhvwdjhxbsqakvlmoddvdswkydnhtefyjcpdudwyopag";
	if (string("kasjelpylqqlhhohhvwdjhxbsqakvlmoddvdswkydnhtefyjcpdudwyopag") == string("kasjelpylqqlhhohhvwdjhxbsqakvlmoddvdswkydnhtefyjcpdudwyopag")) {
		int zmwwnsuhfh;
		for (zmwwnsuhfh = 69; zmwwnsuhfh > 0; zmwwnsuhfh--) {
			continue;
		}
	}
	if (string("kasjelpylqqlhhohhvwdjhxbsqakvlmoddvdswkydnhtefyjcpdudwyopag") != string("kasjelpylqqlhhohhvwdjhxbsqakvlmoddvdswkydnhtefyjcpdudwyopag")) {
		int zmwfpvwti;
		for (zmwfpvwti = 91; zmwfpvwti > 0; zmwfpvwti--) {
			continue;
		}
	}
	if (string("kasjelpylqqlhhohhvwdjhxbsqakvlmoddvdswkydnhtefyjcpdudwyopag") == string("kasjelpylqqlhhohhvwdjhxbsqakvlmoddvdswkydnhtefyjcpdudwyopag")) {
		int tywr;
		for (tywr = 17; tywr > 0; tywr--) {
			continue;
		}
	}

}

double aubtrjc::pqanofnkrpr(string mqylccs, double prjjnjyddrg, bool qbymgh) {
	double tudohjdxezkbjl = 12391;
	double cgwtkcjqfroh = 17346;
	string bvpyvhyzewfsf = "pvudouxesllvqoflupxonagcqnwfijancnwmcubtjsxbmv";
	bool aqavenvufvhi = false;
	int phpzqawzdchn = 4716;
	int ezeayffq = 1737;
	if (false != false) {
		int ixyzrelcct;
		for (ixyzrelcct = 83; ixyzrelcct > 0; ixyzrelcct--) {
			continue;
		}
	}
	if (12391 == 12391) {
		int sfgfnqt;
		for (sfgfnqt = 98; sfgfnqt > 0; sfgfnqt--) {
			continue;
		}
	}
	if (4716 == 4716) {
		int mkshuhhovo;
		for (mkshuhhovo = 17; mkshuhhovo > 0; mkshuhhovo--) {
			continue;
		}
	}
	return 98834;
}

int aubtrjc::kwsrifumajbguawckjxlghw(int cntycrvxsgblniu, bool aqumtcu, bool zkpdfur, int jghbhtrjfzayk, double zzjkrxiafpwqk, double hbyutveou, string owitixztgc, string gozaifqesqouh, bool qnwkmskengnt, double xaralgubjiu) {
	bool iygsibpegipvwg = true;
	int elpvmsw = 1703;
	string kfcuzvepyztughd = "epdxixvdfrbuarvqdzyxxciklzhnkq";
	double xdtzlt = 16962;
	int gnlrbabj = 1400;
	int fcgkvsyiqqkf = 2499;
	double rsvhpp = 23436;
	return 25030;
}

string aubtrjc::dhepvjkxtxdixzy(int bruxblvloacmgpp) {
	int tgbwm = 230;
	double sfywrag = 21816;
	int wneddka = 2553;
	int gsgfzk = 1769;
	if (230 != 230) {
		int soq;
		for (soq = 6; soq > 0; soq--) {
			continue;
		}
	}
	if (230 == 230) {
		int jyfo;
		for (jyfo = 70; jyfo > 0; jyfo--) {
			continue;
		}
	}
	return string("kbkhngwtptxwqqln");
}

double aubtrjc::kvbkwhjacvrqmeb(double adrige, int ufmtcc, bool jbzjwcvcm, double dmmlgpqwlmvf, bool mljhjy, bool iuohjl, string jolazzcqsf, double ksftvlwfkaah, double xtnyee) {
	int hratmchjhymrs = 5636;
	bool wmonmqkmkn = false;
	if (false == false) {
		int ubxyk;
		for (ubxyk = 33; ubxyk > 0; ubxyk--) {
			continue;
		}
	}
	if (false != false) {
		int bdja;
		for (bdja = 93; bdja > 0; bdja--) {
			continue;
		}
	}
	if (false != false) {
		int mmnwo;
		for (mmnwo = 48; mmnwo > 0; mmnwo--) {
			continue;
		}
	}
	if (false == false) {
		int iylid;
		for (iylid = 53; iylid > 0; iylid--) {
			continue;
		}
	}
	return 25415;
}

bool aubtrjc::hujpzvmhwppjan(bool ilfkeoqj, bool fhlbhjsqhybxels, string tqholb, int houzyjtubpq, int yyjylqloy, double gzntaimdmt, double tzqgmdifxaabyav) {
	bool ljxwwntlwxlpo = true;
	double rqmer = 11944;
	double xopjakffqmer = 53;
	bool ppsdrgt = true;
	string yhgmk = "ymkkofcisqtfdnouzcmxytguhqhineniqgapjwfoudexdgwzokaiqasmkxogexshmanqeatyqlycqbshwgpwmcaiijaz";
	double dqnqkk = 32155;
	string fmfbwmgaua = "bgofazehttvxjtzvnyljrrwvmtkhbexydlkpgmf";
	string ikpqflcsxqnaopv = "txuxlpwa";
	string bctyikvvpnpz = "jzfa";
	int ovithcryel = 555;
	if (string("txuxlpwa") == string("txuxlpwa")) {
		int gnwozfmrhu;
		for (gnwozfmrhu = 29; gnwozfmrhu > 0; gnwozfmrhu--) {
			continue;
		}
	}
	if (53 != 53) {
		int pmagtvk;
		for (pmagtvk = 29; pmagtvk > 0; pmagtvk--) {
			continue;
		}
	}
	return true;
}

double aubtrjc::qifczqkthf(bool gahquqqgwcgl, int jyjwg, int hbccs, double sozijwzvb, int wjmnohw) {
	bool dbrrvb = true;
	int kmeif = 852;
	bool fkuuiccmt = true;
	bool fjdyekeagjc = false;
	double fglmitcnhr = 25770;
	string kyzxsqlre = "dnmdgkxiwxlyfregqkuzdnqykjslt";
	int hqpnwkqfsmgaww = 7962;
	int ujfagfizgupl = 1034;
	bool knqcjesbezp = false;
	string jrycvza = "okinqvecwlrlttzwczeizgxahgwypcoolsbcdfnagvtjlqmfealhpjiwbswkwlaymsdarysbmarlqnzwqzkcloxvbzmoqf";
	if (25770 == 25770) {
		int fn;
		for (fn = 48; fn > 0; fn--) {
			continue;
		}
	}
	if (7962 != 7962) {
		int cgdmqzo;
		for (cgdmqzo = 97; cgdmqzo > 0; cgdmqzo--) {
			continue;
		}
	}
	if (true != true) {
		int xwukqgsxlm;
		for (xwukqgsxlm = 30; xwukqgsxlm > 0; xwukqgsxlm--) {
			continue;
		}
	}
	return 30060;
}

bool aubtrjc::nkcrzpnieqpw(bool cubemuhwyn, int rbnesvtt) {
	double kpjylgnjy = 12662;
	bool yduckxjmainfte = false;
	double gqqpfn = 55627;
	int fikimdsmeaezxy = 2626;
	int utmukcvfdydtkxe = 204;
	bool btxwqi = false;
	double inwqoxtlps = 5938;
	if (55627 != 55627) {
		int jmvof;
		for (jmvof = 49; jmvof > 0; jmvof--) {
			continue;
		}
	}
	if (204 != 204) {
		int brsjugs;
		for (brsjugs = 40; brsjugs > 0; brsjugs--) {
			continue;
		}
	}
	if (55627 == 55627) {
		int oomdprl;
		for (oomdprl = 79; oomdprl > 0; oomdprl--) {
			continue;
		}
	}
	return false;
}

void aubtrjc::vdmxpypfrbgc(int iaficubyktv, double gopcns, string dlnpmuzhmcumfvv) {
	int xeppofrafpjzgv = 1252;
	bool xjntvbua = false;
	int yaiueimicerc = 609;
	bool sucomxefim = false;
	int cyjvsfkvmz = 629;
	int qllfomlshoa = 2149;
	string lvviyky = "ehijyjgwqzvcslypeeuyzrhbtvdhrpvittwymmlpwwxjblsngwlaqmvjhkffjsrjrkxsoptkgixi";
	double sotepxwsszjxfsl = 19414;
	bool gmrhngxaavxax = false;
	double lyhtpmjgrkeua = 10043;
	if (1252 == 1252) {
		int cn;
		for (cn = 70; cn > 0; cn--) {
			continue;
		}
	}
	if (609 != 609) {
		int vxhfoyrxam;
		for (vxhfoyrxam = 55; vxhfoyrxam > 0; vxhfoyrxam--) {
			continue;
		}
	}
	if (1252 == 1252) {
		int dvmjcept;
		for (dvmjcept = 40; dvmjcept > 0; dvmjcept--) {
			continue;
		}
	}

}

double aubtrjc::euqwtjygbabufvxjwcvytyvr() {
	double sisfkshw = 6929;
	string dlrmzsgobwokgt = "";
	string feujgitlqoam = "erebyxkibuenreqwbaeyhajjjgoizngtyadygfifjmamasukapjypnjkcqhfbbbveoi";
	if (6929 == 6929) {
		int nj;
		for (nj = 39; nj > 0; nj--) {
			continue;
		}
	}
	if (string("") != string("")) {
		int apvtt;
		for (apvtt = 57; apvtt > 0; apvtt--) {
			continue;
		}
	}
	if (string("erebyxkibuenreqwbaeyhajjjgoizngtyadygfifjmamasukapjypnjkcqhfbbbveoi") == string("erebyxkibuenreqwbaeyhajjjgoizngtyadygfifjmamasukapjypnjkcqhfbbbveoi")) {
		int fzhmoeteh;
		for (fzhmoeteh = 8; fzhmoeteh > 0; fzhmoeteh--) {
			continue;
		}
	}
	return 58884;
}

int aubtrjc::jkmqtfqxfgftiz() {
	double ofefoqwcpkkitp = 51918;
	if (51918 == 51918) {
		int afaqu;
		for (afaqu = 98; afaqu > 0; afaqu--) {
			continue;
		}
	}
	if (51918 == 51918) {
		int dixb;
		for (dixb = 93; dixb > 0; dixb--) {
			continue;
		}
	}
	if (51918 != 51918) {
		int jppvajlw;
		for (jppvajlw = 34; jppvajlw > 0; jppvajlw--) {
			continue;
		}
	}
	if (51918 != 51918) {
		int xr;
		for (xr = 13; xr > 0; xr--) {
			continue;
		}
	}
	return 64737;
}

int aubtrjc::fswusexiflevs(int xcnkaymkpy, double jwywpuhsdhfbap, int dwrkiwtgqhe, int ttrteyzyqzwc, string lfniozdzhv) {
	double zfgwpdionl = 22265;
	string zndnnkuxc = "tenevmqlpqyslccuzxtqohrrmkdbklwtcnwdbcifncscnguwqyigvbwppeoikno";
	bool yqdtz = false;
	double eoiybazgphidfsr = 75387;
	bool youui = false;
	double flejydu = 29323;
	if (false == false) {
		int zumg;
		for (zumg = 5; zumg > 0; zumg--) {
			continue;
		}
	}
	return 21422;
}

bool aubtrjc::vpehudouxddvgggeysw(double mycusegsvbmtms) {
	return false;
}

int aubtrjc::tofqyaoypmbijwrluvu(double zvzxnsc) {
	int mxrsop = 1954;
	bool dkciq = true;
	string ofgfa = "qdyrytvmdzrirhxrlhsqxnuhbzwzpjqfjrvgvuunwijrrzq";
	double gcovxsdfgykn = 6590;
	string qnncvqujbmefn = "zztzyxcwrvqubfocccisxbixuvqindbh";
	string ulive = "sbqtnhgsmjmztmpssndytefjhutimifafyaahxkqmofckoedfsydcoyyqnlypqgksbrontwoxq";
	int xklqxpafqkfxdhq = 3027;
	bool iodxjlf = false;
	if (string("sbqtnhgsmjmztmpssndytefjhutimifafyaahxkqmofckoedfsydcoyyqnlypqgksbrontwoxq") != string("sbqtnhgsmjmztmpssndytefjhutimifafyaahxkqmofckoedfsydcoyyqnlypqgksbrontwoxq")) {
		int ngi;
		for (ngi = 47; ngi > 0; ngi--) {
			continue;
		}
	}
	return 29512;
}

aubtrjc::aubtrjc() {
	this->hujpzvmhwppjan(false, false, string("nyxpvnwlewkqhtcpwswozepktnsqyvpihkxjvgvdrpjj"), 6427, 423, 6030, 7370);
	this->qifczqkthf(false, 6992, 1100, 72209, 782);
	this->nkcrzpnieqpw(false, 5269);
	this->vdmxpypfrbgc(3568, 23791, string("xuipjpnejjnkcnoyjhwnkhabgnbrtihadbqiqjasyby"));
	this->euqwtjygbabufvxjwcvytyvr();
	this->jkmqtfqxfgftiz();
	this->fswusexiflevs(403, 21159, 1513, 267, string("mlhyhfgliiqozakurwafrooqrpbeonstaiwpadswzpkcganjhhrye"));
	this->vpehudouxddvgggeysw(9591);
	this->tofqyaoypmbijwrluvu(25016);
	this->kwsrifumajbguawckjxlghw(5822, false, false, 5468, 45312, 53357, string("osiidfjqbtivbyxpnvzefrigqkefyncopiaqzwwxckslk"), string("eaupfhdhmlnadmkfzwidqxtvdhkwreixlwggxruebgezmwatfsotqdmjkurjvcyeixzfhmwxp"), true, 62875);
	this->dhepvjkxtxdixzy(5074);
	this->kvbkwhjacvrqmeb(15997, 2888, false, 51459, true, false, string("dcyiquegmixaehknwoyaxigzuvjpfgdcvgpfkwhbgkyjkdxiqnmardavjzwqhkzckvvchmjpxvgqfloshggnecxnveptlmqnfw"), 24298, 59528);
	this->ajwvwdvifpqgxqtnlkw(2453, string(""), 5487, false, 2976, 42989, string("lqbg"), 1099, string("myrkbooesvyskcwkhbxuuktqlutuac"));
	this->assrkoepfdmvvqhkzjkgiut(81129, string("adxakenhxesjofmdoyhaghussftfkdmmnkyjaxxklancsajhuvbdgohswlqpznflkyljke"));
	this->ykhuiokchafmz(3832, false, 26404, true, false, 6442, string("agnwxcwionchwdxwdchiwwhkfgeiypecmgqfad"));
	this->pqanofnkrpr(string("wqdbdcdufyrsrqjsbmtztbomcmrjhdltjjoovudqv"), 30685, false);
}
