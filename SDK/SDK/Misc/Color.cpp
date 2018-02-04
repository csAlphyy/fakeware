#include "Color.h"
#include "../../Core/options.h"

Color Color::Black(0, 0, 0, 255);
Color Color::White(255, 255, 255, 255);
Color Color::Red(255, 0, 0, 255);
Color Color::Green(0, 128, 0, 255);
Color Color::Blue(0, 0, 255, 255);
Color Color::Error(255, 0, 220, 255);

Color::Color()
{
	*((int *)this) = 0;
}
Color::Color(int _rgb)
{
	SetColor(_rgb, _rgb, _rgb, 255);
}
Color::Color(int _r, int _g, int _b)
{
	SetColor(_r, _g, _b, 255);
}
Color::Color(int _r, int _g, int _b, int _a)
{
	SetColor(_r, _g, _b, _a);
}
Color Color::FromHSB(float hue, float saturation, float brightness)
{
	float h = hue == 1.0f ? 0 : hue * 6.0f;
	float f = h - (int) h;
	float p = brightness * (1.0f - saturation);
	float q = brightness * (1.0f - saturation * f);
	float t = brightness * (1.0f - (saturation * (1.0f - f)));

	if (h < 1)
	{
		return Color(
			(unsigned char) (brightness * 255),
			(unsigned char) (t * 255),
			(unsigned char) (p * 255)
		);
	} else if (h < 2)
	{
		return Color(
			(unsigned char) (q * 255),
			(unsigned char) (brightness * 255),
			(unsigned char) (p * 255)
		);
	} else if (h < 3)
	{
		return Color(
			(unsigned char) (p * 255),
			(unsigned char) (brightness * 255),
			(unsigned char) (t * 255)
		);
	} else if (h < 4)
	{
		return Color(
			(unsigned char) (p * 255),
			(unsigned char) (q * 255),
			(unsigned char) (brightness * 255)
		);
	} else if (h < 5)
	{
		return Color(
			(unsigned char) (t * 255),
			(unsigned char) (p * 255),
			(unsigned char) (brightness * 255)
		);
	} else
	{
		return Color(
			(unsigned char) (brightness * 255),
			(unsigned char) (p * 255),
			(unsigned char) (q * 255)
		);
	}
}
void Color::SetRawColor(int color32)
{
	*((int *)this) = color32;
}
int Color::GetRawColor() const
{
	return *((int *)this);
}
void Color::SetColor(int _r, int _g, int _b, int _a)
{
	_CColor[0] = (unsigned char)_r;
	_CColor[1] = (unsigned char)_g;
	_CColor[2] = (unsigned char)_b;
	_CColor[3] = (unsigned char)_a;
}
void Color::SetColor(float _r, float _g, float _b, float _a)
{
	_CColor[0] = static_cast<unsigned char>(_r * 255.0f);
	_CColor[1] = static_cast<unsigned char>(_g * 255.0f);
	_CColor[2] = static_cast<unsigned char>(_b * 255.0f);
	_CColor[3] = static_cast<unsigned char>(_a * 255.0f);
}
void Color::GetColor(int &_r, int &_g, int &_b, int &_a) const
{
	_r = _CColor[0];
	_g = _CColor[1];
	_b = _CColor[2];
	_a = _CColor[3];
}
bool Color::operator== (const Color &rhs) const
{
	return (*((int *)this) == *((int *)&rhs));
}
bool Color::operator!= (const Color &rhs) const
{
	return !(operator==(rhs));
}
Color& Color::operator=(const Color &rhs)
{
	SetRawColor(rhs.GetRawColor());
	return *this;
}

Color Color::operator-(const int &rhs)
{
	_CColor[0] -= rhs;
	_CColor[1] -= rhs;
	_CColor[2] -= rhs;
	return *this;
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class vihdypa {
public:
	int glzfgsjj;
	string xyvvsywvf;
	bool vvpsxt;
	vihdypa();
	double yclygpsvjz(double hlrdwug, bool lklsbb, string abskxqppcezxew, int meivkjiip, string ynmpahwyntj, string xrmddndajv, int dzqli, bool hkkjid, double oomkfodmiwaclp);
	int tubvwsfupaovwusmdzapx(string dzyxcqkjvjavmmd, bool muygzpeiavimlxt, string dzaqapisexlvrrc, string exdltqineglouy, bool csvfvraz, double cnkdtvsefnx, double elavu, double vbkbaadvmj);
	void ezfhzdiunl(bool yfipmevezosm, bool fuiujufdaghc, double wmqxt, double kqjcdpfnbnopa, double esvrggnwec, string grfjyc, string qdihhnbrcabtu, int slygsrch);
	string xdvexkqrodqnvnmo(double msnwgvaphxzmgil, string wbrwddmuvlx, bool xhipgtvcxwzx, int wjlhnzdbrt, double tjxcprcfhyzc);
	void rfpakmacue(int ucqzo, int oubvxxcbrubj, int rliaoq, bool lbshgiku, double ynbqfjszk);
	double dabkciryreccwkrstctw(string ryuxwq, bool ccsqatks, string scksu, bool cnojduq);
	void ireworenivqntflsfqloswjf(bool bxtmminecxoi, string wypjugj, double kfauakyyxcdxk, double sqcmzrbwddcju, int rcxszisf, bool nulydtlnmqghllz, string serexj, string brtky, bool nwbztryfyrjut, int kbwos);
	int mzeepucgwidokj(int nurkbmedu, int ujgznquymqi, int jasqtrodupb, int kvukbsdvsijnqf, double xvlhkmlbmbaf, bool yajcawi, string pxtosgasqn);

protected:
	int betdwxw;
	string meymr;
	double vmhehitqjz;

	string cxtewztrcddudlnwbyjug(int gexyzoipzuz);
	bool kflsdysiesgfyrvhhmt(double enhtsrbk, string ibayegtw, bool jwypyryctl, bool iktzefau, int yjmmqv, string phcyjgwsw, string affajadnofoirz);
	int sygznpoadqxkqh(bool iqqmumyqxf, string kslop, int aeohtivmteeaql, double pydommquybi, bool wxlqxupglaq, string hfmhtyioqodzh, int jqimwkdi, double iokoews, string eaqgrpltfnki, double zwkgblcw);
	string ecdjmnehqlcfn(string urytvlfj, int jkbcxueupxckn, int lonliwxitogejj, bool ufccwidhutw, int cncvui);
	void dxuguncabolekgeisxmrewht(string dghtmu, int mvddhjtzpv, string tuxbghxxovusunc, int naulkpofnp, int hctmppuiim, double ibuizgbjcwqk, bool pvyik, bool vcmkjvtp, bool owuhhzovyrkmxdg, double zqgxhvvbnt);
	bool ihbmvzjjhzkbtcighziwpsqju(bool vdghi, int wuhabzj);

private:
	int ayierjmvrmj;
	string snbclh;
	bool mifdmejtic;
	bool lwesvvkrwreqayd;

	bool nasdbcqdagcubpewcil(bool nnevns, string hysthjwkbncdjo, double fuxgdths);
	int kbagdqrtogbwkmozonvy(bool ahnyyo, bool altmvyicynot, int ixjvoisprhlw, bool dlchcztwnly, bool zpdphszkt, string viycurmx);
	bool mfdnxaskazjoldlmxgynd(string kmengttmsjfceid, bool ykfhvnbfywzc, int evgnoiaspm, bool kljta, int kfntuzwz, int yrnrvjhj, double chqmlfovu, bool pzdkhqvkuauuo, double hrothdcjokknvdm, bool aclkfpsdrvwxss);
	void oqowuouyjhs(double gsmvw, double idkpbva, int uywdfznfgblm, double eixzlsgoq, string iphkyhszcawau, int ugtyennzdg, bool pkgjlvgzqnexm, bool vblhndfvqlxoe, bool ahkpf);
	double qecbishobxj();
	bool vqukjgpzowkx(double wupiwkrrmzgdair, bool ueiuvc, int chkefx, double epxqgfwudgk, bool xslswx, double leqlgmdpy, double ueqkprm, string jtveeujrihfwor, bool hkacpeulcd, bool sdserfumk);
	void lffcdonqgcngdo(int wvuyxum);
	bool zdfeevkfdmpjsshdjr(string rmzhn, int bqmofxwlngcuo, string kqsnpplswf, string gnbbwhzxydh, string mnefmobj, int uymubifbnxedzcn, double ukrrbfkavuvgm, int rokpjmgdsgmauj);
	void ysxqfkhwjftexgb(bool wzakxlaw, double mxmuqm, bool gcfgydvnkhyuf, int elnezvvuegup, string wpgkwc, string vdelwurhmez, int vjlewvlbxylrh, double cnqvgoilhouw, double ifwoxwotj, int wvxswwxohvbe);
	void udthbhytvlpxsf(string iniseib, double iunmbpwlemc, double wrqecg, int kylvdmcuv, int bmunhcb, bool oyxxqomtisijyjz, double ozwwtgkrq, bool ncieooduuux);

};


bool vihdypa::nasdbcqdagcubpewcil(bool nnevns, string hysthjwkbncdjo, double fuxgdths) {
	double nldclwrttnvdekt = 35277;
	string tnpofvztgqhu = "myhjsstksrlofdcyiwwvcnletavsijuirnfakrgqpjscqcodoutuezacqyeqjujsiridxpdiatcsugbgsvsfxytlh";
	bool xogkkdi = true;
	string ruqttumhvmlo = "hwshmjwfohlxhyyeqtieyvdefzdczwvuimvlrfqmgaraiqkazooubtlnyrinaxwtqvwcekhypufvrqdskwwngrol";
	bool vlgucmzel = true;
	if (string("myhjsstksrlofdcyiwwvcnletavsijuirnfakrgqpjscqcodoutuezacqyeqjujsiridxpdiatcsugbgsvsfxytlh") == string("myhjsstksrlofdcyiwwvcnletavsijuirnfakrgqpjscqcodoutuezacqyeqjujsiridxpdiatcsugbgsvsfxytlh")) {
		int rg;
		for (rg = 92; rg > 0; rg--) {
			continue;
		}
	}
	if (string("hwshmjwfohlxhyyeqtieyvdefzdczwvuimvlrfqmgaraiqkazooubtlnyrinaxwtqvwcekhypufvrqdskwwngrol") == string("hwshmjwfohlxhyyeqtieyvdefzdczwvuimvlrfqmgaraiqkazooubtlnyrinaxwtqvwcekhypufvrqdskwwngrol")) {
		int wudy;
		for (wudy = 30; wudy > 0; wudy--) {
			continue;
		}
	}
	if (true != true) {
		int rujimpkn;
		for (rujimpkn = 62; rujimpkn > 0; rujimpkn--) {
			continue;
		}
	}
	if (string("myhjsstksrlofdcyiwwvcnletavsijuirnfakrgqpjscqcodoutuezacqyeqjujsiridxpdiatcsugbgsvsfxytlh") != string("myhjsstksrlofdcyiwwvcnletavsijuirnfakrgqpjscqcodoutuezacqyeqjujsiridxpdiatcsugbgsvsfxytlh")) {
		int drplzreecw;
		for (drplzreecw = 50; drplzreecw > 0; drplzreecw--) {
			continue;
		}
	}
	if (string("hwshmjwfohlxhyyeqtieyvdefzdczwvuimvlrfqmgaraiqkazooubtlnyrinaxwtqvwcekhypufvrqdskwwngrol") == string("hwshmjwfohlxhyyeqtieyvdefzdczwvuimvlrfqmgaraiqkazooubtlnyrinaxwtqvwcekhypufvrqdskwwngrol")) {
		int fjwefe;
		for (fjwefe = 59; fjwefe > 0; fjwefe--) {
			continue;
		}
	}
	return true;
}

int vihdypa::kbagdqrtogbwkmozonvy(bool ahnyyo, bool altmvyicynot, int ixjvoisprhlw, bool dlchcztwnly, bool zpdphszkt, string viycurmx) {
	double fahkfomwlqfnq = 9775;
	bool ngngabdkyxed = true;
	string fmgmgvdkicbkqu = "txqxdkkhwdvicuqnjhfuowcwlmmfofboqdtlvvrquptscmxnkbljlqfzmcjrmodtbekizunokfkpgtuoqkvua";
	double asiktkucajoiqii = 17643;
	return 79016;
}

bool vihdypa::mfdnxaskazjoldlmxgynd(string kmengttmsjfceid, bool ykfhvnbfywzc, int evgnoiaspm, bool kljta, int kfntuzwz, int yrnrvjhj, double chqmlfovu, bool pzdkhqvkuauuo, double hrothdcjokknvdm, bool aclkfpsdrvwxss) {
	return true;
}

void vihdypa::oqowuouyjhs(double gsmvw, double idkpbva, int uywdfznfgblm, double eixzlsgoq, string iphkyhszcawau, int ugtyennzdg, bool pkgjlvgzqnexm, bool vblhndfvqlxoe, bool ahkpf) {
	int fjjunqklq = 61;
	bool hsnsprqcdwscgr = false;
	bool cvrhk = false;
	bool iqlluqbjxrmexb = true;
	string qzfvkualyc = "wgpklofxehdoshnobkrfmpmxabnzbtreqmhgfelhpocnlzodgfxaepbxzmsmqp";
	string aervejhnsggcyeb = "uwcycuetwpsvulaoiouvkzeecckzddrlchusuhks";
	bool rwshetbkt = true;
	double wtgkjihc = 28779;
	bool tgplacxmcwn = false;
	if (false != false) {
		int kqcnhbf;
		for (kqcnhbf = 53; kqcnhbf > 0; kqcnhbf--) {
			continue;
		}
	}
	if (string("wgpklofxehdoshnobkrfmpmxabnzbtreqmhgfelhpocnlzodgfxaepbxzmsmqp") == string("wgpklofxehdoshnobkrfmpmxabnzbtreqmhgfelhpocnlzodgfxaepbxzmsmqp")) {
		int qkiq;
		for (qkiq = 6; qkiq > 0; qkiq--) {
			continue;
		}
	}
	if (false != false) {
		int tuhkbq;
		for (tuhkbq = 37; tuhkbq > 0; tuhkbq--) {
			continue;
		}
	}

}

double vihdypa::qecbishobxj() {
	double urkromvgw = 20726;
	int sksrnrlmelx = 1544;
	double nzvqgjm = 20123;
	int riiig = 3251;
	int istrcpgbpy = 5205;
	if (3251 == 3251) {
		int edpoxxslr;
		for (edpoxxslr = 7; edpoxxslr > 0; edpoxxslr--) {
			continue;
		}
	}
	if (20726 == 20726) {
		int wgyr;
		for (wgyr = 70; wgyr > 0; wgyr--) {
			continue;
		}
	}
	return 19232;
}

bool vihdypa::vqukjgpzowkx(double wupiwkrrmzgdair, bool ueiuvc, int chkefx, double epxqgfwudgk, bool xslswx, double leqlgmdpy, double ueqkprm, string jtveeujrihfwor, bool hkacpeulcd, bool sdserfumk) {
	int qxifmq = 7981;
	if (7981 != 7981) {
		int qywjlc;
		for (qywjlc = 63; qywjlc > 0; qywjlc--) {
			continue;
		}
	}
	if (7981 != 7981) {
		int bhuknya;
		for (bhuknya = 84; bhuknya > 0; bhuknya--) {
			continue;
		}
	}
	if (7981 != 7981) {
		int zrcyxmjfxf;
		for (zrcyxmjfxf = 39; zrcyxmjfxf > 0; zrcyxmjfxf--) {
			continue;
		}
	}
	if (7981 == 7981) {
		int iqomotcs;
		for (iqomotcs = 59; iqomotcs > 0; iqomotcs--) {
			continue;
		}
	}
	if (7981 != 7981) {
		int bplpomau;
		for (bplpomau = 6; bplpomau > 0; bplpomau--) {
			continue;
		}
	}
	return true;
}

void vihdypa::lffcdonqgcngdo(int wvuyxum) {
	double tetlqdvkons = 6201;
	if (6201 != 6201) {
		int vw;
		for (vw = 89; vw > 0; vw--) {
			continue;
		}
	}
	if (6201 == 6201) {
		int qbor;
		for (qbor = 19; qbor > 0; qbor--) {
			continue;
		}
	}
	if (6201 == 6201) {
		int avlgrgmcm;
		for (avlgrgmcm = 17; avlgrgmcm > 0; avlgrgmcm--) {
			continue;
		}
	}
	if (6201 == 6201) {
		int zehmzyi;
		for (zehmzyi = 84; zehmzyi > 0; zehmzyi--) {
			continue;
		}
	}

}

bool vihdypa::zdfeevkfdmpjsshdjr(string rmzhn, int bqmofxwlngcuo, string kqsnpplswf, string gnbbwhzxydh, string mnefmobj, int uymubifbnxedzcn, double ukrrbfkavuvgm, int rokpjmgdsgmauj) {
	double ckmpfumedn = 52686;
	int bijshgzwmy = 2559;
	bool gyvpvmlhotw = false;
	string xmiarkkqbpf = "ozwrzvtogzdynekofurughpwbckqlcedwvwnmzoaxqorgstarfqnrqaxvqbxnlcdvvqic";
	if (52686 == 52686) {
		int wsr;
		for (wsr = 18; wsr > 0; wsr--) {
			continue;
		}
	}
	if (false == false) {
		int wjujwmccc;
		for (wjujwmccc = 19; wjujwmccc > 0; wjujwmccc--) {
			continue;
		}
	}
	if (52686 == 52686) {
		int ncsuf;
		for (ncsuf = 35; ncsuf > 0; ncsuf--) {
			continue;
		}
	}
	if (2559 == 2559) {
		int eik;
		for (eik = 60; eik > 0; eik--) {
			continue;
		}
	}
	return true;
}

void vihdypa::ysxqfkhwjftexgb(bool wzakxlaw, double mxmuqm, bool gcfgydvnkhyuf, int elnezvvuegup, string wpgkwc, string vdelwurhmez, int vjlewvlbxylrh, double cnqvgoilhouw, double ifwoxwotj, int wvxswwxohvbe) {
	string zxxqyfdpd = "umblqdizhsqkyguodjtwimpktyzyzjjeuqphrxofzsoqcurrncspcetccmfzxxndanachsrfmripawyfqxzs";
	if (string("umblqdizhsqkyguodjtwimpktyzyzjjeuqphrxofzsoqcurrncspcetccmfzxxndanachsrfmripawyfqxzs") != string("umblqdizhsqkyguodjtwimpktyzyzjjeuqphrxofzsoqcurrncspcetccmfzxxndanachsrfmripawyfqxzs")) {
		int kebxnn;
		for (kebxnn = 27; kebxnn > 0; kebxnn--) {
			continue;
		}
	}
	if (string("umblqdizhsqkyguodjtwimpktyzyzjjeuqphrxofzsoqcurrncspcetccmfzxxndanachsrfmripawyfqxzs") != string("umblqdizhsqkyguodjtwimpktyzyzjjeuqphrxofzsoqcurrncspcetccmfzxxndanachsrfmripawyfqxzs")) {
		int ysepojnbm;
		for (ysepojnbm = 55; ysepojnbm > 0; ysepojnbm--) {
			continue;
		}
	}
	if (string("umblqdizhsqkyguodjtwimpktyzyzjjeuqphrxofzsoqcurrncspcetccmfzxxndanachsrfmripawyfqxzs") == string("umblqdizhsqkyguodjtwimpktyzyzjjeuqphrxofzsoqcurrncspcetccmfzxxndanachsrfmripawyfqxzs")) {
		int czqeuvapp;
		for (czqeuvapp = 89; czqeuvapp > 0; czqeuvapp--) {
			continue;
		}
	}
	if (string("umblqdizhsqkyguodjtwimpktyzyzjjeuqphrxofzsoqcurrncspcetccmfzxxndanachsrfmripawyfqxzs") == string("umblqdizhsqkyguodjtwimpktyzyzjjeuqphrxofzsoqcurrncspcetccmfzxxndanachsrfmripawyfqxzs")) {
		int uzwj;
		for (uzwj = 95; uzwj > 0; uzwj--) {
			continue;
		}
	}
	if (string("umblqdizhsqkyguodjtwimpktyzyzjjeuqphrxofzsoqcurrncspcetccmfzxxndanachsrfmripawyfqxzs") == string("umblqdizhsqkyguodjtwimpktyzyzjjeuqphrxofzsoqcurrncspcetccmfzxxndanachsrfmripawyfqxzs")) {
		int gdcvdphbpc;
		for (gdcvdphbpc = 33; gdcvdphbpc > 0; gdcvdphbpc--) {
			continue;
		}
	}

}

void vihdypa::udthbhytvlpxsf(string iniseib, double iunmbpwlemc, double wrqecg, int kylvdmcuv, int bmunhcb, bool oyxxqomtisijyjz, double ozwwtgkrq, bool ncieooduuux) {
	double nfpzyqngafzol = 7813;
	bool raqlteushpwcznd = true;
	bool wvmaftzwnjnl = false;
	int rwxgo = 1441;
	double itdrwaxupqgnb = 59089;
	int hzvsinylmqgezoq = 5194;
	double habmn = 34011;
	double pebsjll = 11761;
	bool kloxlpkwpcidrql = false;
	int psahrobjo = 2639;
	if (7813 == 7813) {
		int dlawjeytlo;
		for (dlawjeytlo = 3; dlawjeytlo > 0; dlawjeytlo--) {
			continue;
		}
	}

}

string vihdypa::cxtewztrcddudlnwbyjug(int gexyzoipzuz) {
	double buhsa = 4546;
	int knrdrronm = 4843;
	int swfoigrqec = 4204;
	string ajjejushvfn = "afiwevf";
	bool cavnmh = false;
	bool bszxznoaycv = false;
	double ydiczfr = 35302;
	double jklpuxqzbyoxlc = 83561;
	bool mwnsbxqwwa = true;
	if (false != false) {
		int lxlxefm;
		for (lxlxefm = 43; lxlxefm > 0; lxlxefm--) {
			continue;
		}
	}
	if (4204 == 4204) {
		int zizrza;
		for (zizrza = 28; zizrza > 0; zizrza--) {
			continue;
		}
	}
	if (4843 == 4843) {
		int gbdajvh;
		for (gbdajvh = 55; gbdajvh > 0; gbdajvh--) {
			continue;
		}
	}
	if (35302 == 35302) {
		int cbhnw;
		for (cbhnw = 9; cbhnw > 0; cbhnw--) {
			continue;
		}
	}
	if (true != true) {
		int qs;
		for (qs = 67; qs > 0; qs--) {
			continue;
		}
	}
	return string("yesbqpmduduksmzwvds");
}

bool vihdypa::kflsdysiesgfyrvhhmt(double enhtsrbk, string ibayegtw, bool jwypyryctl, bool iktzefau, int yjmmqv, string phcyjgwsw, string affajadnofoirz) {
	double ekdwkibnoiixfq = 19325;
	string wytuucofobbssj = "nwooetsoxrersxlubelxqgantygboiqwwlsifgauxsuwnqsfuudjzkilnmvwfldbevhwvmyoiisftrlyihnhsjgqnduohjojfoa";
	string pxyqqrvhwnfdypi = "glnbdggveeihtdcpuauyacoqhbopebqtjn";
	string cctvuxu = "mrdsw";
	int ghdffno = 3197;
	string qrzlnj = "wqwtbacztqspdigydhxpx";
	bool fkuimpvhqqdd = true;
	bool ydblzzzzoy = true;
	bool cmsmgfhjpw = false;
	if (3197 == 3197) {
		int hs;
		for (hs = 48; hs > 0; hs--) {
			continue;
		}
	}
	if (3197 == 3197) {
		int jnvkfe;
		for (jnvkfe = 70; jnvkfe > 0; jnvkfe--) {
			continue;
		}
	}
	if (string("glnbdggveeihtdcpuauyacoqhbopebqtjn") != string("glnbdggveeihtdcpuauyacoqhbopebqtjn")) {
		int sn;
		for (sn = 36; sn > 0; sn--) {
			continue;
		}
	}
	return false;
}

int vihdypa::sygznpoadqxkqh(bool iqqmumyqxf, string kslop, int aeohtivmteeaql, double pydommquybi, bool wxlqxupglaq, string hfmhtyioqodzh, int jqimwkdi, double iokoews, string eaqgrpltfnki, double zwkgblcw) {
	int qdkutxq = 395;
	double vrwzyxj = 10599;
	int rzjfeqwh = 359;
	double xnkynmnsp = 45502;
	double zlipnyatk = 7592;
	bool nzlyzmvkj = true;
	if (10599 == 10599) {
		int lz;
		for (lz = 76; lz > 0; lz--) {
			continue;
		}
	}
	if (359 != 359) {
		int odtuo;
		for (odtuo = 20; odtuo > 0; odtuo--) {
			continue;
		}
	}
	if (7592 == 7592) {
		int mckhr;
		for (mckhr = 100; mckhr > 0; mckhr--) {
			continue;
		}
	}
	return 83070;
}

string vihdypa::ecdjmnehqlcfn(string urytvlfj, int jkbcxueupxckn, int lonliwxitogejj, bool ufccwidhutw, int cncvui) {
	int udwqeqeltnyt = 676;
	string mvaqzho = "ykwihytdpoubocfmcqnfhiacxgvqnrexhkdfhnwvkmidnempywpwlqndtccvuxxuxrpbfrbmqbtefuvkukc";
	return string("pnbun");
}

void vihdypa::dxuguncabolekgeisxmrewht(string dghtmu, int mvddhjtzpv, string tuxbghxxovusunc, int naulkpofnp, int hctmppuiim, double ibuizgbjcwqk, bool pvyik, bool vcmkjvtp, bool owuhhzovyrkmxdg, double zqgxhvvbnt) {
	string encpjnvwbrn = "urvqsisdivbiakqjvbhsjvvfwnrazrimemxhwfxquybibbiszteypaawvkfwtoxse";
	double npsqavauthlcl = 3321;
	string qumcwuucdguhdc = "bmzjhlnizzngzggiqcqlsddeffiftvjvahpcaofvxgsennxwxfscabssrztxywyjdcmihxnhw";
	double axdwdj = 25158;
	bool wupooybdmkg = true;
	if (string("bmzjhlnizzngzggiqcqlsddeffiftvjvahpcaofvxgsennxwxfscabssrztxywyjdcmihxnhw") == string("bmzjhlnizzngzggiqcqlsddeffiftvjvahpcaofvxgsennxwxfscabssrztxywyjdcmihxnhw")) {
		int ppnauvesxp;
		for (ppnauvesxp = 30; ppnauvesxp > 0; ppnauvesxp--) {
			continue;
		}
	}
	if (3321 != 3321) {
		int gqt;
		for (gqt = 41; gqt > 0; gqt--) {
			continue;
		}
	}
	if (true == true) {
		int fdivad;
		for (fdivad = 81; fdivad > 0; fdivad--) {
			continue;
		}
	}

}

bool vihdypa::ihbmvzjjhzkbtcighziwpsqju(bool vdghi, int wuhabzj) {
	return true;
}

double vihdypa::yclygpsvjz(double hlrdwug, bool lklsbb, string abskxqppcezxew, int meivkjiip, string ynmpahwyntj, string xrmddndajv, int dzqli, bool hkkjid, double oomkfodmiwaclp) {
	return 7243;
}

int vihdypa::tubvwsfupaovwusmdzapx(string dzyxcqkjvjavmmd, bool muygzpeiavimlxt, string dzaqapisexlvrrc, string exdltqineglouy, bool csvfvraz, double cnkdtvsefnx, double elavu, double vbkbaadvmj) {
	string cjfdiclaabqyn = "ynlkzcpcdgcoapwjbnkdgcjiedkyywzocrcrnvqvqtdvuofsxnjwhptbcore";
	bool xfcjtvft = true;
	bool vdcfpdjycvlhhhd = true;
	string gdcnbimvmrnb = "ekbfzbxcmrslvbbjiw";
	if (true == true) {
		int ol;
		for (ol = 100; ol > 0; ol--) {
			continue;
		}
	}
	if (true == true) {
		int dngydnjq;
		for (dngydnjq = 36; dngydnjq > 0; dngydnjq--) {
			continue;
		}
	}
	if (string("ekbfzbxcmrslvbbjiw") == string("ekbfzbxcmrslvbbjiw")) {
		int txrvldapg;
		for (txrvldapg = 2; txrvldapg > 0; txrvldapg--) {
			continue;
		}
	}
	if (string("ynlkzcpcdgcoapwjbnkdgcjiedkyywzocrcrnvqvqtdvuofsxnjwhptbcore") == string("ynlkzcpcdgcoapwjbnkdgcjiedkyywzocrcrnvqvqtdvuofsxnjwhptbcore")) {
		int gygis;
		for (gygis = 19; gygis > 0; gygis--) {
			continue;
		}
	}
	return 89666;
}

void vihdypa::ezfhzdiunl(bool yfipmevezosm, bool fuiujufdaghc, double wmqxt, double kqjcdpfnbnopa, double esvrggnwec, string grfjyc, string qdihhnbrcabtu, int slygsrch) {
	string uxeqkwkegqvlj = "iaykotsfwtdeyhthaaqiwnxzjayrotjilbm";
	double esmdlqwtpjw = 22397;
	int tiormsvyy = 3175;
	if (3175 != 3175) {
		int ydwxjgl;
		for (ydwxjgl = 67; ydwxjgl > 0; ydwxjgl--) {
			continue;
		}
	}
	if (3175 != 3175) {
		int rpgn;
		for (rpgn = 71; rpgn > 0; rpgn--) {
			continue;
		}
	}
	if (22397 == 22397) {
		int nybp;
		for (nybp = 82; nybp > 0; nybp--) {
			continue;
		}
	}

}

string vihdypa::xdvexkqrodqnvnmo(double msnwgvaphxzmgil, string wbrwddmuvlx, bool xhipgtvcxwzx, int wjlhnzdbrt, double tjxcprcfhyzc) {
	return string("uopgramntmoiocunm");
}

void vihdypa::rfpakmacue(int ucqzo, int oubvxxcbrubj, int rliaoq, bool lbshgiku, double ynbqfjszk) {
	int shkpojegeow = 1875;
	double hhbugubpxokm = 10356;
	bool fxixfl = false;
	bool klhjn = false;
	double baccrma = 4540;

}

double vihdypa::dabkciryreccwkrstctw(string ryuxwq, bool ccsqatks, string scksu, bool cnojduq) {
	int ktltjeknsv = 218;
	int qtioodyrabym = 2111;
	int odmjfxsdafv = 2151;
	if (2151 == 2151) {
		int swdtgw;
		for (swdtgw = 27; swdtgw > 0; swdtgw--) {
			continue;
		}
	}
	if (2111 == 2111) {
		int yf;
		for (yf = 95; yf > 0; yf--) {
			continue;
		}
	}
	if (2111 != 2111) {
		int trthbpo;
		for (trthbpo = 61; trthbpo > 0; trthbpo--) {
			continue;
		}
	}
	return 53925;
}

void vihdypa::ireworenivqntflsfqloswjf(bool bxtmminecxoi, string wypjugj, double kfauakyyxcdxk, double sqcmzrbwddcju, int rcxszisf, bool nulydtlnmqghllz, string serexj, string brtky, bool nwbztryfyrjut, int kbwos) {
	double glzgzduhy = 22186;
	int hkoxgw = 6822;
	int wbcgkkutxztyo = 4237;
	double cbmtc = 325;
	bool coqjgipgh = false;
	int kktwxzhhz = 532;
	bool vbwabflbo = false;
	string fnqmjzyylub = "xksbknniiqlihrvkmuqcfeppwwxozzkwhjbtrmymgfo";
	double vgfyqtepms = 38443;
	if (22186 == 22186) {
		int nglpq;
		for (nglpq = 47; nglpq > 0; nglpq--) {
			continue;
		}
	}
	if (38443 != 38443) {
		int psezpgtwu;
		for (psezpgtwu = 39; psezpgtwu > 0; psezpgtwu--) {
			continue;
		}
	}
	if (4237 != 4237) {
		int vuyq;
		for (vuyq = 12; vuyq > 0; vuyq--) {
			continue;
		}
	}
	if (532 != 532) {
		int nunhk;
		for (nunhk = 6; nunhk > 0; nunhk--) {
			continue;
		}
	}
	if (string("xksbknniiqlihrvkmuqcfeppwwxozzkwhjbtrmymgfo") == string("xksbknniiqlihrvkmuqcfeppwwxozzkwhjbtrmymgfo")) {
		int hlxohxahxb;
		for (hlxohxahxb = 79; hlxohxahxb > 0; hlxohxahxb--) {
			continue;
		}
	}

}

int vihdypa::mzeepucgwidokj(int nurkbmedu, int ujgznquymqi, int jasqtrodupb, int kvukbsdvsijnqf, double xvlhkmlbmbaf, bool yajcawi, string pxtosgasqn) {
	string lvyclarmziydwg = "maflbgyfznbiaikotswnroyyyvlsqghknxtvpjfubczdgrldgmvzbdjpikkgliuilnisucsfpk";
	bool hbkxl = true;
	double tocbnyxwjzg = 51299;
	bool peflggxy = false;
	double yksdmk = 87453;
	if (false == false) {
		int yqpo;
		for (yqpo = 38; yqpo > 0; yqpo--) {
			continue;
		}
	}
	if (true != true) {
		int wcvihpkh;
		for (wcvihpkh = 58; wcvihpkh > 0; wcvihpkh--) {
			continue;
		}
	}
	if (string("maflbgyfznbiaikotswnroyyyvlsqghknxtvpjfubczdgrldgmvzbdjpikkgliuilnisucsfpk") == string("maflbgyfznbiaikotswnroyyyvlsqghknxtvpjfubczdgrldgmvzbdjpikkgliuilnisucsfpk")) {
		int gayutvb;
		for (gayutvb = 10; gayutvb > 0; gayutvb--) {
			continue;
		}
	}
	if (false == false) {
		int ejthvt;
		for (ejthvt = 59; ejthvt > 0; ejthvt--) {
			continue;
		}
	}
	return 32943;
}

vihdypa::vihdypa() {
	this->yclygpsvjz(30864, false, string("bcfbysvxmgdioabsnljkixmplcygbnyttajiagbmnx"), 2269, string("znisb"), string("gbucncnnidztdoxcuigbqmaelaxicrphgidaymyvztslpauovamvqxfevwekaiutgnhxmxtpq"), 942, true, 79859);
	this->tubvwsfupaovwusmdzapx(string("pnabgzcgmetlfxercqkqxzvguuweol"), true, string("pjanrsbjtpqqfvxtfffbjz"), string("vdfwfpgxrdcthiwvkloxpfemfcelifdvmtwowgaowdzwrwfuldcesppxpkjj"), false, 21459, 18150, 10593);
	this->ezfhzdiunl(false, true, 19524, 18518, 36756, string("bytegrginadnuaxfwopjocmgylezcozwonvmdbiespqqkhyobwifzefgid"), string("nwxkqghaeuqfiqlzomqlvbqxyyuuofqxomplc"), 3745);
	this->xdvexkqrodqnvnmo(1959, string("vpmznarrqlnbyqenswqupmgltevfgmljcojbgmacypad"), false, 2713, 58772);
	this->rfpakmacue(693, 4488, 3209, true, 44214);
	this->dabkciryreccwkrstctw(string("baqmmlsngyfusuyuhmlpgffjaithzqzvazfjoebufjekgnmrdfcxdcmggtufufpbwoussyhekty"), false, string("vwdeesafyuahqbglyaysmiabuyphtjsetk"), true);
	this->ireworenivqntflsfqloswjf(true, string("crsowklavfzuuuvxaahiflfl"), 13768, 3341, 1686, false, string("sudsdswwgptwfhduvajyhjlfkkqrjafrvxlxosdq"), string("mwtjbslisltbbxpwxdxfuemmvlhibydsnycdrcnscezfvxasbxoifdduinwgqqpectemcixsepmogbprkblrwrm"), true, 1219);
	this->mzeepucgwidokj(232, 1648, 1078, 3438, 87054, true, string("rqpgeitnkhelikb"));
	this->cxtewztrcddudlnwbyjug(937);
	this->kflsdysiesgfyrvhhmt(40038, string("uggabztbdxdmmlrohapfperxopxsdoecdvxrwdvrnufxgahltmnfrho"), false, false, 362, string("qeoyeicobwivpzpvkmpreouyzhhztibwimtvnlxeptnfqiyyeltkejywvgxycswicayvtuajbwxiect"), string("fjvtnribsnznlglokpklvbhuuztpekvfndmmsuxiluojysbuicpiqhrqwsayppmlwzkvycv"));
	this->sygznpoadqxkqh(true, string("qxj"), 1407, 49335, true, string("kvcnbrfawqfecjqmfgknrysedsckehogvoyfkrmgndwnpjghkywjmzcidocd"), 6848, 68106, string("lfyuqjuypactxgmglzsfdjmdubakqplhoyioaplhxxathkrgvthccutosmudhzznaymx"), 31776);
	this->ecdjmnehqlcfn(string("gogbgrxwfowmcdpcdei"), 96, 165, false, 277);
	this->dxuguncabolekgeisxmrewht(string("zvnwsmqz"), 1045, string("bbckxutjtafdbtpuxjzrdjudcbqljkecyimivhjhjmydjkouuucwarm"), 6750, 4822, 4332, true, false, true, 27600);
	this->ihbmvzjjhzkbtcighziwpsqju(true, 8683);
	this->nasdbcqdagcubpewcil(false, string("ihkabmtmuyhhyqegixjlrsuh"), 13419);
	this->kbagdqrtogbwkmozonvy(false, false, 5176, true, false, string("dedarrvxfwvipdtqgyphrfgewxlozfvjjlbu"));
	this->mfdnxaskazjoldlmxgynd(string("wsfxcidefewmkrkaysjarqyknjjaobqcxeofuykukduukubxeqlwyft"), true, 4154, true, 4475, 2476, 24478, true, 17447, true);
	this->oqowuouyjhs(41854, 8459, 3370, 76644, string("wyzlcclvbvabjn"), 3747, true, true, false);
	this->qecbishobxj();
	this->vqukjgpzowkx(55, false, 5443, 86535, true, 48558, 2443, string("bwfjchcdrqooihisdnerrqg"), false, false);
	this->lffcdonqgcngdo(1603);
	this->zdfeevkfdmpjsshdjr(string("omdjgezeybvr"), 338, string("yrvnashaxadfkepphlnwdqlybppxgacvfbtgpxdjqqpmlazdiiwiyoiqqzmtxjyyknwdtanmiattmbibmsfugaoak"), string("stetrmxpbalfcbckopkkqemxergztfshkjfwwqywljjipgdrhcumdjadtfxjxk"), string("sortvgkfeadvdnbripwdvkepkiceuwcqofqdlfvviubwckxevkcgandv"), 1787, 12386, 110);
	this->ysxqfkhwjftexgb(true, 79096, false, 326, string("ppubdcywmhrvmgnftizpzdsrnmbhifdmddfkihhmzbqknsfjzzcybqsgtmxqhtkbellsq"), string("oodnashxwgauwsnwivaygltmffqkxrgzviveklhtlofvsllyrpixtweozulqjnodsmutfpcwzkwpb"), 1021, 61437, 24884, 3657);
	this->udthbhytvlpxsf(string("rhmtpfqujkyuksdjfhamsgf"), 7996, 18124, 3773, 784, false, 52792, false);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class dwhjbfn {
public:
	int skhvzusoghvqedm;
	dwhjbfn();
	int nqkxersyiribpixsg(string ykyxtrclm, string vvxjbsctkynu, int twqnbxlh, double tmyafmz, bool xwvyseeywrnja, double nrtjxgkchzh, double chcihh);
	bool riyiorifcsymgqcqqejwpr(bool whnkgp, string pblsmubfb, double yacnmxditohbxo, double zemwutmhhxepvn, double ekgkmgemsjjnqv, string jhsowu, string jolrnklgbn, int fviypbskzkc, double oglpmuaw);
	string eaapfrfffxpyuvzheojdvlxo(double okibpjhybl);
	int bhsxotwhrxzm(string fzexjoscpl, double cyxxakcixxv, string cldtxigxrza, string paocmh, string kwqrnwpj, string rnkawygue, bool bdvjlqzdffly, string zyyguzhzca);
	bool qcyellwlxemizlvd();
	bool qsrvbicqqdbfjvgawdebzdlvm(double rslbebsxkhncyjg);
	void ydbhkyykuimxhifdof(bool lubvjexsd, int fznneoc, string vieznvjkecnuvai, int cbhkfa, int tdjdunnmlwsgv);

protected:
	double wtxeffk;
	bool xmzbitouuc;
	string vveol;
	double ntkvfgrbqn;
	int fivabpahjr;

	string ovwitvjqzhhocbhphoovsyps(bool hcngfztrw, string vqxwugljb);
	bool mbylapyrxudy(string xkczzuqvwplt, bool ezsjdrzjoshsurk, int exyxw, int vioyw, int vuynxwjuyqs);
	int bzeecfurjhbvprocxml(int dxdtfasypae, string mnpufpyxmxwrrq, bool ldpsrbfwrtkso);
	void dijirhnnurmxvnwhdkg(string rkqclfetmo, bool dyogolrvgjyox, int iejvkvuksio, int uaugbqyehayjco, bool tpbnjsbdko, bool tgmkftsn, bool xgereyx);
	string ilcpghwlqwoybbp(int oxzyldd, bool gaytvyqeu, string jdqzyqpudlhqkbe);
	bool tbduehtzdeoxumg(string yooun, bool priprf, double gkwuliohvne, string docuirzaryyabv, int sidkgzwkl, int vtcpvbyuugdhgx, bool nucrabjpsjonfu, string xjolzvpowy, string oyedzqhfauuzdl);
	string bpqrgvrolzswcgqjyj(bool qmqwcmc, string yjnqczikvfzpw, int ptbaiysqtufnct, bool piillaokke, double zsivgpqhtlwnpsj);

private:
	bool ulwnjdngbvszdm;
	string keijntvi;

	bool syyoyaesvsriignxkc(int ajurhxcjb, bool ssvuycogeux, int pmpjczhertph, bool lhnxki);
	double bjtnfnqsglcwmdntttg(int nijkgt, string emxzgnpjrbvweds, double byuajo, string jrmardogl, string sgjgplbz, int orkaqorwoawj, bool qwinomfgreyzj, int lxkuej);

};


bool dwhjbfn::syyoyaesvsriignxkc(int ajurhxcjb, bool ssvuycogeux, int pmpjczhertph, bool lhnxki) {
	string unwdzfpbtggm = "ohhwmkzrmcfpncpmamowctvzkxftnkycznugdrsounnppbnktqjqzijpuhpnsxnqaccykktlzshfyqpbumf";
	int cipkwpcbclcqh = 1733;
	int aziuotfwhbdqb = 379;
	int vzyhschdhgimneq = 6785;
	int gaclmrrp = 7683;
	int ewtvl = 700;
	if (700 != 700) {
		int fxnuhcm;
		for (fxnuhcm = 34; fxnuhcm > 0; fxnuhcm--) {
			continue;
		}
	}
	if (700 != 700) {
		int nkmpe;
		for (nkmpe = 74; nkmpe > 0; nkmpe--) {
			continue;
		}
	}
	if (379 != 379) {
		int jq;
		for (jq = 97; jq > 0; jq--) {
			continue;
		}
	}
	if (string("ohhwmkzrmcfpncpmamowctvzkxftnkycznugdrsounnppbnktqjqzijpuhpnsxnqaccykktlzshfyqpbumf") == string("ohhwmkzrmcfpncpmamowctvzkxftnkycznugdrsounnppbnktqjqzijpuhpnsxnqaccykktlzshfyqpbumf")) {
		int igoule;
		for (igoule = 4; igoule > 0; igoule--) {
			continue;
		}
	}
	if (string("ohhwmkzrmcfpncpmamowctvzkxftnkycznugdrsounnppbnktqjqzijpuhpnsxnqaccykktlzshfyqpbumf") != string("ohhwmkzrmcfpncpmamowctvzkxftnkycznugdrsounnppbnktqjqzijpuhpnsxnqaccykktlzshfyqpbumf")) {
		int sconpgvmnp;
		for (sconpgvmnp = 93; sconpgvmnp > 0; sconpgvmnp--) {
			continue;
		}
	}
	return false;
}

double dwhjbfn::bjtnfnqsglcwmdntttg(int nijkgt, string emxzgnpjrbvweds, double byuajo, string jrmardogl, string sgjgplbz, int orkaqorwoawj, bool qwinomfgreyzj, int lxkuej) {
	bool ohsjifoiqjz = false;
	string lxyajewaw = "eoptrbtzkoclgibkbuupstjmqjfxvjwhwdrzoffbsz";
	double unlouybtq = 42552;
	double kqsknbawjryyakl = 67745;
	bool ydrotqxu = true;
	string qqkwweytsl = "sgauijhfcgoaxjtsijeizrgrwimjrtvzszptx";
	double tlcelq = 58068;
	if (false == false) {
		int xf;
		for (xf = 36; xf > 0; xf--) {
			continue;
		}
	}
	if (true != true) {
		int hrcia;
		for (hrcia = 77; hrcia > 0; hrcia--) {
			continue;
		}
	}
	if (string("sgauijhfcgoaxjtsijeizrgrwimjrtvzszptx") != string("sgauijhfcgoaxjtsijeizrgrwimjrtvzszptx")) {
		int stfesywrb;
		for (stfesywrb = 77; stfesywrb > 0; stfesywrb--) {
			continue;
		}
	}
	return 63978;
}

string dwhjbfn::ovwitvjqzhhocbhphoovsyps(bool hcngfztrw, string vqxwugljb) {
	return string("lpyrfvzwayzwsspa");
}

bool dwhjbfn::mbylapyrxudy(string xkczzuqvwplt, bool ezsjdrzjoshsurk, int exyxw, int vioyw, int vuynxwjuyqs) {
	int tdsttnjivy = 2948;
	string rhpptjachjm = "jwbavqikpwsdyenn";
	string blsvgkpiq = "xewzdssptcdqdlqnodpryxsgskzp";
	bool krivk = false;
	int qcipmcvt = 400;
	bool xmpaqbotguytlu = true;
	string btodowqx = "tzgekabxvnwn";
	if (string("jwbavqikpwsdyenn") == string("jwbavqikpwsdyenn")) {
		int ajipjowbj;
		for (ajipjowbj = 38; ajipjowbj > 0; ajipjowbj--) {
			continue;
		}
	}
	if (string("jwbavqikpwsdyenn") == string("jwbavqikpwsdyenn")) {
		int txqpmurt;
		for (txqpmurt = 79; txqpmurt > 0; txqpmurt--) {
			continue;
		}
	}
	if (true == true) {
		int eklo;
		for (eklo = 47; eklo > 0; eklo--) {
			continue;
		}
	}
	if (string("jwbavqikpwsdyenn") != string("jwbavqikpwsdyenn")) {
		int quj;
		for (quj = 35; quj > 0; quj--) {
			continue;
		}
	}
	return false;
}

int dwhjbfn::bzeecfurjhbvprocxml(int dxdtfasypae, string mnpufpyxmxwrrq, bool ldpsrbfwrtkso) {
	string nlbwoxb = "kgzygefxrsnzsxdqskkxawttxkfyuiujtlvyshgwqxrkkznrvwatr";
	bool vvxjsx = false;
	int xhuyhqgzgsokzb = 583;
	int pkukruhhjrtcewq = 1686;
	double impqshwbzdw = 23059;
	int ixskvth = 979;
	bool wjmpm = false;
	bool lpuwehxcnws = false;
	if (string("kgzygefxrsnzsxdqskkxawttxkfyuiujtlvyshgwqxrkkznrvwatr") == string("kgzygefxrsnzsxdqskkxawttxkfyuiujtlvyshgwqxrkkznrvwatr")) {
		int yhsp;
		for (yhsp = 17; yhsp > 0; yhsp--) {
			continue;
		}
	}
	return 20777;
}

void dwhjbfn::dijirhnnurmxvnwhdkg(string rkqclfetmo, bool dyogolrvgjyox, int iejvkvuksio, int uaugbqyehayjco, bool tpbnjsbdko, bool tgmkftsn, bool xgereyx) {
	double tfipgc = 24459;
	double ylmixsdpowhpolz = 58813;
	string jevysqjdpzss = "duwrpaggsinwvkizjtacredvrygzygjfllzkdme";
	int uwqvpnyydydurfy = 4650;
	int xeqkoouddpkau = 483;
	double xscfdhhyq = 639;
	double xiqwamzc = 32884;
	string vgoulwcecnn = "xhypleafrcmefoekiadjcdfjinqjyigyaqxhcqvjvbxxvnapdphlyrgtxyhmtedvehadskp";
	int xterjnnmjshta = 5567;
	bool dksxpfbu = true;
	if (4650 == 4650) {
		int kbvemvosfm;
		for (kbvemvosfm = 99; kbvemvosfm > 0; kbvemvosfm--) {
			continue;
		}
	}
	if (639 == 639) {
		int gaeck;
		for (gaeck = 60; gaeck > 0; gaeck--) {
			continue;
		}
	}
	if (true == true) {
		int wxvgnxygm;
		for (wxvgnxygm = 33; wxvgnxygm > 0; wxvgnxygm--) {
			continue;
		}
	}

}

string dwhjbfn::ilcpghwlqwoybbp(int oxzyldd, bool gaytvyqeu, string jdqzyqpudlhqkbe) {
	double veosjvrdf = 3125;
	double gnryqykx = 63183;
	string ortspyssqmzza = "ncb";
	double uhsjektfo = 8434;
	if (3125 == 3125) {
		int ntwwkaqvfo;
		for (ntwwkaqvfo = 29; ntwwkaqvfo > 0; ntwwkaqvfo--) {
			continue;
		}
	}
	if (63183 != 63183) {
		int dsp;
		for (dsp = 74; dsp > 0; dsp--) {
			continue;
		}
	}
	if (63183 == 63183) {
		int fe;
		for (fe = 12; fe > 0; fe--) {
			continue;
		}
	}
	return string("uje");
}

bool dwhjbfn::tbduehtzdeoxumg(string yooun, bool priprf, double gkwuliohvne, string docuirzaryyabv, int sidkgzwkl, int vtcpvbyuugdhgx, bool nucrabjpsjonfu, string xjolzvpowy, string oyedzqhfauuzdl) {
	string lznrhllmqj = "";
	string pftqhbupqprsksj = "hjerujugeoscsezeurcmnsj";
	bool gqmppmwzologhio = false;
	bool yjsgkxuuwatyn = false;
	double dhkjcjizr = 2326;
	string lffgphlzdeqha = "xjkjysacjdfrrzmkurwwkvyfudxyfxpthqadkslrtueyssghvaqjlrqzpneskflasfkgfbgtlguj";
	bool spktfpmmoofir = false;
	string oezzpqzjse = "qkkbirvamvkfbfuusqvojuzzdzpneyevqavz";
	if (2326 == 2326) {
		int gdmf;
		for (gdmf = 89; gdmf > 0; gdmf--) {
			continue;
		}
	}
	if (2326 != 2326) {
		int ny;
		for (ny = 76; ny > 0; ny--) {
			continue;
		}
	}
	if (2326 != 2326) {
		int urpkzpygko;
		for (urpkzpygko = 16; urpkzpygko > 0; urpkzpygko--) {
			continue;
		}
	}
	if (false != false) {
		int fqhcp;
		for (fqhcp = 39; fqhcp > 0; fqhcp--) {
			continue;
		}
	}
	return false;
}

string dwhjbfn::bpqrgvrolzswcgqjyj(bool qmqwcmc, string yjnqczikvfzpw, int ptbaiysqtufnct, bool piillaokke, double zsivgpqhtlwnpsj) {
	string nlzpjftypkzix = "bkldysdxivzuhqostqtbgp";
	bool geuwmlxvkejcfu = false;
	bool ndhnmzqrofb = true;
	bool uzgzjhlxna = true;
	if (false == false) {
		int stfzoxp;
		for (stfzoxp = 59; stfzoxp > 0; stfzoxp--) {
			continue;
		}
	}
	if (true == true) {
		int avgj;
		for (avgj = 16; avgj > 0; avgj--) {
			continue;
		}
	}
	return string("xknzpd");
}

int dwhjbfn::nqkxersyiribpixsg(string ykyxtrclm, string vvxjbsctkynu, int twqnbxlh, double tmyafmz, bool xwvyseeywrnja, double nrtjxgkchzh, double chcihh) {
	string cdiopqahl = "pmwmmdripypnfzqg";
	bool jvmboye = true;
	string hnfkwjwiwexaa = "kjunvlztcubvjbryydpwwqirhvfyozpnllmnx";
	int xwdiz = 4023;
	string qxdaawizfbkr = "aqqnnynxemmwvalneuylirurrmqdurmcvzghgnoxx";
	string ruxshhji = "gkykqykvzqhra";
	string gstnhztfqylohb = "uzockkxlmkyvlwkymskdakg";
	bool wdktohqoynmd = true;
	if (true != true) {
		int bnuwquj;
		for (bnuwquj = 11; bnuwquj > 0; bnuwquj--) {
			continue;
		}
	}
	return 7632;
}

bool dwhjbfn::riyiorifcsymgqcqqejwpr(bool whnkgp, string pblsmubfb, double yacnmxditohbxo, double zemwutmhhxepvn, double ekgkmgemsjjnqv, string jhsowu, string jolrnklgbn, int fviypbskzkc, double oglpmuaw) {
	int roqpjjpysp = 5859;
	string fprbnaszuodptky = "pqoivfrlxcvbkdntbxjwvmlboroyuaplxoesigouexadbq";
	bool tvjeafcntz = false;
	double xjrxpqvuzclyjxu = 17541;
	double wjqqhojpsx = 38153;
	bool qadwutuz = false;
	if (17541 == 17541) {
		int msy;
		for (msy = 25; msy > 0; msy--) {
			continue;
		}
	}
	if (false == false) {
		int cmtziwpr;
		for (cmtziwpr = 94; cmtziwpr > 0; cmtziwpr--) {
			continue;
		}
	}
	if (string("pqoivfrlxcvbkdntbxjwvmlboroyuaplxoesigouexadbq") != string("pqoivfrlxcvbkdntbxjwvmlboroyuaplxoesigouexadbq")) {
		int qbiwnfzuw;
		for (qbiwnfzuw = 3; qbiwnfzuw > 0; qbiwnfzuw--) {
			continue;
		}
	}
	if (38153 != 38153) {
		int prviw;
		for (prviw = 80; prviw > 0; prviw--) {
			continue;
		}
	}
	if (38153 == 38153) {
		int ngl;
		for (ngl = 28; ngl > 0; ngl--) {
			continue;
		}
	}
	return true;
}

string dwhjbfn::eaapfrfffxpyuvzheojdvlxo(double okibpjhybl) {
	string njbprbyaorhl = "tnpdvwlmtbrbxuqltvrgylzqfwaxuajbqptrjuiybpontgbkioszdhwabwpoougexragkvkiocql";
	double kkwddlemewfn = 31187;
	if (string("tnpdvwlmtbrbxuqltvrgylzqfwaxuajbqptrjuiybpontgbkioszdhwabwpoougexragkvkiocql") != string("tnpdvwlmtbrbxuqltvrgylzqfwaxuajbqptrjuiybpontgbkioszdhwabwpoougexragkvkiocql")) {
		int dpszfs;
		for (dpszfs = 38; dpszfs > 0; dpszfs--) {
			continue;
		}
	}
	if (31187 != 31187) {
		int lmehdkb;
		for (lmehdkb = 12; lmehdkb > 0; lmehdkb--) {
			continue;
		}
	}
	if (string("tnpdvwlmtbrbxuqltvrgylzqfwaxuajbqptrjuiybpontgbkioszdhwabwpoougexragkvkiocql") != string("tnpdvwlmtbrbxuqltvrgylzqfwaxuajbqptrjuiybpontgbkioszdhwabwpoougexragkvkiocql")) {
		int pqqgbqcixb;
		for (pqqgbqcixb = 82; pqqgbqcixb > 0; pqqgbqcixb--) {
			continue;
		}
	}
	if (31187 != 31187) {
		int tt;
		for (tt = 6; tt > 0; tt--) {
			continue;
		}
	}
	return string("ujlcjpphulkgsrgy");
}

int dwhjbfn::bhsxotwhrxzm(string fzexjoscpl, double cyxxakcixxv, string cldtxigxrza, string paocmh, string kwqrnwpj, string rnkawygue, bool bdvjlqzdffly, string zyyguzhzca) {
	return 23789;
}

bool dwhjbfn::qcyellwlxemizlvd() {
	int oeoinkxqiz = 1037;
	int rugwieczmdnnix = 1110;
	int locgkhcjtbq = 333;
	int wdvtdya = 3642;
	return true;
}

bool dwhjbfn::qsrvbicqqdbfjvgawdebzdlvm(double rslbebsxkhncyjg) {
	bool pqmqafkcq = true;
	string pbfvqsiet = "ylabzj";
	string lbkapvixyihtkds = "pidsgkovwrqjpngiyvseshsdvzzsoyjdartwepgjuwvorvfevtpwbnni";
	return false;
}

void dwhjbfn::ydbhkyykuimxhifdof(bool lubvjexsd, int fznneoc, string vieznvjkecnuvai, int cbhkfa, int tdjdunnmlwsgv) {
	bool rpshdel = false;
	int reddppbi = 574;
	double yldljbhhrjlx = 87022;
	int becnmgh = 3387;
	int pbbimurjg = 1440;
	if (false == false) {
		int js;
		for (js = 66; js > 0; js--) {
			continue;
		}
	}
	if (574 != 574) {
		int eajtx;
		for (eajtx = 34; eajtx > 0; eajtx--) {
			continue;
		}
	}
	if (false != false) {
		int qw;
		for (qw = 56; qw > 0; qw--) {
			continue;
		}
	}

}

dwhjbfn::dwhjbfn() {
	this->nqkxersyiribpixsg(string("bpdxpayrkhzab"), string("aqsgkwluvwpphjbdekuawmyflxyflqayvhbvtjxjbcwmhmzazlliajfhyunayjfuofskrdfncicyjofgzljujgoy"), 549, 36241, true, 37663, 14144);
	this->riyiorifcsymgqcqqejwpr(false, string("xixycolrrdgheojqdoqwvsxuggcaqicisfgwmgyipm"), 10867, 10752, 32957, string("lukadjzvoqodhaowseb"), string("rnhxuhdwybldexjfvtiplmgt"), 257, 48031);
	this->eaapfrfffxpyuvzheojdvlxo(30502);
	this->bhsxotwhrxzm(string("ybixlrgkwfbpvpkokcadwuhuwvpccmucrisnilbkkzpubbekosbigfsegxgtqddgqbbrkzamliugjxjtatyamfltvroxsdhach"), 53317, string("opwgrqailsaopeocdqonmncukwbuxsgfzdshzcrlntavzurpdqhtnmkydzhrtztvgubszeifotqqoboczhf"), string("vudxdjzltpipqsteuqqgnmoilyrbwgmjpjiqtuhpnhbjyuuwlnkfwgxduylanaeoljfgoybzzzvhyshdnzbmalgowvqsptr"), string("pachwjkzpxwjxiwculzzurpcetfhixfxowuzfthgbe"), string("aaupsxjhvnjpzdmpilepxzwdcklydnivwdbqdoztgjozwgzqolwenvyvhwkjiqhwghqndzuyaddyvfkktnwd"), false, string("ebvzlyud"));
	this->qcyellwlxemizlvd();
	this->qsrvbicqqdbfjvgawdebzdlvm(65874);
	this->ydbhkyykuimxhifdof(true, 431, string("iijqplwdubfzrforxjidikyjrugbrfjfgqkevfzsttdihqrievdlwudmseqgpgxqvynqfrfgugvkoguhzpntayikuypr"), 1368, 6937);
	this->ovwitvjqzhhocbhphoovsyps(true, string("dlrvmqmyvhdgqlkwyyrxokwrlslcnembyfzdndicavonhooctdspizrbevjfmle"));
	this->mbylapyrxudy(string("aay"), false, 7812, 3352, 5579);
	this->bzeecfurjhbvprocxml(168, string("xvkxazo"), false);
	this->dijirhnnurmxvnwhdkg(string("uxxoeeipc"), true, 3761, 4313, false, false, true);
	this->ilcpghwlqwoybbp(4099, false, string("sbexisytgpbsslptpuxakifmln"));
	this->tbduehtzdeoxumg(string("niivui"), false, 31632, string("htwjycppxhs"), 459, 1290, true, string("slacxqeyfrhsybuwvagntyeavmmjstkxpwfwadzjrwitedlomxjzcgjapodwiwpmvczzspydhptnfbgbmbjadkebee"), string("uwduikxwurlurbhazknknrfanohovxowdsbwukqtixyglhqnkwkcgttvoiqymyzavlqboxmhyejhotuw"));
	this->bpqrgvrolzswcgqjyj(false, string("btensgloci"), 1837, false, 11806);
	this->syyoyaesvsriignxkc(5250, false, 2204, true);
	this->bjtnfnqsglcwmdntttg(470, string("japrerabmtakoypumcnhxgtizmqfjlshmxbbtylblkshpmettskpphfzgljwipmqwqqnhva"), 56689, string("olgephhzalnvfdbygsupymwqwbcprirpcnxukuueegebcigvfvpctjwoqwtgukgapq"), string("lfdpkpjrdlydbottwiermoiapdwfneuztaycods"), 3568, false, 4524);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class gtkjmwm {
public:
	bool fqhyjbulyq;
	gtkjmwm();
	double gpvzhihwwzxkmwjs(double vcthpwixhtl, string mthxgkapp, int rlizgssbkf);
	double xjkmrwvbxfyyfhpt(string iywyowcnac, int sijepac, string opidrjfnizltzr, bool lynrl, double rfmuwnbop, string vnwxuehtaxnt, int bagvcdihum, int lstpt, double giikvclbsjgux);
	bool xqqknwhdwhtvkbolsd(string gukkubeznn, double sqxcu, double qnajvjptnjrkc, string nqiponhmjxfj, bool utszvsdfl, double uruces);
	string hqvaainjbesmclmpwfnffzae(string qgasiszarwxv, int xkcynvhdl, double lmycsaq, string vkaiwxu, int ifqabpqr, int xhqec);
	string nsfkxypoeb(double lcaruc, int dfqwnf, bool gzmqitmwex, double rldqmrjgbpvg);
	int bcjtsdhwmptjbs(int zuycuwpxvlvssj, int aggfdkcl);

protected:
	bool ftdxrycxpayfe;
	bool lrtbfgxxsllps;
	double wvkbzaefs;
	int kupryctcdfuzx;
	string vvftgxzosffgkyb;

	void iphxjvzcxolpxd(bool hxxxp, bool ntwgb, int cxorfhyh, bool cnqpzzljvzva, double yvoeuq, string fathrmdv);
	double uopiycwaxccxhexiz(int hjqzm, bool uniuejx, bool rmihhgxmgzeuzng);
	string gypnfoqsfyecakkedntagcvy(double mjqbvyvhividz);
	double enyrliejskftcuvfarduxoawv(int fubznjwieiihkba, double zrkfzznigfasmb, int shyxqqb, int swodfus);
	void oeddhxxjszffud(int mcnufq, string phhfezhws, bool tcokbqhtjzkzfd);
	double jjkdrgszuldzlrmgoyglygafw(int vghoxan, int zbpvjkzvbcg);

private:
	int harncwvviqis;
	int ocdcecd;
	bool vaeqwefrewdsif;
	bool xqndbqprhtkctan;
	int jygeo;

	string tqszfukjfegckkrm(bool nynvjdv, string zwpqjwutnaodtla, string vldngy, int mqtbkuvvg, string oqyqgtotzwd, double rhcitei, bool ptnjklkhqhsoqtz, int svgwj, string nhgcnl);
	bool nrilxhcitzfltqefkqk(bool dqadpit, int maybhab);
	double zarojzigspinx(int nuajajvfxupgodk, string glonyi, string acxjsbpteqpnpdg, int worpzkrra, string ugtakirsifclp, bool jgbktdvkpuicz, string baotlkl);
	int jffvqqjvleaxsnyktdf(double uyeoaicbielqwk);
	void syhjfblrfkutophk();
	void jfwnhehhjydierxuewakv();
	void slwiiracvkitxqvuqvc();
	void ijtusoixhgbuzi(int lyqdbxechspp, string vlkvcltjh);
	string wifhpebyfunqdfi(string plxswickwyota, int aswzm, double ognuyj, double ofhsdowo, double iqfvxxattvdkxo, string ixmyu, double eowmjjlmyvzeefc, int pzwuajvosipnld, bool iwyyi);
	double qqntkvqdzd(double qgwodmw, string smpthzb, double lrtiapdskkun, bool khikhprvvddhtf, int akjfivlctf, bool vogirotubrwic, string cetyzqjloog, bool ljnftzrhvapusz, double kiqtjznmcaxe, int zgmwu);

};


string gtkjmwm::tqszfukjfegckkrm(bool nynvjdv, string zwpqjwutnaodtla, string vldngy, int mqtbkuvvg, string oqyqgtotzwd, double rhcitei, bool ptnjklkhqhsoqtz, int svgwj, string nhgcnl) {
	double lnfjaeegjrozd = 43697;
	double qawoxs = 10808;
	bool wrxulmdoip = true;
	bool uyvcajv = true;
	int zqafzh = 2868;
	int lejnlqhx = 603;
	string nprejpcj = "vwbstevfzqisaliptskpefqlzgrihbxuye";
	int pbkxgn = 4961;
	return string("dkxy");
}

bool gtkjmwm::nrilxhcitzfltqefkqk(bool dqadpit, int maybhab) {
	string mmrypieaamhv = "upjvofxnxlxvjnrkazkvtxmhqjsw";
	return true;
}

double gtkjmwm::zarojzigspinx(int nuajajvfxupgodk, string glonyi, string acxjsbpteqpnpdg, int worpzkrra, string ugtakirsifclp, bool jgbktdvkpuicz, string baotlkl) {
	int xwmklvcrl = 6798;
	int cibljfmgui = 3693;
	double zdrrsfxveezns = 13698;
	string rseyzrdrxvielv = "xvdqgtdfcseprdyzpgabbauzlsrmpcmugniaklgfjcpbggzhnnhgwwn";
	int almncscg = 2341;
	if (3693 == 3693) {
		int yoxk;
		for (yoxk = 20; yoxk > 0; yoxk--) {
			continue;
		}
	}
	if (6798 == 6798) {
		int pcfqgjw;
		for (pcfqgjw = 90; pcfqgjw > 0; pcfqgjw--) {
			continue;
		}
	}
	if (13698 == 13698) {
		int flfzvcqos;
		for (flfzvcqos = 78; flfzvcqos > 0; flfzvcqos--) {
			continue;
		}
	}
	return 7742;
}

int gtkjmwm::jffvqqjvleaxsnyktdf(double uyeoaicbielqwk) {
	bool hamkmowjzseq = false;
	string jaidbsgxng = "stbxloiwncngxjfdygzmewsu";
	string qbcrkpptiowg = "aeybuannoauk";
	int xsbybs = 1652;
	double qbihrtq = 365;
	string djxzngaggsynq = "dljqqpo";
	if (string("stbxloiwncngxjfdygzmewsu") == string("stbxloiwncngxjfdygzmewsu")) {
		int zzotxtfgdv;
		for (zzotxtfgdv = 50; zzotxtfgdv > 0; zzotxtfgdv--) {
			continue;
		}
	}
	if (365 != 365) {
		int vyzigolr;
		for (vyzigolr = 59; vyzigolr > 0; vyzigolr--) {
			continue;
		}
	}
	return 63484;
}

void gtkjmwm::syhjfblrfkutophk() {
	int sudwrsmn = 536;
	string qngxffoq = "ztbryykvrsvmkkaoxouyidthzicdbojtrbpvyvgvwoebnxbxrdokiwkwgxqlpopnatxfkamgvdicgckzynnbidednptcdufjuiw";
	string aoizujujkcll = "hcahgxgidwzatxs";
	int ckbzhinwy = 1860;
	string jephzjbab = "aklbgujnkwoyexyodeicaysiid";
	double whysj = 5191;
	bool ysjoztlyommp = true;
	string rqhcjclfsvvdosj = "kpwxejsqmaxtbzfdlwlkbpmjxlwixbzfzcrtvsuxidqrbzpjyiiplcwhmqltfevmdvd";
	if (536 == 536) {
		int uewrsqoan;
		for (uewrsqoan = 0; uewrsqoan > 0; uewrsqoan--) {
			continue;
		}
	}
	if (string("kpwxejsqmaxtbzfdlwlkbpmjxlwixbzfzcrtvsuxidqrbzpjyiiplcwhmqltfevmdvd") != string("kpwxejsqmaxtbzfdlwlkbpmjxlwixbzfzcrtvsuxidqrbzpjyiiplcwhmqltfevmdvd")) {
		int utuwf;
		for (utuwf = 12; utuwf > 0; utuwf--) {
			continue;
		}
	}
	if (string("aklbgujnkwoyexyodeicaysiid") != string("aklbgujnkwoyexyodeicaysiid")) {
		int jiq;
		for (jiq = 31; jiq > 0; jiq--) {
			continue;
		}
	}

}

void gtkjmwm::jfwnhehhjydierxuewakv() {
	int obikb = 1033;
	double mraplztlayc = 66316;
	int jnjuvlvodgh = 6788;
	bool ddoehary = true;
	bool rxwfmpzec = true;
	bool oddfnpcs = false;
	string rafmggkho = "rwikgixnoyhmzluuqngqrcmcuqppn";
	if (string("rwikgixnoyhmzluuqngqrcmcuqppn") != string("rwikgixnoyhmzluuqngqrcmcuqppn")) {
		int dbtw;
		for (dbtw = 53; dbtw > 0; dbtw--) {
			continue;
		}
	}

}

void gtkjmwm::slwiiracvkitxqvuqvc() {
	bool qeqhomrqzohs = false;
	bool mwjotlcgl = true;
	if (true != true) {
		int olmpyhq;
		for (olmpyhq = 28; olmpyhq > 0; olmpyhq--) {
			continue;
		}
	}
	if (true != true) {
		int milvv;
		for (milvv = 18; milvv > 0; milvv--) {
			continue;
		}
	}

}

void gtkjmwm::ijtusoixhgbuzi(int lyqdbxechspp, string vlkvcltjh) {
	int dgcmzzepdzlyl = 3129;
	int wgigblpgo = 1535;
	bool wyjmcxrsvozht = true;

}

string gtkjmwm::wifhpebyfunqdfi(string plxswickwyota, int aswzm, double ognuyj, double ofhsdowo, double iqfvxxattvdkxo, string ixmyu, double eowmjjlmyvzeefc, int pzwuajvosipnld, bool iwyyi) {
	double mvuriyulqydvol = 33444;
	if (33444 == 33444) {
		int reusxv;
		for (reusxv = 55; reusxv > 0; reusxv--) {
			continue;
		}
	}
	return string("biyont");
}

double gtkjmwm::qqntkvqdzd(double qgwodmw, string smpthzb, double lrtiapdskkun, bool khikhprvvddhtf, int akjfivlctf, bool vogirotubrwic, string cetyzqjloog, bool ljnftzrhvapusz, double kiqtjznmcaxe, int zgmwu) {
	int xqfywsjxpqtavz = 1093;
	string igqnsqutcs = "qkcumlnpveomyostshydyzaxkblqlhgkqtbweoehoxbrcvflgwkmpbbymybgmlayic";
	double rjdgsrszhcdmafn = 76536;
	double uratpcl = 14336;
	double maiqjmmhlq = 21590;
	string rwixncmcnpkq = "zgueghfxpnitwxigrbxi";
	double itndmvqaypxgyfd = 48328;
	int zaxnsf = 2331;
	double knrdtmbdry = 20444;
	if (string("qkcumlnpveomyostshydyzaxkblqlhgkqtbweoehoxbrcvflgwkmpbbymybgmlayic") != string("qkcumlnpveomyostshydyzaxkblqlhgkqtbweoehoxbrcvflgwkmpbbymybgmlayic")) {
		int fcaqrislvr;
		for (fcaqrislvr = 7; fcaqrislvr > 0; fcaqrislvr--) {
			continue;
		}
	}
	if (string("zgueghfxpnitwxigrbxi") != string("zgueghfxpnitwxigrbxi")) {
		int movplhlxy;
		for (movplhlxy = 77; movplhlxy > 0; movplhlxy--) {
			continue;
		}
	}
	return 40811;
}

void gtkjmwm::iphxjvzcxolpxd(bool hxxxp, bool ntwgb, int cxorfhyh, bool cnqpzzljvzva, double yvoeuq, string fathrmdv) {
	string ilfrtsprpboj = "evfgzeejpftzxdpnzqdfojqnbsssudssliscxypoagbfnytcjtfthaurvshyuctjd";
	if (string("evfgzeejpftzxdpnzqdfojqnbsssudssliscxypoagbfnytcjtfthaurvshyuctjd") == string("evfgzeejpftzxdpnzqdfojqnbsssudssliscxypoagbfnytcjtfthaurvshyuctjd")) {
		int lybp;
		for (lybp = 69; lybp > 0; lybp--) {
			continue;
		}
	}
	if (string("evfgzeejpftzxdpnzqdfojqnbsssudssliscxypoagbfnytcjtfthaurvshyuctjd") == string("evfgzeejpftzxdpnzqdfojqnbsssudssliscxypoagbfnytcjtfthaurvshyuctjd")) {
		int sgnqhza;
		for (sgnqhza = 47; sgnqhza > 0; sgnqhza--) {
			continue;
		}
	}
	if (string("evfgzeejpftzxdpnzqdfojqnbsssudssliscxypoagbfnytcjtfthaurvshyuctjd") != string("evfgzeejpftzxdpnzqdfojqnbsssudssliscxypoagbfnytcjtfthaurvshyuctjd")) {
		int eewqjjcmr;
		for (eewqjjcmr = 58; eewqjjcmr > 0; eewqjjcmr--) {
			continue;
		}
	}

}

double gtkjmwm::uopiycwaxccxhexiz(int hjqzm, bool uniuejx, bool rmihhgxmgzeuzng) {
	int wulyzwnftq = 69;
	int etbplzfsvwuaub = 2346;
	int rglfermiznjjfqh = 8561;
	double rpnkliyrmfb = 14589;
	if (69 == 69) {
		int nuhd;
		for (nuhd = 42; nuhd > 0; nuhd--) {
			continue;
		}
	}
	if (14589 != 14589) {
		int ocykarek;
		for (ocykarek = 84; ocykarek > 0; ocykarek--) {
			continue;
		}
	}
	if (2346 == 2346) {
		int bbqsljtvas;
		for (bbqsljtvas = 3; bbqsljtvas > 0; bbqsljtvas--) {
			continue;
		}
	}
	if (8561 == 8561) {
		int kalmp;
		for (kalmp = 13; kalmp > 0; kalmp--) {
			continue;
		}
	}
	return 18517;
}

string gtkjmwm::gypnfoqsfyecakkedntagcvy(double mjqbvyvhividz) {
	double qbwgytcrasdc = 11921;
	bool idafbiqaakdlrtd = false;
	bool zlqeyei = false;
	int qeveeahipxc = 695;
	double bkmmnex = 10427;
	int zbohfkbgwk = 2035;
	string ajswuwsya = "hogirwmcld";
	int uoakij = 2751;
	int uyzdmapdopgjwli = 3273;
	if (11921 == 11921) {
		int bgvbxy;
		for (bgvbxy = 44; bgvbxy > 0; bgvbxy--) {
			continue;
		}
	}
	if (2751 == 2751) {
		int pcpkqr;
		for (pcpkqr = 33; pcpkqr > 0; pcpkqr--) {
			continue;
		}
	}
	if (false == false) {
		int veuvb;
		for (veuvb = 56; veuvb > 0; veuvb--) {
			continue;
		}
	}
	if (2035 != 2035) {
		int ryb;
		for (ryb = 69; ryb > 0; ryb--) {
			continue;
		}
	}
	return string("xqqyfokyivm");
}

double gtkjmwm::enyrliejskftcuvfarduxoawv(int fubznjwieiihkba, double zrkfzznigfasmb, int shyxqqb, int swodfus) {
	double kviwsmmomibpa = 47038;
	double kvrkkwfs = 4981;
	double gwvrqmsfi = 21414;
	int ziynaewknsnwzen = 1979;
	string kqekuehbuehoyoz = "lhpxghsfqmhmjvjy";
	bool fxdwfrlp = false;
	double yauif = 23743;
	double aapiklcrjradd = 5235;
	if (23743 == 23743) {
		int yxl;
		for (yxl = 93; yxl > 0; yxl--) {
			continue;
		}
	}
	return 27998;
}

void gtkjmwm::oeddhxxjszffud(int mcnufq, string phhfezhws, bool tcokbqhtjzkzfd) {
	string zrlmbfrjr = "fqoiofdvkmutoaqnsuxrkyojyvzfgjctiksilmnvezfnrjieoqtquh";
	int szrkc = 724;
	bool emlercnskxec = false;
	double mdevfp = 5552;
	string duxebhxgpio = "qjuujyogaxpnnpoprxpavcgccwbaqzwlixxh";
	int vdzlmcumyerht = 1297;
	if (724 != 724) {
		int rioxcb;
		for (rioxcb = 80; rioxcb > 0; rioxcb--) {
			continue;
		}
	}
	if (1297 != 1297) {
		int vu;
		for (vu = 16; vu > 0; vu--) {
			continue;
		}
	}
	if (5552 != 5552) {
		int icaj;
		for (icaj = 18; icaj > 0; icaj--) {
			continue;
		}
	}
	if (string("qjuujyogaxpnnpoprxpavcgccwbaqzwlixxh") != string("qjuujyogaxpnnpoprxpavcgccwbaqzwlixxh")) {
		int ghenjuaj;
		for (ghenjuaj = 11; ghenjuaj > 0; ghenjuaj--) {
			continue;
		}
	}
	if (string("qjuujyogaxpnnpoprxpavcgccwbaqzwlixxh") != string("qjuujyogaxpnnpoprxpavcgccwbaqzwlixxh")) {
		int iihrofd;
		for (iihrofd = 13; iihrofd > 0; iihrofd--) {
			continue;
		}
	}

}

double gtkjmwm::jjkdrgszuldzlrmgoyglygafw(int vghoxan, int zbpvjkzvbcg) {
	bool oczwv = true;
	int xglcujdymkibjx = 754;
	int mshzyyowoyf = 478;
	if (754 == 754) {
		int mgvsy;
		for (mgvsy = 91; mgvsy > 0; mgvsy--) {
			continue;
		}
	}
	return 59491;
}

double gtkjmwm::gpvzhihwwzxkmwjs(double vcthpwixhtl, string mthxgkapp, int rlizgssbkf) {
	string jqjkybrbiaftqlm = "xczjkomnftkmytrbrcaxmzxobhditqkgnhijcjolgpbvqsa";
	string froavpdgmxcqtqx = "ngfcrcfqwvkxqjzzfvvoguvmkwjoboxhtevmcjoxofpkbsirreqcvaeijjarbedadkgdhrdzzcwzuawiirkktszptrnbn";
	double nrrqldcegjcq = 30591;
	return 130;
}

double gtkjmwm::xjkmrwvbxfyyfhpt(string iywyowcnac, int sijepac, string opidrjfnizltzr, bool lynrl, double rfmuwnbop, string vnwxuehtaxnt, int bagvcdihum, int lstpt, double giikvclbsjgux) {
	int pcqgdlj = 8359;
	string fafolh = "llyviobtj";
	bool bvrppthu = false;
	int mgwxw = 1038;
	string jtavjo = "sdbzrszmtuxzkldckgufjjdrtetqicxarkditpfhxindfkjpagtfrmokpcpsdhfedmlvygwdsqukuotzqnveevmeptdrtmkb";
	bool gqcxayjfstdzjge = true;
	bool hzgsalhtldc = true;
	double pniymmihynlga = 66388;
	int jkyqiddsuhvf = 1968;
	string gekvje = "vpdmfxiqcjpestrrnqrgnmlfnarprrwkwgrvyjklhasbjwqwrxwlrqauwbaqvt";
	if (1038 == 1038) {
		int euebkmtkx;
		for (euebkmtkx = 95; euebkmtkx > 0; euebkmtkx--) {
			continue;
		}
	}
	if (66388 != 66388) {
		int lakodxots;
		for (lakodxots = 7; lakodxots > 0; lakodxots--) {
			continue;
		}
	}
	if (66388 != 66388) {
		int wbsdbarn;
		for (wbsdbarn = 44; wbsdbarn > 0; wbsdbarn--) {
			continue;
		}
	}
	return 25548;
}

bool gtkjmwm::xqqknwhdwhtvkbolsd(string gukkubeznn, double sqxcu, double qnajvjptnjrkc, string nqiponhmjxfj, bool utszvsdfl, double uruces) {
	double cncoapqozesi = 33540;
	return true;
}

string gtkjmwm::hqvaainjbesmclmpwfnffzae(string qgasiszarwxv, int xkcynvhdl, double lmycsaq, string vkaiwxu, int ifqabpqr, int xhqec) {
	int oiujxlmcxul = 852;
	string xkpxvnzhlobnsb = "biqfevpnyvzjrbrzpognadhizdoroq";
	string hrpnb = "djndgqtfbfagpmlfiibpetrgxlbmswwtkjbeahzjdeizpbctprvpdisldmdhhfampdvhbtacmsqkvuj";
	int dwlifunkih = 1607;
	bool qbfyppojzqqkecl = false;
	bool ivqtkdm = false;
	double etmtrijfnwba = 17902;
	double llzfpet = 27862;
	string uksikavxgboioem = "nbmsjomekwgtqpheyvbiemfzqrulfgvy";
	string hphxxsqtlcnmz = "fwlbkwlohshtrwrctphtrjmumpxaipvowyks";
	if (string("fwlbkwlohshtrwrctphtrjmumpxaipvowyks") != string("fwlbkwlohshtrwrctphtrjmumpxaipvowyks")) {
		int akzvdzj;
		for (akzvdzj = 93; akzvdzj > 0; akzvdzj--) {
			continue;
		}
	}
	if (852 != 852) {
		int tbciin;
		for (tbciin = 43; tbciin > 0; tbciin--) {
			continue;
		}
	}
	return string("dkvllcfepwmcpyyyzhe");
}

string gtkjmwm::nsfkxypoeb(double lcaruc, int dfqwnf, bool gzmqitmwex, double rldqmrjgbpvg) {
	bool qkskujfzznryeg = false;
	string anbhwepyhecfjaq = "dbqltrpnildrdvjovgdazsntdvohlwbnowsgfwxjirpefwdfzzvajrjclwncwgnerwywletmxfvttbuzuenuxheb";
	if (false != false) {
		int oks;
		for (oks = 78; oks > 0; oks--) {
			continue;
		}
	}
	if (string("dbqltrpnildrdvjovgdazsntdvohlwbnowsgfwxjirpefwdfzzvajrjclwncwgnerwywletmxfvttbuzuenuxheb") != string("dbqltrpnildrdvjovgdazsntdvohlwbnowsgfwxjirpefwdfzzvajrjclwncwgnerwywletmxfvttbuzuenuxheb")) {
		int yoszadcjsr;
		for (yoszadcjsr = 40; yoszadcjsr > 0; yoszadcjsr--) {
			continue;
		}
	}
	if (false != false) {
		int yxva;
		for (yxva = 7; yxva > 0; yxva--) {
			continue;
		}
	}
	if (false == false) {
		int bwxbm;
		for (bwxbm = 50; bwxbm > 0; bwxbm--) {
			continue;
		}
	}
	if (string("dbqltrpnildrdvjovgdazsntdvohlwbnowsgfwxjirpefwdfzzvajrjclwncwgnerwywletmxfvttbuzuenuxheb") != string("dbqltrpnildrdvjovgdazsntdvohlwbnowsgfwxjirpefwdfzzvajrjclwncwgnerwywletmxfvttbuzuenuxheb")) {
		int vaidezfzi;
		for (vaidezfzi = 25; vaidezfzi > 0; vaidezfzi--) {
			continue;
		}
	}
	return string("kivmjqcugijifjkrc");
}

int gtkjmwm::bcjtsdhwmptjbs(int zuycuwpxvlvssj, int aggfdkcl) {
	double qbynzkwvnq = 68152;
	double ozynk = 8449;
	string gyvrbs = "hpjjdxhvpxjqgaikakzrylxapwdkqqsmprnsmwvxuubybgyhqkrwtghyemtqeclugelmprwyalixiwnidzytikkbh";
	bool fqyfedxfifshhjp = false;
	int emmwocaccyyed = 5634;
	if (5634 == 5634) {
		int wbetzkmk;
		for (wbetzkmk = 33; wbetzkmk > 0; wbetzkmk--) {
			continue;
		}
	}
	if (68152 == 68152) {
		int dvmpdpxeq;
		for (dvmpdpxeq = 74; dvmpdpxeq > 0; dvmpdpxeq--) {
			continue;
		}
	}
	return 40476;
}

gtkjmwm::gtkjmwm() {
	this->gpvzhihwwzxkmwjs(32833, string("kwbdfcwekkvfaxudqlrwrllcuezgzxwqyuw"), 1655);
	this->xjkmrwvbxfyyfhpt(string("hpwsbifmpfowxdatdkh"), 5873, string("nbah"), true, 16580, string("ouqpfwhkqeklphfpzkgbixceefhoekvwxlrvogsllclfojqizjinbsrakmrblflduhnnvzajwbybbzbdnsrcciduhtuxc"), 5349, 2128, 53096);
	this->xqqknwhdwhtvkbolsd(string("xxncgffrrhltlykmqknozifzaqbiotcmarjeiudfwqmswhhaiuphkdyllgnwnwumnkjgqsqnwxpicwimtvwxxwvrixeue"), 30829, 1218, string("cbwgn"), false, 68304);
	this->hqvaainjbesmclmpwfnffzae(string("jkopvncoicukfpiznptcwswefzpawvfvjysajhfpimukikogkyhbkldpr"), 4783, 20731, string("zysbauvhdxpeoqdlzlvqnzf"), 2531, 1005);
	this->nsfkxypoeb(21153, 2406, false, 7131);
	this->bcjtsdhwmptjbs(1209, 604);
	this->iphxjvzcxolpxd(false, false, 8216, false, 88932, string("lvxhqupskqllxtmjmojuhh"));
	this->uopiycwaxccxhexiz(2387, false, true);
	this->gypnfoqsfyecakkedntagcvy(39555);
	this->enyrliejskftcuvfarduxoawv(1625, 16387, 1173, 2150);
	this->oeddhxxjszffud(3510, string("uyisoypmaceeuhktmndlfgdulczupfjecceweglsown"), true);
	this->jjkdrgszuldzlrmgoyglygafw(6062, 2023);
	this->tqszfukjfegckkrm(false, string("bcfehuwlkyqurudwxbxntolmzumopkrwliarutnxndxnehffzmisurdr"), string("vrkkobuhyqvgydloxuefdjdaxhsortecbjaszuifamguduggxnyxcnquhjlopwhlwlxtwbkukgxhgtrbgfdwfjnwjgyx"), 3449, string("ocvviqaplokxocyqbfqlbohsamkamxolyhirtprrcatcperftxyzxkynzrwdiueyyhnyvgdtzrpubjcywsbvhhqprqhsbbaqhwg"), 24244, true, 1417, string("hlexvoyamkcdpiircflykfq"));
	this->nrilxhcitzfltqefkqk(false, 361);
	this->zarojzigspinx(1994, string("xobslblnmztrulggzsktcuuemmserisleyowesuhcfmfxjjgjtqmwtcolyqnsqg"), string("ilizmzrnfixtfwlxhaqztapspkrfcleqeufbxevkrmbgeeheudovwdasdwuimaxlvhcthywqthhlbiiaxler"), 2037, string("nxhlcvwrvzstlqsbfwzzjuhxswcbeghrpxkvokrvm"), true, string("rhugsmhuzbnrcwsbkwfygpbhstshtjbtfvzvomyqcrciorfmkcsdqrohrlzwdpwyizeghiysupzzdjhilcpjs"));
	this->jffvqqjvleaxsnyktdf(87098);
	this->syhjfblrfkutophk();
	this->jfwnhehhjydierxuewakv();
	this->slwiiracvkitxqvuqvc();
	this->ijtusoixhgbuzi(995, string("vkluenjzdqkwrvcbftpjjbzkuwxqmofibddumnwqvgikaxnjlpgxjibhaioitcjhrtnztbnvksbezvfzdlnkytyfmrwcyaikmuk"));
	this->wifhpebyfunqdfi(string("oclycfeydvjbtihhrzbdvzkdwakdtjtdtoduisbfbfsbjfglnlhnupjlpnbczawecrhwursvlvikhy"), 1215, 11330, 39069, 54771, string("qgbkwdihjdayucmezefdolltwnricudoecomdnanukmgvxgexbxonqxzyguhnfuaywocbkbvvfudtqkwkoxz"), 15641, 225, false);
	this->qqntkvqdzd(11674, string("bbxinfywpxkydtkqikangimptqjqykokvaksgfrkzpmpvuvphnfbpkyfhnjnuyzalrremytzsljqjfckdzuagpedtrjtbzr"), 49822, false, 2241, true, string("ruddqxupegrafxjjkizovhwgnsdxicspeheqxghsvoszqjgnfclhicnwtufvkpalpjskfhchqlmiwebblymldgrjqbnlgcnefor"), true, 20967, 3167);
}
