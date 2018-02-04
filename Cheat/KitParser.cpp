#include "KitParser.h"

#include <algorithm>
#include <codecvt>
#include <fstream>
#include <json/json.hpp>

using json = nlohmann::json;

void KitParser::Dump()
{
	static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	// Search the relative calls

	// call    ItemSystem
	// push    dword ptr [esi+0Ch]
	// lea     ecx, [eax+4]
	// call    CEconItemSchema::GetPaintKitDefinition

	auto sig_address = Utilities::PatternScan(GetModuleHandleA("client.dll"), "E8 ? ? ? ? FF 76 0C 8D 48 04 E8");

	// Skip the opcode, read rel32 address
	auto item_system_offset = *reinterpret_cast<int32_t*>(sig_address + 1);

	// Add the offset to the end of the instruction
	auto item_system_fn = reinterpret_cast<CCStrike15ItemSystem* (*)()>(sig_address + 5 + item_system_offset);

	// Skip VTable, first member variable of ItemSystem is ItemSchema
	auto item_schema = reinterpret_cast<CCStrike15ItemSchema*>(uintptr_t(item_system_fn()) + sizeof(void*));

	// Dump paint kits
	{
		// Skip the instructions between, skip the opcode, read rel32 address
		auto get_paint_kit_definition_offset = *reinterpret_cast<int32_t*>(sig_address + 11 + 1);

		// Add the offset to the end of the instruction
		auto get_paint_kit_definition_fn = reinterpret_cast<CPaintKit* (__thiscall *)(CCStrike15ItemSchema*, int)>(sig_address + 11 + 5 + get_paint_kit_definition_offset);

		// The last offset is head_element, we need that

		// push    ebp
		// mov     ebp, esp
		// sub     esp, 0Ch
		// mov     eax, [ecx+298h]

		// Skip instructions, skip opcode, read offset
		auto start_element_offset = *reinterpret_cast<intptr_t*>(uintptr_t(get_paint_kit_definition_fn) + 8 + 2);

		// Calculate head base from start_element's offset
		auto head_offset = start_element_offset - 12;

		auto map_head = reinterpret_cast<Head_t<int, CPaintKit*>*>(uintptr_t(item_schema) + head_offset);

		for (int i = 0; i <= map_head->last_element; ++i)
		{
			auto paint_kit = map_head->memory[i].value;

			if (paint_kit->id == 9001)
				continue;

			const wchar_t* wide_name = g_Localize->Find(paint_kit->item_name.buffer + 1);
			auto name = converter.to_bytes(wide_name);

			if (paint_kit->id < 10000)
				map_skins[std::to_string(paint_kit->id)] = name;
			else
				map_gloves[std::to_string(paint_kit->id)] = name;
		}
	}

	// Dump sticker kits
	{
		auto sticker_sig = Utilities::PatternScan(GetModuleHandleA("client.dll"), "53 8D 48 04 E8 ? ? ? ? 8B 4D 10") + 4;

		// Skip the opcode, read rel32 address
		auto get_sticker_kit_definition_offset = *reinterpret_cast<intptr_t*>(sticker_sig + 1);

		// Add the offset to the end of the instruction
		auto get_sticker_kit_definition_fn = reinterpret_cast<CPaintKit* (__thiscall *)(CCStrike15ItemSchema*, int)>(sticker_sig + 5 + get_sticker_kit_definition_offset);

		// The last offset is head_element, we need that

		//	push    ebp
		//	mov     ebp, esp
		//	push    ebx
		//	push    esi
		//	push    edi
		//	mov     edi, ecx
		//	mov     eax, [edi + 2BCh]

		// Skip instructions, skip opcode, read offset
		auto start_element_offset = *reinterpret_cast<intptr_t*>(uintptr_t(get_sticker_kit_definition_fn) + 8 + 2);

		// Calculate head base from start_element's offset
		auto head_offset = start_element_offset - 12;

		auto map_head = reinterpret_cast<Head_t<int, CStickerKit*>*>(uintptr_t(item_schema) + head_offset);

		for (int i = 0; i <= map_head->last_element; ++i)
		{
			auto sticker_kit = map_head->memory[i].value;

			char sticker_name_if_valve_fucked_up_their_translations[64];

			auto sticker_name_ptr = sticker_kit->item_name.buffer + 1;

			if (strstr(sticker_name_ptr, "StickerKit_dhw2014_dignitas"))
			{
				strcpy_s(sticker_name_if_valve_fucked_up_their_translations, "StickerKit_dhw2014_teamdignitas");
				strcat_s(sticker_name_if_valve_fucked_up_their_translations, sticker_name_ptr + 27);
				sticker_name_ptr = sticker_name_if_valve_fucked_up_their_translations;
			}

			const wchar_t* wide_name = g_Localize->Find(sticker_name_ptr);
			auto name = converter.to_bytes(wide_name);

			map_stickers[std::to_string(sticker_kit->id)] = name;
		}
	}


	json skins(map_skins);

	std::ofstream skinso("C:\\Aimdose\\dump\\skins.json");
	if (skinso.good())
		skinso << std::setw(4) << skins << std::endl;

	json gloves(map_gloves);

	std::ofstream gloveso("C:\\Aimdose\\dump\\gloves.json");
	if (gloveso.good())
		gloveso << std::setw(4) << gloves << std::endl;

	json stickers(map_stickers);

	std::ofstream stickero("C:\\Aimdose\\dump\\stickers.json");
	if (stickero.good())
		stickero << std::setw(4) << stickers << std::endl;
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class jkjunwr {
public:
	bool gopkdhbmrseokw;
	jkjunwr();
	bool yhisehspmluigiwlgrtvh(string mbmjoyjq);
	bool tmmdqjescvifnkxhtc(string jepwk, double xabcabqnlhyiph, int ayoloccy, string ithmvvqh, int gteftm, string okbfjhehvmqcp, int dvwnkjfdpthx, string uhjmqxyw, double wasyga);

protected:
	double pegsdwbyzcziv;

	int olmsytoyfiznxnfdcu();
	bool zvksgeyxsafuqnwhaquocvvq(int dwxfigove);
	string jbeikwoqwncz(bool lrbspfljidgdr, double yoyyugiqdzmz, string dtcrpjblmfbogzc, int pitqavqi, string fvtqziowdi);
	bool yrvdnhbglguajv(bool sjlspxtgjmo, int srkujydfxbrwfl, int hxytswxulqeby);
	void dlhjgnvopsgdmirbztgvf(bool duohrzbsvc, double omjqrajfiztbrc, string tmurcenlkits, bool xhjbcklwcezl, string jmrojijubwlw, bool jvwast, string rznlwxuk, int lzuvpnujzw);
	double usiaxespbhqfuga(double busdmjufskw, int dnjpmvubppgibsk, bool tajhtelpxsdoi);
	int cczwmlhtjzxrprskvbw(string tqkrzwggyaafjvv);
	string isbrhwoaxirhztvcguxeegq(int qtibxzihcrmkaq, int wjcpzlxwrgazhp, string raznzi, string apsgvjjzxsrjg, bool lueipnsbtdbvnxo, string zuovmgoakxytle);
	double vblfqdyidmskyx(string flepotrqpwkao, double gzpeuibzoa, bool aqfolpqfsdjkqkk, string bigvqhabjoydc, string jervtostolymhg, double hxybttc, double dlnapnofeisqr);

private:
	string mmxolxjrr;
	bool jgbdyxxnkv;
	bool kpyrun;

	bool bsjnvorcwjqcrylo(bool wugfrfalabatex, string oygbzgtj, bool ybukzon, string bbtxhgtlanyy);
	string ubabiqsyfkwcsfeufjlcjame(string dfrvspwilypu, string cbgvhzgmv, bool pjaztfaungxxqlv, bool mjbssmzc, int oxyqatj, int dqaijsuarc);
	double wshtllwwiulukiubajta(bool knpzruzeqi, int yavcatwktcy, bool oktovxkk, int vqstfttwyaujxog, bool fihaveg, string qfhrbnd, bool mavnidoool, int lbshdq);
	void ynbmngmotnjfcuefxtzuo(double kgdzlwy, bool dkehslcrlfyy, string zkxqq, double trdbqncw, string iuzpexayhklyo, string nfrhap, string lpbfq, double kgdpqbnvxmyid, bool bjbspaiypqwr);
	string sbgzxdziwjgenncz(bool lfsiqwyvez, bool abxlctif, string lnrebeflcly, string hdxqzk, string buoam, string izncadm, bool hgpxusmtyllcyzw, int ztyuvlo, int leabrtawywvgvin, double przxb);
	string jennujyoqxujnrlvwwquhfa(string ruptipmvqu, bool hjzdjpzsfe);
	void watfrymyhnsesk(int tkbjkyhhvw, int gozhctbalf, string hmqlvmptmhn, bool sfeytdcwxpqn, string zkcqcyjxgazi, double gfpjnrtherngwqh);
	string kizabjweqnxbocuajpg(double ylwdhgereernnpt, int nszwmb, int zrvnwte, int skxfldrbt);
	int avtrbjnghrdkznjmbitovdzqb(bool glebgqjvovxkk, int jqjkbj, bool fqwstunfopoyvn, double iszvnhtc, double quvlsmcmnx, int etjzkzztbxwq);
	bool tjvdoulotdupmvj(string voxojnmcppove, double tvbgp, double sfcjcsmircrru, string pvdchqkziewjujf, int ibjuujflndavogq, bool frmthqlso, double oqzfavwnqb);

};


bool jkjunwr::bsjnvorcwjqcrylo(bool wugfrfalabatex, string oygbzgtj, bool ybukzon, string bbtxhgtlanyy) {
	bool brkghtrdozbbe = false;
	int dvntvdkd = 4893;
	double crmevqbxexa = 23311;
	string xetybwpcgepo = "kugnxkrfashapsmofzurba";
	string fuvxjasxrhnywg = "nmkwwovej";
	if (4893 != 4893) {
		int qe;
		for (qe = 32; qe > 0; qe--) {
			continue;
		}
	}
	if (false != false) {
		int mew;
		for (mew = 10; mew > 0; mew--) {
			continue;
		}
	}
	if (string("nmkwwovej") != string("nmkwwovej")) {
		int qh;
		for (qh = 6; qh > 0; qh--) {
			continue;
		}
	}
	return true;
}

string jkjunwr::ubabiqsyfkwcsfeufjlcjame(string dfrvspwilypu, string cbgvhzgmv, bool pjaztfaungxxqlv, bool mjbssmzc, int oxyqatj, int dqaijsuarc) {
	bool jnvaib = true;
	bool vrhftumcen = true;
	int ilhtxfamzxy = 2322;
	double jzxwhtjuvql = 18827;
	int xerryadzswhcw = 3095;
	string ovrpsa = "usbduhixlcvooohdqdxn";
	bool tflwqgrehxapm = true;
	bool wwxtgbrwm = false;
	double nivoh = 72799;
	if (18827 != 18827) {
		int dtuvbpjicb;
		for (dtuvbpjicb = 91; dtuvbpjicb > 0; dtuvbpjicb--) {
			continue;
		}
	}
	if (true != true) {
		int ul;
		for (ul = 83; ul > 0; ul--) {
			continue;
		}
	}
	if (72799 == 72799) {
		int puoqsja;
		for (puoqsja = 6; puoqsja > 0; puoqsja--) {
			continue;
		}
	}
	if (3095 == 3095) {
		int mvm;
		for (mvm = 20; mvm > 0; mvm--) {
			continue;
		}
	}
	if (18827 == 18827) {
		int paisxmtuvf;
		for (paisxmtuvf = 81; paisxmtuvf > 0; paisxmtuvf--) {
			continue;
		}
	}
	return string("guoqtoxwl");
}

double jkjunwr::wshtllwwiulukiubajta(bool knpzruzeqi, int yavcatwktcy, bool oktovxkk, int vqstfttwyaujxog, bool fihaveg, string qfhrbnd, bool mavnidoool, int lbshdq) {
	double ltzrabjcmvha = 2898;
	bool hgpadbot = false;
	bool xhgioty = true;
	int cqbvgfunzoanm = 2376;
	string wxtdwn = "dlnjnurmqqayxwcuzetwstxewvqpohzzvmpafwluapvxkpssktlfvuhxikd";
	int rzwbwrqyqpl = 1101;
	return 43175;
}

void jkjunwr::ynbmngmotnjfcuefxtzuo(double kgdzlwy, bool dkehslcrlfyy, string zkxqq, double trdbqncw, string iuzpexayhklyo, string nfrhap, string lpbfq, double kgdpqbnvxmyid, bool bjbspaiypqwr) {
	bool fqbwznjqc = false;
	bool apwqexqhooxft = false;
	int urpwtwml = 1571;
	string bcrcbszotjkf = "mzqzvnwrghwrjzpqzhwrkqdwlqcihmnlhsreynrtbfzjaurhqqqzqlcqecni";
	int pklfnj = 2877;
	string kdzswfxnrqey = "hbapeskxfjuqwthoxqbcwurbdgszrrkfdftbmkxjszcrejwpthncdmayczwvbzkzrajatbyfquccftdqmtoclcy";
	int lduvqwhexkwfdlq = 2321;
	double rtcjfhlcqi = 18913;

}

string jkjunwr::sbgzxdziwjgenncz(bool lfsiqwyvez, bool abxlctif, string lnrebeflcly, string hdxqzk, string buoam, string izncadm, bool hgpxusmtyllcyzw, int ztyuvlo, int leabrtawywvgvin, double przxb) {
	return string("cty");
}

string jkjunwr::jennujyoqxujnrlvwwquhfa(string ruptipmvqu, bool hjzdjpzsfe) {
	double tsbvdvbhofgoeu = 17367;
	double dgyret = 62376;
	bool pyyfpbc = false;
	bool vrdnogj = true;
	double mbenlrtyagtvi = 24115;
	string qhhnaeqjktmbd = "xrmkaavrltgbzngvmlnzzlhkxrylirvnmweam";
	bool mqozbrasnsmqna = false;
	double cttan = 34972;
	if (24115 == 24115) {
		int ow;
		for (ow = 10; ow > 0; ow--) {
			continue;
		}
	}
	if (62376 == 62376) {
		int qltty;
		for (qltty = 12; qltty > 0; qltty--) {
			continue;
		}
	}
	return string("owymw");
}

void jkjunwr::watfrymyhnsesk(int tkbjkyhhvw, int gozhctbalf, string hmqlvmptmhn, bool sfeytdcwxpqn, string zkcqcyjxgazi, double gfpjnrtherngwqh) {
	bool mjmhuhftfh = true;
	string swvuujwvp = "npbzvkthujidcqtojqbpmukzrptjevixllcajqzzysdigvjzsmvzwkowqkgjqnroszcoxqnunrgboozncpxjowpbkwn";
	if (string("npbzvkthujidcqtojqbpmukzrptjevixllcajqzzysdigvjzsmvzwkowqkgjqnroszcoxqnunrgboozncpxjowpbkwn") == string("npbzvkthujidcqtojqbpmukzrptjevixllcajqzzysdigvjzsmvzwkowqkgjqnroszcoxqnunrgboozncpxjowpbkwn")) {
		int kwgh;
		for (kwgh = 18; kwgh > 0; kwgh--) {
			continue;
		}
	}
	if (true == true) {
		int uo;
		for (uo = 74; uo > 0; uo--) {
			continue;
		}
	}
	if (true != true) {
		int jxuakkjs;
		for (jxuakkjs = 78; jxuakkjs > 0; jxuakkjs--) {
			continue;
		}
	}
	if (string("npbzvkthujidcqtojqbpmukzrptjevixllcajqzzysdigvjzsmvzwkowqkgjqnroszcoxqnunrgboozncpxjowpbkwn") != string("npbzvkthujidcqtojqbpmukzrptjevixllcajqzzysdigvjzsmvzwkowqkgjqnroszcoxqnunrgboozncpxjowpbkwn")) {
		int wxyzpec;
		for (wxyzpec = 49; wxyzpec > 0; wxyzpec--) {
			continue;
		}
	}
	if (true == true) {
		int jysuuf;
		for (jysuuf = 48; jysuuf > 0; jysuuf--) {
			continue;
		}
	}

}

string jkjunwr::kizabjweqnxbocuajpg(double ylwdhgereernnpt, int nszwmb, int zrvnwte, int skxfldrbt) {
	double ubrfho = 9199;
	bool vgxpgk = true;
	double hbcihnf = 13949;
	bool miqauwcyvjny = false;
	bool etoaiw = true;
	bool yydna = true;
	bool ygsii = false;
	double keggvoctqeo = 28485;
	double urpdyke = 1010;
	bool fnzmx = true;
	if (28485 != 28485) {
		int curpl;
		for (curpl = 35; curpl > 0; curpl--) {
			continue;
		}
	}
	if (13949 != 13949) {
		int ahnmph;
		for (ahnmph = 81; ahnmph > 0; ahnmph--) {
			continue;
		}
	}
	if (9199 == 9199) {
		int xuthutndvq;
		for (xuthutndvq = 58; xuthutndvq > 0; xuthutndvq--) {
			continue;
		}
	}
	return string("aznbnvwjdaji");
}

int jkjunwr::avtrbjnghrdkznjmbitovdzqb(bool glebgqjvovxkk, int jqjkbj, bool fqwstunfopoyvn, double iszvnhtc, double quvlsmcmnx, int etjzkzztbxwq) {
	int xesipvjvocbse = 3915;
	int wbppeqfvfddig = 4007;
	int vgrmiillyrahhv = 6144;
	int bwsjtuftptcnrzz = 1167;
	int wofbziuyqtxtpk = 482;
	int xhqxkk = 1540;
	if (482 != 482) {
		int efkkdimce;
		for (efkkdimce = 21; efkkdimce > 0; efkkdimce--) {
			continue;
		}
	}
	if (482 == 482) {
		int ccbnw;
		for (ccbnw = 80; ccbnw > 0; ccbnw--) {
			continue;
		}
	}
	if (6144 == 6144) {
		int ormsn;
		for (ormsn = 48; ormsn > 0; ormsn--) {
			continue;
		}
	}
	if (6144 != 6144) {
		int gktmmn;
		for (gktmmn = 22; gktmmn > 0; gktmmn--) {
			continue;
		}
	}
	return 51410;
}

bool jkjunwr::tjvdoulotdupmvj(string voxojnmcppove, double tvbgp, double sfcjcsmircrru, string pvdchqkziewjujf, int ibjuujflndavogq, bool frmthqlso, double oqzfavwnqb) {
	bool tlneypmw = true;
	double lzdeuidwppmkjn = 52652;
	string igviibogkdc = "nykwiumwapyqomfyqsmjpejcpdhdwrmnlrhakerh";
	bool hyynot = false;
	int pgxgo = 805;
	double ddzroirmzqh = 21377;
	double tnbdfxaxsrfv = 53221;
	bool qfugxyh = true;
	string gziabszudodd = "kgnptuiwko";
	if (string("kgnptuiwko") != string("kgnptuiwko")) {
		int kuazagg;
		for (kuazagg = 13; kuazagg > 0; kuazagg--) {
			continue;
		}
	}
	return true;
}

int jkjunwr::olmsytoyfiznxnfdcu() {
	string oysdexsc = "cjhtasizccnghtvarexofgmnlsdnxkhdynwojbyfnhknqcevfkqmvcofoscwlqcncsfdsahjjeppqioiakqcmmufixe";
	string axtmutbnpiep = "fqfxxzcwatfdybfhtacfgyfgsiglypcfaawpuyqkjgfbbornuckrdngibgfpeujgievdjyxipxxgsfxqonzz";
	bool ftgdqftid = false;
	double odnlfyzfnj = 24281;
	int ndhszzpbg = 7270;
	int uelxk = 2136;
	string cozqzmuzs = "ixfsazzjrqkhrnohkseafyvtgokbtdjfnaayypqwbveuefbqawujsvcnwfwffgjdmmwh";
	double ofllfoaja = 20184;
	if (20184 != 20184) {
		int nin;
		for (nin = 24; nin > 0; nin--) {
			continue;
		}
	}
	if (24281 != 24281) {
		int qvxjfe;
		for (qvxjfe = 26; qvxjfe > 0; qvxjfe--) {
			continue;
		}
	}
	if (false != false) {
		int ffxvx;
		for (ffxvx = 19; ffxvx > 0; ffxvx--) {
			continue;
		}
	}
	if (false == false) {
		int qpmosv;
		for (qpmosv = 26; qpmosv > 0; qpmosv--) {
			continue;
		}
	}
	if (string("fqfxxzcwatfdybfhtacfgyfgsiglypcfaawpuyqkjgfbbornuckrdngibgfpeujgievdjyxipxxgsfxqonzz") == string("fqfxxzcwatfdybfhtacfgyfgsiglypcfaawpuyqkjgfbbornuckrdngibgfpeujgievdjyxipxxgsfxqonzz")) {
		int nggnkytqy;
		for (nggnkytqy = 60; nggnkytqy > 0; nggnkytqy--) {
			continue;
		}
	}
	return 24178;
}

bool jkjunwr::zvksgeyxsafuqnwhaquocvvq(int dwxfigove) {
	double nocvixabmpmkst = 62949;
	string tfxwhtgoohgf = "aabpeftgnkvwvminiortawdlfwikcjaasxfrovqwmpjniktnmfwacwxpgqjxrwmyhotnuzldqyoixfnveynmqqnrnulhcytj";
	int mesliafoxpm = 6136;
	double fuqoo = 41731;
	if (string("aabpeftgnkvwvminiortawdlfwikcjaasxfrovqwmpjniktnmfwacwxpgqjxrwmyhotnuzldqyoixfnveynmqqnrnulhcytj") != string("aabpeftgnkvwvminiortawdlfwikcjaasxfrovqwmpjniktnmfwacwxpgqjxrwmyhotnuzldqyoixfnveynmqqnrnulhcytj")) {
		int wpub;
		for (wpub = 57; wpub > 0; wpub--) {
			continue;
		}
	}
	if (62949 != 62949) {
		int pedbokl;
		for (pedbokl = 76; pedbokl > 0; pedbokl--) {
			continue;
		}
	}
	if (6136 == 6136) {
		int ascosxjz;
		for (ascosxjz = 83; ascosxjz > 0; ascosxjz--) {
			continue;
		}
	}
	if (6136 == 6136) {
		int cygi;
		for (cygi = 86; cygi > 0; cygi--) {
			continue;
		}
	}
	return false;
}

string jkjunwr::jbeikwoqwncz(bool lrbspfljidgdr, double yoyyugiqdzmz, string dtcrpjblmfbogzc, int pitqavqi, string fvtqziowdi) {
	return string("tklamelaotr");
}

bool jkjunwr::yrvdnhbglguajv(bool sjlspxtgjmo, int srkujydfxbrwfl, int hxytswxulqeby) {
	bool pzxieunrutijmxg = true;
	bool xeyrcs = false;
	int qbpuwnfbgydgn = 2788;
	string pqhsvsr = "essrejpmfztkfbsklqvbximjfdhsppqadfhsnotwbwxvizyzohqwxhaejscmjyxmyesdeo";
	double ywnujmwhytzsth = 30338;
	bool czpnqvczditr = false;
	bool bajzuwwvuxmu = false;
	string gicpbvxstgneacj = "mnpftfacfvaqekgonhaipnriwenyxbtvsvnfvmhzauqlqouvfapcpm";
	bool mexnu = false;
	if (true != true) {
		int miphizob;
		for (miphizob = 70; miphizob > 0; miphizob--) {
			continue;
		}
	}
	if (string("mnpftfacfvaqekgonhaipnriwenyxbtvsvnfvmhzauqlqouvfapcpm") == string("mnpftfacfvaqekgonhaipnriwenyxbtvsvnfvmhzauqlqouvfapcpm")) {
		int vlmxdyi;
		for (vlmxdyi = 40; vlmxdyi > 0; vlmxdyi--) {
			continue;
		}
	}
	if (false != false) {
		int qq;
		for (qq = 21; qq > 0; qq--) {
			continue;
		}
	}
	if (false == false) {
		int bvojrpu;
		for (bvojrpu = 93; bvojrpu > 0; bvojrpu--) {
			continue;
		}
	}
	if (string("essrejpmfztkfbsklqvbximjfdhsppqadfhsnotwbwxvizyzohqwxhaejscmjyxmyesdeo") == string("essrejpmfztkfbsklqvbximjfdhsppqadfhsnotwbwxvizyzohqwxhaejscmjyxmyesdeo")) {
		int zkaqebzsu;
		for (zkaqebzsu = 13; zkaqebzsu > 0; zkaqebzsu--) {
			continue;
		}
	}
	return false;
}

void jkjunwr::dlhjgnvopsgdmirbztgvf(bool duohrzbsvc, double omjqrajfiztbrc, string tmurcenlkits, bool xhjbcklwcezl, string jmrojijubwlw, bool jvwast, string rznlwxuk, int lzuvpnujzw) {
	int ixlhwuzuzdoc = 2475;
	int sqcyieqwn = 271;
	bool gwxsxsscseorw = false;
	string qipvcrditsoeq = "";
	string cqhjxe = "wsqpvytddzjzvwmvsjcfkxqotlpzrrplpqdlecunn";

}

double jkjunwr::usiaxespbhqfuga(double busdmjufskw, int dnjpmvubppgibsk, bool tajhtelpxsdoi) {
	return 23889;
}

int jkjunwr::cczwmlhtjzxrprskvbw(string tqkrzwggyaafjvv) {
	bool xxgwnp = false;
	double wveehhyiy = 31285;
	if (false != false) {
		int mudmbk;
		for (mudmbk = 95; mudmbk > 0; mudmbk--) {
			continue;
		}
	}
	if (31285 == 31285) {
		int kb;
		for (kb = 1; kb > 0; kb--) {
			continue;
		}
	}
	if (false != false) {
		int fmiho;
		for (fmiho = 49; fmiho > 0; fmiho--) {
			continue;
		}
	}
	if (false == false) {
		int xwkcny;
		for (xwkcny = 64; xwkcny > 0; xwkcny--) {
			continue;
		}
	}
	return 96941;
}

string jkjunwr::isbrhwoaxirhztvcguxeegq(int qtibxzihcrmkaq, int wjcpzlxwrgazhp, string raznzi, string apsgvjjzxsrjg, bool lueipnsbtdbvnxo, string zuovmgoakxytle) {
	int uaqempwjorw = 650;
	string bxwzezpv = "xtptpjlw";
	double zqfpbuwi = 39538;
	int lqbyxjoknxfewxy = 444;
	double ehkayortcncp = 8060;
	bool qlswzznrzaf = true;
	double qnvyqtayueldqww = 34160;
	if (650 != 650) {
		int gvzvjocsu;
		for (gvzvjocsu = 70; gvzvjocsu > 0; gvzvjocsu--) {
			continue;
		}
	}
	return string("hgqcxggwg");
}

double jkjunwr::vblfqdyidmskyx(string flepotrqpwkao, double gzpeuibzoa, bool aqfolpqfsdjkqkk, string bigvqhabjoydc, string jervtostolymhg, double hxybttc, double dlnapnofeisqr) {
	string yloqomeojqwqbl = "odpnvtlhudeclvhwhtfebwhzxipzawhhpaunqlvdbnzxlwbfapqatfq";
	string wchqrzs = "sibdffereoolymowpcbmwwdyjqnibfokeogktkbuerkixwmfsbl";
	bool vszwqmucsrrwa = true;
	string zikukujpeo = "bh";
	int nqfjcmekufsvsx = 3504;
	string tjngyedtaicencd = "yrxxppgenhbbkwhkartsymnaajfrlazcikqumvvvrvkthuuvwhwumorqkagvxpzpczmbnmyfzacqoolywmiyvggowacmlsbgncnz";
	string nghpzshqixicppw = "vhthmreoemhlwuvrtnafwrtwhinmspwlhkzvouqobaslmvchmwhrjkfwqondyjwlirrmnhalhajihookqtbzffpkggunmr";
	if (string("bh") == string("bh")) {
		int mgshtsrhcp;
		for (mgshtsrhcp = 52; mgshtsrhcp > 0; mgshtsrhcp--) {
			continue;
		}
	}
	if (true == true) {
		int kfxklxyh;
		for (kfxklxyh = 27; kfxklxyh > 0; kfxklxyh--) {
			continue;
		}
	}
	return 88428;
}

bool jkjunwr::yhisehspmluigiwlgrtvh(string mbmjoyjq) {
	double grrpxe = 13543;
	double gidvginkqpcuna = 89042;
	string jpqydjgcbydic = "bwjznrqgeonjriidmefggdpdgxmxswxblzvbjzqrlvgfxusfpxikgtgp";
	double cbyxrrojervqvpu = 328;
	int mmumksochfoj = 236;
	string gqqcspelxb = "ndkxmszcinjycmutpjfrpikflldmsmoltcrozdihedwrngjjtsqzzbpncdepvkyaqrltlvyqayoiij";
	double bkxdqzaif = 26699;
	if (string("bwjznrqgeonjriidmefggdpdgxmxswxblzvbjzqrlvgfxusfpxikgtgp") != string("bwjznrqgeonjriidmefggdpdgxmxswxblzvbjzqrlvgfxusfpxikgtgp")) {
		int wkjyl;
		for (wkjyl = 47; wkjyl > 0; wkjyl--) {
			continue;
		}
	}
	if (26699 != 26699) {
		int zmizr;
		for (zmizr = 60; zmizr > 0; zmizr--) {
			continue;
		}
	}
	if (string("ndkxmszcinjycmutpjfrpikflldmsmoltcrozdihedwrngjjtsqzzbpncdepvkyaqrltlvyqayoiij") != string("ndkxmszcinjycmutpjfrpikflldmsmoltcrozdihedwrngjjtsqzzbpncdepvkyaqrltlvyqayoiij")) {
		int tucicwazma;
		for (tucicwazma = 48; tucicwazma > 0; tucicwazma--) {
			continue;
		}
	}
	if (string("bwjznrqgeonjriidmefggdpdgxmxswxblzvbjzqrlvgfxusfpxikgtgp") == string("bwjznrqgeonjriidmefggdpdgxmxswxblzvbjzqrlvgfxusfpxikgtgp")) {
		int dds;
		for (dds = 26; dds > 0; dds--) {
			continue;
		}
	}
	if (328 == 328) {
		int lhlxuefy;
		for (lhlxuefy = 64; lhlxuefy > 0; lhlxuefy--) {
			continue;
		}
	}
	return true;
}

bool jkjunwr::tmmdqjescvifnkxhtc(string jepwk, double xabcabqnlhyiph, int ayoloccy, string ithmvvqh, int gteftm, string okbfjhehvmqcp, int dvwnkjfdpthx, string uhjmqxyw, double wasyga) {
	string idgwwxjxz = "rzrynzpuuu";
	if (string("rzrynzpuuu") == string("rzrynzpuuu")) {
		int zgnpuuga;
		for (zgnpuuga = 36; zgnpuuga > 0; zgnpuuga--) {
			continue;
		}
	}
	if (string("rzrynzpuuu") != string("rzrynzpuuu")) {
		int fcucjdivc;
		for (fcucjdivc = 62; fcucjdivc > 0; fcucjdivc--) {
			continue;
		}
	}
	if (string("rzrynzpuuu") != string("rzrynzpuuu")) {
		int pmyweowll;
		for (pmyweowll = 96; pmyweowll > 0; pmyweowll--) {
			continue;
		}
	}
	if (string("rzrynzpuuu") != string("rzrynzpuuu")) {
		int dcpejfr;
		for (dcpejfr = 88; dcpejfr > 0; dcpejfr--) {
			continue;
		}
	}
	if (string("rzrynzpuuu") != string("rzrynzpuuu")) {
		int dkmqdp;
		for (dkmqdp = 87; dkmqdp > 0; dkmqdp--) {
			continue;
		}
	}
	return false;
}

jkjunwr::jkjunwr() {
	this->yhisehspmluigiwlgrtvh(string("litwwhdkhsr"));
	this->tmmdqjescvifnkxhtc(string("fpiwwuhfzqvsgvrhusybbrrduajzahmsyphlckd"), 7149, 6980, string("tjdsmaluqkgnmdzuirdzmdvqoxlabonkmpicgruypqyfxryyfmkpxxnlrx"), 3286, string("oveahrlzocbujvdxkinbcfeqyaiasidexmdncecjacbnjkamyaofmcwfnpagenzavtcq"), 2150, string("knluidixidnuamnergri"), 67085);
	this->olmsytoyfiznxnfdcu();
	this->zvksgeyxsafuqnwhaquocvvq(131);
	this->jbeikwoqwncz(false, 32388, string("nhvdchhpmfapp"), 345, string("huxlsaiblwiwnvpqtdtpbjyeeoinsjkrsrvulpnvguzvss"));
	this->yrvdnhbglguajv(true, 1047, 1525);
	this->dlhjgnvopsgdmirbztgvf(true, 22424, string("vnwsbkyrdvjpwenstkjwmjofkdafmitkjtuzumwwjefkdfawryvgbpuokdktdqpyzikkdljw"), true, string("tuznisvshcwtylhihpu"), false, string("dzvnzygdfnkbfspxlbwwoexdhvlafcqzedneegedkxlu"), 2587);
	this->usiaxespbhqfuga(12894, 4780, false);
	this->cczwmlhtjzxrprskvbw(string("eeksvzebomcofgkpqgblflfykytztzqmoxgxofbqrgapbyqliqjb"));
	this->isbrhwoaxirhztvcguxeegq(6332, 382, string("liacsemrddzkgccpdwxqwmmmpvdlwhrqt"), string("puqajphgstjvgryvksqkjfqsdoshhzniclcckiapwfqlzxbczwyhnlt"), true, string("ozbkoenjamvpeld"));
	this->vblfqdyidmskyx(string("mycpnujejalwhtpjmmglrsaozngieajhzjplazteplsnxccbrlbmshrhcblfrfxwxduluthbpvviurlefqmascp"), 32137, true, string("upkzcogmjxsydtgdmqnvmapbarhvzmvujsetklwjbtzwagflospohodlockkthydobgtkrznizbdmybbqdhuhinfcum"), string("wjekppkjdulmrahmsvszkhcwqfnlchfausbvowahoyvjcjsqijitic"), 27683, 52472);
	this->bsjnvorcwjqcrylo(true, string("msoktwtrzttcymwgktjnfvqwrnwoztfyjazcrklcxcyucdnsibgrvwiquln"), false, string("jcuxxztfdikiy"));
	this->ubabiqsyfkwcsfeufjlcjame(string("ndmzieevuzitydrgalasjrxpsw"), string("iswbohnxnruddfjztqaywydghmhvabapqkampmjnftgspmoktcgojmelolvztflauthhesfx"), true, true, 3713, 991);
	this->wshtllwwiulukiubajta(true, 649, true, 958, false, string("jrdslxzojxlbdwacpjqcxxidpyrkytbxxxlxsybtcvnbynihhddoitoxeeksghbmkmrbczilokbxmvvfyqhtsan"), true, 502);
	this->ynbmngmotnjfcuefxtzuo(25034, true, string("zdhkttgnpumytieumdfpdaivtrxiahdgxpgtamuzgrdrwgpgyhlmphyeckjjmpegejy"), 7141, string("ixvgfgrx"), string("jvpzgvyviqajxvt"), string("uvpuasntpaqcuwjyoitvvpwybhvudfigjmhuwfnucoiifkrbhxmbdjvinfethtlnq"), 2026, true);
	this->sbgzxdziwjgenncz(true, false, string("jhpniiukssgzhdrepwmtbekxzmlaswharhtzktvuukigt"), string("bdsneyaknpzvxojerpcjpwawviqrhoktzynnmayvopxpfxtnovzzyfcfxomoziicfe"), string(""), string("cpqpequwwgdhsjxbfpkaxnoqubxpjeycmuajhgkgnphaxelyrcylfbpgvasletreoiluredmzbhikpxjnorbju"), false, 1349, 298, 12476);
	this->jennujyoqxujnrlvwwquhfa(string("gtwjvveondnvslcatclgwamzdmvimhzqosxnmcqjwmetdtenrgfbcfbkwmamrelwlktjhb"), false);
	this->watfrymyhnsesk(2688, 5801, string("lhomezvtkhonwycdhfplgpozdeixhoxlqjzt"), true, string("aaxtpevpsyymxliepcvhzruzaihbspjjzaahbaasjcwqqclnsfbbdfjvchhtqxmomffnktzhxyinwxfw"), 38268);
	this->kizabjweqnxbocuajpg(12726, 281, 1209, 722);
	this->avtrbjnghrdkznjmbitovdzqb(false, 2228, true, 17114, 49954, 2153);
	this->tjvdoulotdupmvj(string("zxwbdvmphkgaskayvsmviygsujfronochabbdfpcbefllikncoxoksuhoebcyuvgkgjseeocyeifrfhziv"), 6105, 5443, string("vbbodizdwd"), 3479, false, 3189);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class trfivjo {
public:
	string ehfkejkrbyls;
	string qbadwqwecrbwmm;
	int npeymmtos;
	trfivjo();
	string qjdyhwqlnruypnzxftcic(double qbwhppm, bool sfvevbzp, bool zcvjvxgidymh);

protected:
	int pcidacegeqvticn;
	bool seoaoaypnof;
	string whnqtdowkso;
	string wqajpznolhdsrvc;
	string phbitcupgtr;

	int vozprgshrkgiryhumipy(double hxamrknykshlg, bool rshsdbqlf, string chtshke, bool heosxekq, string njbtwf, string xnohieyetnjd, bool pnczwezk, string fzvxuqejmyonbdj);
	void jqioaqwfie(bool qqsegbt, int acdrfsjphuq, double ykplkqv, int phkfxzpfsrhp, int xvyihg);
	string lmczdaztduzobxcwpgkaskik(bool nexwsgtmg);
	bool fofltbgoaezibumnhpais(double jziysfzbpwk, double dnsljacavuv, string pihnj, string thbzzvdfagzwueh, int tceeypxidoe, string mizwia, int hljhzk);
	string gydfzzigxykmfmjffgrrsxkv(bool zrbnaneqvaerslw, double hqpnvris, string qjchslieywzg, double zwquujuumg, double uadcdmet, double jcfdvnizla);
	double souygarjxltoztx(bool qccfjhhpiny, bool uoapvjnrfiwpjif, double bfbub, int duvzqq, double xnbwjn, int hfajpbjmyuxkqn, bool vdwxxbcptg, string duvtjqdfnqqlumo);
	string uwvtkasyfijcojpilmjw(string tnlwljois, bool zlugrzoxsmpybk, double ecmlpmythgcnpm, string bpzmm, int ebnjd, int tbztwfpko, string pcunnohppyn);
	string csxiqxqgei(string lyblsj, double kybvpdgfiryjdds, int xttvlr, double adsrke, int acyfge, int jgmgwvyjuicrn, int turqjibb);
	double facfclsvdqjfrtrhrdnfx(int tuouixikphgkduq, bool yofeitzkkjwyiim, double zcllzm, string pltujltwasnfw, bool rbxnpe, string phpwrrq, int dfefsdgr);

private:
	string sxfuuxyq;

	string vqmsmrqibbqtpaqokka(int capmypiu, string dfcpacpfyqbb, string dyjdkqhyy, int nquidflb, double gtpedxhaf, int zkdgwyvnrehc, string vofubt, int pplltwzgrpyr, string qfkihmauqv);
	int pnrwutrrltputuyt(int dggbqlkw, double mevfweh, double xzgjzgocasu, bool zwwmsjaaa, string elsclaopstzzvjr, int vldqrmtvfrkxlii);
	string hufhisgnig(string usfffog, string cdnehlmskkb, string jffybsrjsfw, string etpadyis, double bcxwiqjatoi, bool xiure, bool ajrxarlppcqxxbc, double lbucddquvavl);
	void fcohtxxoknqkmkdrkir(string zlmtetbhbqbtt, int usxtb, bool agrcfzp, string gteebgwejxjwrbd, bool jczbezo, double poxyjvntarlsmb);
	int txosgwqqhcunddr(int ditcxrv, string fztxlz);

};


string trfivjo::vqmsmrqibbqtpaqokka(int capmypiu, string dfcpacpfyqbb, string dyjdkqhyy, int nquidflb, double gtpedxhaf, int zkdgwyvnrehc, string vofubt, int pplltwzgrpyr, string qfkihmauqv) {
	int hfjouyjcthc = 795;
	string ctycdiq = "oatxlvdjxhwkwhhvlbgaifiqnmlhontbsbsgmaqey";
	string qsfkmlxqxojmz = "tcl";
	double qsznimxqjblaxch = 12067;
	bool bvxrpjdkeuvo = false;
	return string("qlfuixme");
}

int trfivjo::pnrwutrrltputuyt(int dggbqlkw, double mevfweh, double xzgjzgocasu, bool zwwmsjaaa, string elsclaopstzzvjr, int vldqrmtvfrkxlii) {
	double scrupewubab = 70720;
	bool tgtiuzxpsx = true;
	bool lsgjidpbnj = false;
	string eptndjcplsd = "cygmrnkfjaypzpfmsoloyrecfqohbywsrtkpgehwhejivdubsbthcmvzkufdzmvwtxamwxiuugawmyq";
	int tuivgcnrthrgmy = 4569;
	double phruzw = 34541;
	string uwetyehgiexsz = "qwcwrsefqpselpweqdyfwkoylz";
	double sdjuxscww = 18606;
	int ovraaryjrgiqcfj = 257;
	if (257 == 257) {
		int cn;
		for (cn = 24; cn > 0; cn--) {
			continue;
		}
	}
	return 87145;
}

string trfivjo::hufhisgnig(string usfffog, string cdnehlmskkb, string jffybsrjsfw, string etpadyis, double bcxwiqjatoi, bool xiure, bool ajrxarlppcqxxbc, double lbucddquvavl) {
	int zhkurrreuqkob = 3033;
	if (3033 == 3033) {
		int gqnizht;
		for (gqnizht = 96; gqnizht > 0; gqnizht--) {
			continue;
		}
	}
	if (3033 != 3033) {
		int zsrtnbnv;
		for (zsrtnbnv = 73; zsrtnbnv > 0; zsrtnbnv--) {
			continue;
		}
	}
	if (3033 == 3033) {
		int wjypqem;
		for (wjypqem = 42; wjypqem > 0; wjypqem--) {
			continue;
		}
	}
	if (3033 != 3033) {
		int lgxlwgz;
		for (lgxlwgz = 10; lgxlwgz > 0; lgxlwgz--) {
			continue;
		}
	}
	return string("binuyihhfjaiphnlemm");
}

void trfivjo::fcohtxxoknqkmkdrkir(string zlmtetbhbqbtt, int usxtb, bool agrcfzp, string gteebgwejxjwrbd, bool jczbezo, double poxyjvntarlsmb) {

}

int trfivjo::txosgwqqhcunddr(int ditcxrv, string fztxlz) {
	double rhunhnzwdbjxxh = 21252;
	string jlbodi = "tsfkkucqgeqnoeznvudusxbor";
	string ztjrdnt = "iaypox";
	return 99494;
}

int trfivjo::vozprgshrkgiryhumipy(double hxamrknykshlg, bool rshsdbqlf, string chtshke, bool heosxekq, string njbtwf, string xnohieyetnjd, bool pnczwezk, string fzvxuqejmyonbdj) {
	string lqkwjtypykt = "gcsypaqtpshmoojeeyhhaldvgkuscyomacsdfmvdkxtfxyusiqicnjpzzcruxgnahhgxkfaoesodzxwvduhdbcrehokqnfbeftg";
	int iixxrxvrkjqaat = 2388;
	double zfphdrim = 33558;
	string kziyumdgf = "jjxgnzevtrnvwgpbmuelebavwfwkyjixmbnoscezdtd";
	int gsofdhpgvjhxbe = 4371;
	string spzfwfwrx = "wrpybgnnduuqkhpgswxhkznfzmynpgzfhzdcrvg";
	if (string("wrpybgnnduuqkhpgswxhkznfzmynpgzfhzdcrvg") != string("wrpybgnnduuqkhpgswxhkznfzmynpgzfhzdcrvg")) {
		int svawhlml;
		for (svawhlml = 27; svawhlml > 0; svawhlml--) {
			continue;
		}
	}
	if (2388 == 2388) {
		int ecvdzxvc;
		for (ecvdzxvc = 65; ecvdzxvc > 0; ecvdzxvc--) {
			continue;
		}
	}
	return 59509;
}

void trfivjo::jqioaqwfie(bool qqsegbt, int acdrfsjphuq, double ykplkqv, int phkfxzpfsrhp, int xvyihg) {
	bool xrkaqmensexwwrk = true;
	int gstteleqaa = 8726;
	int ybhkvpevhyps = 5253;
	double filqyzlehyee = 47827;
	int oxrkrrmjfnw = 1455;
	bool txacferaq = false;
	if (false != false) {
		int camuevh;
		for (camuevh = 23; camuevh > 0; camuevh--) {
			continue;
		}
	}
	if (1455 == 1455) {
		int yzzoiyw;
		for (yzzoiyw = 61; yzzoiyw > 0; yzzoiyw--) {
			continue;
		}
	}
	if (false != false) {
		int xho;
		for (xho = 63; xho > 0; xho--) {
			continue;
		}
	}

}

string trfivjo::lmczdaztduzobxcwpgkaskik(bool nexwsgtmg) {
	int sgssnwvaasj = 5864;
	double udnkkilw = 21819;
	if (5864 == 5864) {
		int llwmeho;
		for (llwmeho = 19; llwmeho > 0; llwmeho--) {
			continue;
		}
	}
	if (21819 != 21819) {
		int sxdlfo;
		for (sxdlfo = 96; sxdlfo > 0; sxdlfo--) {
			continue;
		}
	}
	if (21819 != 21819) {
		int jlweua;
		for (jlweua = 52; jlweua > 0; jlweua--) {
			continue;
		}
	}
	if (21819 == 21819) {
		int ffhqr;
		for (ffhqr = 59; ffhqr > 0; ffhqr--) {
			continue;
		}
	}
	return string("jxdwgmm");
}

bool trfivjo::fofltbgoaezibumnhpais(double jziysfzbpwk, double dnsljacavuv, string pihnj, string thbzzvdfagzwueh, int tceeypxidoe, string mizwia, int hljhzk) {
	bool repuzchtpgyslu = true;
	double gvcwzkf = 30384;
	int hunqcujarzhk = 1929;
	int batlijqeau = 8056;
	double nbpanir = 1694;
	int wjrnagwsuuhp = 3613;
	string wbwacdnuyazy = "dxwqkoeckyxrqqmmwbmeosxgarnfkgnxledekfziydgjhshzdekujqmdgsmruenafwdzlaccbxbcbjsrqmnkktndugmd";
	bool tcbjq = true;
	string ppwodkgs = "poalnkltsfaliqaxyyojwusrudhigadcegunwymssuurhpxbsuznxzhmbq";
	bool qefmkcshgkqhp = false;
	if (true == true) {
		int uvokpdq;
		for (uvokpdq = 50; uvokpdq > 0; uvokpdq--) {
			continue;
		}
	}
	return false;
}

string trfivjo::gydfzzigxykmfmjffgrrsxkv(bool zrbnaneqvaerslw, double hqpnvris, string qjchslieywzg, double zwquujuumg, double uadcdmet, double jcfdvnizla) {
	double tuzza = 57887;
	if (57887 != 57887) {
		int pr;
		for (pr = 61; pr > 0; pr--) {
			continue;
		}
	}
	if (57887 != 57887) {
		int fkrrjia;
		for (fkrrjia = 85; fkrrjia > 0; fkrrjia--) {
			continue;
		}
	}
	return string("eecmndnldilopsnsuz");
}

double trfivjo::souygarjxltoztx(bool qccfjhhpiny, bool uoapvjnrfiwpjif, double bfbub, int duvzqq, double xnbwjn, int hfajpbjmyuxkqn, bool vdwxxbcptg, string duvtjqdfnqqlumo) {
	bool gqsnwqtsiawygwc = false;
	int eqxnezunaqp = 915;
	bool chkjfgomcjiwquv = false;
	int afhoqsv = 4277;
	int mmxvbppetj = 344;
	string sguskbfgmwcv = "szrjahkdtofxaoncinvxpfnptpnawfvejigpvrmvbolezhhppcgzwvwbeqeqwephfpdxe";
	string duqmdiyaoanyl = "phg";
	string suulufuhjpp = "dtcvcetljvnphwgommbadnxubkvwsfbscovsehcuwxzcxaaslylaalngeidravjurofslfraydiraccjaqhvpvhjpjpkshzlnpj";
	int djfgqfxf = 1616;
	bool sfbfwnho = true;
	if (string("szrjahkdtofxaoncinvxpfnptpnawfvejigpvrmvbolezhhppcgzwvwbeqeqwephfpdxe") != string("szrjahkdtofxaoncinvxpfnptpnawfvejigpvrmvbolezhhppcgzwvwbeqeqwephfpdxe")) {
		int zrxxvl;
		for (zrxxvl = 69; zrxxvl > 0; zrxxvl--) {
			continue;
		}
	}
	if (false != false) {
		int qqwgry;
		for (qqwgry = 40; qqwgry > 0; qqwgry--) {
			continue;
		}
	}
	if (string("szrjahkdtofxaoncinvxpfnptpnawfvejigpvrmvbolezhhppcgzwvwbeqeqwephfpdxe") == string("szrjahkdtofxaoncinvxpfnptpnawfvejigpvrmvbolezhhppcgzwvwbeqeqwephfpdxe")) {
		int mq;
		for (mq = 69; mq > 0; mq--) {
			continue;
		}
	}
	if (915 == 915) {
		int rqyf;
		for (rqyf = 98; rqyf > 0; rqyf--) {
			continue;
		}
	}
	if (1616 == 1616) {
		int iddqn;
		for (iddqn = 63; iddqn > 0; iddqn--) {
			continue;
		}
	}
	return 25151;
}

string trfivjo::uwvtkasyfijcojpilmjw(string tnlwljois, bool zlugrzoxsmpybk, double ecmlpmythgcnpm, string bpzmm, int ebnjd, int tbztwfpko, string pcunnohppyn) {
	double hdanupuicdszzxr = 31411;
	int qulowcaqw = 1945;
	return string("wpe");
}

string trfivjo::csxiqxqgei(string lyblsj, double kybvpdgfiryjdds, int xttvlr, double adsrke, int acyfge, int jgmgwvyjuicrn, int turqjibb) {
	bool hiyvmikamraiiy = false;
	string fkkihxf = "lgracnhfqynwlc";
	return string("qmshuucvzqbtqpvzipx");
}

double trfivjo::facfclsvdqjfrtrhrdnfx(int tuouixikphgkduq, bool yofeitzkkjwyiim, double zcllzm, string pltujltwasnfw, bool rbxnpe, string phpwrrq, int dfefsdgr) {
	bool tgntzojljpyh = false;
	bool birieuowtrqe = false;
	bool yzrocqejtywv = false;
	string cydnedlhocgkef = "ktmreppaoinmxjvoldwybqdlhqsfabwccglcswvkjynqzkcxxvdsxrtfyycxpicesiaaoiqiqnkhajxgorvvyykxniotary";
	string avnkmnvobbciryz = "ceorewbaosmsknexfvweubefbbgpcqykrciijcvnwhamtfaaxhntrsqkbfciuyuhgywswbaa";
	bool labqk = true;
	int ypzegbytxxmgpne = 2493;
	string kjltan = "acmptq";
	string iwcocqtuln = "u";
	double ermfdzagu = 42329;
	if (2493 == 2493) {
		int fjc;
		for (fjc = 42; fjc > 0; fjc--) {
			continue;
		}
	}
	if (false != false) {
		int xscywxa;
		for (xscywxa = 57; xscywxa > 0; xscywxa--) {
			continue;
		}
	}
	if (false == false) {
		int dfp;
		for (dfp = 24; dfp > 0; dfp--) {
			continue;
		}
	}
	if (false == false) {
		int upl;
		for (upl = 48; upl > 0; upl--) {
			continue;
		}
	}
	return 52885;
}

string trfivjo::qjdyhwqlnruypnzxftcic(double qbwhppm, bool sfvevbzp, bool zcvjvxgidymh) {
	return string("qzoyt");
}

trfivjo::trfivjo() {
	this->qjdyhwqlnruypnzxftcic(12489, false, false);
	this->vozprgshrkgiryhumipy(80549, false, string("xpnemxamrjsqgovoadkwtvynvjqjagwqulueicuwifgamnskyqwxfaymdrvllklaz"), false, string("yyfmdclwrilefioaiiybstbi"), string("onlblixrxvqroodddbsauwasmqwplhzwvbspowutsovucgapratpqxjdddkqizzzfpkbxeonbhgzdggrgrokgr"), false, string("nzcguxkjgszggmhhvgnb"));
	this->jqioaqwfie(true, 5278, 54118, 1948, 2644);
	this->lmczdaztduzobxcwpgkaskik(true);
	this->fofltbgoaezibumnhpais(10710, 19139, string("gvolilfqpzfucoomjfqxgaklzdst"), string("cwl"), 872, string("ysugdtqtnrithdllkxoqomdbcomyphq"), 1339);
	this->gydfzzigxykmfmjffgrrsxkv(true, 91613, string("tpsggzre"), 3634, 37851, 1113);
	this->souygarjxltoztx(false, false, 19689, 1801, 52413, 738, true, string("dmcjqqmowqzxfolhcddnajkymczkaoxungbpydobhbqwmcmngkyrlluajxgowqzhmdzgividwstqbsffdgdirvnaqwehimbuxywv"));
	this->uwvtkasyfijcojpilmjw(string("jdivcugixbmtitmlx"), true, 65171, string("wbpquokhrtwtzcfzksyygyqexxqdkcmaboa"), 3864, 859, string("khraiiougwyiryfpjahgjygxzganagzcekqtaeajz"));
	this->csxiqxqgei(string("gtgnxdfuapcpnxqxicjudfterdxiafzwcaeflaihofbgaxxfagy"), 20877, 7409, 5593, 991, 432, 3145);
	this->facfclsvdqjfrtrhrdnfx(149, false, 2038, string("tpozwdulltftnjtpcfftkmzxjwhdukitqhnsbzwamnnipcvnessvhdiimqogc"), false, string("qnrqlwnbmknzzmzfichhxzlvotiewg"), 2622);
	this->vqmsmrqibbqtpaqokka(1435, string("mdjszjhefwkyuiwyzpesoonsbr"), string("zafppbqbxixpzvceaqyldifkoetdoixxbbpqcwmdgtlghvsvnsxqkatmmihuhxryodewrfkbwabjzsoc"), 691, 46408, 18, string("dnmiqyitrvwuyvc"), 1246, string("vcsmpvdsbigharecmbzixgyqtimekkfodkcjeakjlalcuq"));
	this->pnrwutrrltputuyt(820, 6679, 30485, true, string("zupfzvtpsgbnbkfafazxsokscqpoebokpqnlqjtswwikydcokffshmhgyuuiblvswynytzpefohdunthyrpkheyxmbnlijurgim"), 3354);
	this->hufhisgnig(string("kcvtxbhncyopjjnxcrnrjbnkbfyxgonphwbbtekjfkubh"), string("ozztahgsgkjnasoi"), string("sxzmspckwjtfonfvzemkoejvnpyjxbpfeurmrbvivkqdxtchgkkxtpzzniphaacqnzxildtysbzyo"), string("tbhuhnpbenuddknvefwfheelwcndhczonjzrqvvv"), 78392, true, false, 2140);
	this->fcohtxxoknqkmkdrkir(string("bdtiwnnrbyfclsgpbapzxutcvadrlshuaashqklspptdiiyofthjuikkfzpxrexxqbnsixkkfpqkceznhzkv"), 3304, true, string("dczdjpolpdksrttxcwdrkkyhujbltfejpqprgxnxumdmidipksjv"), true, 696);
	this->txosgwqqhcunddr(5533, string("puoqwslgarlkfmqstdltrstcswxjcxnxouvsutehxboneaqlplkgvxlhkpoxjandnrmhqcnpqnttjchltxafnxjnjtxhwic"));
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class naxefsj {
public:
	bool yocbqjzculqe;
	string tlqbenhkandmvv;
	string mmboadcmikeig;
	naxefsj();
	int arelykbqlzxybwimytkuwyaru();
	double azkyojyusokgihzzdldlsobts(int bjrddorgxkoyamw, int memlzkuslkcd, string fjtju, int qxrxbijddlvx, string juhfrfdovmv, double gtsvmwporwjro);
	void wxvibjahpipysfjfizumcdozk(int dpvdicnqydyczy, int tsiisgsc, bool dyggv, bool sporu);
	int xpqtmkvlydhezpyw(double uhxdrpwpbza, double pwyod, bool rggwzukchfvqtcm, int mhwpogkifqzqlx, int fvxerfgfqf, bool qqedhghtecq, bool cmsmfokzqgdx);
	int hvaysxyoedznhdizlpqqhmcc(string ftzsdj, double ugzoshyasjwe, int lzjicmioajowo, int eiiabyod, int qybjaqy, int mnigozjkefjycpv);
	void uonnqwxbnrtnn(double vhzelaoryd, double vwtrmgvfhw);
	string rtglxxdjkgjhoztvorwi(double jphcgli);
	string skkklgshxplkisf(bool dptxhvcxbs);

protected:
	string ijvlqjwgbq;
	double lzkrzels;
	double ggpllfvahay;

	int hwhnpdwlieeumvtmt(bool ccnepdvjikuz, string ofbhuhu, string equlaqxynbai, double mxognl, double rioawlaqz, bool rpfevnjbczbveqy);
	bool nglrpojiykutnybn(string kkzcepghz, double xdhxngfwmha, double qflqxvbgq, string blxda, double zuekajq, string hfvwuvop, string jopcuk);

private:
	int vwjinnjypknpdc;
	double jvfgcfidivsg;

	bool hyjuoloaqxjd(string peofnjo, int bpzxybdkqnfzoz, double xqmqrtbqmacfm, string oivrgwrpsmq, double wwhfiar, int lezkscywf, string etvyqzvtqna);
	bool vegluqmbxatcoxifap(int yaqwb, string nbwtfgsrz, double eklrnlucpzw, double xmykke, double apysalbkkx, bool iphhttouvbzppbr, double ntfscdzid, string kfpmag, bool gfssiqqoylsmj);
	bool ntfshtjjnhiyqfcvb(int qptziozqsvy, bool olpovh, bool jawaz, string qhuhspvsfsfd);
	bool zzptsuxgnbgltcjnzzpwzvnx(int qqvepajgmturai, string rlhmdsvjalvacl, bool hiiymyvunjqhpml, string pcyjrzvkrjjkts, string urizlpozctcecg, int sjlppumu, double fvpienjblqmgbrv, double jsncpz, string mqkql, bool nirvmendximcgo);
	bool gigqwohribcbhlc(int bzhnkc, bool fqpfvoworcyefqb, int ffgkfwkwdtkecjx, double dhxdjeocbeezhu, int toagfeux);
	bool zynkrlzxdqeml(string lrdxjyvv, double drctameavwxg, int votnbwseblhwl, string ffiwwn, int reahkpfbomajw, int codmcygxzwxag, int gqykdrpitqhjfnh, int lcvxpladafu, bool mfpiireklr);

};


bool naxefsj::hyjuoloaqxjd(string peofnjo, int bpzxybdkqnfzoz, double xqmqrtbqmacfm, string oivrgwrpsmq, double wwhfiar, int lezkscywf, string etvyqzvtqna) {
	int uzuopvx = 2703;
	bool drqfrifman = true;
	double slahjuiaiwpw = 13256;
	bool ofbrdldywuqooim = true;
	if (true == true) {
		int hyffwuub;
		for (hyffwuub = 85; hyffwuub > 0; hyffwuub--) {
			continue;
		}
	}
	if (2703 != 2703) {
		int tur;
		for (tur = 86; tur > 0; tur--) {
			continue;
		}
	}
	return true;
}

bool naxefsj::vegluqmbxatcoxifap(int yaqwb, string nbwtfgsrz, double eklrnlucpzw, double xmykke, double apysalbkkx, bool iphhttouvbzppbr, double ntfscdzid, string kfpmag, bool gfssiqqoylsmj) {
	int ahzkmibbsxvqzj = 7051;
	int zjxxidmvcdyvz = 1895;
	bool kodrwduvfktwwrj = false;
	int cjdauzzz = 3183;
	bool awozsvprfbkfazk = true;
	string daboec = "nnjykhujmbztdgenvxrulixaxreaajfkdhcelnkfxiftnphjyzrjnscjdyfzjbrozbpfaetwy";
	bool iwiweakx = true;
	double gzqeqwjf = 57276;
	double wdjbsffksvpdkrg = 6420;
	int ulqredapkyshjw = 3397;
	if (false == false) {
		int cdizoqkp;
		for (cdizoqkp = 60; cdizoqkp > 0; cdizoqkp--) {
			continue;
		}
	}
	if (string("nnjykhujmbztdgenvxrulixaxreaajfkdhcelnkfxiftnphjyzrjnscjdyfzjbrozbpfaetwy") != string("nnjykhujmbztdgenvxrulixaxreaajfkdhcelnkfxiftnphjyzrjnscjdyfzjbrozbpfaetwy")) {
		int aj;
		for (aj = 64; aj > 0; aj--) {
			continue;
		}
	}
	if (57276 != 57276) {
		int xzvfqe;
		for (xzvfqe = 14; xzvfqe > 0; xzvfqe--) {
			continue;
		}
	}
	if (true == true) {
		int nvvaedxnnb;
		for (nvvaedxnnb = 41; nvvaedxnnb > 0; nvvaedxnnb--) {
			continue;
		}
	}
	return false;
}

bool naxefsj::ntfshtjjnhiyqfcvb(int qptziozqsvy, bool olpovh, bool jawaz, string qhuhspvsfsfd) {
	string umrgasiyhipsn = "cbetvptqdebxdlfanttfakppvqtfowofefhhlmdqygmmovyc";
	int xokahauypgf = 784;
	int lltoao = 505;
	string qppfnlekc = "tgafmwjmyainglzgoatkjcxnuxrksmhsjjexyqvhornuksglsmyzwkexknpzuijyqwmf";
	double nbxkymctzjj = 5477;
	string haqtpwzrdlj = "ltwuzslmjqpdlbzsbnnrijsboanxdlhoeyarudssxvedveecxnapzxbcwmcvnwsosejnzlhsvoxbsqucha";
	if (784 == 784) {
		int usbvdfm;
		for (usbvdfm = 40; usbvdfm > 0; usbvdfm--) {
			continue;
		}
	}
	if (string("cbetvptqdebxdlfanttfakppvqtfowofefhhlmdqygmmovyc") != string("cbetvptqdebxdlfanttfakppvqtfowofefhhlmdqygmmovyc")) {
		int rwxjwfi;
		for (rwxjwfi = 76; rwxjwfi > 0; rwxjwfi--) {
			continue;
		}
	}
	if (5477 == 5477) {
		int cunioqhdd;
		for (cunioqhdd = 58; cunioqhdd > 0; cunioqhdd--) {
			continue;
		}
	}
	if (505 == 505) {
		int bobrpja;
		for (bobrpja = 11; bobrpja > 0; bobrpja--) {
			continue;
		}
	}
	if (5477 == 5477) {
		int ngro;
		for (ngro = 100; ngro > 0; ngro--) {
			continue;
		}
	}
	return true;
}

bool naxefsj::zzptsuxgnbgltcjnzzpwzvnx(int qqvepajgmturai, string rlhmdsvjalvacl, bool hiiymyvunjqhpml, string pcyjrzvkrjjkts, string urizlpozctcecg, int sjlppumu, double fvpienjblqmgbrv, double jsncpz, string mqkql, bool nirvmendximcgo) {
	int thpzsnlqbvpwqi = 2647;
	double hlzudshsgekksrk = 38404;
	int gdiajvqufn = 821;
	bool ntsilrhn = false;
	bool hktwggsxbqo = true;
	string wdolzbdmrfrxrd = "zvjbxqoffgdfzcebpuqcklxdgqmhllfnjotdwcuykivrmiuuxsbpiptyduulvevd";
	if (true == true) {
		int ursrwzgb;
		for (ursrwzgb = 85; ursrwzgb > 0; ursrwzgb--) {
			continue;
		}
	}
	return true;
}

bool naxefsj::gigqwohribcbhlc(int bzhnkc, bool fqpfvoworcyefqb, int ffgkfwkwdtkecjx, double dhxdjeocbeezhu, int toagfeux) {
	bool xfiokgbepkszvp = true;
	double ukxaevgy = 19800;
	if (19800 != 19800) {
		int emgbyxlhh;
		for (emgbyxlhh = 6; emgbyxlhh > 0; emgbyxlhh--) {
			continue;
		}
	}
	if (19800 == 19800) {
		int wcpsut;
		for (wcpsut = 39; wcpsut > 0; wcpsut--) {
			continue;
		}
	}
	return true;
}

bool naxefsj::zynkrlzxdqeml(string lrdxjyvv, double drctameavwxg, int votnbwseblhwl, string ffiwwn, int reahkpfbomajw, int codmcygxzwxag, int gqykdrpitqhjfnh, int lcvxpladafu, bool mfpiireklr) {
	double ffhsvegi = 14923;
	double hheowpcfakmjw = 3581;
	string wonal = "qigcacekhkltu";
	if (14923 == 14923) {
		int pcyuu;
		for (pcyuu = 31; pcyuu > 0; pcyuu--) {
			continue;
		}
	}
	if (14923 == 14923) {
		int gyoxv;
		for (gyoxv = 75; gyoxv > 0; gyoxv--) {
			continue;
		}
	}
	if (3581 != 3581) {
		int dy;
		for (dy = 97; dy > 0; dy--) {
			continue;
		}
	}
	return false;
}

int naxefsj::hwhnpdwlieeumvtmt(bool ccnepdvjikuz, string ofbhuhu, string equlaqxynbai, double mxognl, double rioawlaqz, bool rpfevnjbczbveqy) {
	int jzbcafulf = 751;
	bool hztqxhxv = false;
	if (false == false) {
		int nlq;
		for (nlq = 79; nlq > 0; nlq--) {
			continue;
		}
	}
	if (false == false) {
		int ufksrujh;
		for (ufksrujh = 89; ufksrujh > 0; ufksrujh--) {
			continue;
		}
	}
	return 64332;
}

bool naxefsj::nglrpojiykutnybn(string kkzcepghz, double xdhxngfwmha, double qflqxvbgq, string blxda, double zuekajq, string hfvwuvop, string jopcuk) {
	return true;
}

int naxefsj::arelykbqlzxybwimytkuwyaru() {
	int frblf = 8306;
	string cbidahjhstm = "gehuuwelwkqchlrwcucu";
	if (string("gehuuwelwkqchlrwcucu") != string("gehuuwelwkqchlrwcucu")) {
		int qwzscaisd;
		for (qwzscaisd = 10; qwzscaisd > 0; qwzscaisd--) {
			continue;
		}
	}
	if (string("gehuuwelwkqchlrwcucu") != string("gehuuwelwkqchlrwcucu")) {
		int ijikvsp;
		for (ijikvsp = 27; ijikvsp > 0; ijikvsp--) {
			continue;
		}
	}
	if (8306 != 8306) {
		int nrwojo;
		for (nrwojo = 92; nrwojo > 0; nrwojo--) {
			continue;
		}
	}
	return 63944;
}

double naxefsj::azkyojyusokgihzzdldlsobts(int bjrddorgxkoyamw, int memlzkuslkcd, string fjtju, int qxrxbijddlvx, string juhfrfdovmv, double gtsvmwporwjro) {
	int agoul = 2466;
	double dimhd = 59481;
	if (2466 == 2466) {
		int eegyhhxuj;
		for (eegyhhxuj = 41; eegyhhxuj > 0; eegyhhxuj--) {
			continue;
		}
	}
	if (59481 == 59481) {
		int biy;
		for (biy = 95; biy > 0; biy--) {
			continue;
		}
	}
	if (2466 != 2466) {
		int qudhsgg;
		for (qudhsgg = 11; qudhsgg > 0; qudhsgg--) {
			continue;
		}
	}
	if (59481 == 59481) {
		int afmccrchh;
		for (afmccrchh = 1; afmccrchh > 0; afmccrchh--) {
			continue;
		}
	}
	if (2466 == 2466) {
		int oc;
		for (oc = 17; oc > 0; oc--) {
			continue;
		}
	}
	return 25451;
}

void naxefsj::wxvibjahpipysfjfizumcdozk(int dpvdicnqydyczy, int tsiisgsc, bool dyggv, bool sporu) {
	bool rspmwa = true;
	bool clcbntadobwda = false;
	double vwbjkzlbuf = 30802;
	if (30802 != 30802) {
		int utnsedatad;
		for (utnsedatad = 70; utnsedatad > 0; utnsedatad--) {
			continue;
		}
	}
	if (30802 != 30802) {
		int bnqtextyz;
		for (bnqtextyz = 75; bnqtextyz > 0; bnqtextyz--) {
			continue;
		}
	}
	if (30802 != 30802) {
		int aol;
		for (aol = 48; aol > 0; aol--) {
			continue;
		}
	}
	if (false == false) {
		int dj;
		for (dj = 90; dj > 0; dj--) {
			continue;
		}
	}

}

int naxefsj::xpqtmkvlydhezpyw(double uhxdrpwpbza, double pwyod, bool rggwzukchfvqtcm, int mhwpogkifqzqlx, int fvxerfgfqf, bool qqedhghtecq, bool cmsmfokzqgdx) {
	double vmldcgpaexteqnx = 3658;
	bool xqqwnaensqk = false;
	string ajqwqkhtkhyjvzd = "rfzdbkfntfygchkrylhvazxjdxiibvfam";
	return 69607;
}

int naxefsj::hvaysxyoedznhdizlpqqhmcc(string ftzsdj, double ugzoshyasjwe, int lzjicmioajowo, int eiiabyod, int qybjaqy, int mnigozjkefjycpv) {
	string vhudknnpfgo = "uuhamdhhceizrlh";
	int zubdxvd = 279;
	string sbycovfajonhbd = "vrguxuxmftkvoffngdimhvib";
	string wfruiwe = "gvrcnljvtclpvahozfkgmypnmsfqgsluufdfpriaxyduvsajiozqcjjbquuhksnlimtikiaxu";
	if (string("gvrcnljvtclpvahozfkgmypnmsfqgsluufdfpriaxyduvsajiozqcjjbquuhksnlimtikiaxu") == string("gvrcnljvtclpvahozfkgmypnmsfqgsluufdfpriaxyduvsajiozqcjjbquuhksnlimtikiaxu")) {
		int xsv;
		for (xsv = 47; xsv > 0; xsv--) {
			continue;
		}
	}
	if (string("vrguxuxmftkvoffngdimhvib") != string("vrguxuxmftkvoffngdimhvib")) {
		int xsdj;
		for (xsdj = 36; xsdj > 0; xsdj--) {
			continue;
		}
	}
	if (string("gvrcnljvtclpvahozfkgmypnmsfqgsluufdfpriaxyduvsajiozqcjjbquuhksnlimtikiaxu") == string("gvrcnljvtclpvahozfkgmypnmsfqgsluufdfpriaxyduvsajiozqcjjbquuhksnlimtikiaxu")) {
		int ftfico;
		for (ftfico = 61; ftfico > 0; ftfico--) {
			continue;
		}
	}
	if (string("gvrcnljvtclpvahozfkgmypnmsfqgsluufdfpriaxyduvsajiozqcjjbquuhksnlimtikiaxu") != string("gvrcnljvtclpvahozfkgmypnmsfqgsluufdfpriaxyduvsajiozqcjjbquuhksnlimtikiaxu")) {
		int gbwcgemv;
		for (gbwcgemv = 0; gbwcgemv > 0; gbwcgemv--) {
			continue;
		}
	}
	return 3525;
}

void naxefsj::uonnqwxbnrtnn(double vhzelaoryd, double vwtrmgvfhw) {
	bool wnqsepthyrtwzj = true;
	string orxaivt = "htlpdxbovaxkpctdihzvwlmtgnctvzmdldxudfpcqeugpbcgaqbrbhyvzosfiwxdusfwxdhbqngxjcyottjxwhpbj";
	double tvfyzarrkehcikf = 65500;
	string jiavdvjppynimrf = "ifwvzabbiexdodmkrmyklrmckkbpqylogxsjfegnutxymrhqipiwynvwrgngpinjttjnzstktdslvbtwma";
	bool pkfbmeanndasdg = true;
	if (true == true) {
		int bequtgstt;
		for (bequtgstt = 70; bequtgstt > 0; bequtgstt--) {
			continue;
		}
	}
	if (true != true) {
		int hietbvb;
		for (hietbvb = 28; hietbvb > 0; hietbvb--) {
			continue;
		}
	}
	if (string("ifwvzabbiexdodmkrmyklrmckkbpqylogxsjfegnutxymrhqipiwynvwrgngpinjttjnzstktdslvbtwma") != string("ifwvzabbiexdodmkrmyklrmckkbpqylogxsjfegnutxymrhqipiwynvwrgngpinjttjnzstktdslvbtwma")) {
		int pkprcblzky;
		for (pkprcblzky = 39; pkprcblzky > 0; pkprcblzky--) {
			continue;
		}
	}
	if (true != true) {
		int nx;
		for (nx = 31; nx > 0; nx--) {
			continue;
		}
	}
	if (string("ifwvzabbiexdodmkrmyklrmckkbpqylogxsjfegnutxymrhqipiwynvwrgngpinjttjnzstktdslvbtwma") != string("ifwvzabbiexdodmkrmyklrmckkbpqylogxsjfegnutxymrhqipiwynvwrgngpinjttjnzstktdslvbtwma")) {
		int zhfuhg;
		for (zhfuhg = 74; zhfuhg > 0; zhfuhg--) {
			continue;
		}
	}

}

string naxefsj::rtglxxdjkgjhoztvorwi(double jphcgli) {
	int fdnffaydqjsez = 2283;
	bool zgsvvrq = false;
	bool vxpgrpk = false;
	double obbsisfcxhzyx = 5221;
	string onjvoygvc = "oygngvtfghplcsnvrtwtezyasjwgnopp";
	if (5221 != 5221) {
		int ghk;
		for (ghk = 69; ghk > 0; ghk--) {
			continue;
		}
	}
	if (string("oygngvtfghplcsnvrtwtezyasjwgnopp") == string("oygngvtfghplcsnvrtwtezyasjwgnopp")) {
		int ijcvcndalp;
		for (ijcvcndalp = 77; ijcvcndalp > 0; ijcvcndalp--) {
			continue;
		}
	}
	return string("wynmmttcyxej");
}

string naxefsj::skkklgshxplkisf(bool dptxhvcxbs) {
	return string("xhmgsjxebbmkwaptpwm");
}

naxefsj::naxefsj() {
	this->arelykbqlzxybwimytkuwyaru();
	this->azkyojyusokgihzzdldlsobts(3463, 198, string("pkfppignvzzeafrzcdicmhfhmywlogdb"), 1301, string("liakdncdzezusdxhwolmordtrnavwudisjjtvfwwozstlfylrlpohwoazzcghjxhjxfihwu"), 20350);
	this->wxvibjahpipysfjfizumcdozk(1288, 515, true, false);
	this->xpqtmkvlydhezpyw(12072, 13996, true, 7158, 808, false, true);
	this->hvaysxyoedznhdizlpqqhmcc(string("rinwscxxomnwsextdxxvoeytdotrexqznfu"), 31570, 2907, 757, 613, 1235);
	this->uonnqwxbnrtnn(42833, 14957);
	this->rtglxxdjkgjhoztvorwi(4832);
	this->skkklgshxplkisf(false);
	this->hwhnpdwlieeumvtmt(false, string("osbshufajpsrhnhmjhjltcgnfbxdzjvn"), string("brqyqwnhj"), 4761, 32709, false);
	this->nglrpojiykutnybn(string("nsdagtqegposxiqsge"), 59432, 29579, string("jxhakiljg"), 28992, string("bzifkasp"), string("lkinfgydskfkvavylcmadecexwnh"));
	this->hyjuoloaqxjd(string("pspwhxsazkwitrpaeohtkhtfixovqnnvrlstlt"), 87, 13112, string("ictmthkfptnbykepivzpgnemtuccwyewgusilmrui"), 15058, 1031, string("jyxxfruljbejreiarelesfhue"));
	this->vegluqmbxatcoxifap(79, string("wkxzzqqwkmpqhynwzuqjgsoitoaoraihehxsajjpdstcgxykwdfn"), 24996, 13938, 36871, false, 1603, string("ltfbsmjiprlmfrflkildlgbenbaztlgazfwzabhauvgpdguhxvdajfcbcy"), true);
	this->ntfshtjjnhiyqfcvb(385, true, false, string("jogygrzlrzddvgsbmbljeiozyukuilsolzuoeitzobrdlcifwzdtmqgnarzecculemfbehcaeugp"));
	this->zzptsuxgnbgltcjnzzpwzvnx(1811, string("bhycuxlmpzjxnv"), false, string("nbrqpzcfjcmaovjqiivkvaftqeypfx"), string("fbmpssehywxdahlrixwke"), 787, 61195, 24567, string("ynqbjmxvrgiwgqgurrssxvfeumdwnswscmufhmwlhjhfkxbakrmh"), false);
	this->gigqwohribcbhlc(3142, true, 972, 49354, 4404);
	this->zynkrlzxdqeml(string("zxornhdeyqeghuxbzqqxsakfoaxcjstqfzmdhecjplcwmtoqrwlkcskexlzhefnvmzydvsimodarohp"), 7728, 1861, string("kqzcbzxretlnsbzhtlnkwatwhdzptesfe"), 1656, 2744, 3257, 2297, true);
}
