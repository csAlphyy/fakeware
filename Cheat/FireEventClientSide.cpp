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
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class gaktryf {
public:
	int otxci;
	int aytmgqihzfj;
	int azwueroeg;
	int nsihxozvtyfftn;
	gaktryf();
	void ltpzfnbrbduoilqgpsa(string rpyrvnwuqer);
	int djiyhokfodqj();
	int eoxvdntqkg(bool eqghris, int lnmsywfeyb, bool jqezgqzmh, double ypunhyrivga, bool ksenysxlx);
	string vtjjjcckpzcydlrqianopekci(int spuwbwq, double gitphurnprb, string coxtclytnu, double lylveyyyjekyqys, int dgqnwlk, int ocodjehdbsua, int vyfmi, bool yfjbvpdbt, bool gmablgduec);
	void skqbjnxyhfurchsdlcxa(bool ijpouwgkkax, double vpadbzy, double yuwvpbby, double cjvtmiikslthgi, bool xowktcnhmhcnv, bool edvzwmredon, bool gisem, bool htodoqidrvvit, int kcyiwessng);
	void lkorbfihqwyeeyzsurlzwblj();
	void zbwyhsmczfn(bool texngjfzng, string xbzysepilaufhe, string druqo, string blhnqejawmgmwiz, string dviaayapdsfhp, int kqvlzibkw, string ujhisniyq, int sqzpquwikrzbuj, bool suqamsfedo, int yltcmftnhfxhglo);
	void shgynpwlpn();
	bool gtlweptslkutu(string gywytbgtjqayin, double qyrhbki, string bgimshp, int zxpgtzhoieuc, int udttdziknpeh, double sdnjwzfhnl);
	double rwhwhoxowrckfbsseqqnsol(string ldmvuckdz, int dvqzxmqqfzoght);

protected:
	int iqkozcuvpwmfxb;
	int uahrmvpjuly;

	string tgycatsykeffdv(double zutfdgfdrcqbam, double wrdslhs, int cphwji, double pajfzmkmh, int zqsnqcj, double tbbbwmkur, double qkomwsonzxmgzj);
	void hoeygekacgybyckmccczkx(bool qbqfoj, int qmguzh, string qvfzcbsx, string jvllzdgc, double bbnxtdxhiorgwoc, int xhboypkwcato, double phiom, double bxaxdh, int ogipwwpy);
	string nefguwicqppkg(string rdfjil, bool bqtwnsmi, string wfxeu, double hcakidrzhpa);
	bool vxdxcuddjltinh(int tmbopurycl, string nbjcd, string wditbmlkv, string mcadxuhyekibctv);
	double ldgpbdyrolbz(double zflequysazyct, bool bhnfalv, string onelkcqbilju, int xielgontmnw);
	void orhzwnhimpdfwzyhwmeph(int llbvscqtfcc, string pkntfyuhrykn, string ywtdylgocyc, string czarsqposlmcod, int jxetugjpiwiveza, string mxysovoh);
	void bdyswrlbfizynwy(bool lcmrbrciyimbig, bool htzxufwqln, bool sizstomcycfbfwj, double kvhdkaramzetsjv, int smqvnuengm, double yxxvvrwmm, bool mafprmnjrpxauh);
	string mbrigtauvnrgueubwbzezku(string unnvwiod, bool uzzhbml, bool eeihgpppi, int vulmpeclmxqsh, double gyuxkvembhmyh, bool mynxktgkaryzbyk);
	int iekmcfbwyjrhyykhud(string czqyabpcq, string gefqra, int ecqygymumsz, double hqazpv, double pazexo, int jrlzfpt, bool dtislr, bool whovnql, bool myuzxysn);
	string sizhynmluzrmowlr();

private:
	double yzszbiwfxv;

	string obqirazgolaclcfwxrugjvk(bool fjztbeqekgiggck, string ifhrsiyyglpk, bool unygndwuuaeplb, bool uxdpnutmnnal, int bmmdogvbcwi, bool tlfrqhvqcfhv, bool asfjcye);

};


string gaktryf::obqirazgolaclcfwxrugjvk(bool fjztbeqekgiggck, string ifhrsiyyglpk, bool unygndwuuaeplb, bool uxdpnutmnnal, int bmmdogvbcwi, bool tlfrqhvqcfhv, bool asfjcye) {
	int aleiq = 3299;
	int klqhdubrgh = 5579;
	double kwviryqkxrl = 7540;
	bool nndsrzrtfw = false;
	int adnennvqw = 2649;
	double lzpzfzxltmzvnzm = 31887;
	double qlcvnudjtpur = 18236;
	if (5579 == 5579) {
		int kymbziynwf;
		for (kymbziynwf = 67; kymbziynwf > 0; kymbziynwf--) {
			continue;
		}
	}
	if (false == false) {
		int hednlt;
		for (hednlt = 90; hednlt > 0; hednlt--) {
			continue;
		}
	}
	return string("lzprdoarjjwmhsdurs");
}

string gaktryf::tgycatsykeffdv(double zutfdgfdrcqbam, double wrdslhs, int cphwji, double pajfzmkmh, int zqsnqcj, double tbbbwmkur, double qkomwsonzxmgzj) {
	string wwlyibl = "zhvethyufiyedpdaragxebpexomhclqmfjvwmulacxocnjuulilcdbqgvzjxhibxlmmypczpttjdzxirkcqlkefraphftkscoa";
	double cazylxpjisewxl = 48734;
	string ybbdigjeyfwyi = "zfrxkvusjecgprzqyqufvnfhpyctpexwtptygbdhjotxqrajtyultybpzoehszmmmxkik";
	string kgzejp = "xgikzgvxntjpixtgbtfnfodwmsganaqactaqnnrxkmwrsvvptmptyatrrpessoadyqlwbnxpsplfjlaxlqvokkineqqbz";
	int oxhclgma = 5431;
	double zbheevnivyiin = 57777;
	if (5431 == 5431) {
		int entfhvtb;
		for (entfhvtb = 80; entfhvtb > 0; entfhvtb--) {
			continue;
		}
	}
	if (57777 == 57777) {
		int regzc;
		for (regzc = 5; regzc > 0; regzc--) {
			continue;
		}
	}
	if (5431 != 5431) {
		int obxth;
		for (obxth = 23; obxth > 0; obxth--) {
			continue;
		}
	}
	if (string("zhvethyufiyedpdaragxebpexomhclqmfjvwmulacxocnjuulilcdbqgvzjxhibxlmmypczpttjdzxirkcqlkefraphftkscoa") != string("zhvethyufiyedpdaragxebpexomhclqmfjvwmulacxocnjuulilcdbqgvzjxhibxlmmypczpttjdzxirkcqlkefraphftkscoa")) {
		int yrrfu;
		for (yrrfu = 46; yrrfu > 0; yrrfu--) {
			continue;
		}
	}
	if (5431 != 5431) {
		int pec;
		for (pec = 7; pec > 0; pec--) {
			continue;
		}
	}
	return string("ypeyfrtvvzloix");
}

void gaktryf::hoeygekacgybyckmccczkx(bool qbqfoj, int qmguzh, string qvfzcbsx, string jvllzdgc, double bbnxtdxhiorgwoc, int xhboypkwcato, double phiom, double bxaxdh, int ogipwwpy) {
	double lvloxu = 35377;
	bool hrsfqctehfkvz = false;
	double ddvxhkddmn = 8365;
	double qszngjarqdqzqi = 16603;
	bool bwyxy = true;
	double gssbhjud = 769;
	int eyhjal = 5822;
	int udalzytsnv = 854;
	bool fubwyp = false;

}

string gaktryf::nefguwicqppkg(string rdfjil, bool bqtwnsmi, string wfxeu, double hcakidrzhpa) {
	string qbvpfgnssjoj = "awmnpfptjlirfakadbylnqeyuizhpiultobqcnqcgaxffcjvhbkyme";
	bool hbckurxuxscwi = true;
	string zgbywkjffjlyoyh = "uwzspciatmugsdtsc";
	string ubxqtmqnmcdo = "roxrxxhygrgfxvpxzuhhaphvvfakrtvdjesutlzj";
	int zunerrvaebnt = 6475;
	bool oknqrrbkxgx = true;
	if (string("roxrxxhygrgfxvpxzuhhaphvvfakrtvdjesutlzj") != string("roxrxxhygrgfxvpxzuhhaphvvfakrtvdjesutlzj")) {
		int gug;
		for (gug = 94; gug > 0; gug--) {
			continue;
		}
	}
	return string("lzjgyk");
}

bool gaktryf::vxdxcuddjltinh(int tmbopurycl, string nbjcd, string wditbmlkv, string mcadxuhyekibctv) {
	double bwkohrvgdzjblc = 54686;
	int pcwiukcvikhxegi = 2430;
	int imzwljiesfo = 404;
	int vtmakadcypf = 5032;
	string colakhm = "spdfiqbcoydqszkbhnkyenjywldviknktqmbramhevrtobgxsesawdlvftabogagafafrywcmwgoezsvnbwyhuekomzs";
	int iqttygbdxjoxc = 7152;
	bool spedr = false;
	string uzfardpyoufj = "dldylbldwquoijdrtwljjixwcvregkqltabs";
	bool urnlb = true;
	bool kyakmpixgz = true;
	if (false == false) {
		int misaizdbjd;
		for (misaizdbjd = 97; misaizdbjd > 0; misaizdbjd--) {
			continue;
		}
	}
	if (string("dldylbldwquoijdrtwljjixwcvregkqltabs") != string("dldylbldwquoijdrtwljjixwcvregkqltabs")) {
		int auv;
		for (auv = 0; auv > 0; auv--) {
			continue;
		}
	}
	if (false != false) {
		int oqoxfvsx;
		for (oqoxfvsx = 64; oqoxfvsx > 0; oqoxfvsx--) {
			continue;
		}
	}
	if (404 == 404) {
		int woc;
		for (woc = 51; woc > 0; woc--) {
			continue;
		}
	}
	if (false == false) {
		int ri;
		for (ri = 35; ri > 0; ri--) {
			continue;
		}
	}
	return false;
}

double gaktryf::ldgpbdyrolbz(double zflequysazyct, bool bhnfalv, string onelkcqbilju, int xielgontmnw) {
	int jecqluerkkum = 866;
	string mevmsvbnysn = "wgtpqxbkjgvsmanybbpdtboldatbkwbvrqngtyofwezugqsbksbfasnuxebjczlxnagak";
	double gsjvqvkjlmgre = 9176;
	string kslxhyjliw = "rjefblzdgkbaoyypmklnbgujprelsscydgvaitutohbvdfccqhyyqgfmrtmhbowwhrhkyc";
	bool cvjysjfeavf = false;
	double haptrgsaadj = 11640;
	double brmdrv = 60787;
	string epvkr = "udddcyckyzwisqrryckpyxagcuhschuznnrlfgazzkcffwwieueivlbsipjkwtmhruqhwaebavtvokaycggidptxytn";
	double keqqcyvmnzx = 13417;
	if (string("wgtpqxbkjgvsmanybbpdtboldatbkwbvrqngtyofwezugqsbksbfasnuxebjczlxnagak") == string("wgtpqxbkjgvsmanybbpdtboldatbkwbvrqngtyofwezugqsbksbfasnuxebjczlxnagak")) {
		int azvmirgft;
		for (azvmirgft = 90; azvmirgft > 0; azvmirgft--) {
			continue;
		}
	}
	if (13417 != 13417) {
		int hyfee;
		for (hyfee = 23; hyfee > 0; hyfee--) {
			continue;
		}
	}
	return 65975;
}

void gaktryf::orhzwnhimpdfwzyhwmeph(int llbvscqtfcc, string pkntfyuhrykn, string ywtdylgocyc, string czarsqposlmcod, int jxetugjpiwiveza, string mxysovoh) {
	int auvckqkiwprlu = 270;
	bool saimfwobk = true;
	bool yudaolcvlcam = false;
	string swlcbhxpu = "stfccqvqrrzqnydolipfxdzdcxhtrumrxdfenkwjxdatohanoybfinlmblsitlskqmcfehtivxjyuepkqpkludzl";
	string mpcnpjtk = "kfgvgo";
	string osiqtbl = "zaeoybmmctliopyoflelvhgpiyoahyuwqunni";
	string ossubmmrxmi = "slkbcnqjchkofzejaoraaacsjhanndpanrozrjhzakjbotzlsuzuurvwftkdofmwaunydxhwkgoufheyosmo";
	double zgynq = 11351;
	string cttyqz = "jqjrljxlyurlnyjweceviabewpnjzmmasfkultgonzmzwmbokzhyvvxasgbkdomf";
	if (string("zaeoybmmctliopyoflelvhgpiyoahyuwqunni") != string("zaeoybmmctliopyoflelvhgpiyoahyuwqunni")) {
		int lnixyamaci;
		for (lnixyamaci = 43; lnixyamaci > 0; lnixyamaci--) {
			continue;
		}
	}
	if (string("jqjrljxlyurlnyjweceviabewpnjzmmasfkultgonzmzwmbokzhyvvxasgbkdomf") == string("jqjrljxlyurlnyjweceviabewpnjzmmasfkultgonzmzwmbokzhyvvxasgbkdomf")) {
		int sxmnobmrw;
		for (sxmnobmrw = 27; sxmnobmrw > 0; sxmnobmrw--) {
			continue;
		}
	}
	if (string("kfgvgo") == string("kfgvgo")) {
		int pwxhs;
		for (pwxhs = 43; pwxhs > 0; pwxhs--) {
			continue;
		}
	}

}

void gaktryf::bdyswrlbfizynwy(bool lcmrbrciyimbig, bool htzxufwqln, bool sizstomcycfbfwj, double kvhdkaramzetsjv, int smqvnuengm, double yxxvvrwmm, bool mafprmnjrpxauh) {
	bool nvyyxzl = true;
	int qhija = 36;
	string sjytwfwr = "bvwxgnvqdriuay";
	string sxrdisjgmbde = "wgxyrqzngftsrozdavtfzqingbajduuesbkyioesnukpotlnrfjt";
	int zatraezcfy = 1296;
	if (string("bvwxgnvqdriuay") != string("bvwxgnvqdriuay")) {
		int fepcwfdpeh;
		for (fepcwfdpeh = 97; fepcwfdpeh > 0; fepcwfdpeh--) {
			continue;
		}
	}
	if (36 != 36) {
		int yfi;
		for (yfi = 44; yfi > 0; yfi--) {
			continue;
		}
	}
	if (string("wgxyrqzngftsrozdavtfzqingbajduuesbkyioesnukpotlnrfjt") != string("wgxyrqzngftsrozdavtfzqingbajduuesbkyioesnukpotlnrfjt")) {
		int riv;
		for (riv = 42; riv > 0; riv--) {
			continue;
		}
	}

}

string gaktryf::mbrigtauvnrgueubwbzezku(string unnvwiod, bool uzzhbml, bool eeihgpppi, int vulmpeclmxqsh, double gyuxkvembhmyh, bool mynxktgkaryzbyk) {
	bool cqoyjmjgzq = false;
	int eluxjcbdkqzgl = 2856;
	int fluricoyeq = 16;
	int qezzrrippfpnp = 1541;
	bool tpqwyrjqur = false;
	if (2856 != 2856) {
		int ykiv;
		for (ykiv = 7; ykiv > 0; ykiv--) {
			continue;
		}
	}
	if (16 != 16) {
		int mal;
		for (mal = 66; mal > 0; mal--) {
			continue;
		}
	}
	if (2856 != 2856) {
		int awa;
		for (awa = 18; awa > 0; awa--) {
			continue;
		}
	}
	if (false == false) {
		int vznpbqsl;
		for (vznpbqsl = 80; vznpbqsl > 0; vznpbqsl--) {
			continue;
		}
	}
	return string("lbejsktwexmagscosq");
}

int gaktryf::iekmcfbwyjrhyykhud(string czqyabpcq, string gefqra, int ecqygymumsz, double hqazpv, double pazexo, int jrlzfpt, bool dtislr, bool whovnql, bool myuzxysn) {
	string uzweehvlrkk = "ikvzimxssbygssicvkacfbwpsehuazhxkopbszuxxgzwhdmaiqbibnqkcdsfppzwhlgfbkwyknn";
	double mquytazol = 466;
	int nyjhuaih = 668;
	string evmdydh = "ppewfkarwpbnfsq";
	if (466 == 466) {
		int mclcdwbiqg;
		for (mclcdwbiqg = 94; mclcdwbiqg > 0; mclcdwbiqg--) {
			continue;
		}
	}
	if (string("ikvzimxssbygssicvkacfbwpsehuazhxkopbszuxxgzwhdmaiqbibnqkcdsfppzwhlgfbkwyknn") != string("ikvzimxssbygssicvkacfbwpsehuazhxkopbszuxxgzwhdmaiqbibnqkcdsfppzwhlgfbkwyknn")) {
		int qyri;
		for (qyri = 63; qyri > 0; qyri--) {
			continue;
		}
	}
	if (668 != 668) {
		int nx;
		for (nx = 36; nx > 0; nx--) {
			continue;
		}
	}
	if (string("ikvzimxssbygssicvkacfbwpsehuazhxkopbszuxxgzwhdmaiqbibnqkcdsfppzwhlgfbkwyknn") != string("ikvzimxssbygssicvkacfbwpsehuazhxkopbszuxxgzwhdmaiqbibnqkcdsfppzwhlgfbkwyknn")) {
		int wodhslx;
		for (wodhslx = 2; wodhslx > 0; wodhslx--) {
			continue;
		}
	}
	return 49848;
}

string gaktryf::sizhynmluzrmowlr() {
	bool hlbzl = false;
	int ckgmmdanl = 334;
	int lyitouukxovfg = 6606;
	if (6606 == 6606) {
		int ftl;
		for (ftl = 96; ftl > 0; ftl--) {
			continue;
		}
	}
	if (6606 != 6606) {
		int voaugfwaz;
		for (voaugfwaz = 15; voaugfwaz > 0; voaugfwaz--) {
			continue;
		}
	}
	return string("fciyaj");
}

void gaktryf::ltpzfnbrbduoilqgpsa(string rpyrvnwuqer) {
	bool cxkxhxhethwkqir = true;
	string ogzibzepdawceuj = "bmcebigaudnauyz";
	double vovaqxvzwddfua = 9975;
	string oujgr = "gafyveoqxmqfzeexbbdmbjrsmgnxivnspnknbsnkhdujaaxwazgchpigcejncmfpqfhlizvyksauhnryqatzehkqzwyg";
	string hhruyxtfpatyqje = "iougfiezjlhfxzbufstzuqrdeinyqcetqegmtmsebhfmplidwianbpmbmvfafecxnxilgnagmeqvhckdtcw";
	double wjpscaldrkf = 9135;
	bool gxdyjifucia = false;
	if (9975 == 9975) {
		int azbonto;
		for (azbonto = 22; azbonto > 0; azbonto--) {
			continue;
		}
	}
	if (string("gafyveoqxmqfzeexbbdmbjrsmgnxivnspnknbsnkhdujaaxwazgchpigcejncmfpqfhlizvyksauhnryqatzehkqzwyg") == string("gafyveoqxmqfzeexbbdmbjrsmgnxivnspnknbsnkhdujaaxwazgchpigcejncmfpqfhlizvyksauhnryqatzehkqzwyg")) {
		int nzyzp;
		for (nzyzp = 27; nzyzp > 0; nzyzp--) {
			continue;
		}
	}

}

int gaktryf::djiyhokfodqj() {
	double hjqpuzlxvg = 7215;
	string ixylbzylikibvc = "dcmyfokzh";
	bool ctrhdctxmxp = true;
	if (string("dcmyfokzh") == string("dcmyfokzh")) {
		int qxa;
		for (qxa = 72; qxa > 0; qxa--) {
			continue;
		}
	}
	if (string("dcmyfokzh") != string("dcmyfokzh")) {
		int qlzsy;
		for (qlzsy = 17; qlzsy > 0; qlzsy--) {
			continue;
		}
	}
	if (true != true) {
		int jwuddodovi;
		for (jwuddodovi = 63; jwuddodovi > 0; jwuddodovi--) {
			continue;
		}
	}
	return 68243;
}

int gaktryf::eoxvdntqkg(bool eqghris, int lnmsywfeyb, bool jqezgqzmh, double ypunhyrivga, bool ksenysxlx) {
	bool jdvtcvhhswcy = false;
	string wqvjut = "bwthmtvnhykvmkqyxcqtruvoazmqhposrfr";
	double ymvbzifafimc = 4919;
	bool wiashjepyhn = true;
	bool mbxcovnakjaw = false;
	double lbveocy = 38671;
	string vuzxmbrj = "fzpoqzrdctbautew";
	double uiiypvqjds = 47054;
	bool dekliy = true;
	if (38671 == 38671) {
		int hewuql;
		for (hewuql = 29; hewuql > 0; hewuql--) {
			continue;
		}
	}
	if (false == false) {
		int jqqsok;
		for (jqqsok = 37; jqqsok > 0; jqqsok--) {
			continue;
		}
	}
	return 6107;
}

string gaktryf::vtjjjcckpzcydlrqianopekci(int spuwbwq, double gitphurnprb, string coxtclytnu, double lylveyyyjekyqys, int dgqnwlk, int ocodjehdbsua, int vyfmi, bool yfjbvpdbt, bool gmablgduec) {
	int pgtahxplembb = 4260;
	string wqknbqc = "ulynbxenufcrlcqxraghoquyystpjaxmvzkbnepgvfkcaffitqiwzpzdypzxonyrmf";
	double vjyqqzupdbxb = 7757;
	if (string("ulynbxenufcrlcqxraghoquyystpjaxmvzkbnepgvfkcaffitqiwzpzdypzxonyrmf") != string("ulynbxenufcrlcqxraghoquyystpjaxmvzkbnepgvfkcaffitqiwzpzdypzxonyrmf")) {
		int mywc;
		for (mywc = 42; mywc > 0; mywc--) {
			continue;
		}
	}
	return string("jak");
}

void gaktryf::skqbjnxyhfurchsdlcxa(bool ijpouwgkkax, double vpadbzy, double yuwvpbby, double cjvtmiikslthgi, bool xowktcnhmhcnv, bool edvzwmredon, bool gisem, bool htodoqidrvvit, int kcyiwessng) {
	double prveybopqgxuimk = 4746;
	string spydygndkysoi = "oovieplleyaskwiakmrkiwuixhfszyorytkuymuatrdle";
	int ilwnbaeghlznb = 2348;
	bool ddgevfal = true;
	int tdmkp = 1066;
	if (2348 == 2348) {
		int wtgrb;
		for (wtgrb = 85; wtgrb > 0; wtgrb--) {
			continue;
		}
	}
	if (4746 == 4746) {
		int ndxzcso;
		for (ndxzcso = 34; ndxzcso > 0; ndxzcso--) {
			continue;
		}
	}
	if (4746 != 4746) {
		int coohhelj;
		for (coohhelj = 1; coohhelj > 0; coohhelj--) {
			continue;
		}
	}
	if (4746 != 4746) {
		int jbn;
		for (jbn = 63; jbn > 0; jbn--) {
			continue;
		}
	}
	if (2348 != 2348) {
		int wjpytlq;
		for (wjpytlq = 31; wjpytlq > 0; wjpytlq--) {
			continue;
		}
	}

}

void gaktryf::lkorbfihqwyeeyzsurlzwblj() {

}

void gaktryf::zbwyhsmczfn(bool texngjfzng, string xbzysepilaufhe, string druqo, string blhnqejawmgmwiz, string dviaayapdsfhp, int kqvlzibkw, string ujhisniyq, int sqzpquwikrzbuj, bool suqamsfedo, int yltcmftnhfxhglo) {

}

void gaktryf::shgynpwlpn() {
	double kotkmuvpkgfq = 75332;
	double oivggnpwy = 47973;
	double hdjxtgjkw = 65978;
	int qjqvugb = 582;

}

bool gaktryf::gtlweptslkutu(string gywytbgtjqayin, double qyrhbki, string bgimshp, int zxpgtzhoieuc, int udttdziknpeh, double sdnjwzfhnl) {
	return false;
}

double gaktryf::rwhwhoxowrckfbsseqqnsol(string ldmvuckdz, int dvqzxmqqfzoght) {
	double hlkxjcl = 25818;
	bool dzrzduyjuxosf = true;
	int vneqyl = 1457;
	string lxrbzbdyjgrgp = "egdakxbfpizokbbavsegdvkgeunkaiynlupsjrlfngoofxjpqdzrdumvqczsasgwplz";
	double jpzlr = 1847;
	bool bwtcslz = true;
	bool gbzlxpfja = true;
	if (25818 == 25818) {
		int mjtyc;
		for (mjtyc = 20; mjtyc > 0; mjtyc--) {
			continue;
		}
	}
	return 53356;
}

gaktryf::gaktryf() {
	this->ltpzfnbrbduoilqgpsa(string("yhdppjnqfikfdeiaorjnpweubdtritcjwenfeegipdkxidqljsttpffvtzhbeapdhymtdddiyutoxorppjzqtkicmgbutczxvk"));
	this->djiyhokfodqj();
	this->eoxvdntqkg(true, 1171, false, 7880, false);
	this->vtjjjcckpzcydlrqianopekci(2057, 52274, string("smehfoqfzjdwhpyumgqbnevwburujlypxctayflurygy"), 34918, 1766, 3272, 189, true, true);
	this->skqbjnxyhfurchsdlcxa(true, 14815, 22222, 13794, false, false, true, false, 519);
	this->lkorbfihqwyeeyzsurlzwblj();
	this->zbwyhsmczfn(false, string("qupepzpdbgluyvriwxpheitkurldlwlschfrpfscqa"), string("sqncpidbmbmpzdmqhdaknselgssouvpuuudapdgarwtl"), string("ujbrlfszuxcgmvwqzuzxfzclzbbnqqjzcctuoqbpwvfhc"), string("cgsyilwkuxlvvjnteldrzvotndywmibtiywlncavowrxxob"), 42, string("tqqdwclayqriatijuplhiqzllpatehixicesisznonrfedqjnuhqifutrrdhawauryapswehyxmieaewyhztnx"), 6168, true, 847);
	this->shgynpwlpn();
	this->gtlweptslkutu(string("jeyltivtswxeavbbgbpstsxbbvqfwb"), 16384, string("ogpuhyzrukiqgvfdmebvepkmamxgydfalyvqdyifimghlwteajjvlkahgpmyyjzmuskgtkkhglabwoadfv"), 236, 2614, 46874);
	this->rwhwhoxowrckfbsseqqnsol(string("rphbtsfomdigrqvoblzhurpsg"), 2125);
	this->tgycatsykeffdv(17864, 7743, 1857, 909, 5377, 10471, 16912);
	this->hoeygekacgybyckmccczkx(true, 2949, string("ukyhzmwemfzteyevvljewfwadzhiekuxvdbwokwtwxno"), string("vkdluwvvcdvlrkabplmcwhoiuephlgiuddkyspcexlniizaabzhlfzuxykkojerfahxoor"), 29065, 2863, 16283, 24588, 199);
	this->nefguwicqppkg(string("vdtmlcafunevqbtfrimpvkagipplzztwrmkxjrpmrwjkppmwjhopakwsodwj"), false, string("bcazhifr"), 8233);
	this->vxdxcuddjltinh(2348, string("dpyzahdeojaqifedwdoqjbgmnmqntdgfcnlzjtslbc"), string("brypfuipokgtyhpnrfkptxxhpmrnvlgcedpxrlidsdkpxwlhoevqtzkqfgcofuajgdzpgtnktoia"), string("qcapmlizjjlhycbucpuqqyhvfypdkbzgpcqtioptrzvxorxgnpv"));
	this->ldgpbdyrolbz(26873, true, string("wqjezpmcjnjnlhdbtretrfkeztvaxuwgzctdibyahalogrhzreukijirfirdezongcqk"), 1246);
	this->orhzwnhimpdfwzyhwmeph(2534, string("fenkjkjqeqwdiuztmbhmjjizlswndnjlrdgcleagiebifvmgaxdmrhfpbzqubody"), string("pugeziwybyovcyjpotcdkiqfajwxszeqifnmysywbujaapyfaywjpahmtpbgrotloxojlkk"), string("kvriqtjkszjtedqdgimifcqecpbiflopjqaggtysrbnontwioo"), 1331, string("maygxdpmladumciuaauxznebaztdtrulpyymeoergrvjcwxcuoedrpiadtcfrzizqnofkhqlaawgmofqmqkhuujstgchs"));
	this->bdyswrlbfizynwy(true, true, false, 27787, 273, 80643, false);
	this->mbrigtauvnrgueubwbzezku(string("jvakpsruevynbdduf"), true, true, 3725, 8006, false);
	this->iekmcfbwyjrhyykhud(string("rhyyowbctrthkbqyklbu"), string("hbnhcxitrgmjogjsvejwqqznjnuekweeupvtpavd"), 463, 26, 28041, 2005, true, true, false);
	this->sizhynmluzrmowlr();
	this->obqirazgolaclcfwxrugjvk(true, string("bcjmilekqlhnidydlnuedbemdcrxedkmydccalegqnhgvqrvdqixl"), false, false, 5602, false, false);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class zodgmlk {
public:
	bool tdcjkdvalogqbkn;
	bool rdtuu;
	zodgmlk();
	bool nbmxmtiafqmkdlxonpomypei(string xfcmjfrbgteay, double dlgau, double utczuvvjolvbic);
	int kdqcycyjvrv(double qajniknifb, int vcmdpojx, double utyxdxzwfvb, double przqiicq, int dwfaotsnsxqawv);
	bool dxwrqlnhorscrxnbrulbyuq(string gwzmfodmbblqt, string xwjjxfiuvvcfnh, string jzvbjcnwdadc, string ytbzcmj, string igeognjj, string zpvetsptvgzea, double eljajwlxqbr);
	bool tqbmllghdxtmu(double hwpolgowygahm, double evmhrkedpmi, double krfxu, double tupqfgijopquub, bool pnpaj, double onifiym);
	double lpxcggdnpqgn(double aqeqoiv, double fknssgrm, string bibfxa, string xzbvhghnfig);
	string ihirxajswxtzdyolay(double uyxztqunorkflt, int trcgyrfxajlzpk, int xskxilbp, string eniaweggmhlv, string bfynelnpvduxkp, bool fmeuynhnbmppinc, double cnzpygcmq, double cvnfm);
	int ughsaggljehdqafvhmxdvwt(int sgfhxgfkxzpsb, string vwebwrkhtezr, double ikvvxsvptgofd);
	bool owlgsbvsinukeldvhtvisd(int ffqelxqidc, string kgpxt, double gnubgogaqdac, bool sddbwl, int prykbw, double pgnnv, int fmzxzvly);
	string rgdelrnrrkyadslv(bool gqpzna, int nisrmprat, string eqkjkpq, bool kpvnuitpsiwnne, string hngrobcl, int lmpgjzmugjbd, bool ceobjmn, string hbcwmvga, int cthqelzld, int fjyhxrppggehkfs);
	int popxlpyvbyhuprkddtq(double cltbqctqdzfbmy, bool xzwpsi, bool hmmfxjihz, int sagwtmvdk, int lqrdjfwrzgu);

protected:
	double xwfidph;

	bool foftumewgkkrtghiu(int xatdpnbu);
	bool ssyhaawtmxmodatb(bool uwynmyghym, bool bnkcdromdrjh, bool lsshttgfr, double zpnhg, bool binmnyhya, double ylplfsuxfeq, bool rfbexybyd);
	int zwsntvmapcqtsuvjgwxp(string wnzmctyjp, string uritos, double tnntwuxbsnwikv);
	bool khxqxbijkjipilulgidxjfg(bool vzsvvfsnpvebxzd, int gqrpuuaaeyhet, double sjftdxtvxz, double vrsqwllefljpe, double tccuaeuwham, bool ulelftfypxlcpf);

private:
	string ktajtmu;
	bool ozpfinacekuboh;
	double rixbymwlcsjq;
	double xzlnxirxpoiuevi;

	void bqhbmvfuznbqi(double batdckbznts, bool qtkiwitbuklisxh, bool mpmqmv, int rfrmfnnk, string fvclflswgkjt, int wzjbvyhxfayibiz, double jjsezelyusmcayp);
	bool sppifksvyqxtvbqstmzhqr(int zfpeuzgp, int qqycui, double lwdbotdpeghygs, bool veamhpoe);
	bool wdwoewvbqjxdh();
	string sgbtoeoktgjovupxkvb(bool ydmmqtx, bool rxkymfxarzktjy, string hrapdavfedpm, string vfyepolyd, string jicsfyvtxxroe);
	bool inpeouifreljvdgdzomaqr(int amygmletk, double fuhszo, double jwrcchzgw);
	int jowwgcomvlrdlnfci(bool jyehadaaqgofrxu, int xtzpnakr, double sfbavl);

};


void zodgmlk::bqhbmvfuznbqi(double batdckbznts, bool qtkiwitbuklisxh, bool mpmqmv, int rfrmfnnk, string fvclflswgkjt, int wzjbvyhxfayibiz, double jjsezelyusmcayp) {
	bool qijkraepbezmavy = false;
	string nzzoqgtvvkuxl = "w";
	string fnhbjoqbvofji = "zummxshvqscpekeoalzplkikohqxynpmbjifvuwhyqenuztobwcajbtgwnvrubebykydhrkxdh";
	string jigooelkvfnjxo = "fltaulmivqcntqynqwequmlseppfta";
	string vipuzuhko = "ffuhrtghiaktdpojhpxnihublnomycpqirrujqcdvsbylnx";
	bool mgygo = false;
	bool bcevz = true;
	string hihcora = "anmsccijlqlevibuhzxxoxlaxatqgayywmxkubuoefeidokamogsufoqkyiwxvpsfnhjxefgs";
	if (false == false) {
		int etftyigbvp;
		for (etftyigbvp = 48; etftyigbvp > 0; etftyigbvp--) {
			continue;
		}
	}
	if (string("w") == string("w")) {
		int fbcwpyt;
		for (fbcwpyt = 69; fbcwpyt > 0; fbcwpyt--) {
			continue;
		}
	}
	if (string("ffuhrtghiaktdpojhpxnihublnomycpqirrujqcdvsbylnx") == string("ffuhrtghiaktdpojhpxnihublnomycpqirrujqcdvsbylnx")) {
		int zpaqz;
		for (zpaqz = 85; zpaqz > 0; zpaqz--) {
			continue;
		}
	}

}

bool zodgmlk::sppifksvyqxtvbqstmzhqr(int zfpeuzgp, int qqycui, double lwdbotdpeghygs, bool veamhpoe) {
	bool aldcq = true;
	double mpynqm = 29181;
	bool oexojxmpcobjsyz = false;
	bool tuarw = true;
	bool wbfywt = true;
	bool wtfzmskxpcoch = true;
	double acnrmlqlrroe = 32481;
	bool mtdwokty = false;
	string mmxixcxgiexbemn = "yqldoeyplhmtpggpbnqbntqcvyrcjjkzvqzptmf";
	bool touggwikjycxob = true;
	if (string("yqldoeyplhmtpggpbnqbntqcvyrcjjkzvqzptmf") != string("yqldoeyplhmtpggpbnqbntqcvyrcjjkzvqzptmf")) {
		int aovo;
		for (aovo = 23; aovo > 0; aovo--) {
			continue;
		}
	}
	return false;
}

bool zodgmlk::wdwoewvbqjxdh() {
	return true;
}

string zodgmlk::sgbtoeoktgjovupxkvb(bool ydmmqtx, bool rxkymfxarzktjy, string hrapdavfedpm, string vfyepolyd, string jicsfyvtxxroe) {
	string lcltdsklpqh = "ddfqdbicjqnhwyfcqaawopuaubzxdadbvfhynsrguyufidwvqwffevysfkrglvaavipsekqvzrzqeahr";
	string afysqkm = "jobwfkrlrneqhqamgrwilqdnutrkwysqwoievepwkdajiwpclbqbgpurqrr";
	int clvtjp = 6606;
	string mfpdqkhvfxhqwe = "daozystkpakdqqieklgbhjbydxuzxhrsgrqxtqvaosmzpdbljwjreg";
	double ueqobbkdaabbh = 73764;
	bool atykkdi = false;
	bool ifxpbtxzkvncces = false;
	bool emddvml = true;
	double upjsn = 22306;
	if (6606 == 6606) {
		int uj;
		for (uj = 92; uj > 0; uj--) {
			continue;
		}
	}
	if (string("jobwfkrlrneqhqamgrwilqdnutrkwysqwoievepwkdajiwpclbqbgpurqrr") == string("jobwfkrlrneqhqamgrwilqdnutrkwysqwoievepwkdajiwpclbqbgpurqrr")) {
		int oq;
		for (oq = 6; oq > 0; oq--) {
			continue;
		}
	}
	if (22306 == 22306) {
		int trbnwzfqrj;
		for (trbnwzfqrj = 2; trbnwzfqrj > 0; trbnwzfqrj--) {
			continue;
		}
	}
	return string("lww");
}

bool zodgmlk::inpeouifreljvdgdzomaqr(int amygmletk, double fuhszo, double jwrcchzgw) {
	string pkobudrfk = "uzhsgnprlu";
	int rqagncz = 6563;
	int ckfvqmkvsrr = 1747;
	bool ruhafkdyehjk = false;
	string afdjxn = "cxytkxzccqrcucyvyxgkjsxbozpmreefymmpkohdrojniyewxqklxaqiytyxul";
	bool gaiqlstginuoix = false;
	if (false != false) {
		int swbcvt;
		for (swbcvt = 69; swbcvt > 0; swbcvt--) {
			continue;
		}
	}
	if (1747 != 1747) {
		int gecbuhh;
		for (gecbuhh = 47; gecbuhh > 0; gecbuhh--) {
			continue;
		}
	}
	return false;
}

int zodgmlk::jowwgcomvlrdlnfci(bool jyehadaaqgofrxu, int xtzpnakr, double sfbavl) {
	bool tdyrctg = false;
	bool ghdttv = true;
	double uicdwkpnsmjjwo = 8048;
	int hqyxfaoyaii = 1561;
	bool aswsfmgo = true;
	double xelqx = 11267;
	if (1561 == 1561) {
		int tclueeli;
		for (tclueeli = 77; tclueeli > 0; tclueeli--) {
			continue;
		}
	}
	if (true == true) {
		int decqmph;
		for (decqmph = 69; decqmph > 0; decqmph--) {
			continue;
		}
	}
	if (11267 != 11267) {
		int whsfh;
		for (whsfh = 59; whsfh > 0; whsfh--) {
			continue;
		}
	}
	if (11267 == 11267) {
		int cf;
		for (cf = 51; cf > 0; cf--) {
			continue;
		}
	}
	if (8048 != 8048) {
		int ydqhrgxcm;
		for (ydqhrgxcm = 75; ydqhrgxcm > 0; ydqhrgxcm--) {
			continue;
		}
	}
	return 74823;
}

bool zodgmlk::foftumewgkkrtghiu(int xatdpnbu) {
	int gmythwwze = 597;
	if (597 != 597) {
		int fxroqn;
		for (fxroqn = 81; fxroqn > 0; fxroqn--) {
			continue;
		}
	}
	if (597 == 597) {
		int euyyccszb;
		for (euyyccszb = 40; euyyccszb > 0; euyyccszb--) {
			continue;
		}
	}
	if (597 != 597) {
		int cyaf;
		for (cyaf = 86; cyaf > 0; cyaf--) {
			continue;
		}
	}
	return false;
}

bool zodgmlk::ssyhaawtmxmodatb(bool uwynmyghym, bool bnkcdromdrjh, bool lsshttgfr, double zpnhg, bool binmnyhya, double ylplfsuxfeq, bool rfbexybyd) {
	bool hhotcitwggtwqi = true;
	string qsrzqo = "mftzcfxwphjsxmcguflxydphxmpppsutluayjyrklcvdjiiodrzwapjx";
	double djnffcztz = 59624;
	bool rxltb = true;
	double ozuryjh = 41290;
	string hdwqltasz = "d";
	double hdraxhnnjkwn = 12694;
	int dxxyjjw = 891;
	if (41290 != 41290) {
		int xfsvbl;
		for (xfsvbl = 88; xfsvbl > 0; xfsvbl--) {
			continue;
		}
	}
	return false;
}

int zodgmlk::zwsntvmapcqtsuvjgwxp(string wnzmctyjp, string uritos, double tnntwuxbsnwikv) {
	string yaipksn = "ofsrpssbjzaubzcrghboxumznvdkjzughqygwvomllnihlnam";
	double sscnazqonvtftt = 56835;
	string xagqjg = "uwfxreuqmsrbqzaodvumtp";
	int ehsetlka = 4616;
	int hubxwdiq = 2217;
	int weyipivnry = 1243;
	bool kufeid = false;
	int erppxpxary = 3835;
	if (4616 != 4616) {
		int phsjn;
		for (phsjn = 40; phsjn > 0; phsjn--) {
			continue;
		}
	}
	if (string("uwfxreuqmsrbqzaodvumtp") == string("uwfxreuqmsrbqzaodvumtp")) {
		int ofqc;
		for (ofqc = 47; ofqc > 0; ofqc--) {
			continue;
		}
	}
	if (false != false) {
		int ccoyswg;
		for (ccoyswg = 52; ccoyswg > 0; ccoyswg--) {
			continue;
		}
	}
	if (2217 != 2217) {
		int sk;
		for (sk = 0; sk > 0; sk--) {
			continue;
		}
	}
	if (string("ofsrpssbjzaubzcrghboxumznvdkjzughqygwvomllnihlnam") == string("ofsrpssbjzaubzcrghboxumznvdkjzughqygwvomllnihlnam")) {
		int zlbb;
		for (zlbb = 89; zlbb > 0; zlbb--) {
			continue;
		}
	}
	return 46805;
}

bool zodgmlk::khxqxbijkjipilulgidxjfg(bool vzsvvfsnpvebxzd, int gqrpuuaaeyhet, double sjftdxtvxz, double vrsqwllefljpe, double tccuaeuwham, bool ulelftfypxlcpf) {
	double quosb = 29351;
	double pvtbcsk = 29422;
	double xlaobfielmqwidv = 22446;
	double bxpsjnfptmv = 55410;
	int dbpkiqoz = 1829;
	bool yklobp = true;
	bool ogmmirkog = true;
	if (29351 != 29351) {
		int xwazqoy;
		for (xwazqoy = 27; xwazqoy > 0; xwazqoy--) {
			continue;
		}
	}
	if (55410 != 55410) {
		int zmhjcfhz;
		for (zmhjcfhz = 41; zmhjcfhz > 0; zmhjcfhz--) {
			continue;
		}
	}
	if (true == true) {
		int bnoyxk;
		for (bnoyxk = 58; bnoyxk > 0; bnoyxk--) {
			continue;
		}
	}
	return true;
}

bool zodgmlk::nbmxmtiafqmkdlxonpomypei(string xfcmjfrbgteay, double dlgau, double utczuvvjolvbic) {
	bool uigtfdwhapruyhk = false;
	int ussfpvgososenmx = 2826;
	double ucicaoupm = 5290;
	double numbrd = 97125;
	string blyfsejtgqpwrw = "ypuommqweuvvbklnyeoqmwulujspsdu";
	int gfsjh = 4387;
	double qavdaiofgzxkop = 2300;
	bool idkbuokah = false;
	bool nwvfpgof = false;
	if (string("ypuommqweuvvbklnyeoqmwulujspsdu") == string("ypuommqweuvvbklnyeoqmwulujspsdu")) {
		int ivpxuvcd;
		for (ivpxuvcd = 28; ivpxuvcd > 0; ivpxuvcd--) {
			continue;
		}
	}
	if (2300 != 2300) {
		int fq;
		for (fq = 37; fq > 0; fq--) {
			continue;
		}
	}
	if (string("ypuommqweuvvbklnyeoqmwulujspsdu") != string("ypuommqweuvvbklnyeoqmwulujspsdu")) {
		int btlt;
		for (btlt = 44; btlt > 0; btlt--) {
			continue;
		}
	}
	if (5290 == 5290) {
		int depqhhvfp;
		for (depqhhvfp = 51; depqhhvfp > 0; depqhhvfp--) {
			continue;
		}
	}
	if (2826 == 2826) {
		int sgybexy;
		for (sgybexy = 81; sgybexy > 0; sgybexy--) {
			continue;
		}
	}
	return true;
}

int zodgmlk::kdqcycyjvrv(double qajniknifb, int vcmdpojx, double utyxdxzwfvb, double przqiicq, int dwfaotsnsxqawv) {
	string crykoirjcdcpk = "xkfigaefrgffiidtnuuqfzqisohcfkpmbofjyyvmgcfzqdbdofhllhjzfhdktt";
	double fhkzdwhydeveol = 15556;
	bool wlozu = true;
	bool arviqmlejtjeky = false;
	bool kkkhkrtofo = false;
	bool ffgoxzvuafx = true;
	double mrorkbr = 8995;
	int epwtnlaaphndcq = 3511;
	bool shkuzceog = true;
	if (true == true) {
		int gjqfhz;
		for (gjqfhz = 100; gjqfhz > 0; gjqfhz--) {
			continue;
		}
	}
	if (3511 == 3511) {
		int bwxgtxpz;
		for (bwxgtxpz = 37; bwxgtxpz > 0; bwxgtxpz--) {
			continue;
		}
	}
	if (3511 == 3511) {
		int sx;
		for (sx = 75; sx > 0; sx--) {
			continue;
		}
	}
	return 75573;
}

bool zodgmlk::dxwrqlnhorscrxnbrulbyuq(string gwzmfodmbblqt, string xwjjxfiuvvcfnh, string jzvbjcnwdadc, string ytbzcmj, string igeognjj, string zpvetsptvgzea, double eljajwlxqbr) {
	double qtczjzwhgxe = 70258;
	string ccfbtfvwugnql = "wnkapn";
	int brmggp = 7703;
	double vjlia = 383;
	if (70258 != 70258) {
		int wlfcnohpel;
		for (wlfcnohpel = 67; wlfcnohpel > 0; wlfcnohpel--) {
			continue;
		}
	}
	if (7703 != 7703) {
		int gefugly;
		for (gefugly = 57; gefugly > 0; gefugly--) {
			continue;
		}
	}
	if (string("wnkapn") == string("wnkapn")) {
		int phbeyv;
		for (phbeyv = 51; phbeyv > 0; phbeyv--) {
			continue;
		}
	}
	if (70258 != 70258) {
		int ae;
		for (ae = 43; ae > 0; ae--) {
			continue;
		}
	}
	if (string("wnkapn") == string("wnkapn")) {
		int ftsuox;
		for (ftsuox = 100; ftsuox > 0; ftsuox--) {
			continue;
		}
	}
	return false;
}

bool zodgmlk::tqbmllghdxtmu(double hwpolgowygahm, double evmhrkedpmi, double krfxu, double tupqfgijopquub, bool pnpaj, double onifiym) {
	double jtixbmmm = 59565;
	return true;
}

double zodgmlk::lpxcggdnpqgn(double aqeqoiv, double fknssgrm, string bibfxa, string xzbvhghnfig) {
	int zubtxzkvp = 1274;
	bool nietdzvinhipao = true;
	int btitmbmpsyxhys = 1750;
	double thyit = 25720;
	bool qcmhnneger = true;
	if (1274 == 1274) {
		int qseflt;
		for (qseflt = 26; qseflt > 0; qseflt--) {
			continue;
		}
	}
	if (25720 != 25720) {
		int trw;
		for (trw = 93; trw > 0; trw--) {
			continue;
		}
	}
	if (1274 == 1274) {
		int rkhuws;
		for (rkhuws = 71; rkhuws > 0; rkhuws--) {
			continue;
		}
	}
	return 41735;
}

string zodgmlk::ihirxajswxtzdyolay(double uyxztqunorkflt, int trcgyrfxajlzpk, int xskxilbp, string eniaweggmhlv, string bfynelnpvduxkp, bool fmeuynhnbmppinc, double cnzpygcmq, double cvnfm) {
	return string("dimbpdzaqmdhcfa");
}

int zodgmlk::ughsaggljehdqafvhmxdvwt(int sgfhxgfkxzpsb, string vwebwrkhtezr, double ikvvxsvptgofd) {
	bool bebmefgmtduzpz = true;
	double arsxrddlzbsxkx = 14549;
	if (true == true) {
		int jzrxqdulc;
		for (jzrxqdulc = 16; jzrxqdulc > 0; jzrxqdulc--) {
			continue;
		}
	}
	if (14549 == 14549) {
		int wg;
		for (wg = 67; wg > 0; wg--) {
			continue;
		}
	}
	return 82185;
}

bool zodgmlk::owlgsbvsinukeldvhtvisd(int ffqelxqidc, string kgpxt, double gnubgogaqdac, bool sddbwl, int prykbw, double pgnnv, int fmzxzvly) {
	int lcdrcmfvddkyljr = 5780;
	double duldxuhirkaaprc = 19544;
	bool hlionj = false;
	bool tbeimtelnvjr = false;
	double admtzlodpimmmp = 51515;
	if (51515 == 51515) {
		int vu;
		for (vu = 80; vu > 0; vu--) {
			continue;
		}
	}
	return true;
}

string zodgmlk::rgdelrnrrkyadslv(bool gqpzna, int nisrmprat, string eqkjkpq, bool kpvnuitpsiwnne, string hngrobcl, int lmpgjzmugjbd, bool ceobjmn, string hbcwmvga, int cthqelzld, int fjyhxrppggehkfs) {
	double lxlxpqw = 6694;
	string befkrcnlh = "uikoyxbgikjtdsyxiqhwoqzynomklylgqo";
	bool qwjqlfnoishj = false;
	int qeamfnat = 5262;
	double gvcqzinc = 51965;
	bool wwwvuxvbjbjhz = false;
	if (false != false) {
		int uatmnsv;
		for (uatmnsv = 80; uatmnsv > 0; uatmnsv--) {
			continue;
		}
	}
	if (false == false) {
		int phx;
		for (phx = 39; phx > 0; phx--) {
			continue;
		}
	}
	if (string("uikoyxbgikjtdsyxiqhwoqzynomklylgqo") == string("uikoyxbgikjtdsyxiqhwoqzynomklylgqo")) {
		int dspbnyex;
		for (dspbnyex = 63; dspbnyex > 0; dspbnyex--) {
			continue;
		}
	}
	return string("eojqfbsocu");
}

int zodgmlk::popxlpyvbyhuprkddtq(double cltbqctqdzfbmy, bool xzwpsi, bool hmmfxjihz, int sagwtmvdk, int lqrdjfwrzgu) {
	return 93628;
}

zodgmlk::zodgmlk() {
	this->nbmxmtiafqmkdlxonpomypei(string("qvelbzlqactyzmaqszwxhepbjwnpvpqwfxsbvvuztmeosimbyjduvdrqplpnonwklwgbnoijkxbjybmsaapy"), 8397, 58945);
	this->kdqcycyjvrv(23221, 4598, 10217, 1352, 704);
	this->dxwrqlnhorscrxnbrulbyuq(string("ohfwtwgusgut"), string("jnilsxwwkttqeipglcsykap"), string("orjxyxzyxukqbzhraknkfhwp"), string("yigdwchuwidfwpkkhv"), string("skruxhkumjgmevsnjlyfcccigklnhametmydueqiwzvcmrgfvqiqpcxwzlhmjprodaapvbezufzqzmitpkjojulhklidbsom"), string("ugnmmrnjmikswlwyxcbhqqnedxxwtgqillsmvkvnmxchgyuvwaxvjtprab"), 8902);
	this->tqbmllghdxtmu(41329, 50254, 93306, 1016, true, 22244);
	this->lpxcggdnpqgn(15975, 1150, string("vcirkauykewlongdyqnnxlnndi"), string("srxwgukbbrmhakbkldvazthbygtbkwzdceck"));
	this->ihirxajswxtzdyolay(9304, 1536, 2275, string("eiicvrcdwbbotkr"), string("ayectfearsavsrabhtixohhizloxwoywvgbyccrorytgtscuyjcothopsn"), true, 81948, 2693);
	this->ughsaggljehdqafvhmxdvwt(4459, string("nmifhcquovlhdbuzezsqwhkfejhdpcnvitwzizcuxixzxobcqrcuqutbfgxakutvaiu"), 32668);
	this->owlgsbvsinukeldvhtvisd(3920, string("lulkonjjjjnytlsgagjkbilohhspkxxhygwkxerlhpwvslaqaylemvjayb"), 5674, false, 1661, 9383, 2108);
	this->rgdelrnrrkyadslv(true, 1239, string("vbgsdwpljwvcxjzeiaotxznkkcmlkujbnojuyvp"), true, string("metpboarwtblwnjfagwngrruyuypwgghlihscvzwbhzffygyvgghzikvgusgensuddthrsiryozyaxbowyhifylivssguqlq"), 680, true, string("hcpkshffvcevjlgssvodnwykdjhavhpktajvdhxbnyxtcxycohm"), 1443, 820);
	this->popxlpyvbyhuprkddtq(8101, false, false, 842, 301);
	this->foftumewgkkrtghiu(544);
	this->ssyhaawtmxmodatb(false, true, false, 54714, true, 1056, false);
	this->zwsntvmapcqtsuvjgwxp(string("amqkuqzziswwkvruqcmoxtmfu"), string("yosnyaysbtcqkprjwopjopajjtsxzlckolzwdyosfwxenvziootbuqk"), 15342);
	this->khxqxbijkjipilulgidxjfg(false, 999, 6111, 73322, 41000, false);
	this->bqhbmvfuznbqi(12302, false, true, 1384, string("ntenjqhrxxfapiwwltykanycraxawazygfubmqlxckjlbhzyxbdmk"), 3352, 26259);
	this->sppifksvyqxtvbqstmzhqr(4165, 6220, 10319, false);
	this->wdwoewvbqjxdh();
	this->sgbtoeoktgjovupxkvb(true, true, string("fxfsaaxrejeokuerxgnxwbjktclkclqrttqrklcjnlolpmdahwrgvdnqfaqrnylrddofiqllgmihmjsqfazufnpubi"), string("dtxlfncfdrnofowepexuyccdtyeicvrcuvcvwdpqlfmccwwmpwfqeltjmoalbphmwriayjytyjzycnezlmzv"), string("juaqircfnasgpnqwvkzbeovaahgskzsabmmqulvqqgimtrqspmpyqswhkmvsenrzxvvipkuj"));
	this->inpeouifreljvdgdzomaqr(4219, 53714, 25304);
	this->jowwgcomvlrdlnfci(false, 109, 3072);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ggaufav {
public:
	double hvimfmkmrsc;
	double usptylygprjj;
	ggaufav();
	void jowcxwzgzhvm(string rfoiyg, int pbonhxp);
	string otkzvueoltvucoetwtrrfowj();
	void kyfzfvpnev(double kygalxizj, string eumodpf, int zopbwvkwn, bool jptvenyvnnh, string ymoikz, double cqpkmebgu, bool avmxmda, double ocmpqgzpq, bool xongudo);
	string isojoezcbuvirtyn(string cwlvdv, string akbjmaubmktef, bool cpxbpnyemn, int jgxmnyibhydkykb, int kvzpeboata, double zphsqoceksj);

protected:
	string ilukc;
	double xvoerpcjt;

	string wnetceddrtlhnvvpbimhvedyn(bool sfphquxxa, bool szrgaftnp, string zcyqrfokm, string wpemuvbmsu, bool bfizpt, double aixpr, bool irdzelahrsjovk, bool qqnlb, int kfpueolvuz);
	void edflgukpan(int zbnwmf, int grtfbgmfwvattkh, int dqypc, string wnggvgqa, int oexsswwvjhgkslu, bool iztikhd, int udtyhfpn, int drsrytrs, int eowyigyfxgxv);
	int xfnzbtkoslgnochbb(double efdbbbqcu, string cabctnkn, int mtqgawhuct);
	bool dsbiuavhyzkcwarrlihhhblfc();
	void syamsmgnyuwf(int ghvoxal, double yffvl, int etfsjutxf, string qjpqs, double fdcujrwbtmnc, bool gupkcnss, double nkjcubasfej, string vpetmwrqmipjnvr);
	string hqqsxktypbpj(bool aeosbdhna, double yxggwu, bool mgnsgvo, double ednmduhe, string yfrifahftlaxtym, string kfeihbeggdbwk);
	void nfyaidamvxhpmkl(int hskrihifstjncxi, bool ioyexbfdspnznnd, string amxiwbje, double lfoztk, string nlryk, bool rukrmyyuav, int scffmfx);
	int xjdfkwdmiupkqomuw(string bhhnsgvgmy, int frsbfnbf, int wpqbrcwvydgg, bool ljtqqyvsvduaapr, bool vkqalijxrkr, double uegsxppfpq);
	double royjrfybvvldevgxdf(string yvffcxcevn, string fnkzjkfoeuakula, bool oskiljwczy, double nogktqyhw, double ohreyfql, bool qqkkhlghpnv, bool qvfzi, double bhtqweffdexnxry, string hqjnfhln, string voumvetd);
	bool hwqujjbmbrbuxxcuqf(int bervtkj, int khtiuvs, bool avlrpbwodjofryh, string mewba, string ybnkqzxbhw, string icwzgfiqudxwm, double bejwyxsaysqsnba, bool onqflmszdzeydfv, bool dyvzvuqfu);

private:
	double ajenweojhikehf;
	double ifrabmhoowmfomv;
	double bwxfgjobvfnna;
	int fnfpusc;
	bool khyqecqbj;

	int uzjkawiqrvfkmo(double mmyhfwpkae, bool zxktz, string yxgjlsdlleeip, string stopfno, string gdfzaxs, int otxpwdnfcxbn, string diivcvmaf, string badncegyulalq, int mjigvnkwgeqorov);

};


int ggaufav::uzjkawiqrvfkmo(double mmyhfwpkae, bool zxktz, string yxgjlsdlleeip, string stopfno, string gdfzaxs, int otxpwdnfcxbn, string diivcvmaf, string badncegyulalq, int mjigvnkwgeqorov) {
	string wocvxgwybjq = "yvsmbwwptulygpaxhsiuiuzjbcaoaqdkkneixfhrbewtfctleuiidnyhrxkvlibdnmufletulibkqxumdjwil";
	int qloldywkpmljlnz = 3770;
	bool nylagi = true;
	if (true == true) {
		int kuvonqhq;
		for (kuvonqhq = 4; kuvonqhq > 0; kuvonqhq--) {
			continue;
		}
	}
	if (true != true) {
		int xa;
		for (xa = 83; xa > 0; xa--) {
			continue;
		}
	}
	if (string("yvsmbwwptulygpaxhsiuiuzjbcaoaqdkkneixfhrbewtfctleuiidnyhrxkvlibdnmufletulibkqxumdjwil") != string("yvsmbwwptulygpaxhsiuiuzjbcaoaqdkkneixfhrbewtfctleuiidnyhrxkvlibdnmufletulibkqxumdjwil")) {
		int auh;
		for (auh = 99; auh > 0; auh--) {
			continue;
		}
	}
	if (string("yvsmbwwptulygpaxhsiuiuzjbcaoaqdkkneixfhrbewtfctleuiidnyhrxkvlibdnmufletulibkqxumdjwil") != string("yvsmbwwptulygpaxhsiuiuzjbcaoaqdkkneixfhrbewtfctleuiidnyhrxkvlibdnmufletulibkqxumdjwil")) {
		int ryq;
		for (ryq = 55; ryq > 0; ryq--) {
			continue;
		}
	}
	return 86750;
}

string ggaufav::wnetceddrtlhnvvpbimhvedyn(bool sfphquxxa, bool szrgaftnp, string zcyqrfokm, string wpemuvbmsu, bool bfizpt, double aixpr, bool irdzelahrsjovk, bool qqnlb, int kfpueolvuz) {
	int tgvqjpcakg = 2078;
	return string("bazzuitabyzeog");
}

void ggaufav::edflgukpan(int zbnwmf, int grtfbgmfwvattkh, int dqypc, string wnggvgqa, int oexsswwvjhgkslu, bool iztikhd, int udtyhfpn, int drsrytrs, int eowyigyfxgxv) {
	int dsppjqitsdw = 2396;
	bool lmwzon = false;
	double ezchxizeur = 5949;
	double ywyqduclljgt = 77452;
	double srwnql = 3756;
	int arlppt = 2355;
	double cqzmeuuj = 34626;
	bool mmtwphapkxmqb = false;
	if (5949 != 5949) {
		int ulowscqmd;
		for (ulowscqmd = 94; ulowscqmd > 0; ulowscqmd--) {
			continue;
		}
	}
	if (77452 != 77452) {
		int wwvjbzloqb;
		for (wwvjbzloqb = 33; wwvjbzloqb > 0; wwvjbzloqb--) {
			continue;
		}
	}
	if (false == false) {
		int cqua;
		for (cqua = 15; cqua > 0; cqua--) {
			continue;
		}
	}
	if (2355 == 2355) {
		int bfhlusoza;
		for (bfhlusoza = 52; bfhlusoza > 0; bfhlusoza--) {
			continue;
		}
	}
	if (2396 == 2396) {
		int uegkfkvtu;
		for (uegkfkvtu = 49; uegkfkvtu > 0; uegkfkvtu--) {
			continue;
		}
	}

}

int ggaufav::xfnzbtkoslgnochbb(double efdbbbqcu, string cabctnkn, int mtqgawhuct) {
	int lgnmhnokdmzwmdo = 1498;
	int ztrpygaaipta = 4662;
	int aieudwobcnl = 6685;
	double gbuaoht = 31596;
	double aiwornaxbk = 17766;
	bool uzrasnwniygt = false;
	return 98929;
}

bool ggaufav::dsbiuavhyzkcwarrlihhhblfc() {
	string gvylogtl = "dwsqnhwropjanwxdoytpfdffzuqnwehuwzduyluaysfqnbdu";
	bool rvyub = true;
	bool epzac = false;
	if (false != false) {
		int umfoo;
		for (umfoo = 40; umfoo > 0; umfoo--) {
			continue;
		}
	}
	if (true == true) {
		int ltiaarnx;
		for (ltiaarnx = 69; ltiaarnx > 0; ltiaarnx--) {
			continue;
		}
	}
	if (true != true) {
		int avbp;
		for (avbp = 95; avbp > 0; avbp--) {
			continue;
		}
	}
	if (string("dwsqnhwropjanwxdoytpfdffzuqnwehuwzduyluaysfqnbdu") != string("dwsqnhwropjanwxdoytpfdffzuqnwehuwzduyluaysfqnbdu")) {
		int dijkq;
		for (dijkq = 89; dijkq > 0; dijkq--) {
			continue;
		}
	}
	return true;
}

void ggaufav::syamsmgnyuwf(int ghvoxal, double yffvl, int etfsjutxf, string qjpqs, double fdcujrwbtmnc, bool gupkcnss, double nkjcubasfej, string vpetmwrqmipjnvr) {
	bool szivrswcmv = true;
	if (true != true) {
		int oqwkywb;
		for (oqwkywb = 2; oqwkywb > 0; oqwkywb--) {
			continue;
		}
	}
	if (true != true) {
		int ldfx;
		for (ldfx = 67; ldfx > 0; ldfx--) {
			continue;
		}
	}
	if (true != true) {
		int oyvquq;
		for (oyvquq = 65; oyvquq > 0; oyvquq--) {
			continue;
		}
	}
	if (true == true) {
		int bxuseqetv;
		for (bxuseqetv = 31; bxuseqetv > 0; bxuseqetv--) {
			continue;
		}
	}
	if (true != true) {
		int iocqg;
		for (iocqg = 32; iocqg > 0; iocqg--) {
			continue;
		}
	}

}

string ggaufav::hqqsxktypbpj(bool aeosbdhna, double yxggwu, bool mgnsgvo, double ednmduhe, string yfrifahftlaxtym, string kfeihbeggdbwk) {
	bool mqlotup = false;
	string rdzpbcyjq = "glpnunokkwyyrdbzixipiamipekbhgmwkcjwflnoghrqqeh";
	int pfwwrafi = 634;
	double bbukhxgwqan = 11235;
	string lwurwvjgtqkhph = "navlrhabrhtt";
	int bwlwpvqpd = 1212;
	string jdmijgbn = "zgihweo";
	if (1212 != 1212) {
		int cfjuiw;
		for (cfjuiw = 89; cfjuiw > 0; cfjuiw--) {
			continue;
		}
	}
	if (false != false) {
		int tjz;
		for (tjz = 2; tjz > 0; tjz--) {
			continue;
		}
	}
	return string("pbthrcuqktgambatbvu");
}

void ggaufav::nfyaidamvxhpmkl(int hskrihifstjncxi, bool ioyexbfdspnznnd, string amxiwbje, double lfoztk, string nlryk, bool rukrmyyuav, int scffmfx) {
	int ugytpseej = 4522;
	double xhyuxjmj = 26456;
	double ckslzwahg = 53299;
	string cutzpsge = "ylpdedkufpjkbsbyjzgxaqgvkhokiutpnbzudlvbskhwvyvewdrkhyjuvncxlfyssugoguactm";
	string yfmoycabxrkeu = "femiaghrkcrwjaluifuadodswvgpfwmgvthgsjmdnrzcxls";
	bool grzben = true;
	int bhhdg = 4854;
	double eltglpois = 18333;
	string byqmdocedxbjv = "eyvddyugkakxyoamarldiljpxprwbyildpjfphsylcbdnjuklszuvyornv";
	if (4522 != 4522) {
		int njxkjft;
		for (njxkjft = 4; njxkjft > 0; njxkjft--) {
			continue;
		}
	}

}

int ggaufav::xjdfkwdmiupkqomuw(string bhhnsgvgmy, int frsbfnbf, int wpqbrcwvydgg, bool ljtqqyvsvduaapr, bool vkqalijxrkr, double uegsxppfpq) {
	int mtalbfswbaki = 283;
	string xzotsikiinmn = "jwnuuorybsaqlyhwjnf";
	return 63906;
}

double ggaufav::royjrfybvvldevgxdf(string yvffcxcevn, string fnkzjkfoeuakula, bool oskiljwczy, double nogktqyhw, double ohreyfql, bool qqkkhlghpnv, bool qvfzi, double bhtqweffdexnxry, string hqjnfhln, string voumvetd) {
	return 29643;
}

bool ggaufav::hwqujjbmbrbuxxcuqf(int bervtkj, int khtiuvs, bool avlrpbwodjofryh, string mewba, string ybnkqzxbhw, string icwzgfiqudxwm, double bejwyxsaysqsnba, bool onqflmszdzeydfv, bool dyvzvuqfu) {
	int kbwplb = 1954;
	string wgkbirmou = "efpecqnjurtryqnscuorogxge";
	int tscprsrk = 1236;
	bool ypwea = true;
	double ghpnjpamlrs = 47357;
	bool wwzqjstslo = false;
	string cfwhszdhajrnklu = "rstcrojvtvdeiwkrbqwywjnpvxyhxpipjdijwaximgfqawdfurkriiqojjgtdgxeke";
	int ggbjallw = 4545;
	int bmznhavefdtnh = 6874;
	if (47357 == 47357) {
		int med;
		for (med = 97; med > 0; med--) {
			continue;
		}
	}
	if (1954 == 1954) {
		int xtpynsfp;
		for (xtpynsfp = 96; xtpynsfp > 0; xtpynsfp--) {
			continue;
		}
	}
	return false;
}

void ggaufav::jowcxwzgzhvm(string rfoiyg, int pbonhxp) {
	double mxynqmiywpkbdj = 67941;
	double usmtved = 18075;
	bool dwaucbjlhrsq = true;
	string ttnluuulpntf = "dmbyabvbqigszxtcormukddlnzuvcoccrqjkblufdebvxhdzvluyyrbnyebcmktgkfiyxpfexbil";
	int mubwep = 9120;
	string ozngiuxyed = "iclhvabxsmigbbpbcwucnsqpygpdnpqwtiuvragnhotbrabqhipbankttyeykulgsyilxdcq";
	double czyxf = 54376;
	int zdpvcxvfaqa = 1358;
	int wrcquonxldzxor = 8027;
	double fapel = 34006;

}

string ggaufav::otkzvueoltvucoetwtrrfowj() {
	bool qiqlrwodaohwinx = false;
	if (false != false) {
		int lwirimbxzj;
		for (lwirimbxzj = 49; lwirimbxzj > 0; lwirimbxzj--) {
			continue;
		}
	}
	if (false != false) {
		int wipzdavv;
		for (wipzdavv = 23; wipzdavv > 0; wipzdavv--) {
			continue;
		}
	}
	if (false != false) {
		int ltbdoks;
		for (ltbdoks = 69; ltbdoks > 0; ltbdoks--) {
			continue;
		}
	}
	if (false == false) {
		int lsiynarpyq;
		for (lsiynarpyq = 41; lsiynarpyq > 0; lsiynarpyq--) {
			continue;
		}
	}
	return string("xkryvpb");
}

void ggaufav::kyfzfvpnev(double kygalxizj, string eumodpf, int zopbwvkwn, bool jptvenyvnnh, string ymoikz, double cqpkmebgu, bool avmxmda, double ocmpqgzpq, bool xongudo) {
	bool cahmt = true;
	string fiffrhutosfmrsf = "sgxozajnlfeuahwwkhkdbfrbgwvcrdcz";
	bool pdbhklhbp = false;
	double yzzxld = 4979;
	int ycgxr = 1189;
	bool bxfsi = true;
	double sgiwc = 33940;
	double hnexkpavdpjvk = 19124;
	bool rghuusaosakyti = false;
	string exxncgsgglcpti = "rpffzvzzkbxkzprcbjokpupadxnmooefpqrzbhlzprhjiznenkqgqatwrljqccrypzhzsb";
	if (false != false) {
		int snraqj;
		for (snraqj = 56; snraqj > 0; snraqj--) {
			continue;
		}
	}

}

string ggaufav::isojoezcbuvirtyn(string cwlvdv, string akbjmaubmktef, bool cpxbpnyemn, int jgxmnyibhydkykb, int kvzpeboata, double zphsqoceksj) {
	string bdhydzle = "wlksiwjkdhjjahwdpoxmoalnwlrjvctqyaaobudzhwonpimsopbxcractmzihugbbrpvpffuuyhuidhjfysjurzpegqshasedygk";
	string sjqbvjqnswh = "xgtkzkgdohfmzlmpjrbrdmqcydtrlsfnmutjssmfyckitjhjfwgutbchhoteemkupukqzgugtwnos";
	int dwlrc = 2141;
	int dxephmenreeyjts = 5567;
	double obfyeyiuqtrmcoi = 58230;
	string eieewcildmhcogf = "gvivaghubmgqmlse";
	bool grfgih = true;
	double jngiuuivnwevyz = 3851;
	return string("gmiqhd");
}

ggaufav::ggaufav() {
	this->jowcxwzgzhvm(string("mi"), 1967);
	this->otkzvueoltvucoetwtrrfowj();
	this->kyfzfvpnev(6130, string("wbktothxwqhathtqqaedribyfpbitfomlrvetg"), 7567, true, string("okyjnktfrlxyrlkqqqlyxopxoxhfxqzicdegrpuhqbudqdmrbxfchcbiqgqykojuhfbpkxgadadmjutxikhqyx"), 2886, true, 8798, false);
	this->isojoezcbuvirtyn(string("toegyybpwntucxnbjabbrqlelxxrkymdvqblgk"), string("niojfeomjzm"), false, 3174, 6902, 21614);
	this->wnetceddrtlhnvvpbimhvedyn(true, false, string("cyjzswpyzreqpjpmaoupmrpgjohqiuewwcauziw"), string("asibqfexlcfumtmmhlesxsxoswiik"), false, 28972, false, true, 5129);
	this->edflgukpan(2728, 4181, 4065, string("xenwbbapfgbataiczkgpuej"), 4315, false, 982, 3239, 367);
	this->xfnzbtkoslgnochbb(43388, string("ppqcfljuogilfzqrlvjjjyikxtxbtocgqovbhyiijwjnsrl"), 879);
	this->dsbiuavhyzkcwarrlihhhblfc();
	this->syamsmgnyuwf(428, 14897, 133, string("snuzrlzfbubhrankbjsrmdcafyydgevpoypdltjgyivodflacc"), 6147, false, 69733, string("puoalxnjfhjecqyddfsjsaycbbqrknjakbska"));
	this->hqqsxktypbpj(false, 55324, false, 31586, string("fqiifavuxckzuevnr"), string("wsjahpofpjfzakclntamwzmskhzxzzyyxhsqyecwtfaskmpbrwbyagwdukzm"));
	this->nfyaidamvxhpmkl(1568, false, string("wqhbmiobykcs"), 10961, string("usnfxnnihqtqocnhzqpmzkutqixblhjflbjpwofhbndrywpttojuwpqvyboihriurqrchndgqhewtenxjyxthailsqxm"), true, 417);
	this->xjdfkwdmiupkqomuw(string("wcymwjzvfrszepnaatapvmnpgrpvnlsevyqcseozudtmbpkxjuypsfdtvpyaepsmfmmypgqxcnqku"), 51, 1785, false, false, 21436);
	this->royjrfybvvldevgxdf(string("ucrvlopgzxpsnojmbdrpu"), string("lolcrwqhgszmahsmxwtntqmzjguokwqakceazsnhydpcztqlj"), true, 2980, 20869, true, true, 46700, string("iqacnjpzmxedyxhtmslcjxpjmgqrcde"), string("sf"));
	this->hwqujjbmbrbuxxcuqf(623, 1590, true, string("xerkwdunioxbrtzybchhsrsrpuroqcxnfg"), string(""), string("wpqnwvvjuwrbqpdwcwwoiwq"), 11116, false, false);
	this->uzjkawiqrvfkmo(6908, false, string("iwzuutmfqtacgrzletkoywnsyusqv"), string("budutigbuqrtlnaqvzzqhgoqwbrdyostreteweuvegrvmwhxenwdfgwkwbwrqpwiqokvustocaiqnsbitdzosiclz"), string("prokgrrcejzifmrmthglhpjilxyjefpfndofjvnmsrjctoybqtnbxivvcpbokjjarmljslg"), 5973, string("ktzgjntbefyrlczkhcqfwcknfarubxfjfpjgpayhttoiitfpidhrlktwdmljuhyhcrgcovkqhgyqabvxbwsxcooliktani"), string("oozlgoopryizqlfyvbneepihjxpxxwgsrmyuosexxosjxkmitauljfrwedezxrsanhwhaqjgorckzbltwjwhlqxjkzeehcvjyl"), 5435);
}
