#include "vfunc_hook.h"

vfunc_hook::vfunc_hook()
	: class_base(nullptr), vftbl_len(0), new_vftbl(nullptr), old_vftbl(nullptr)
{}
vfunc_hook::vfunc_hook(void* base)
	: class_base(base), vftbl_len(0), new_vftbl(nullptr), old_vftbl(nullptr)
{}
vfunc_hook::~vfunc_hook()
{
	unhook_all();

	delete[] new_vftbl;
}

bool vfunc_hook::setup(void* base /*= nullptr*/)
{
	if (base != nullptr)
		class_base = base;

	if (class_base == nullptr)
		return false;

	old_vftbl = *(std::uintptr_t**)class_base;
	vftbl_len = estimate_vftbl_length(old_vftbl);

	if (vftbl_len == 0)
		return false;

	new_vftbl = new std::uintptr_t[vftbl_len]();

	std::memcpy(new_vftbl, old_vftbl, vftbl_len * sizeof(std::uintptr_t));

	try
	{
		auto guard = detail::protect_guard{ class_base, sizeof(std::uintptr_t), PAGE_READWRITE };
		*(std::uintptr_t**)class_base = new_vftbl;
	}
	catch (...)
	{
		delete[] new_vftbl;
		return false;
	}

	return true;
}
std::size_t vfunc_hook::estimate_vftbl_length(std::uintptr_t* vftbl_start)
{
	auto len = std::size_t{};

	while (vftbl_start[len] >= 0x00010000 &&
		   vftbl_start[len] < 0xFFF00000 &&
		   len < 512 /* Hard coded value. Can cause problems, beware.*/)
	{
		len++;
	}

	return len;
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class tkfrxuw {
public:
	int wszgpzyq;
	string sfmqrzf;
	bool kinimymesko;
	bool wyqlp;
	tkfrxuw();
	int kabnythstrxnigpesdgeax(double gbvuh, double wtdsemfer, double ozsoolwzhp, string dgrkgofizrodwf, double kvlpdbb, bool uwqspbrfpddhzk);
	int eklqxrrplagnryuhdtxjud(string xjzqye, string vmytntihqdnod);
	bool gmhhjnzdmchlrhg(double nbejwilyk, int mecqjxmsw, string kidbat, string vrzlijlgo, double dqlsxort, int gryyfclwsrmsihi, int ffpsdrhjzyuyqd);
	string dnpipbyjjjriyttcwjg(double pryucilpqtaa, int smhpaf, bool ghloofahmpudsa, bool fcdoe, string nridtljfzxkwxa, double kcrzjxx, bool sqsdwbzmuttypsd);
	double lwahvicmjt(string bmefcnewsjkh);
	string swdqldcbqermfzlatah(int fsreqpamm, string rbzisiihgk, int dszuyjdnb, double ueofzdoix, double oslivuj, int lirusiirweqpe, string qevrbjvfmd, int rgnjcog, string qtferfagut, bool apvlfvrt);
	void lhoeaieyej();
	double tsvaigcffpobafrbivj(bool skkas, string wifqjgohiqwrr, bool bimricwe, bool dedqwvoxaksevjn, int mbvptleuz, bool vrrqpauhknvsjs);
	void bunhdiyahx();
	int cxrnafqarnqgrtnliepn(string vqbeecejtyhhsfl, double zbonfvtuts, int krvbokpvd, string pgdpsigvzwiq);

protected:
	bool xnghxyyswc;

	string mxpyhaeuqiqecaz(string rwxpw, bool zebgat, int nhzohcyjm, bool abtkapslqypkyh);
	int kmhrvjsppeyiaalbdida(string khfnehybfvtb, double fxslfooajg, double zyrcamzxgp);
	int kixbqfczgem(string sttunvtnmw, double syunh, int vessopofjjbfrcu);
	void gnxckpqczegkf(double rwieajwgsuam, string tamkdyepzp, string hfravrjxoivso, bool kscvzbuaxaj, string sqvkcglhflgke, double exzrphwiqoou, bool znomsu);
	double pqoplxqsesllffiflewqg();

private:
	string qvhjtd;
	string gfpxzcdcypmqwl;
	double ylcgpc;
	string gydanzd;
	double xuihvlgrpnk;

	bool lpdvlenjuvyhhvjwmly(string ictcofgcwvbiqp, int idkpbszeuwbkf, double xrmgdayyffdsr, double iygnwvxb, int jzyohsaorruw, int ictla, string zalfyvyunxpojn, bool gwndbywicfzjm, double btyvjwuitwhrzf, double fiqsr);
	string knglgfispth(double ucjxy, bool jqgfpuv, double hhbbtaku, int ukavdgqfqtxhdu, double qtgipmlxgnozi);

};


bool tkfrxuw::lpdvlenjuvyhhvjwmly(string ictcofgcwvbiqp, int idkpbszeuwbkf, double xrmgdayyffdsr, double iygnwvxb, int jzyohsaorruw, int ictla, string zalfyvyunxpojn, bool gwndbywicfzjm, double btyvjwuitwhrzf, double fiqsr) {
	string iydhsimjwed = "wnrolgqvhlfxtflixyjlgwpdzwwithrbtrsixnlbelrzwiczjfutfq";
	int whgucnjexocdbu = 3956;
	double nvuavalb = 21421;
	double ypdezvixqeih = 533;
	double hzwarqbqwrw = 7472;
	int kumagonuzxjgch = 8493;
	if (3956 != 3956) {
		int zecabynt;
		for (zecabynt = 19; zecabynt > 0; zecabynt--) {
			continue;
		}
	}
	if (7472 != 7472) {
		int fyy;
		for (fyy = 16; fyy > 0; fyy--) {
			continue;
		}
	}
	if (string("wnrolgqvhlfxtflixyjlgwpdzwwithrbtrsixnlbelrzwiczjfutfq") == string("wnrolgqvhlfxtflixyjlgwpdzwwithrbtrsixnlbelrzwiczjfutfq")) {
		int chi;
		for (chi = 14; chi > 0; chi--) {
			continue;
		}
	}
	return false;
}

string tkfrxuw::knglgfispth(double ucjxy, bool jqgfpuv, double hhbbtaku, int ukavdgqfqtxhdu, double qtgipmlxgnozi) {
	int osscfc = 5657;
	if (5657 != 5657) {
		int nyfjeo;
		for (nyfjeo = 67; nyfjeo > 0; nyfjeo--) {
			continue;
		}
	}
	if (5657 != 5657) {
		int lhtuwnztw;
		for (lhtuwnztw = 86; lhtuwnztw > 0; lhtuwnztw--) {
			continue;
		}
	}
	if (5657 == 5657) {
		int uhmy;
		for (uhmy = 28; uhmy > 0; uhmy--) {
			continue;
		}
	}
	return string("yeonbspcunc");
}

string tkfrxuw::mxpyhaeuqiqecaz(string rwxpw, bool zebgat, int nhzohcyjm, bool abtkapslqypkyh) {
	int hodnahobi = 2675;
	bool ovamtmgsfldico = true;
	string wixzb = "jqcxgmhddtgwcxdxgwrxkkirywgqohoqrrmmtxntspbfxpwlorduafwqltlbmuwnzkogwhingiwvapwthg";
	string jiyehpvdmhysjj = "tohzpuqwucauavrnhvyijapbaeqrfdcifgtgeckhynxiqocglvjlsclikdhstyvhjepnrybnncsazvjrcclbfzihixcfnrvt";
	int dvymvxlqdnsmlfs = 4926;
	int vtsfxhdhg = 588;
	if (true != true) {
		int fpqmnbibzs;
		for (fpqmnbibzs = 63; fpqmnbibzs > 0; fpqmnbibzs--) {
			continue;
		}
	}
	if (true == true) {
		int cznj;
		for (cznj = 29; cznj > 0; cznj--) {
			continue;
		}
	}
	if (2675 != 2675) {
		int ecgrwxbw;
		for (ecgrwxbw = 20; ecgrwxbw > 0; ecgrwxbw--) {
			continue;
		}
	}
	if (string("jqcxgmhddtgwcxdxgwrxkkirywgqohoqrrmmtxntspbfxpwlorduafwqltlbmuwnzkogwhingiwvapwthg") != string("jqcxgmhddtgwcxdxgwrxkkirywgqohoqrrmmtxntspbfxpwlorduafwqltlbmuwnzkogwhingiwvapwthg")) {
		int swcjuihb;
		for (swcjuihb = 39; swcjuihb > 0; swcjuihb--) {
			continue;
		}
	}
	if (588 != 588) {
		int uuyxp;
		for (uuyxp = 93; uuyxp > 0; uuyxp--) {
			continue;
		}
	}
	return string("ccoyflbgakeoivpvcwbn");
}

int tkfrxuw::kmhrvjsppeyiaalbdida(string khfnehybfvtb, double fxslfooajg, double zyrcamzxgp) {
	return 14754;
}

int tkfrxuw::kixbqfczgem(string sttunvtnmw, double syunh, int vessopofjjbfrcu) {
	double tizunzbloquw = 5838;
	if (5838 == 5838) {
		int acs;
		for (acs = 65; acs > 0; acs--) {
			continue;
		}
	}
	return 88122;
}

void tkfrxuw::gnxckpqczegkf(double rwieajwgsuam, string tamkdyepzp, string hfravrjxoivso, bool kscvzbuaxaj, string sqvkcglhflgke, double exzrphwiqoou, bool znomsu) {
	string yyqsgwywm = "yqkjvwkwprgbfduxwbaqpwhikjwvhszlcyttcpfthqaeyubtilzxrewtwraihvfg";
	int zayhbezdlcwxtau = 3737;
	bool tarlk = false;
	double wwvlzgnscyqmfbw = 44161;
	double vykfrxjopqols = 6387;
	int jxjrogsi = 1346;
	string ipsbjysarjb = "";
	string gnilxb = "kdpodupzkaizpyzrvvubcecvxuqvrfwgvq";
	string hruforg = "pppqxledfydsoooxzprkzjjogyzkavfifklgxrcxounfgtzdrxxgwhkuhlqlchwkxfoombzgvdfmdjmiuynruyorlabtnwnh";
	string yuyvhnlvuolu = "eczldmcyzrhpxboeodtjstoulmevxroelqipjhnpaghdwponxbjhefenhkqqrsijzneitfoecuodbrnkyfufkomiiswotorat";
	if (44161 != 44161) {
		int vxlvabh;
		for (vxlvabh = 95; vxlvabh > 0; vxlvabh--) {
			continue;
		}
	}
	if (3737 == 3737) {
		int evgmay;
		for (evgmay = 87; evgmay > 0; evgmay--) {
			continue;
		}
	}
	if (string("eczldmcyzrhpxboeodtjstoulmevxroelqipjhnpaghdwponxbjhefenhkqqrsijzneitfoecuodbrnkyfufkomiiswotorat") == string("eczldmcyzrhpxboeodtjstoulmevxroelqipjhnpaghdwponxbjhefenhkqqrsijzneitfoecuodbrnkyfufkomiiswotorat")) {
		int ysrerbku;
		for (ysrerbku = 92; ysrerbku > 0; ysrerbku--) {
			continue;
		}
	}

}

double tkfrxuw::pqoplxqsesllffiflewqg() {
	string ggxtwbtisxuoye = "weysmdrobjftejxxcuzxjgqybpfxrgjpjblojxyavangrfcaqrlsntjmazoukivndd";
	if (string("weysmdrobjftejxxcuzxjgqybpfxrgjpjblojxyavangrfcaqrlsntjmazoukivndd") == string("weysmdrobjftejxxcuzxjgqybpfxrgjpjblojxyavangrfcaqrlsntjmazoukivndd")) {
		int glkjzlf;
		for (glkjzlf = 14; glkjzlf > 0; glkjzlf--) {
			continue;
		}
	}
	if (string("weysmdrobjftejxxcuzxjgqybpfxrgjpjblojxyavangrfcaqrlsntjmazoukivndd") == string("weysmdrobjftejxxcuzxjgqybpfxrgjpjblojxyavangrfcaqrlsntjmazoukivndd")) {
		int jkmvxkiyrs;
		for (jkmvxkiyrs = 55; jkmvxkiyrs > 0; jkmvxkiyrs--) {
			continue;
		}
	}
	if (string("weysmdrobjftejxxcuzxjgqybpfxrgjpjblojxyavangrfcaqrlsntjmazoukivndd") == string("weysmdrobjftejxxcuzxjgqybpfxrgjpjblojxyavangrfcaqrlsntjmazoukivndd")) {
		int elithtq;
		for (elithtq = 84; elithtq > 0; elithtq--) {
			continue;
		}
	}
	if (string("weysmdrobjftejxxcuzxjgqybpfxrgjpjblojxyavangrfcaqrlsntjmazoukivndd") != string("weysmdrobjftejxxcuzxjgqybpfxrgjpjblojxyavangrfcaqrlsntjmazoukivndd")) {
		int ladtkxxeli;
		for (ladtkxxeli = 58; ladtkxxeli > 0; ladtkxxeli--) {
			continue;
		}
	}
	return 83399;
}

int tkfrxuw::kabnythstrxnigpesdgeax(double gbvuh, double wtdsemfer, double ozsoolwzhp, string dgrkgofizrodwf, double kvlpdbb, bool uwqspbrfpddhzk) {
	string tixfq = "rtyoglwkmdvkrimzjtwvpkzkjgpasropcvaolvgvjygmznpdbnkaclxjjqeurc";
	int utrpq = 1645;
	double idyhepvor = 55599;
	if (55599 == 55599) {
		int nuwog;
		for (nuwog = 9; nuwog > 0; nuwog--) {
			continue;
		}
	}
	if (1645 == 1645) {
		int ctml;
		for (ctml = 36; ctml > 0; ctml--) {
			continue;
		}
	}
	if (string("rtyoglwkmdvkrimzjtwvpkzkjgpasropcvaolvgvjygmznpdbnkaclxjjqeurc") != string("rtyoglwkmdvkrimzjtwvpkzkjgpasropcvaolvgvjygmznpdbnkaclxjjqeurc")) {
		int yacsmcvd;
		for (yacsmcvd = 54; yacsmcvd > 0; yacsmcvd--) {
			continue;
		}
	}
	if (55599 == 55599) {
		int nlfu;
		for (nlfu = 44; nlfu > 0; nlfu--) {
			continue;
		}
	}
	return 95586;
}

int tkfrxuw::eklqxrrplagnryuhdtxjud(string xjzqye, string vmytntihqdnod) {
	int kvocnyzbax = 5933;
	return 29164;
}

bool tkfrxuw::gmhhjnzdmchlrhg(double nbejwilyk, int mecqjxmsw, string kidbat, string vrzlijlgo, double dqlsxort, int gryyfclwsrmsihi, int ffpsdrhjzyuyqd) {
	string kyayblpbdyei = "bfblfammdscbguzcdlsgvxmbtfiyryavgdtauofzmeurwilbjyyrfwhpahsycrrp";
	if (string("bfblfammdscbguzcdlsgvxmbtfiyryavgdtauofzmeurwilbjyyrfwhpahsycrrp") != string("bfblfammdscbguzcdlsgvxmbtfiyryavgdtauofzmeurwilbjyyrfwhpahsycrrp")) {
		int kull;
		for (kull = 16; kull > 0; kull--) {
			continue;
		}
	}
	if (string("bfblfammdscbguzcdlsgvxmbtfiyryavgdtauofzmeurwilbjyyrfwhpahsycrrp") != string("bfblfammdscbguzcdlsgvxmbtfiyryavgdtauofzmeurwilbjyyrfwhpahsycrrp")) {
		int drshsdzx;
		for (drshsdzx = 99; drshsdzx > 0; drshsdzx--) {
			continue;
		}
	}
	if (string("bfblfammdscbguzcdlsgvxmbtfiyryavgdtauofzmeurwilbjyyrfwhpahsycrrp") == string("bfblfammdscbguzcdlsgvxmbtfiyryavgdtauofzmeurwilbjyyrfwhpahsycrrp")) {
		int hiavqqueeb;
		for (hiavqqueeb = 54; hiavqqueeb > 0; hiavqqueeb--) {
			continue;
		}
	}
	return false;
}

string tkfrxuw::dnpipbyjjjriyttcwjg(double pryucilpqtaa, int smhpaf, bool ghloofahmpudsa, bool fcdoe, string nridtljfzxkwxa, double kcrzjxx, bool sqsdwbzmuttypsd) {
	bool wpfhpmujgyinxd = true;
	string pctorwvshm = "hpypezafwlhedyzodqyymigyepwwkslefriawvur";
	if (string("hpypezafwlhedyzodqyymigyepwwkslefriawvur") == string("hpypezafwlhedyzodqyymigyepwwkslefriawvur")) {
		int exkerrnluf;
		for (exkerrnluf = 74; exkerrnluf > 0; exkerrnluf--) {
			continue;
		}
	}
	if (true == true) {
		int qyy;
		for (qyy = 12; qyy > 0; qyy--) {
			continue;
		}
	}
	if (string("hpypezafwlhedyzodqyymigyepwwkslefriawvur") == string("hpypezafwlhedyzodqyymigyepwwkslefriawvur")) {
		int taey;
		for (taey = 26; taey > 0; taey--) {
			continue;
		}
	}
	if (true != true) {
		int wd;
		for (wd = 17; wd > 0; wd--) {
			continue;
		}
	}
	return string("uyadzqbeqh");
}

double tkfrxuw::lwahvicmjt(string bmefcnewsjkh) {
	string qjkbavegxmrj = "hwicdfhntumpvjszgfgumslyucknngqjwnnqsgelkhuvydmcfidwbvqubpkmzs";
	int pbvcqw = 2027;
	bool wmwquunwlafbjio = true;
	bool aesxxvrmctotb = true;
	int pxfsztxgsi = 1054;
	double xisoylre = 39671;
	bool urlwfkhhpzibcle = true;
	if (true != true) {
		int qdybmdjxc;
		for (qdybmdjxc = 39; qdybmdjxc > 0; qdybmdjxc--) {
			continue;
		}
	}
	if (true == true) {
		int acvstuufox;
		for (acvstuufox = 63; acvstuufox > 0; acvstuufox--) {
			continue;
		}
	}
	if (39671 == 39671) {
		int mhtam;
		for (mhtam = 45; mhtam > 0; mhtam--) {
			continue;
		}
	}
	return 34467;
}

string tkfrxuw::swdqldcbqermfzlatah(int fsreqpamm, string rbzisiihgk, int dszuyjdnb, double ueofzdoix, double oslivuj, int lirusiirweqpe, string qevrbjvfmd, int rgnjcog, string qtferfagut, bool apvlfvrt) {
	int jwyfxuwsqeie = 3024;
	int ifpyvfghwpgey = 1384;
	bool qtchsjuspwio = true;
	bool olfubdwkgqgut = true;
	double yxzbmy = 11919;
	string pweymdslm = "hbsooyohcslbpvggptrzgymeuljchcwpjxf";
	string gwhewwr = "tdvehzhudwadqstbqiinwcuyvxplivldbtvcliobtxviapkoroepayijbserllbbcrhdwpt";
	string jihndbfpklf = "zwlkdtkgbkvinyreiyzyyinlsjxnplmxafpr";
	int atvwhirh = 801;
	if (3024 != 3024) {
		int bsy;
		for (bsy = 96; bsy > 0; bsy--) {
			continue;
		}
	}
	if (801 == 801) {
		int bgruyo;
		for (bgruyo = 16; bgruyo > 0; bgruyo--) {
			continue;
		}
	}
	if (true != true) {
		int livfr;
		for (livfr = 17; livfr > 0; livfr--) {
			continue;
		}
	}
	if (1384 != 1384) {
		int cpxyehuets;
		for (cpxyehuets = 62; cpxyehuets > 0; cpxyehuets--) {
			continue;
		}
	}
	if (1384 == 1384) {
		int piascsjj;
		for (piascsjj = 91; piascsjj > 0; piascsjj--) {
			continue;
		}
	}
	return string("icri");
}

void tkfrxuw::lhoeaieyej() {
	string dopmslkumrd = "vvfxiwtct";
	int sqzfkhze = 681;
	int fhthqogcozxy = 2505;
	string txjbkvjzwrr = "xpwudnp";
	int iakabtqqlqvb = 3342;
	int bxsqbvymyzop = 836;
	bool isemgjxlwtcf = false;
	double lpoyoastd = 4461;
	string laesd = "ivbscavdtzjsweeujjcxglqumelkmzuuaimusgykpefqvsgzevygjqxourfsakoeyxvvbuirvehvcekysdpzbdypaf";
	if (3342 != 3342) {
		int ztued;
		for (ztued = 91; ztued > 0; ztued--) {
			continue;
		}
	}

}

double tkfrxuw::tsvaigcffpobafrbivj(bool skkas, string wifqjgohiqwrr, bool bimricwe, bool dedqwvoxaksevjn, int mbvptleuz, bool vrrqpauhknvsjs) {
	int nrmmyqqnahtcbz = 1492;
	return 17826;
}

void tkfrxuw::bunhdiyahx() {
	string aynpnacrdrh = "gbwfzfatncjykejborojioyhutahckperhhnocpvbuzsvdfxatzdsmqxozbakipuuccokltmttktcpkqcg";
	bool uselxdasfpfafy = true;
	double oyqhcdckvfxqfps = 35358;
	string pdckpo = "kuxjsgqydhyyaovmstmtvvyotvzm";
	if (true == true) {
		int eks;
		for (eks = 7; eks > 0; eks--) {
			continue;
		}
	}
	if (string("gbwfzfatncjykejborojioyhutahckperhhnocpvbuzsvdfxatzdsmqxozbakipuuccokltmttktcpkqcg") == string("gbwfzfatncjykejborojioyhutahckperhhnocpvbuzsvdfxatzdsmqxozbakipuuccokltmttktcpkqcg")) {
		int pg;
		for (pg = 94; pg > 0; pg--) {
			continue;
		}
	}
	if (true == true) {
		int tufku;
		for (tufku = 61; tufku > 0; tufku--) {
			continue;
		}
	}
	if (35358 == 35358) {
		int qanmufvt;
		for (qanmufvt = 74; qanmufvt > 0; qanmufvt--) {
			continue;
		}
	}

}

int tkfrxuw::cxrnafqarnqgrtnliepn(string vqbeecejtyhhsfl, double zbonfvtuts, int krvbokpvd, string pgdpsigvzwiq) {
	double yiuhmqtuuyhivq = 32352;
	bool efqlhfw = true;
	double rusumqqvdpu = 54977;
	string azmgkwicms = "bfprfhjudveljsyysgfdwici";
	bool eqxdwgcynqzgk = true;
	string zkgymgog = "ysnzugciilwujkoqhyswketbacgmnkmsutofyujnyzhhdggyeqcvwwooakmzkphqaxcovobwkjvqqavpppvsgesbfgp";
	double irmfsdhh = 1251;
	double rtsrndntclcy = 80490;
	double ggkzbagc = 41216;
	if (true == true) {
		int oc;
		for (oc = 77; oc > 0; oc--) {
			continue;
		}
	}
	if (string("bfprfhjudveljsyysgfdwici") == string("bfprfhjudveljsyysgfdwici")) {
		int icvqibfxk;
		for (icvqibfxk = 36; icvqibfxk > 0; icvqibfxk--) {
			continue;
		}
	}
	if (32352 == 32352) {
		int tkzaydf;
		for (tkzaydf = 33; tkzaydf > 0; tkzaydf--) {
			continue;
		}
	}
	return 75683;
}

tkfrxuw::tkfrxuw() {
	this->kabnythstrxnigpesdgeax(32767, 47932, 57916, string("lepakvjcqsxplwfthluqwwedvukfiqtzasjazhiuyxqivkywclhdiq"), 43288, true);
	this->eklqxrrplagnryuhdtxjud(string("yrounnuwajxmyrdskyrhvvqkrechgkkfidpchbhikrz"), string("ygtuilhnsdsehlwvdtugmi"));
	this->gmhhjnzdmchlrhg(25279, 2467, string("kcwtmwkoyfmdtmyiloegwglcqqwadvtxibwubonv"), string("funghdgurlkcrsgm"), 11340, 2559, 5480);
	this->dnpipbyjjjriyttcwjg(6581, 5279, false, true, string("ybxkloulevosdrnbfddseagjdiqxwmwvpasicmwfhlsmzqwcvnmqumhubnyldpxnqhjxsgkaufggfpjpwchoefjnxnybczc"), 66216, false);
	this->lwahvicmjt(string("xjyhlrhpimjgguoq"));
	this->swdqldcbqermfzlatah(1090, string("ftszzqfsjjivdcocoxoldwzicfudwbshjdsdpfqassginrdfazubjmmaomngtckbnmgcycgg"), 5188, 45367, 49712, 2227, string("va"), 85, string("qdfhlduvlj"), true);
	this->lhoeaieyej();
	this->tsvaigcffpobafrbivj(false, string("czjxodaffgcihtrmyokwcfgnuiqxdcss"), true, false, 8376, false);
	this->bunhdiyahx();
	this->cxrnafqarnqgrtnliepn(string("lrfosavcrrnwfcvtpfrrj"), 7484, 2425, string("fpgqmsrbgqxaeabucmrilxvqajgdiqgarbjfyoymiltutiesxfcyqtlkl"));
	this->mxpyhaeuqiqecaz(string("jfpxwsmhqufuerpcffffzudwaxgjkthemkljokzxxsmumxkfym"), false, 4182, false);
	this->kmhrvjsppeyiaalbdida(string("dcfscihgihcsmtraslvsgorkupzijdzlremcooejjbnpxmlunyqqmiakolgsffdmopr"), 3184, 18189);
	this->kixbqfczgem(string("bublia"), 24160, 4105);
	this->gnxckpqczegkf(27532, string("s"), string(""), false, string("xhu"), 2523, true);
	this->pqoplxqsesllffiflewqg();
	this->lpdvlenjuvyhhvjwmly(string("tkmqbpsnsybitwnsyzhidvitkjtsrfevxfdroyxtwcohvxzqxdxkzhgeghlkmplefraardchgmdwujwvermtkkgxkd"), 2403, 45032, 38529, 3758, 1528, string("jldgpwsuowqwalbxvyqfguouebjkepvyjyspnerpvtjnwthlarbcj"), true, 19539, 3011);
	this->knglgfispth(4842, true, 35934, 1905, 30066);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class kminrzz {
public:
	bool fwfmnzbepcke;
	int lpidbprrtuz;
	int bpxsm;
	int jcwmtfisyfsn;
	kminrzz();
	bool ncxbxqmactybpvqkiphrdoqq(bool khcpluv, double bekrrwukdrc, int yqxaoantlpe, double ogfsshkj, bool aievbolpmhbcb, bool rbsrhuoyt, string ifaziuikhvjzoq, string yuvujmmmwt, int orwonoye);
	double ljybiehqvdofzaqeaid(double mcghmfiy, bool ozvklkdl, int atbsfzxheki, string rjbxj, double ogswuck, double ysevzesdcwgs, string oqmzwjxqxwstzx);
	string sqrmijvlykgetxp(bool jqxiont, double mbtrsomcpo, int rosnrwnoh, int hglwpwdcu);
	int nkcjzdyvmzyfgejuac(double kbtldpdyhg, int ojkndhjjnoheugo);
	int uanfsmvzzqoacjleykjkr(string meukjsnbyxs, string xkxpiyad, bool lwgpoupfnchj, string irquihhb, bool hnklboajmieg, string ehkimocmwwlbvj, bool kxlkldtb, string fbacgbps, bool pkngtbfk, bool fzfjqekp);
	string cdeiwmhpvypqcnyfww();
	string mhtyjfvbuh(string ldtbzorxy, double dztckxm);

protected:
	double ogducwoushwz;

	void tpcfnueuffvjxvnevemvt(double hzrrnvvpvkwqy, bool qzgnybxmlw);
	void gsyplyywbl(string serxrommrylhg, double scdbqoinlgw, int iqdvhgjjeqs, int pmvfazicmehx, int czdiu, int okhgsm);

private:
	double twown;
	string yllaqzrtcbryoq;
	bool kzgigcj;

	bool xqqhtjtqrhmlcn(int ohcmenkyrigoj, double yfpbg, bool pdwmehmwxpf, double hahly, bool bmslygexqcfgsq, string netjzfpqmdh, int bsshkgycdctx, int oovqc, int reisrkeezk);
	int rtdeeeemdcknkxcirefk(string zxjeov);
	bool ffpssehkbjzjrsyyrpofl(int ilnngenbktdka, bool thlvmnntrgfyn, bool mpfsvnheld, double utgssyipm, int ermszjasnkmrb);
	bool iziuylrjtiiqewwjcmzcpkws(bool kgyuzkc, bool gdrzk, string xttkxlyczuy, double ttncnpy, bool uzgcxdbxgh, int qehgbricvtrdelj, double opfia, string owpstxdudfevqjv, int cknepllpbxkska, bool blfxkbjjbkr);
	bool rwrwzhzywet(int quaqlujjyqocazq, double sdepua, int pxlhyvv, string urxehdokwlqp, string uhoftksqbli, string enekkhimztumlu, double icwpv, string cnwqkranckvcjie, int cgjwafwaotoce);
	string rgbpayebhtaeqq(int wqaxovwps, string rpxylxlqgugkrnk, string lpphc, int ogjuq);
	bool oiybzbkpmo(int ceciebbnhg, bool qggrtijyu, string edpnrvc, double rhdqfv, int gzzzhylfdmhidmn, double mdomdwlpaoqql, string cpvwnincbg, int gevlesvqqqvqjxr, bool qgqtwzk, string qpjososkadx);
	string whzxkhelfigjkshvfadkkv(string omxadmliofoyhm, int cputyoxu, int sxmupvtytcrh, int ezejfhvi, int wxpdjmmt, bool stgmolzxge, string aridtkapzpe, double gybdambo);
	int iliuiojrsxtrdiqghzgnxwqpx(string nbdjifskfjrfu, string jovuyhiq, int wfjqzxdvs);

};


bool kminrzz::xqqhtjtqrhmlcn(int ohcmenkyrigoj, double yfpbg, bool pdwmehmwxpf, double hahly, bool bmslygexqcfgsq, string netjzfpqmdh, int bsshkgycdctx, int oovqc, int reisrkeezk) {
	string zqdzmabjcpklifl = "hgdjwxggeytygowucvqrmtilprtgmvufhxxovtxozagfcqjkjqwpywha";
	double srunhyg = 17337;
	double fkmpubewxbmv = 27818;
	double zszabrtfmtxw = 82323;
	int lbrydiu = 3981;
	int mfxxarbiomcj = 1319;
	bool eakvncmn = true;
	int abcajalzadqat = 3066;
	bool gcddw = false;
	if (3066 != 3066) {
		int hpbh;
		for (hpbh = 59; hpbh > 0; hpbh--) {
			continue;
		}
	}
	return true;
}

int kminrzz::rtdeeeemdcknkxcirefk(string zxjeov) {
	bool dzknhojfkltl = true;
	int pshnbyljw = 740;
	if (740 != 740) {
		int ekhsgqlw;
		for (ekhsgqlw = 2; ekhsgqlw > 0; ekhsgqlw--) {
			continue;
		}
	}
	if (740 != 740) {
		int jydt;
		for (jydt = 10; jydt > 0; jydt--) {
			continue;
		}
	}
	if (740 != 740) {
		int lilasv;
		for (lilasv = 74; lilasv > 0; lilasv--) {
			continue;
		}
	}
	if (740 == 740) {
		int emkwz;
		for (emkwz = 19; emkwz > 0; emkwz--) {
			continue;
		}
	}
	if (740 != 740) {
		int ec;
		for (ec = 62; ec > 0; ec--) {
			continue;
		}
	}
	return 29959;
}

bool kminrzz::ffpssehkbjzjrsyyrpofl(int ilnngenbktdka, bool thlvmnntrgfyn, bool mpfsvnheld, double utgssyipm, int ermszjasnkmrb) {
	bool qmjceq = false;
	bool htcfesemdkr = false;
	bool ovvlnr = true;
	if (false == false) {
		int wvbyo;
		for (wvbyo = 78; wvbyo > 0; wvbyo--) {
			continue;
		}
	}
	if (false != false) {
		int lwalai;
		for (lwalai = 24; lwalai > 0; lwalai--) {
			continue;
		}
	}
	if (false != false) {
		int sdvrdw;
		for (sdvrdw = 76; sdvrdw > 0; sdvrdw--) {
			continue;
		}
	}
	if (false == false) {
		int ay;
		for (ay = 49; ay > 0; ay--) {
			continue;
		}
	}
	return false;
}

bool kminrzz::iziuylrjtiiqewwjcmzcpkws(bool kgyuzkc, bool gdrzk, string xttkxlyczuy, double ttncnpy, bool uzgcxdbxgh, int qehgbricvtrdelj, double opfia, string owpstxdudfevqjv, int cknepllpbxkska, bool blfxkbjjbkr) {
	int aanhglykm = 1821;
	if (1821 == 1821) {
		int lfmgtf;
		for (lfmgtf = 2; lfmgtf > 0; lfmgtf--) {
			continue;
		}
	}
	return false;
}

bool kminrzz::rwrwzhzywet(int quaqlujjyqocazq, double sdepua, int pxlhyvv, string urxehdokwlqp, string uhoftksqbli, string enekkhimztumlu, double icwpv, string cnwqkranckvcjie, int cgjwafwaotoce) {
	bool zwyxx = true;
	bool lpedvnvyyszmuca = true;
	int ekxvjnfuvtaysq = 2099;
	string ivrsvexqlstdk = "qicxavpcuwlvlnbgvyplxiuryayukmrbwolbwyvevphvewp";
	bool egehhnmp = false;
	double wvpjies = 56347;
	bool rhpcgvynqho = true;
	double gbrdrsigv = 16190;
	if (16190 == 16190) {
		int jplxjg;
		for (jplxjg = 57; jplxjg > 0; jplxjg--) {
			continue;
		}
	}
	if (false == false) {
		int opqetlc;
		for (opqetlc = 67; opqetlc > 0; opqetlc--) {
			continue;
		}
	}
	if (16190 == 16190) {
		int ioorogkx;
		for (ioorogkx = 33; ioorogkx > 0; ioorogkx--) {
			continue;
		}
	}
	return false;
}

string kminrzz::rgbpayebhtaeqq(int wqaxovwps, string rpxylxlqgugkrnk, string lpphc, int ogjuq) {
	bool fyesjigichl = false;
	bool sdapfuozpr = true;
	double gmudqgrrbo = 6122;
	int ajtojnswm = 459;
	bool iovjicfckd = true;
	int hstnjtlpzws = 3539;
	double vyahdfru = 18017;
	if (459 != 459) {
		int zsbkexap;
		for (zsbkexap = 41; zsbkexap > 0; zsbkexap--) {
			continue;
		}
	}
	if (true == true) {
		int rikkyqdaxz;
		for (rikkyqdaxz = 96; rikkyqdaxz > 0; rikkyqdaxz--) {
			continue;
		}
	}
	if (false == false) {
		int ipbzgosc;
		for (ipbzgosc = 70; ipbzgosc > 0; ipbzgosc--) {
			continue;
		}
	}
	if (18017 != 18017) {
		int obluczpkj;
		for (obluczpkj = 21; obluczpkj > 0; obluczpkj--) {
			continue;
		}
	}
	if (6122 == 6122) {
		int svtdqzqxw;
		for (svtdqzqxw = 71; svtdqzqxw > 0; svtdqzqxw--) {
			continue;
		}
	}
	return string("qautiuzhtiuoozyw");
}

bool kminrzz::oiybzbkpmo(int ceciebbnhg, bool qggrtijyu, string edpnrvc, double rhdqfv, int gzzzhylfdmhidmn, double mdomdwlpaoqql, string cpvwnincbg, int gevlesvqqqvqjxr, bool qgqtwzk, string qpjososkadx) {
	bool ygryzqi = false;
	int bswolitinp = 643;
	double sxicfhdsvwrehyn = 12033;
	double vrcwggo = 2695;
	bool mnujjio = false;
	double hiririx = 8778;
	string zfttbcmnqm = "vxeyyqmxomzslrgcplaxuuipziqvgzugkcmurukuw";
	double xjennipovzu = 14835;
	bool mmspuc = false;
	if (14835 == 14835) {
		int ze;
		for (ze = 56; ze > 0; ze--) {
			continue;
		}
	}
	if (false == false) {
		int xcaq;
		for (xcaq = 81; xcaq > 0; xcaq--) {
			continue;
		}
	}
	if (14835 == 14835) {
		int lxp;
		for (lxp = 70; lxp > 0; lxp--) {
			continue;
		}
	}
	return false;
}

string kminrzz::whzxkhelfigjkshvfadkkv(string omxadmliofoyhm, int cputyoxu, int sxmupvtytcrh, int ezejfhvi, int wxpdjmmt, bool stgmolzxge, string aridtkapzpe, double gybdambo) {
	int wxufvzaetmkp = 7725;
	bool aeqoohsz = false;
	bool sxrdk = true;
	bool rikbpnnfio = true;
	int rrlyfngpu = 620;
	double xkzpv = 17263;
	int kfphb = 635;
	int yufxfrv = 3220;
	double prkadtj = 55067;
	double qsrdzjoyfzksslv = 12302;
	if (true == true) {
		int wxepv;
		for (wxepv = 51; wxepv > 0; wxepv--) {
			continue;
		}
	}
	if (true == true) {
		int jnxtujuu;
		for (jnxtujuu = 55; jnxtujuu > 0; jnxtujuu--) {
			continue;
		}
	}
	if (635 == 635) {
		int cwhps;
		for (cwhps = 58; cwhps > 0; cwhps--) {
			continue;
		}
	}
	if (17263 != 17263) {
		int iwwnjoapyb;
		for (iwwnjoapyb = 34; iwwnjoapyb > 0; iwwnjoapyb--) {
			continue;
		}
	}
	if (635 == 635) {
		int tpxk;
		for (tpxk = 51; tpxk > 0; tpxk--) {
			continue;
		}
	}
	return string("stt");
}

int kminrzz::iliuiojrsxtrdiqghzgnxwqpx(string nbdjifskfjrfu, string jovuyhiq, int wfjqzxdvs) {
	bool gtmkdvnvy = false;
	string piqbk = "ahiqdpxkeqqyoumvxvzohheiwvnqyesfhunrqmkycyjpoenzufroaobei";
	if (false == false) {
		int sbqdvzpur;
		for (sbqdvzpur = 12; sbqdvzpur > 0; sbqdvzpur--) {
			continue;
		}
	}
	if (string("ahiqdpxkeqqyoumvxvzohheiwvnqyesfhunrqmkycyjpoenzufroaobei") == string("ahiqdpxkeqqyoumvxvzohheiwvnqyesfhunrqmkycyjpoenzufroaobei")) {
		int rx;
		for (rx = 5; rx > 0; rx--) {
			continue;
		}
	}
	if (string("ahiqdpxkeqqyoumvxvzohheiwvnqyesfhunrqmkycyjpoenzufroaobei") != string("ahiqdpxkeqqyoumvxvzohheiwvnqyesfhunrqmkycyjpoenzufroaobei")) {
		int wcucelbaj;
		for (wcucelbaj = 87; wcucelbaj > 0; wcucelbaj--) {
			continue;
		}
	}
	if (string("ahiqdpxkeqqyoumvxvzohheiwvnqyesfhunrqmkycyjpoenzufroaobei") == string("ahiqdpxkeqqyoumvxvzohheiwvnqyesfhunrqmkycyjpoenzufroaobei")) {
		int kvn;
		for (kvn = 2; kvn > 0; kvn--) {
			continue;
		}
	}
	return 74384;
}

void kminrzz::tpcfnueuffvjxvnevemvt(double hzrrnvvpvkwqy, bool qzgnybxmlw) {
	string kadouyfwlgpg = "yvvzjwbokodlhvmhb";
	int totfugikekps = 2652;
	bool ayxqw = true;
	bool vtlmnhduivzih = false;
	if (true != true) {
		int gy;
		for (gy = 55; gy > 0; gy--) {
			continue;
		}
	}

}

void kminrzz::gsyplyywbl(string serxrommrylhg, double scdbqoinlgw, int iqdvhgjjeqs, int pmvfazicmehx, int czdiu, int okhgsm) {
	double zzesflqvwle = 15058;
	bool qykvmhsl = false;
	int tmftwqlwkgsgps = 1325;
	double eufhzqpoiswgxc = 12875;
	int gdkiwjckkfmdtp = 1346;
	string fyvkd = "pgzwiemkkyxjorgyhstnulrjtn";
	int kwbncx = 1808;
	double fqpfyzuzif = 49623;
	string wsowky = "mngfjlxgihmezewucmbvjboftmsojeprspqpapgxzmfuvthutvijvbjohctijoghyvdovybvkvmcoicyhoffagkmmjbekwwge";
	if (1346 != 1346) {
		int pwmq;
		for (pwmq = 87; pwmq > 0; pwmq--) {
			continue;
		}
	}

}

bool kminrzz::ncxbxqmactybpvqkiphrdoqq(bool khcpluv, double bekrrwukdrc, int yqxaoantlpe, double ogfsshkj, bool aievbolpmhbcb, bool rbsrhuoyt, string ifaziuikhvjzoq, string yuvujmmmwt, int orwonoye) {
	string zwxgedqd = "valsgcllcpartfyuhvgaskbywxedfhwkjdpdjtfuhhjsngnednngvcejcs";
	int uzryzo = 60;
	string qkabjzdbf = "bseclcguxfyxqcckqympkchixtsiarkhpul";
	bool pfwmhxmox = false;
	bool bqnckehppyi = false;
	bool jmdnflblwkpakyr = true;
	double tmcwajla = 2728;
	if (2728 != 2728) {
		int knmzuko;
		for (knmzuko = 53; knmzuko > 0; knmzuko--) {
			continue;
		}
	}
	if (false == false) {
		int lmdmljr;
		for (lmdmljr = 78; lmdmljr > 0; lmdmljr--) {
			continue;
		}
	}
	return true;
}

double kminrzz::ljybiehqvdofzaqeaid(double mcghmfiy, bool ozvklkdl, int atbsfzxheki, string rjbxj, double ogswuck, double ysevzesdcwgs, string oqmzwjxqxwstzx) {
	double lsmcqjvyuqh = 62989;
	bool kyfkgw = false;
	double zhyzljasrsjqgxh = 12275;
	bool paptpvfpx = false;
	bool wquobtl = true;
	bool khbowwznahoc = true;
	if (true == true) {
		int dq;
		for (dq = 5; dq > 0; dq--) {
			continue;
		}
	}
	if (false != false) {
		int hrzqzqjku;
		for (hrzqzqjku = 87; hrzqzqjku > 0; hrzqzqjku--) {
			continue;
		}
	}
	if (true == true) {
		int wklnffyrw;
		for (wklnffyrw = 1; wklnffyrw > 0; wklnffyrw--) {
			continue;
		}
	}
	if (12275 != 12275) {
		int txwboxrdsg;
		for (txwboxrdsg = 12; txwboxrdsg > 0; txwboxrdsg--) {
			continue;
		}
	}
	if (false == false) {
		int dl;
		for (dl = 47; dl > 0; dl--) {
			continue;
		}
	}
	return 80937;
}

string kminrzz::sqrmijvlykgetxp(bool jqxiont, double mbtrsomcpo, int rosnrwnoh, int hglwpwdcu) {
	double isawxpyqlygfko = 11539;
	bool kfjpwkmlb = false;
	int tlsjxz = 64;
	string xofqusbizujoer = "joackbxuaqtrgditytlrlaeqyockvdqyjjhgwykmvzhluuubasjwkavhmhkujdizpydwggrmee";
	double jscrhpehozao = 68026;
	string uwcnqfouhd = "uqapdmmbeiyfspzp";
	bool kvaxwulpxh = false;
	double dfyhzirk = 29503;
	string figmiuh = "ohdjldp";
	return string("fbzb");
}

int kminrzz::nkcjzdyvmzyfgejuac(double kbtldpdyhg, int ojkndhjjnoheugo) {
	return 64425;
}

int kminrzz::uanfsmvzzqoacjleykjkr(string meukjsnbyxs, string xkxpiyad, bool lwgpoupfnchj, string irquihhb, bool hnklboajmieg, string ehkimocmwwlbvj, bool kxlkldtb, string fbacgbps, bool pkngtbfk, bool fzfjqekp) {
	bool euwogtqtkrq = false;
	bool wkcwbyrabtw = true;
	string zbewkwnchhl = "pei";
	bool svahxe = false;
	int dmcbilhjryiwheu = 1950;
	double bmfjyhgzpfepsj = 50867;
	bool vemonjlczailebr = false;
	if (1950 != 1950) {
		int qfoje;
		for (qfoje = 64; qfoje > 0; qfoje--) {
			continue;
		}
	}
	if (false == false) {
		int szu;
		for (szu = 24; szu > 0; szu--) {
			continue;
		}
	}
	return 33506;
}

string kminrzz::cdeiwmhpvypqcnyfww() {
	bool umgpmm = true;
	bool owrvizfald = true;
	int guagstmpfpk = 5098;
	double gnoog = 29005;
	bool ifcskqoofanmtk = false;
	return string("nqccdzpdgvdq");
}

string kminrzz::mhtyjfvbuh(string ldtbzorxy, double dztckxm) {
	bool hptaasg = false;
	string hgcxvp = "ooughdibyotunjfzwhtvgqwvblwvtvluuyp";
	int edrqgushjtxw = 819;
	bool otvhvp = false;
	double sjbhei = 65776;
	if (819 == 819) {
		int psennii;
		for (psennii = 3; psennii > 0; psennii--) {
			continue;
		}
	}
	return string("vzvnoizw");
}

kminrzz::kminrzz() {
	this->ncxbxqmactybpvqkiphrdoqq(false, 17869, 952, 14646, false, true, string("dmjlrekovgetgiqyqerykdddoldqoecv"), string("skmwzmwllgliumgkxvrcfymixnxnwlkexcfqasmlupewtcdixtwqslsbtjuebapsxwbowionxhqxghzgizmvwdteonzpvtawf"), 6280);
	this->ljybiehqvdofzaqeaid(55707, true, 2102, string("csnnvwwdhjzfxfn"), 28780, 1453, string("mfrodaapekxomsllnrrtojnayussmryncgjrvhlvkbpplhrpouvjyiarmtidztctwvkagvxniy"));
	this->sqrmijvlykgetxp(false, 211, 1320, 551);
	this->nkcjzdyvmzyfgejuac(46504, 1424);
	this->uanfsmvzzqoacjleykjkr(string("cqwwpqgdyzdybjnckgttsgwnykryekhscaokiiuuulnxebzisojbxdrcamypyibdbwfkrnzknwblwmqobflsztiwbekrpvzihbz"), string("aojnmtqfjltqkboiwrebklqpqomahpqdqfxvbmieiimztbrflwzvsuixywug"), false, string("kcslvzelsqebpuxuomadvhrnhlsneulmrrthhesoluidboqzfbufryceopltixfnqteu"), true, string("iimezndhwdwverklqctxgtaxetikijbjlavnyqlkqcudgdjdsiqvspgfzfpjypq"), false, string("qpvuutbmcyitwntxlasyryexphjwfthrmnmvcnvbunqkxnvilqoxttndwzviktypdvvyihvn"), false, false);
	this->cdeiwmhpvypqcnyfww();
	this->mhtyjfvbuh(string("nmsdtuzmonghceptrotzlltcyxomgdexekmeourkoznhrqcoikxqqvkrcapianoebovysuczwdbliwmnormbynwadsvv"), 15382);
	this->tpcfnueuffvjxvnevemvt(46830, true);
	this->gsyplyywbl(string("gnovitdfwhwvqmscfiljrqqrpsjdoolbdxdioqouvcxiijectghzxppdldcxbuyphsmpqbnfcvqxnsljuy"), 39324, 41, 5986, 3520, 5479);
	this->xqqhtjtqrhmlcn(824, 16520, true, 46948, true, string("arliqtwzsoudjomjfqsywaegjjqpxdhvzkrkrjqymbwqqbgqenrvduktyfcomghegpiynoyregkwympmuawjvrysf"), 468, 304, 193);
	this->rtdeeeemdcknkxcirefk(string("cwnd"));
	this->ffpssehkbjzjrsyyrpofl(4829, false, true, 168, 1260);
	this->iziuylrjtiiqewwjcmzcpkws(false, true, string("lcqozqvikdoelhwzulrwualojrbblflzkremsjjw"), 52349, true, 4064, 49881, string("psnirjddoulggydqvifopjsndqewnoxpszeasrsiqasknvcuwjrfrqfeayxhlrdbbwbzmicxwubqygvzzoun"), 8325, true);
	this->rwrwzhzywet(2065, 26828, 5043, string("mkceitbhitvpssnnlwuqyqvsrkzch"), string("cymbsmxhwbwgizciqyjwcugbdovddtoutkkxpp"), string("xtnazhppfrvzkpbfjovhumrzwutmycztfvyqdreashnzevwxfbtcksskilwtlpqrznqsde"), 37270, string("rejfvekhpijewgibtzmlxzpbxaoaygxzbjdlskywowvnnvvuiwqdtzyxpxrxdqwuwmzvtkmywpezwn"), 3440);
	this->rgbpayebhtaeqq(4286, string("psccaoyancyfcmaz"), string("heefvqis"), 4067);
	this->oiybzbkpmo(3191, true, string("yxealsdclqydwctgbgdifkjiuyeeydydsztfxzyglblijamqepht"), 31273, 1155, 27802, string("ixawyuaqexnmxcmmdqfhoipucghhscjgcfpneugjblspddzeqdgexmtmtjrlbnlcsnvnxyrjeqbormc"), 2080, true, string("feexftymllgnijjybbyqurjrzcugqrwtzaaqqeivmasnwhigcbkatcxvnwidglhodhcxpintktzatc"));
	this->whzxkhelfigjkshvfadkkv(string("kzmqsjzlfwprrjznszmnvyzxeozked"), 1219, 251, 1374, 1174, true, string("m"), 16943);
	this->iliuiojrsxtrdiqghzgnxwqpx(string(""), string("xacyzpcvauseqrxtetgmosbhdjlutgvqumustlrurljjdirujfwfcnnhyevguuxlwxsesbsujhelwhkwhrunxyxij"), 924);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class gcyjbrh {
public:
	double egfykmurbn;
	bool rapfnhtlfyucp;
	int dfuvbeivnxe;
	bool rkzhvowrsva;
	gcyjbrh();
	void pnjjdnkdgzdasbkw(bool fombpmnpy, double oagwlrrsxge, int kkredvthx, bool gwtzcqzlch, string tyczgqabvxrpmf, string okcozgsf, double smkxevsqgxpjtmg, bool ntsxdwhh, int zahqveywsenkknv);
	void danggyvhxawvknwy(int sqvspdxtmmryxdi, string iexoube, double qrqpzru, bool wmlwurcha, string jkvvhgiedeuh, double kauxbwdmwpix, string warbejjqzjn, double qwhjcqsl);

protected:
	bool vaurht;
	double flxzy;
	string cgqihpoqhv;

	double xavwwqybmdcysuhtftpl(int iseqgmrf, string eyhjyqcipgwjf, double wbopnddxktuarn, bool wjbefcjfwibpbgc, int qpopskmed);
	double ajeacoztksfdnugwbitq(int kqlsdikwrpys, int abksdu, double ebywlpuulwsekq, double zymcjlvivx);
	string wkrebtksvpmtwyvwlcvbzadld(bool gcewmdob, double ugngnb, bool txwkerpisvjmudh, int qvivrfwjkyhnd);
	int mazxoclzwhwxn(bool yscycwmqsxknqj, bool cdrab, string yzxbcvmhjnl, string pqldpy, double pwhnrguubshmjp, double yxmtoodd);
	bool fhyjxydyqlzfgukfemurd(string hukxaftgtjnuxxd, string ghqdmcsuoahv, bool fvxypwsbzqcyho, string aruyxzcqotqymj, double lvsnqgwkaj, string rmedkeiuxeuoc);
	double eudszfjnwzlhuxqsxkfcgdhg(bool luugj, double axnzjuddgcwz, double tcmqskkklvsylp, string jrvldvflnonj, bool hbdjjqn, bool bufmpi, bool zvyuaxy, bool fihvtqgql, double hwvzvxcft);
	string lxrmiaxnlrqorvhmoonlmpv(bool bxxlhxaawnn, bool jjiugtrsirjxxs, int wnmtbooseyikbqo, int ogwmayvuorf, double zbopols, bool hyidnb, int uajrvzvekzerztz, bool hhgelhgantyd, bool pcotkik);

private:
	int rstnp;
	double lbyuwcpsrnlgl;
	string jztpd;
	bool jcoqfs;
	int bfceztls;

	void ycbcgtshfoqqezc(bool igutqxchnj, double gmtii, double mnqinfzu, int ihowscbuo, int oithvwyau, double bcjwpq, int emnubevqzknyxp, string qkrvisztbsmiar, double cknyoev);
	bool kplrrtkgolhltbfzt(string pblctwn, double fgkfoslzqscvlvv, bool gsigbknfschwsq, double qzltjfsxwvkzh, bool ukgwzckg, string qhugmekmtdcxmn, string vpnrxkpc, string rrwwozzhgh, int noqdnhzunfb, bool kcgrxuj);
	string bavvharbhg(int wpfcqexubcmc, bool feyfkrzbywxu, string pdriz, string ivuswgrdingmxbf, double ngehzgnklzdvwg, bool croetvftyrn, bool xekcedutbxlo);
	string wqyfcbojwnzrnhzxzrrsa(bool dvelikassm);
	bool uhhkatxtgpcr(string bsscr, string ktazkjcgwrns, int juwctfoadwyjygm, int livifwzjzijc, string qwgnjwvdh, int ykcpwceo, bool leyzcnhjr, double eleps);
	void ykolyeasgwcosdjps(string fokmlpjvvuu, int mwrvdxap, double jarbnwhew, double fbhregdrpxjarr, bool skbilrzlkpvct, string nisexwwqdf, int vwptjptzz, int btlmf);
	bool wpkgacnfjyevkecqhnnte(bool gbylkgswho, int jdeggwrfbiq);

};


void gcyjbrh::ycbcgtshfoqqezc(bool igutqxchnj, double gmtii, double mnqinfzu, int ihowscbuo, int oithvwyau, double bcjwpq, int emnubevqzknyxp, string qkrvisztbsmiar, double cknyoev) {
	double ynnypaxqc = 10147;
	string kkarubvil = "frazewwyqvcysnldvhelgywjkrhjowpcixpjqqfyecxog";
	string xawovpmtxoab = "eo";
	double odlki = 15367;
	int zxxjpuxw = 594;
	bool rrosmtqbq = false;
	string wausbxgbjqvq = "tezcavopqxbyipesfcseopjdbimfcf";
	if (string("tezcavopqxbyipesfcseopjdbimfcf") == string("tezcavopqxbyipesfcseopjdbimfcf")) {
		int awmfrr;
		for (awmfrr = 26; awmfrr > 0; awmfrr--) {
			continue;
		}
	}
	if (15367 != 15367) {
		int nsgwec;
		for (nsgwec = 52; nsgwec > 0; nsgwec--) {
			continue;
		}
	}
	if (string("eo") != string("eo")) {
		int usqcorfk;
		for (usqcorfk = 58; usqcorfk > 0; usqcorfk--) {
			continue;
		}
	}

}

bool gcyjbrh::kplrrtkgolhltbfzt(string pblctwn, double fgkfoslzqscvlvv, bool gsigbknfschwsq, double qzltjfsxwvkzh, bool ukgwzckg, string qhugmekmtdcxmn, string vpnrxkpc, string rrwwozzhgh, int noqdnhzunfb, bool kcgrxuj) {
	bool bfgqm = false;
	int rhujopgbjsebkwv = 7698;
	double ncvrojdmxfvadct = 44329;
	double amotlfpviywczc = 7145;
	string elalqsrhsybd = "nmlpcdsiwnthxpyjgyuwawzwgxqwwkhuqbakqjcidokedcbgsobudfgrzmqt";
	if (string("nmlpcdsiwnthxpyjgyuwawzwgxqwwkhuqbakqjcidokedcbgsobudfgrzmqt") == string("nmlpcdsiwnthxpyjgyuwawzwgxqwwkhuqbakqjcidokedcbgsobudfgrzmqt")) {
		int yo;
		for (yo = 2; yo > 0; yo--) {
			continue;
		}
	}
	return false;
}

string gcyjbrh::bavvharbhg(int wpfcqexubcmc, bool feyfkrzbywxu, string pdriz, string ivuswgrdingmxbf, double ngehzgnklzdvwg, bool croetvftyrn, bool xekcedutbxlo) {
	int pcbpzcfu = 1276;
	double puehpyjsxohw = 14102;
	int hirrstael = 765;
	double tvwkowbpwhqyg = 63087;
	string osevvdrjms = "sezfzmvemrpjzhxxlmguvknkrrzimqgtuwvhnkkzyphizjhanxffmdijfaezhhjxbfyuzcrchxizptu";
	int vsanzuyr = 3276;
	string wnntevw = "hqsitqoblptgbmehdkoatjmgpyobwrwejcutuxkfsrmibpyrcdrtrbzihfpchicdzoeglkvmyjcidpqcudpesgincgzwfst";
	bool iiqtshlwfe = true;
	double qpqaisnvey = 7703;
	int drgmmagrids = 4671;
	if (4671 == 4671) {
		int og;
		for (og = 27; og > 0; og--) {
			continue;
		}
	}
	if (string("sezfzmvemrpjzhxxlmguvknkrrzimqgtuwvhnkkzyphizjhanxffmdijfaezhhjxbfyuzcrchxizptu") == string("sezfzmvemrpjzhxxlmguvknkrrzimqgtuwvhnkkzyphizjhanxffmdijfaezhhjxbfyuzcrchxizptu")) {
		int xkpwgp;
		for (xkpwgp = 70; xkpwgp > 0; xkpwgp--) {
			continue;
		}
	}
	if (4671 != 4671) {
		int petqmvm;
		for (petqmvm = 44; petqmvm > 0; petqmvm--) {
			continue;
		}
	}
	return string("nguvfewmfqydfsevscay");
}

string gcyjbrh::wqyfcbojwnzrnhzxzrrsa(bool dvelikassm) {
	int bdvxb = 3988;
	double jpjuitq = 32192;
	bool cnvsrooywq = true;
	int ieiefqjny = 1806;
	int ziczxtbw = 46;
	string xzhxnozsesmu = "sabtdjzaorp";
	bool bgchsizpldx = false;
	string opcyhphbhvpqdfo = "bsawthbtnxjfolnajlehenvptszgsiqbubtxivnsjansqevbaurefnoiufdushmzozolwbzdktsbvwttnobbhmyuhvqc";
	if (string("bsawthbtnxjfolnajlehenvptszgsiqbubtxivnsjansqevbaurefnoiufdushmzozolwbzdktsbvwttnobbhmyuhvqc") != string("bsawthbtnxjfolnajlehenvptszgsiqbubtxivnsjansqevbaurefnoiufdushmzozolwbzdktsbvwttnobbhmyuhvqc")) {
		int ne;
		for (ne = 72; ne > 0; ne--) {
			continue;
		}
	}
	if (32192 != 32192) {
		int aamxxcagz;
		for (aamxxcagz = 90; aamxxcagz > 0; aamxxcagz--) {
			continue;
		}
	}
	if (true != true) {
		int jxoap;
		for (jxoap = 79; jxoap > 0; jxoap--) {
			continue;
		}
	}
	if (1806 != 1806) {
		int htdazbxaa;
		for (htdazbxaa = 40; htdazbxaa > 0; htdazbxaa--) {
			continue;
		}
	}
	return string("nameymj");
}

bool gcyjbrh::uhhkatxtgpcr(string bsscr, string ktazkjcgwrns, int juwctfoadwyjygm, int livifwzjzijc, string qwgnjwvdh, int ykcpwceo, bool leyzcnhjr, double eleps) {
	int xgkdivifgtgbd = 4534;
	if (4534 == 4534) {
		int cvc;
		for (cvc = 74; cvc > 0; cvc--) {
			continue;
		}
	}
	if (4534 == 4534) {
		int xsoirw;
		for (xsoirw = 47; xsoirw > 0; xsoirw--) {
			continue;
		}
	}
	if (4534 == 4534) {
		int wuwoy;
		for (wuwoy = 5; wuwoy > 0; wuwoy--) {
			continue;
		}
	}
	if (4534 != 4534) {
		int kpt;
		for (kpt = 45; kpt > 0; kpt--) {
			continue;
		}
	}
	return false;
}

void gcyjbrh::ykolyeasgwcosdjps(string fokmlpjvvuu, int mwrvdxap, double jarbnwhew, double fbhregdrpxjarr, bool skbilrzlkpvct, string nisexwwqdf, int vwptjptzz, int btlmf) {
	int zsoarhdszaguko = 626;
	if (626 == 626) {
		int gflekwpobe;
		for (gflekwpobe = 42; gflekwpobe > 0; gflekwpobe--) {
			continue;
		}
	}
	if (626 == 626) {
		int bmrnxz;
		for (bmrnxz = 92; bmrnxz > 0; bmrnxz--) {
			continue;
		}
	}
	if (626 != 626) {
		int hoxrqavnjt;
		for (hoxrqavnjt = 59; hoxrqavnjt > 0; hoxrqavnjt--) {
			continue;
		}
	}
	if (626 != 626) {
		int ilsayfnont;
		for (ilsayfnont = 7; ilsayfnont > 0; ilsayfnont--) {
			continue;
		}
	}

}

bool gcyjbrh::wpkgacnfjyevkecqhnnte(bool gbylkgswho, int jdeggwrfbiq) {
	double zpofrijqganqte = 5188;
	int kkbjeogvpvtzrk = 23;
	bool wayrv = false;
	string gdbryki = "tctemhzatstzhzowxnpzohbjiroyomdobdcruxaisojprmxfggwarniswvcteeknncsndpekt";
	bool yzntcqhyokkl = false;
	if (5188 == 5188) {
		int bpirdpyxbp;
		for (bpirdpyxbp = 53; bpirdpyxbp > 0; bpirdpyxbp--) {
			continue;
		}
	}
	if (false != false) {
		int pik;
		for (pik = 93; pik > 0; pik--) {
			continue;
		}
	}
	if (false == false) {
		int nwn;
		for (nwn = 74; nwn > 0; nwn--) {
			continue;
		}
	}
	if (false != false) {
		int cdrzlyzb;
		for (cdrzlyzb = 27; cdrzlyzb > 0; cdrzlyzb--) {
			continue;
		}
	}
	return false;
}

double gcyjbrh::xavwwqybmdcysuhtftpl(int iseqgmrf, string eyhjyqcipgwjf, double wbopnddxktuarn, bool wjbefcjfwibpbgc, int qpopskmed) {
	double mhvxpntotugd = 8653;
	int mrcwcgtbiryuous = 1460;
	string bcwsosx = "uowsbyxsorkhpjvrcfdfmtxcjedkgvyroktbitiitcpdwnotlvojgmdkwjnhlmoifchtrhfmxnvzhqnxh";
	int ixpypvgjer = 2332;
	double wxleqzknv = 28226;
	bool dkpxwdwmybgb = true;
	double ixbwxo = 15114;
	double llbiezynmkjluei = 86072;
	string pvirgagbaanmmhs = "bsnlzlkrdcpaocyddwumrpkoribmkbplvxgolksrrmuennehzidxkmcsjvofcyvgzkad";
	if (86072 == 86072) {
		int hrhvgsga;
		for (hrhvgsga = 1; hrhvgsga > 0; hrhvgsga--) {
			continue;
		}
	}
	if (true == true) {
		int gdm;
		for (gdm = 37; gdm > 0; gdm--) {
			continue;
		}
	}
	if (2332 == 2332) {
		int zabvyo;
		for (zabvyo = 30; zabvyo > 0; zabvyo--) {
			continue;
		}
	}
	if (true != true) {
		int wxjecxsri;
		for (wxjecxsri = 16; wxjecxsri > 0; wxjecxsri--) {
			continue;
		}
	}
	if (15114 != 15114) {
		int nnkix;
		for (nnkix = 43; nnkix > 0; nnkix--) {
			continue;
		}
	}
	return 35582;
}

double gcyjbrh::ajeacoztksfdnugwbitq(int kqlsdikwrpys, int abksdu, double ebywlpuulwsekq, double zymcjlvivx) {
	string yoxyv = "cenacibp";
	bool yiqovmgzniile = true;
	string acsbrx = "eqalgstohluvrhkowtgtbqgywkfufgbxkvfofwbtyqtazxqll";
	int ikvgcrmwnfmhw = 4653;
	double iacoln = 72453;
	int cktebvtf = 5958;
	double xkfseiuldz = 3517;
	int xultxkkqdwu = 5155;
	double zkbzid = 14959;
	if (5958 != 5958) {
		int jfyrtqqpu;
		for (jfyrtqqpu = 31; jfyrtqqpu > 0; jfyrtqqpu--) {
			continue;
		}
	}
	if (5155 != 5155) {
		int husbvpwirv;
		for (husbvpwirv = 35; husbvpwirv > 0; husbvpwirv--) {
			continue;
		}
	}
	if (3517 != 3517) {
		int jsu;
		for (jsu = 32; jsu > 0; jsu--) {
			continue;
		}
	}
	if (5958 != 5958) {
		int eo;
		for (eo = 45; eo > 0; eo--) {
			continue;
		}
	}
	if (string("eqalgstohluvrhkowtgtbqgywkfufgbxkvfofwbtyqtazxqll") != string("eqalgstohluvrhkowtgtbqgywkfufgbxkvfofwbtyqtazxqll")) {
		int prbgb;
		for (prbgb = 100; prbgb > 0; prbgb--) {
			continue;
		}
	}
	return 34703;
}

string gcyjbrh::wkrebtksvpmtwyvwlcvbzadld(bool gcewmdob, double ugngnb, bool txwkerpisvjmudh, int qvivrfwjkyhnd) {
	string yqoufqlbirkdzos = "chnsdpfeeagxscprtzyfygpycsmuogedwiydvxolgfzuko";
	int zcyzzsgeiuy = 423;
	bool eojymgg = true;
	string dkavhrbfuno = "";
	string dyuzfwdpf = "vqtnczrfdzqyslnwixhrpgfdzrvtxefftwkdckisbvuleqvxfexhvydrmmtrwgxfbhihtiieefoaigszbmaq";
	string mikjnqs = "yomzgmteeuvqkrgoavvrc";
	string vorvycd = "ixdhvgmlasdcbgzqdesodjvdrooslcjqgdmbdlwlslldehfdoqzcwjaslglzjiikartqobgymhclofrawofxtr";
	int asuewdsfrwppz = 2090;
	if (2090 == 2090) {
		int txjgyk;
		for (txjgyk = 71; txjgyk > 0; txjgyk--) {
			continue;
		}
	}
	return string("shfpzcbiq");
}

int gcyjbrh::mazxoclzwhwxn(bool yscycwmqsxknqj, bool cdrab, string yzxbcvmhjnl, string pqldpy, double pwhnrguubshmjp, double yxmtoodd) {
	int arfzxyrryyphr = 977;
	double srfqqgxsuy = 11150;
	if (11150 == 11150) {
		int werk;
		for (werk = 75; werk > 0; werk--) {
			continue;
		}
	}
	if (977 != 977) {
		int yfguiiv;
		for (yfguiiv = 30; yfguiiv > 0; yfguiiv--) {
			continue;
		}
	}
	if (977 != 977) {
		int rd;
		for (rd = 70; rd > 0; rd--) {
			continue;
		}
	}
	return 95197;
}

bool gcyjbrh::fhyjxydyqlzfgukfemurd(string hukxaftgtjnuxxd, string ghqdmcsuoahv, bool fvxypwsbzqcyho, string aruyxzcqotqymj, double lvsnqgwkaj, string rmedkeiuxeuoc) {
	return true;
}

double gcyjbrh::eudszfjnwzlhuxqsxkfcgdhg(bool luugj, double axnzjuddgcwz, double tcmqskkklvsylp, string jrvldvflnonj, bool hbdjjqn, bool bufmpi, bool zvyuaxy, bool fihvtqgql, double hwvzvxcft) {
	return 60559;
}

string gcyjbrh::lxrmiaxnlrqorvhmoonlmpv(bool bxxlhxaawnn, bool jjiugtrsirjxxs, int wnmtbooseyikbqo, int ogwmayvuorf, double zbopols, bool hyidnb, int uajrvzvekzerztz, bool hhgelhgantyd, bool pcotkik) {
	string pshlzb = "jgungeqdqnnjlvvarqvlrczeqemllttzckszardhmklzgqtfyvwpprxsdgxgsrwdnkxet";
	string xgjvlmdabpjsvmm = "p";
	double nscammvhgxxm = 15849;
	double nnzpc = 7070;
	int sfzejpwlqwvasvb = 2566;
	int evhimpwtfdsk = 4043;
	double wbshntivwfftn = 62425;
	string mnindzwh = "ksmmyxffwfrmcenoqvpxduugsfxtcwu";
	double uvcvdvrbmxkom = 75221;
	return string("bcvyyqndpzzdpdlhmhao");
}

void gcyjbrh::pnjjdnkdgzdasbkw(bool fombpmnpy, double oagwlrrsxge, int kkredvthx, bool gwtzcqzlch, string tyczgqabvxrpmf, string okcozgsf, double smkxevsqgxpjtmg, bool ntsxdwhh, int zahqveywsenkknv) {
	int cggdkzser = 2511;
	int icuyj = 6897;
	string suvugjckmboq = "qwownebfaewffxkukelruqqiybnobdtbgoweyafbtyuormentonahqdynnxizimmausclkxodhehvlnjvubfpnovpexhbflbvmlt";
	bool bnhagyfcjuk = true;
	bool aetffwfdugegzw = true;
	int oizybbanajxjpn = 1648;
	bool znuhtzgssoqgb = true;
	bool ierctqhnw = false;
	double fowghega = 47113;
	string vzelzhgvjylmz = "cvlbxpeebyagmvugfmisiaqkbkudlazdn";
	if (47113 == 47113) {
		int byhulw;
		for (byhulw = 14; byhulw > 0; byhulw--) {
			continue;
		}
	}

}

void gcyjbrh::danggyvhxawvknwy(int sqvspdxtmmryxdi, string iexoube, double qrqpzru, bool wmlwurcha, string jkvvhgiedeuh, double kauxbwdmwpix, string warbejjqzjn, double qwhjcqsl) {
	string gojagj = "luexeipfnxn";
	bool dbsustxp = true;
	int owykhivl = 5816;
	if (5816 != 5816) {
		int stpdfev;
		for (stpdfev = 63; stpdfev > 0; stpdfev--) {
			continue;
		}
	}
	if (true == true) {
		int yyqh;
		for (yyqh = 77; yyqh > 0; yyqh--) {
			continue;
		}
	}

}

gcyjbrh::gcyjbrh() {
	this->pnjjdnkdgzdasbkw(true, 6938, 3818, false, string("krfgo"), string("ugyagoukemgkutkszfjcjrwiemlial"), 6281, true, 4665);
	this->danggyvhxawvknwy(3574, string("vfqbqbqvcnvtigeznuvfazrbowlddlumvslqoujpxhmvvjazmuirpfhlkhunovjgmylghmlpnnbfijeoccgkadqsw"), 13240, false, string("ksnqneoxyedsvwbphzgzvojnpnrongxeiryjydwkskxhozadbunugwgvirwtdtxwfteobvhkzrotqnetg"), 53682, string("vjnvxriidvyfopjuwxovvuwwqtldizfkvvizvasdtwvmyxxfznbtigiodkyvwmgbfsguq"), 5785);
	this->xavwwqybmdcysuhtftpl(29, string("uwcqdetidpnwmcrgdftswyxipqdmmkgppqqzuecxnqhqffr"), 59080, false, 1211);
	this->ajeacoztksfdnugwbitq(3279, 1047, 11094, 34930);
	this->wkrebtksvpmtwyvwlcvbzadld(true, 9199, false, 4710);
	this->mazxoclzwhwxn(true, false, string("tuokxmdewyhfdpzyjvzdsltyjfwgrmhalbmxvfikppmfkixwqcatabrpy"), string("beubjhvumvsfrnfsusskfknhzdqbrfhb"), 34605, 11179);
	this->fhyjxydyqlzfgukfemurd(string("sbdayjrigofqcjancoqhlmrqehi"), string("klfdtyxljbuelwbkiozmguqxatrgojvibrabaltgvnresmziybpdbiabdppcjidvdmrdrjndnflkajfmghhnehzrowkjrtwo"), false, string("dckddyxpybytkyyvjbsevfebnp"), 57283, string("ukkeleuzwkbhghrcembzbneegrzvbvvodeqbrwudrvjebewmqrpgfdrqzhukig"));
	this->eudszfjnwzlhuxqsxkfcgdhg(true, 19406, 19691, string("yktjjsmubpoi"), false, false, false, false, 251);
	this->lxrmiaxnlrqorvhmoonlmpv(false, false, 2626, 4744, 80776, false, 882, true, false);
	this->ycbcgtshfoqqezc(false, 79667, 9581, 532, 3057, 26698, 251, string("wlmguovpzcbufamujwbiiouevumhrfizlmrtlrxikdrcivmqjnmafjvgjpwyflrfajeii"), 19549);
	this->kplrrtkgolhltbfzt(string("xycfpjdftgtiobwfnfitqcqxeixnwsxalsctbop"), 46333, true, 15390, false, string("yqpwmwpdixjopuyblulchgnwymvmoacvabvksra"), string("nmtenlikoaaoyosgcyihadfhizmefqgggsgtadoyskifomyegrrpofbkdgsjsrhnwu"), string("yuevzprygnafhjxlnhinjkunbwxfzswejwzdntpjka"), 209, true);
	this->bavvharbhg(5126, false, string("opgyqihdwepjhyswnxjypkmjiqatwqdxsnv"), string("wfybbzcynlaqrhiqtjgwdfnjkiuzfbxzxtcgpxdg"), 4131, false, true);
	this->wqyfcbojwnzrnhzxzrrsa(false);
	this->uhhkatxtgpcr(string("tfgmngvpiabllmkshaqnakvmfdhakfitoujanjbmjarabzkstcwvqhdwozdkdqklklbn"), string("kiusosgeotthzyttanadaphuldzkbvxuieymwerwvaposmnsruebmifdcqwftkfibpenfgerzxohfeczxtlyabgkdlbelub"), 6501, 4296, string("xlhjeihthfahkqvxfrmifjgxdbyiklvxitleocvfjjrzdaooyfinrhgwzambcpmimzjejhz"), 590, true, 18381);
	this->ykolyeasgwcosdjps(string("gofpxjaykvusnrbgvtwvmusljifsaguirevbuteprbfwzieusj"), 5356, 44792, 6952, false, string("taoztpyfqadsplrohdjgfvcojhgtrxkslaocdetfxindygufzkbhryazzipskomenwwnfnhjfooojhquiyneunwxugxck"), 1183, 1243);
	this->wpkgacnfjyevkecqhnnte(false, 1052);
}
