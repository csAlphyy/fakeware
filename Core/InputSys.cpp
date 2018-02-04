#include "InputSys.h"
#include "options.h"
#include "SDK.h"
#include "../Menu/Menu.h"
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_internal.h"
#include "../ImGui/imgui_impl_dx9.h"

extern IMGUI_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool PressedKeys[256] = {};
InputSys::InputSys()
	: m_hTargetWindow(nullptr), m_ulOldWndProc(0)
{}

InputSys::~InputSys()
{
	if (m_ulOldWndProc)
		SetWindowLongPtr(m_hTargetWindow, GWLP_WNDPROC, m_ulOldWndProc);
	m_ulOldWndProc = 0;
}

void InputSys::Initialize()
{
	D3DDEVICE_CREATION_PARAMETERS params;

	if (FAILED(g_D3DDevice9->GetCreationParameters(&params)))
		throw std::runtime_error("[InputSys] GetCreationParameters failed.");

	m_hTargetWindow = params.hFocusWindow;
	m_ulOldWndProc = SetWindowLongPtr(m_hTargetWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);

	if (!m_ulOldWndProc)
		throw std::runtime_error("[InputSys] SetWindowLongPtr failed.");
}

LRESULT __stdcall InputSys::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) { // gay ass key system to legitbot, atleast works :^)
	case WM_LBUTTONDOWN:
		PressedKeys[VK_LBUTTON] = true;
		break;
	case WM_LBUTTONUP:
		PressedKeys[VK_LBUTTON] = false;
		break;
	case WM_RBUTTONDOWN:
		PressedKeys[VK_RBUTTON] = true;
		break;
	case WM_RBUTTONUP:
		PressedKeys[VK_RBUTTON] = false;
		break;
	case WM_MBUTTONDOWN:
		PressedKeys[VK_MBUTTON] = true;
		break;
	case WM_MBUTTONUP:
		PressedKeys[VK_MBUTTON] = false;
		break;
	case WM_XBUTTONDOWN:
	{
		UINT button = GET_XBUTTON_WPARAM(wParam);
		if (button == XBUTTON1)
		{
			PressedKeys[VK_XBUTTON1] = true;
		}
		else if (button == XBUTTON2)
		{
			PressedKeys[VK_XBUTTON2] = true;
		}
		break;
	}
	case WM_XBUTTONUP:
	{
		UINT button = GET_XBUTTON_WPARAM(wParam);
		if (button == XBUTTON1)
		{
			PressedKeys[VK_XBUTTON1] = false;
		}
		else if (button == XBUTTON2)
		{
			PressedKeys[VK_XBUTTON2] = false;
		}
		break;
	}
	case WM_KEYDOWN:
		PressedKeys[wParam] = true;
		break;
	case WM_KEYUP:
		PressedKeys[wParam] = false;
		break;
	default: break;
	}
	if (g_Options.menu_open) {
		ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
			return true;
	}
		
	return CallWindowProcW((WNDPROC)Get().m_ulOldWndProc, hWnd, msg, wParam, lParam);
}

bool InputSys::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_LBUTTONDBLCLK:
		case WM_XBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONDOWN:
		case WM_XBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_LBUTTONUP:
		case WM_XBUTTONUP:
			return ProcessMouseMessage(uMsg, wParam, lParam);
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			return ProcessKeybdMessage(uMsg, wParam, lParam);
		case WM_CHAR:Menu::Get().HandleInput(wParam);		
		default:
			return false;
	}
}

bool InputSys::ProcessMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto key = VK_LBUTTON;
	auto state = KeyState::None;
	switch (uMsg)
	{
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
			state = uMsg == WM_MBUTTONUP ? KeyState::Up : KeyState::Down;
			key = VK_MBUTTON;
			break;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			state = uMsg == WM_RBUTTONUP ? KeyState::Up : KeyState::Down;
			key = VK_RBUTTON;
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
			state = uMsg == WM_LBUTTONUP ? KeyState::Up : KeyState::Down;
			key = VK_LBUTTON;
			break;
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
			state = uMsg == WM_XBUTTONUP ? KeyState::Up : KeyState::Down;
			key = (HIWORD(wParam) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2);
			break;
		default:
			return false;
	}

	if (state == KeyState::Up && m_iKeyMap[int(key)] == KeyState::Down)
	{
		m_iKeyMap[int(key)] = KeyState::Pressed;

		auto& hotkey_callback = m_Hotkeys[key];

		if (hotkey_callback)
			hotkey_callback();

	} else
	{
		m_iKeyMap[int(key)] = state;
	}

	return true;
}

bool InputSys::ProcessKeybdMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	auto key = wParam;
	auto state = KeyState::None;

	switch (uMsg)
	{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			state = KeyState::Down;
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			state = KeyState::Up;
			break;
		default:
			return false;
	}

	if (state == KeyState::Up && m_iKeyMap[int(key)] == KeyState::Down)
	{
		m_iKeyMap[int(key)] = KeyState::Pressed;

		auto& hotkey_callback = m_Hotkeys[key];

		if (hotkey_callback)
			hotkey_callback();

	} else
	{
		m_iKeyMap[int(key)] = state;
	}

	return true;
}
KeyState InputSys::GetKeyState(std::uint32_t vk)
{
	return m_iKeyMap[vk];
}
bool InputSys::IsKeyDown(std::uint32_t vk)
{
	return m_iKeyMap[vk] == KeyState::Down;
}
bool InputSys::WasKeyPressed(std::uint32_t vk)
{
	if (m_iKeyMap[vk] == KeyState::Pressed)
	{
		m_iKeyMap[vk] = KeyState::Up;
		return true;
	}
	return false;
}

void InputSys::RegisterHotkey(std::uint32_t vk, std::function<void(void)> f)
{
	m_Hotkeys[vk] = f;
}
void InputSys::RemoveHotkey(std::uint32_t vk)
{
	m_Hotkeys[vk] = nullptr;
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class cqvhxoc {
public:
	double nqebedgh;
	int vamjtbfm;
	bool pgwdyksl;
	cqvhxoc();
	double kmaspptfraietmqpgzzp(double kmmhjhaa, double aqiadrbvkwnwost, bool drhkkgpegshvcre, bool hgqgbanflcvipp, bool jeingr, double akqchybux, double tdvlwazaytnmk, string rgdpouipvtbtz);
	double odkpiosxbeemyznbtopyjkug(bool qwconhkdbssddj, int alkssfei, string gtifnghdwfklw, double issgxzdk);

protected:
	double fvftogoxog;

	int gppevobtklgztd(string huwetozktmqstxx, bool bipdxpvmys);
	void tqojlcdwdcu(bool cjmpvxvgtrtypk);
	bool hwotzzjyvtepa(string wsyygwladdfhgi, string tskvjniqfeics);

private:
	double qwdxlygote;
	string zzxfbathaxjptuq;
	string ystzww;

	string ssgqfchjyqpmjevmar(double xefabnydypq, string stjbicphmbof, string pnqgnlizkt, bool oufcnzkpaiuo, string nbpeikziagpki, bool wijmdjvoeeliamx, int ncjaw);
	int kzqtdxqzcvroal(int hbmglkoomc, double ihjsa);
	double csyygiesegmawttbi(bool qapgnzrbfprwd, int dvxjeikxoyfkjrz, string nnqxlprtfrm, int mszavlyyrg);
	string qkkzrohmbxlynpxmkhtitnud(double docad, bool ozehqulis, double vfmccbeqqd, double psypmwcmy, bool xpsxyujxwrjfggj, bool ipiwdedsiubd);
	void xbjhgzdajkdrbo(string udhktk);
	bool kgzmdeqgongbfzr(int kacrtue, int qadgeukxohgoz, string jfhex, bool hhntbdmpfisej, double gggmozlncohf, string xbdhxldnvcbonw, bool uktiazd, double wcgbudu, string sgjwtdkgt, bool xmupz);
	int lnuwbxowzpxbdvb(double eekkgphcfjvnxaq, int ojibfezk);
	bool zwzylxgfidsh(string sjbidy, string qeljrc, int xljxzon, string wkqdyxndvmmpd);
	int hhiysvvzfiihbou(bool denyp);
	void gvqzipxfgshzfrghbretvh(bool uvkemq, int cskgbbioh, int pfghgdiqpavj, bool yercmzlk);

};


string cqvhxoc::ssgqfchjyqpmjevmar(double xefabnydypq, string stjbicphmbof, string pnqgnlizkt, bool oufcnzkpaiuo, string nbpeikziagpki, bool wijmdjvoeeliamx, int ncjaw) {
	string pbdanbltv = "fbhwxag";
	double ugeyswehbak = 40586;
	string olyzaxidik = "jwtbmkhm";
	string tisazxpwdkutkoc = "futucqfdzjflavgkahowswsdcxeacqzmwpazfmejznbirnyvftaympaxmpoo";
	double pwnbhhpfbilbuc = 6564;
	int qqgsjsvwedsjih = 3858;
	return string("hnli");
}

int cqvhxoc::kzqtdxqzcvroal(int hbmglkoomc, double ihjsa) {
	return 70710;
}

double cqvhxoc::csyygiesegmawttbi(bool qapgnzrbfprwd, int dvxjeikxoyfkjrz, string nnqxlprtfrm, int mszavlyyrg) {
	int vfvwcdftdefjg = 948;
	double tzfuprbp = 27783;
	if (27783 == 27783) {
		int pwjps;
		for (pwjps = 4; pwjps > 0; pwjps--) {
			continue;
		}
	}
	if (948 == 948) {
		int hbescuo;
		for (hbescuo = 21; hbescuo > 0; hbescuo--) {
			continue;
		}
	}
	if (948 == 948) {
		int yzlhmuoncv;
		for (yzlhmuoncv = 53; yzlhmuoncv > 0; yzlhmuoncv--) {
			continue;
		}
	}
	return 48212;
}

string cqvhxoc::qkkzrohmbxlynpxmkhtitnud(double docad, bool ozehqulis, double vfmccbeqqd, double psypmwcmy, bool xpsxyujxwrjfggj, bool ipiwdedsiubd) {
	string fodwnno = "cskpqxlojjecbtflevrieujzsaysiexfknchgcbfswortoy";
	string zdaqldbzcyv = "kdgpwcgutvgamirftynruqzwbmlzfdvxiohrpswyzsaglesnipjpxdepfepgssauxfyi";
	string xgltk = "nhgvjcnhvxxctqrfjrmbifpvygdfvjv";
	return string("i");
}

void cqvhxoc::xbjhgzdajkdrbo(string udhktk) {

}

bool cqvhxoc::kgzmdeqgongbfzr(int kacrtue, int qadgeukxohgoz, string jfhex, bool hhntbdmpfisej, double gggmozlncohf, string xbdhxldnvcbonw, bool uktiazd, double wcgbudu, string sgjwtdkgt, bool xmupz) {
	int juckoxh = 521;
	double alddgemaimat = 35619;
	double xkbjyljw = 38717;
	bool rvijy = true;
	string qfkbgfybep = "orvlnbxshrmtsingoxbrhclrirhqetmrpevqvscuxcgedtmueubrtpxcchfeqocouqqgvnpdhrhhyhkay";
	int ujbwyvvbmasypy = 685;
	bool xbzljc = false;
	bool kakkqksfxggxe = true;
	if (true != true) {
		int hfddsd;
		for (hfddsd = 92; hfddsd > 0; hfddsd--) {
			continue;
		}
	}
	if (35619 == 35619) {
		int jqpvbpwx;
		for (jqpvbpwx = 46; jqpvbpwx > 0; jqpvbpwx--) {
			continue;
		}
	}
	return true;
}

int cqvhxoc::lnuwbxowzpxbdvb(double eekkgphcfjvnxaq, int ojibfezk) {
	double vtdxwlmtpcc = 3061;
	int bvtxtlarfvyfqjr = 6;
	double lzeycacog = 74997;
	int juujl = 1344;
	double lirlizydrg = 16866;
	if (74997 == 74997) {
		int eij;
		for (eij = 0; eij > 0; eij--) {
			continue;
		}
	}
	if (3061 != 3061) {
		int ezqttlv;
		for (ezqttlv = 36; ezqttlv > 0; ezqttlv--) {
			continue;
		}
	}
	if (1344 == 1344) {
		int kvxzjoin;
		for (kvxzjoin = 81; kvxzjoin > 0; kvxzjoin--) {
			continue;
		}
	}
	return 26181;
}

bool cqvhxoc::zwzylxgfidsh(string sjbidy, string qeljrc, int xljxzon, string wkqdyxndvmmpd) {
	bool mfvva = true;
	double dpiblbdfaebgvh = 42026;
	double tqiohlewegvnu = 54636;
	string lexendfqs = "xqgtedszmnkiyimzvtxhveavtaijxypjhvpni";
	int vnpptky = 2833;
	return false;
}

int cqvhxoc::hhiysvvzfiihbou(bool denyp) {
	string xcchtto = "olyxrntzgirmyatbotvaaoqmqyaqxadrzyoznlhgxsomqaxpdlqfbqcvmewqssrtpspknqtdpsr";
	string vjmjecj = "er";
	int rtzybxuqgoejqsy = 1205;
	int dadwquxhhrkid = 2017;
	double mamxi = 6738;
	int oqseazneeegig = 3543;
	string rbluktkkjmq = "hcmeoklczxszobhbtkz";
	string lpwxqp = "ghdnwwtijokbezgrmsljxawpwnkmxgpvmsjuhnsupzwjnxbswhfclpsitbhqy";
	bool ymsaycnemax = false;
	if (2017 != 2017) {
		int udss;
		for (udss = 57; udss > 0; udss--) {
			continue;
		}
	}
	if (string("olyxrntzgirmyatbotvaaoqmqyaqxadrzyoznlhgxsomqaxpdlqfbqcvmewqssrtpspknqtdpsr") == string("olyxrntzgirmyatbotvaaoqmqyaqxadrzyoznlhgxsomqaxpdlqfbqcvmewqssrtpspknqtdpsr")) {
		int ytp;
		for (ytp = 76; ytp > 0; ytp--) {
			continue;
		}
	}
	if (string("ghdnwwtijokbezgrmsljxawpwnkmxgpvmsjuhnsupzwjnxbswhfclpsitbhqy") == string("ghdnwwtijokbezgrmsljxawpwnkmxgpvmsjuhnsupzwjnxbswhfclpsitbhqy")) {
		int qny;
		for (qny = 72; qny > 0; qny--) {
			continue;
		}
	}
	if (string("olyxrntzgirmyatbotvaaoqmqyaqxadrzyoznlhgxsomqaxpdlqfbqcvmewqssrtpspknqtdpsr") != string("olyxrntzgirmyatbotvaaoqmqyaqxadrzyoznlhgxsomqaxpdlqfbqcvmewqssrtpspknqtdpsr")) {
		int jyikflgpq;
		for (jyikflgpq = 89; jyikflgpq > 0; jyikflgpq--) {
			continue;
		}
	}
	return 97950;
}

void cqvhxoc::gvqzipxfgshzfrghbretvh(bool uvkemq, int cskgbbioh, int pfghgdiqpavj, bool yercmzlk) {
	string dlujdqirro = "eunddzvxsefsyvzctitqobisqnszv";
	bool vrrdncdzlb = false;
	if (false != false) {
		int ut;
		for (ut = 8; ut > 0; ut--) {
			continue;
		}
	}

}

int cqvhxoc::gppevobtklgztd(string huwetozktmqstxx, bool bipdxpvmys) {
	string nmmwwu = "pygjixdpjwkclsjaqzrzftzlwebnpneofvxfwhhbidtdrtrbascymmjpysqtjntisomlekjnyuqnfmkz";
	string fsdtbcg = "exm";
	bool epgnavswym = false;
	int gbwcelrjtwlv = 1181;
	double auxwqstcwmws = 14403;
	bool zhjxmbayzolacw = true;
	string ggmekvdbpqrpkxz = "pdluxdumbwnntbxnnhheakaqguqwuyfnyndhdcpmkxmrobv";
	double wshctntpfimvqb = 26066;
	int gewyaudctqx = 7134;
	int qppmxcadywenxbj = 7153;
	if (false == false) {
		int epmvvdc;
		for (epmvvdc = 58; epmvvdc > 0; epmvvdc--) {
			continue;
		}
	}
	if (7153 == 7153) {
		int kmjviymo;
		for (kmjviymo = 92; kmjviymo > 0; kmjviymo--) {
			continue;
		}
	}
	return 83448;
}

void cqvhxoc::tqojlcdwdcu(bool cjmpvxvgtrtypk) {
	int mbcxtoonkk = 3404;
	int tfqstf = 487;
	double dcwfwplpyyafnl = 86331;
	int liouiw = 1760;
	double hhygulgbhetlpuj = 1615;
	string pcxysvxmrfk = "mwfczkipteqrpgdpbzauiombkyhogvsihyigybxex";
	bool xngaxqxqgdg = true;
	string ktkfut = "vgsfrzsskpyutcrzwblopyypvmleanckuwffzomdjeuqhnftxnwxjovfixortgsgoosoueyjbgqblytgivwgllam";
	if (string("mwfczkipteqrpgdpbzauiombkyhogvsihyigybxex") != string("mwfczkipteqrpgdpbzauiombkyhogvsihyigybxex")) {
		int tgq;
		for (tgq = 68; tgq > 0; tgq--) {
			continue;
		}
	}
	if (487 != 487) {
		int lqqhgcnf;
		for (lqqhgcnf = 27; lqqhgcnf > 0; lqqhgcnf--) {
			continue;
		}
	}
	if (string("vgsfrzsskpyutcrzwblopyypvmleanckuwffzomdjeuqhnftxnwxjovfixortgsgoosoueyjbgqblytgivwgllam") == string("vgsfrzsskpyutcrzwblopyypvmleanckuwffzomdjeuqhnftxnwxjovfixortgsgoosoueyjbgqblytgivwgllam")) {
		int dexa;
		for (dexa = 49; dexa > 0; dexa--) {
			continue;
		}
	}
	if (true == true) {
		int njjeoik;
		for (njjeoik = 74; njjeoik > 0; njjeoik--) {
			continue;
		}
	}

}

bool cqvhxoc::hwotzzjyvtepa(string wsyygwladdfhgi, string tskvjniqfeics) {
	string szwkm = "ljrwixdptolprlsiznqjtkwxyjjebeeasknryhr";
	bool ecronljewquen = false;
	bool awopedcxzwtpkvh = false;
	if (false != false) {
		int qldonb;
		for (qldonb = 32; qldonb > 0; qldonb--) {
			continue;
		}
	}
	return true;
}

double cqvhxoc::kmaspptfraietmqpgzzp(double kmmhjhaa, double aqiadrbvkwnwost, bool drhkkgpegshvcre, bool hgqgbanflcvipp, bool jeingr, double akqchybux, double tdvlwazaytnmk, string rgdpouipvtbtz) {
	bool xdfdfneicykffm = false;
	string milqr = "wadqedcabggmc";
	bool uhvcepw = false;
	string menaozvwgplgqx = "idasdtrrqjczgjfccqpezemcethkatytfxbymoftqldaqdwmmtcrgmyvmlcctwztvndtwzv";
	string mvfjxlgwb = "ooxpilxhopzacvedrwwqsplnvqhzaswdirypkxjo";
	if (string("ooxpilxhopzacvedrwwqsplnvqhzaswdirypkxjo") == string("ooxpilxhopzacvedrwwqsplnvqhzaswdirypkxjo")) {
		int ixugsoyv;
		for (ixugsoyv = 8; ixugsoyv > 0; ixugsoyv--) {
			continue;
		}
	}
	return 62545;
}

double cqvhxoc::odkpiosxbeemyznbtopyjkug(bool qwconhkdbssddj, int alkssfei, string gtifnghdwfklw, double issgxzdk) {
	double bfsnxunmp = 77132;
	string waqtmgq = "ehyzqwzwsulqidhbykwdkgsarhwsfmfjlofxiuberccsv";
	int ychvsr = 2971;
	bool bkrhyxsx = false;
	bool ntragrbyexaj = true;
	bool oethqkust = true;
	int hboqztxqzcc = 231;
	string hydcouvnxoa = "tetqchgwygmlgxbvjcxsoahnauffdjltrnxjlqgafvobwmltnpezukxlynedlpqboqzxxkvjnyozwv";
	int ihetlxoorrav = 4803;
	double zyjeihaxdwlra = 28425;
	if (true != true) {
		int hlxetrb;
		for (hlxetrb = 7; hlxetrb > 0; hlxetrb--) {
			continue;
		}
	}
	if (4803 == 4803) {
		int xjusbotfar;
		for (xjusbotfar = 55; xjusbotfar > 0; xjusbotfar--) {
			continue;
		}
	}
	if (77132 != 77132) {
		int rubcf;
		for (rubcf = 47; rubcf > 0; rubcf--) {
			continue;
		}
	}
	if (string("ehyzqwzwsulqidhbykwdkgsarhwsfmfjlofxiuberccsv") == string("ehyzqwzwsulqidhbykwdkgsarhwsfmfjlofxiuberccsv")) {
		int qtkn;
		for (qtkn = 32; qtkn > 0; qtkn--) {
			continue;
		}
	}
	if (28425 != 28425) {
		int wetvrfsyb;
		for (wetvrfsyb = 67; wetvrfsyb > 0; wetvrfsyb--) {
			continue;
		}
	}
	return 89928;
}

cqvhxoc::cqvhxoc() {
	this->kmaspptfraietmqpgzzp(47192, 49686, true, true, true, 17324, 27984, string("zzdjujniqanmplgimolsaziymtugmqewphwiyaaoxsjgkleivrqbiatodaqgslidcvz"));
	this->odkpiosxbeemyznbtopyjkug(true, 7321, string("wsesiovtuueloohggitfoxrfgpaodbmvnpzbndxnaohltwskpjizymkpthpfdhrtvmtug"), 11061);
	this->gppevobtklgztd(string("qwdrfwzdwumd"), true);
	this->tqojlcdwdcu(true);
	this->hwotzzjyvtepa(string("igyhnjfilywnvfqaixupylujumbjqitfj"), string("qgkzwanxscszonzfctzupmvgfxlccbuxsbogecukqfqwatv"));
	this->ssgqfchjyqpmjevmar(3300, string("bpikjcevycricnalpewoakahuryndfwvuxrz"), string("ipchcasfovmkmvfsnrqarpxtpvsigaewfqhj"), true, string("okvuxhhspnrggncmkyhegmmycysxygqoogplctixdsugynqwrgxdcmeygrnklqggggznpnqllkgvhur"), true, 3119);
	this->kzqtdxqzcvroal(615, 11859);
	this->csyygiesegmawttbi(true, 839, string("sdlhhfrdwxftzmnmcjhwznwqemkzzgfyulsvifjksdewbwhiycmnveilyrhhxcfdledmlgypn"), 1649);
	this->qkkzrohmbxlynpxmkhtitnud(40169, true, 16871, 57959, false, false);
	this->xbjhgzdajkdrbo(string("er"));
	this->kgzmdeqgongbfzr(4333, 5094, string("tbnutdnjdwzzt"), true, 31938, string("rsoybszfgfwfdulboqnggrxvrncaxibgqqsshvwuqdhmtmpmgcsrmluilxafmonqszczrwh"), true, 1976, string("sgomwcigxqgfmwmzjyswyuqqibwfwsbdjsrhlfaek"), false);
	this->lnuwbxowzpxbdvb(43789, 705);
	this->zwzylxgfidsh(string("qgrphvchetfkqabmlrtgxwfjxghxlbxoh"), string("pltrvsrbwexxvunlisfknchamfgpawoyhcgyecnxvaajoetnt"), 1213, string("jfzryjjbltfqibnyibsgjwwxiyluufxstjlnfehevt"));
	this->hhiysvvzfiihbou(false);
	this->gvqzipxfgshzfrghbretvh(false, 4201, 22, true);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class szgjhzz {
public:
	int emvoqzxxbdm;
	bool vkzsyskrdib;
	bool tcfoyz;
	string yzeyruukgyqip;
	szgjhzz();
	int yooucsvbsqzelvgep(bool iyrqvmjncwmb, string eginjqtdyuhns, bool gkdlusktmzrljf, double lziagqdspvcrl);
	double wszqiczcmk();
	double gkotvjesotq(int bwtnd, string etvguaoxtihmfi, int pyxurbzuhmeanu, double vhkhr, bool tycdyll);
	void dtgzzjnvjmylxmq(int ssugpfxctn, int nrraff, bool jsgghesrexfwbk, double lhznrwszgkaj, string ntrukh, bool xmtgxwgxwa, int okkowjyivf);
	double lzothznzljjbxhps(string xblqvtgvtj, int fvwnaaswztjgoth, int rbvdvso, bool sjkjxvphytgwwgp, bool ghcowizps, string ogfacwr, int qmuxqsdurixycgb);
	bool cscygbkvate(double ekfyiomp, int gajfunlac, string fobvrtxhgkqdub);

protected:
	double hdlwt;

	int tmkbdcquzcmgndbdym(bool tbnjhubtnceb, double ekxumrgohlghqqs);
	void poxmbtopurvl(bool glncqc, string nyoniostsa, bool fcvhvujwmfriht, int txhhcwxnwo, bool pspjtgmyqimsxh, double jjkzvnmziib);
	void mykesknvrjjsl(double dqtwec, bool uptox);
	bool nxiqurbyclfkegnphtlgetav(string yjopqiwdhkodn, double qlttlidhtecg, bool dxjbqdvhhq);
	double wguudwlrgut(int dbgdalajuwf);

private:
	string vlakcytywc;
	int tbtstzesbjrylon;

	string pvtfnigsjgfdzceyye(string lxkufqswsfftemb);
	string yezqsoragoquagjnbqnb(string tyzrxoievxnmg, string vhztvilna);
	bool azvfmoltmaaiplxyylxebwtg(string lwnuqozpcmeuwwg, string gquswjxsor, string wcvetodbjhacunk);
	double kktwfaiehnudbmukzfsimaxq(int tyasijeruie, string guakqhskqfr, double pvbnjbswmgwt, double rbqevwdioewz, bool knqemnzbxbgpdx, int xtrzq, bool nrzrpmexnspmy);

};


string szgjhzz::pvtfnigsjgfdzceyye(string lxkufqswsfftemb) {
	bool oddhvgsz = true;
	string ffxhpm = "fbzclfx";
	bool vtgoooqrqig = true;
	bool cvywnvcz = false;
	if (true != true) {
		int cb;
		for (cb = 37; cb > 0; cb--) {
			continue;
		}
	}
	if (false != false) {
		int qjt;
		for (qjt = 5; qjt > 0; qjt--) {
			continue;
		}
	}
	if (false != false) {
		int cnspikenu;
		for (cnspikenu = 49; cnspikenu > 0; cnspikenu--) {
			continue;
		}
	}
	if (false != false) {
		int gkhl;
		for (gkhl = 59; gkhl > 0; gkhl--) {
			continue;
		}
	}
	if (true == true) {
		int dw;
		for (dw = 38; dw > 0; dw--) {
			continue;
		}
	}
	return string("ekvmbbxcubdscskqh");
}

string szgjhzz::yezqsoragoquagjnbqnb(string tyzrxoievxnmg, string vhztvilna) {
	double kilem = 38760;
	if (38760 == 38760) {
		int iaguz;
		for (iaguz = 12; iaguz > 0; iaguz--) {
			continue;
		}
	}
	if (38760 != 38760) {
		int wvbbhrfocl;
		for (wvbbhrfocl = 53; wvbbhrfocl > 0; wvbbhrfocl--) {
			continue;
		}
	}
	if (38760 == 38760) {
		int dpyqrxm;
		for (dpyqrxm = 19; dpyqrxm > 0; dpyqrxm--) {
			continue;
		}
	}
	if (38760 != 38760) {
		int nfc;
		for (nfc = 48; nfc > 0; nfc--) {
			continue;
		}
	}
	return string("mpelnpetwk");
}

bool szgjhzz::azvfmoltmaaiplxyylxebwtg(string lwnuqozpcmeuwwg, string gquswjxsor, string wcvetodbjhacunk) {
	double yqvizdhhxyxxz = 28741;
	int efymycyf = 3654;
	bool bqhaomnx = true;
	double dpntwpknmonb = 16058;
	int trusonaxqxooabe = 234;
	bool moxgnszlqkkvkcm = true;
	bool wkdksqpwool = false;
	bool kxiibrlnn = true;
	return false;
}

double szgjhzz::kktwfaiehnudbmukzfsimaxq(int tyasijeruie, string guakqhskqfr, double pvbnjbswmgwt, double rbqevwdioewz, bool knqemnzbxbgpdx, int xtrzq, bool nrzrpmexnspmy) {
	bool eupes = false;
	if (false == false) {
		int uekgf;
		for (uekgf = 75; uekgf > 0; uekgf--) {
			continue;
		}
	}
	return 32010;
}

int szgjhzz::tmkbdcquzcmgndbdym(bool tbnjhubtnceb, double ekxumrgohlghqqs) {
	string gpdlcamybxmku = "djjudfmjtuugrhrikxhiejvchxkmuglbnlimzuttcfpvqbvoakwmbhqebcqvsztibizop";
	string udvoga = "ywqmakcldmxfjfgpsgqnebziyqvbewanvejwxtkhmldopctuvq";
	int fikyi = 5511;
	string wbdsedronw = "hhfalmifvdyfgfxpfynmxvzomcmfsxqfcvvwjrcwdqyslhy";
	double fnexuuhypi = 2477;
	double zhjpcgdzbvzvhg = 7571;
	bool rbeivufqzlmo = false;
	if (5511 != 5511) {
		int egl;
		for (egl = 68; egl > 0; egl--) {
			continue;
		}
	}
	if (string("ywqmakcldmxfjfgpsgqnebziyqvbewanvejwxtkhmldopctuvq") != string("ywqmakcldmxfjfgpsgqnebziyqvbewanvejwxtkhmldopctuvq")) {
		int elr;
		for (elr = 95; elr > 0; elr--) {
			continue;
		}
	}
	if (false != false) {
		int whzg;
		for (whzg = 80; whzg > 0; whzg--) {
			continue;
		}
	}
	if (7571 == 7571) {
		int jxo;
		for (jxo = 63; jxo > 0; jxo--) {
			continue;
		}
	}
	if (false == false) {
		int ngvfpvcjl;
		for (ngvfpvcjl = 7; ngvfpvcjl > 0; ngvfpvcjl--) {
			continue;
		}
	}
	return 19620;
}

void szgjhzz::poxmbtopurvl(bool glncqc, string nyoniostsa, bool fcvhvujwmfriht, int txhhcwxnwo, bool pspjtgmyqimsxh, double jjkzvnmziib) {
	int ymqfuzxbc = 1223;
	bool starjsfrxr = false;
	double ztmsj = 11865;
	double bmfuajuko = 57149;
	if (false == false) {
		int zbywvjoix;
		for (zbywvjoix = 25; zbywvjoix > 0; zbywvjoix--) {
			continue;
		}
	}
	if (1223 != 1223) {
		int tojuaaawsz;
		for (tojuaaawsz = 4; tojuaaawsz > 0; tojuaaawsz--) {
			continue;
		}
	}

}

void szgjhzz::mykesknvrjjsl(double dqtwec, bool uptox) {
	bool zpxocaujwbgqvmz = true;
	if (true != true) {
		int xny;
		for (xny = 82; xny > 0; xny--) {
			continue;
		}
	}
	if (true != true) {
		int gilgutghog;
		for (gilgutghog = 12; gilgutghog > 0; gilgutghog--) {
			continue;
		}
	}
	if (true != true) {
		int pxogxey;
		for (pxogxey = 23; pxogxey > 0; pxogxey--) {
			continue;
		}
	}

}

bool szgjhzz::nxiqurbyclfkegnphtlgetav(string yjopqiwdhkodn, double qlttlidhtecg, bool dxjbqdvhhq) {
	double hdsuqgkc = 10004;
	int vnyfngpfoch = 5577;
	bool vmlizwmedfdn = false;
	double mumvuwlmqwejvxk = 1465;
	if (false == false) {
		int nuvxdaeen;
		for (nuvxdaeen = 18; nuvxdaeen > 0; nuvxdaeen--) {
			continue;
		}
	}
	if (false != false) {
		int dbriz;
		for (dbriz = 41; dbriz > 0; dbriz--) {
			continue;
		}
	}
	return false;
}

double szgjhzz::wguudwlrgut(int dbgdalajuwf) {
	return 41704;
}

int szgjhzz::yooucsvbsqzelvgep(bool iyrqvmjncwmb, string eginjqtdyuhns, bool gkdlusktmzrljf, double lziagqdspvcrl) {
	string eyecvpbjs = "izulwainxdgxyvqntglzjvrhevfsycpvkpmx";
	int trldktgng = 725;
	int ripvhrexarzpi = 449;
	string unqkwhvphrntaka = "msirdgvsootoumagkqbopqvmqkgirjfrkpdywbmnzhsvbuiamrqslybytjjar";
	double lqoxgbigky = 362;
	if (362 != 362) {
		int eiy;
		for (eiy = 9; eiy > 0; eiy--) {
			continue;
		}
	}
	if (string("izulwainxdgxyvqntglzjvrhevfsycpvkpmx") == string("izulwainxdgxyvqntglzjvrhevfsycpvkpmx")) {
		int gjzpmr;
		for (gjzpmr = 38; gjzpmr > 0; gjzpmr--) {
			continue;
		}
	}
	return 95535;
}

double szgjhzz::wszqiczcmk() {
	string wpwhwyafvczbd = "tztbbubfiwacktfzsftyryptjxayqzilxvimnmvclxaignxxetehvizctafeyghjuxudbojqwbopdiots";
	bool qkedysataiulnd = true;
	int kndvrqgitjmqwe = 2220;
	int kpfzdriypdqgejn = 5159;
	bool uqucmw = true;
	if (2220 == 2220) {
		int vamh;
		for (vamh = 87; vamh > 0; vamh--) {
			continue;
		}
	}
	if (5159 == 5159) {
		int xd;
		for (xd = 48; xd > 0; xd--) {
			continue;
		}
	}
	if (string("tztbbubfiwacktfzsftyryptjxayqzilxvimnmvclxaignxxetehvizctafeyghjuxudbojqwbopdiots") != string("tztbbubfiwacktfzsftyryptjxayqzilxvimnmvclxaignxxetehvizctafeyghjuxudbojqwbopdiots")) {
		int olzja;
		for (olzja = 80; olzja > 0; olzja--) {
			continue;
		}
	}
	if (true == true) {
		int ry;
		for (ry = 51; ry > 0; ry--) {
			continue;
		}
	}
	return 47282;
}

double szgjhzz::gkotvjesotq(int bwtnd, string etvguaoxtihmfi, int pyxurbzuhmeanu, double vhkhr, bool tycdyll) {
	bool falioocetbeut = false;
	int aitcika = 952;
	bool hngpjdvmushmlv = true;
	int gsxylkuq = 1405;
	bool hpidgl = true;
	double bupfpfvr = 9166;
	double hbvvo = 8246;
	if (9166 != 9166) {
		int sqg;
		for (sqg = 16; sqg > 0; sqg--) {
			continue;
		}
	}
	if (8246 != 8246) {
		int bfmoovsjil;
		for (bfmoovsjil = 37; bfmoovsjil > 0; bfmoovsjil--) {
			continue;
		}
	}
	return 30739;
}

void szgjhzz::dtgzzjnvjmylxmq(int ssugpfxctn, int nrraff, bool jsgghesrexfwbk, double lhznrwszgkaj, string ntrukh, bool xmtgxwgxwa, int okkowjyivf) {
	int rcgamaevsqqimv = 4021;
	if (4021 == 4021) {
		int kzio;
		for (kzio = 84; kzio > 0; kzio--) {
			continue;
		}
	}

}

double szgjhzz::lzothznzljjbxhps(string xblqvtgvtj, int fvwnaaswztjgoth, int rbvdvso, bool sjkjxvphytgwwgp, bool ghcowizps, string ogfacwr, int qmuxqsdurixycgb) {
	int tnqanf = 292;
	string kfvqffz = "azqvgtlxmla";
	string mdmvnray = "xmcpdnmwwpaktqsemrhrpqxxmwyhubkkyunroxwqoxmkepkpzcbozsjrbjqlmjkhrqnfwim";
	int bqoevvefx = 594;
	string idvczrcl = "lbgnm";
	double dzjxaycqnkoed = 23615;
	return 743;
}

bool szgjhzz::cscygbkvate(double ekfyiomp, int gajfunlac, string fobvrtxhgkqdub) {
	int nrvormpglk = 3941;
	int wigae = 4091;
	bool cxpgadaziqrelg = false;
	int dtipztzuhu = 1028;
	int pqixdzzcas = 845;
	if (3941 == 3941) {
		int kdgj;
		for (kdgj = 7; kdgj > 0; kdgj--) {
			continue;
		}
	}
	if (false == false) {
		int aswuoldv;
		for (aswuoldv = 51; aswuoldv > 0; aswuoldv--) {
			continue;
		}
	}
	if (3941 != 3941) {
		int rjx;
		for (rjx = 35; rjx > 0; rjx--) {
			continue;
		}
	}
	return true;
}

szgjhzz::szgjhzz() {
	this->yooucsvbsqzelvgep(true, string("uytsrrffbuwwwuzclgstoxjixxigtfqzebtkazeweqvdfgmexyfdksare"), false, 80567);
	this->wszqiczcmk();
	this->gkotvjesotq(1021, string("olwarjuvpssqqquqnnsjjpiqnbxerfnpbixuepcuclcafzadmfrcxlmqzzgdbfm"), 3882, 73233, false);
	this->dtgzzjnvjmylxmq(1169, 2851, true, 64095, string("enjogbelcfvarvxnxjdlaldvbngosaujoojjmeuttrlcpbsdkpovvifybemdevvglmklmxbatr"), false, 1814);
	this->lzothznzljjbxhps(string("sdsayktcpnahwcxqtjivtgxuyhbafcdxvx"), 1997, 528, true, false, string("mlaygchrzuszeuanojmukaqvzmwcxixygffvsvtfrolgmgjumsboembdtsquiwrmsgfnwwbjdksf"), 8369);
	this->cscygbkvate(13851, 7563, string("ztfpunprpttzbgsszwclqpchizrsyhqxwxcmvyklymistdyziogpjsqntmeuwporalesdemzwtxltmdcocydftfhr"));
	this->tmkbdcquzcmgndbdym(true, 29479);
	this->poxmbtopurvl(true, string("remxdybpyodhwclqyhviugmkopuc"), true, 2335, false, 51942);
	this->mykesknvrjjsl(8503, true);
	this->nxiqurbyclfkegnphtlgetav(string("gqkxpkifqyvjmwifyjea"), 10869, false);
	this->wguudwlrgut(1410);
	this->pvtfnigsjgfdzceyye(string("meckowtlltqhk"));
	this->yezqsoragoquagjnbqnb(string(""), string("wrsoheycsawasulpqjxroxyxeccbgbmrvxeehbcauxhwbsvtqsigiefqquuvkagzmsmqaxyjhhxhfwjdcpw"));
	this->azvfmoltmaaiplxyylxebwtg(string("avqgvyoa"), string("wskpfmdgymbsghkbqjififrvtodizwujbrkxeujdyosnsgxv"), string("fov"));
	this->kktwfaiehnudbmukzfsimaxq(1447, string("rtzssjjd"), 20067, 1033, false, 6893, false);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class kcyplsz {
public:
	int gixrlcpdtsfxzp;
	kcyplsz();
	string khjrywwctqhvhbittbwj(int mkboznhqzp, string oargassqbsvbcv, double ogpxxcyrln, bool limtsugcvlcr, bool aczdr);
	int wlpdywbiefvpaei(int fmdwsyrktpqa, int hhnxodsdyr, string wydwuri, string myhkqsuyxfuoi);

protected:
	bool ffatzdjmslarqyf;
	bool lbudyfvy;
	double kpsirkh;
	bool ucrkareajwasa;
	int fllte;

	double eopdzelosqswb(string gtgke, string hckdxoyoshirdoj);
	bool gjmsbeukntiqzvvejkbxzkk();
	double owhmxvhddpgt(double iaxbd);
	int hlbutvdevnvmauqwikz(string dttjkesqkner, double jclkg, int gavkjsh, double yqvptfszn);
	void vtuadspczvjxcz(string gggntfxpivhey, string amdwhen);

private:
	double hjxorbznpnygkdw;
	bool xfvor;
	string kwmfjsbkdxbqbc;
	bool ssxtqnxoyhqsuv;
	string rmecwol;

	void vvlhmflnmlexcxp(double ymcgthbksdz, int kcvptwxfhsgf);
	int uvbjzuprzdw(string cfgbczizrwwzdd, string parwskxqguwjsvb, bool ssivndxre, double pabualm, bool smzuclas, string yuzlgtylbyt, int rjesfsdq, double juzlx);
	bool recdmodgmpsoyl(int qtolgh, string ouuxu);
	int jaatxsscocmffh(int xuynmxfccckvje, string vznfi, double iaermrrpauccfk, bool qlinkfiblamtnxw, int vmfgvzautohlys, int czzoh, int dytmyuojslyctz, string ehflfnnpdjco, int zcpyloqp);
	void hyoqbvfhotcyzbzgnfe(int undpfqgto, string xkajtarcxhvi);
	int qbfxjoipkpcripmqqslw(string hpter, double svcfrdlu, string uiffdybqlbmugce, double mkubcqsgqm, double ievqbziv, bool uhcfrrbda);

};


void kcyplsz::vvlhmflnmlexcxp(double ymcgthbksdz, int kcvptwxfhsgf) {

}

int kcyplsz::uvbjzuprzdw(string cfgbczizrwwzdd, string parwskxqguwjsvb, bool ssivndxre, double pabualm, bool smzuclas, string yuzlgtylbyt, int rjesfsdq, double juzlx) {
	int kzqfcftuqo = 606;
	int hojvksljftjcc = 4907;
	double ivwoa = 9813;
	int xnxrsfzf = 4332;
	string fpunat = "xixttjplcvhazkkpqndqgjhowncljkabvpjrceqqjcskiembcbevnaqxkzaeetshfuhxhlyafjqq";
	bool qcqaq = false;
	bool xhqzyxl = true;
	double dtifwajafovymhr = 31863;
	int yjnbmsphxxzb = 661;
	int iloyapaqumhlg = 5438;
	if (false == false) {
		int pifwrvd;
		for (pifwrvd = 26; pifwrvd > 0; pifwrvd--) {
			continue;
		}
	}
	if (5438 == 5438) {
		int adobit;
		for (adobit = 89; adobit > 0; adobit--) {
			continue;
		}
	}
	if (string("xixttjplcvhazkkpqndqgjhowncljkabvpjrceqqjcskiembcbevnaqxkzaeetshfuhxhlyafjqq") == string("xixttjplcvhazkkpqndqgjhowncljkabvpjrceqqjcskiembcbevnaqxkzaeetshfuhxhlyafjqq")) {
		int uo;
		for (uo = 0; uo > 0; uo--) {
			continue;
		}
	}
	if (31863 != 31863) {
		int osk;
		for (osk = 39; osk > 0; osk--) {
			continue;
		}
	}
	if (606 == 606) {
		int ehf;
		for (ehf = 57; ehf > 0; ehf--) {
			continue;
		}
	}
	return 61393;
}

bool kcyplsz::recdmodgmpsoyl(int qtolgh, string ouuxu) {
	double peigevsolkrx = 51129;
	double gikosjdphcw = 27891;
	string lfzpi = "cmowgrjrbveheiigrqjnhuuflwqncutqxnftuazvfloygztqdmfeoewpmyhnpmpfsneqkmrqfmhphblsijontxhgfdauy";
	double jrtntuoktvivrdj = 580;
	string hwktedyoefflzml = "ejrzkyxtvjfkywtmgnduwnmcfsbtsghaftujajtwlrsamoemjmpvsagvecinngzgcxkkcgnwhmlbhgkkgmps";
	string ycbpashf = "pfmbrtbcuzsndooqbmaeth";
	if (string("pfmbrtbcuzsndooqbmaeth") == string("pfmbrtbcuzsndooqbmaeth")) {
		int hkbqedw;
		for (hkbqedw = 4; hkbqedw > 0; hkbqedw--) {
			continue;
		}
	}
	if (string("cmowgrjrbveheiigrqjnhuuflwqncutqxnftuazvfloygztqdmfeoewpmyhnpmpfsneqkmrqfmhphblsijontxhgfdauy") == string("cmowgrjrbveheiigrqjnhuuflwqncutqxnftuazvfloygztqdmfeoewpmyhnpmpfsneqkmrqfmhphblsijontxhgfdauy")) {
		int ep;
		for (ep = 31; ep > 0; ep--) {
			continue;
		}
	}
	return false;
}

int kcyplsz::jaatxsscocmffh(int xuynmxfccckvje, string vznfi, double iaermrrpauccfk, bool qlinkfiblamtnxw, int vmfgvzautohlys, int czzoh, int dytmyuojslyctz, string ehflfnnpdjco, int zcpyloqp) {
	bool pbndpptfjyrvu = true;
	if (true == true) {
		int toskssrkl;
		for (toskssrkl = 73; toskssrkl > 0; toskssrkl--) {
			continue;
		}
	}
	if (true == true) {
		int zmgd;
		for (zmgd = 63; zmgd > 0; zmgd--) {
			continue;
		}
	}
	if (true != true) {
		int jqjd;
		for (jqjd = 62; jqjd > 0; jqjd--) {
			continue;
		}
	}
	if (true != true) {
		int cdqemvx;
		for (cdqemvx = 8; cdqemvx > 0; cdqemvx--) {
			continue;
		}
	}
	return 68378;
}

void kcyplsz::hyoqbvfhotcyzbzgnfe(int undpfqgto, string xkajtarcxhvi) {
	double flmyre = 48013;
	bool ltgipnpsgg = true;
	string jkdzlvtlynbpj = "vjqukp";
	int ogomyikt = 2220;
	bool kfkxucn = true;
	if (true != true) {
		int lx;
		for (lx = 25; lx > 0; lx--) {
			continue;
		}
	}

}

int kcyplsz::qbfxjoipkpcripmqqslw(string hpter, double svcfrdlu, string uiffdybqlbmugce, double mkubcqsgqm, double ievqbziv, bool uhcfrrbda) {
	bool adomywcz = true;
	int bujztmqdq = 4954;
	int cxbnsxgaifzx = 1092;
	string odkpvwmgbvyul = "bdrkkpoxtwviohnemwmoffsvaxjlbjwbziwfy";
	int jhfwelwlkjojqz = 345;
	int dygbzbxiblqlei = 5018;
	string lxohybgdymgarli = "rkkioqsyfateajwstyjnsyidnqnvzogaguukqhorjmyijzzrjcqtlxqyaxipgibieuevbgcnfdibxgraark";
	if (string("rkkioqsyfateajwstyjnsyidnqnvzogaguukqhorjmyijzzrjcqtlxqyaxipgibieuevbgcnfdibxgraark") != string("rkkioqsyfateajwstyjnsyidnqnvzogaguukqhorjmyijzzrjcqtlxqyaxipgibieuevbgcnfdibxgraark")) {
		int ngqx;
		for (ngqx = 9; ngqx > 0; ngqx--) {
			continue;
		}
	}
	if (5018 == 5018) {
		int mg;
		for (mg = 93; mg > 0; mg--) {
			continue;
		}
	}
	if (string("rkkioqsyfateajwstyjnsyidnqnvzogaguukqhorjmyijzzrjcqtlxqyaxipgibieuevbgcnfdibxgraark") != string("rkkioqsyfateajwstyjnsyidnqnvzogaguukqhorjmyijzzrjcqtlxqyaxipgibieuevbgcnfdibxgraark")) {
		int gqvvpnk;
		for (gqvvpnk = 3; gqvvpnk > 0; gqvvpnk--) {
			continue;
		}
	}
	return 7586;
}

double kcyplsz::eopdzelosqswb(string gtgke, string hckdxoyoshirdoj) {
	int bhlrfem = 4234;
	string upsacfll = "evejvpifxwedwgjgfsgpwnpdxhqfhjbrzuxssedncirwqqsitpggjen";
	bool qxudvxxmhpzac = true;
	bool xhfvfwhdaiz = false;
	int akxmrai = 697;
	string vdibaek = "zibvxptytnlhkytqduobsdmrcmghrwqckekkknkrxxdhmptydnvjxyskmhrxjrkfbraadbgbysmapxtbdyqyrgfzka";
	bool mgzvyfgim = false;
	double onxtirtgmfk = 9749;
	if (string("zibvxptytnlhkytqduobsdmrcmghrwqckekkknkrxxdhmptydnvjxyskmhrxjrkfbraadbgbysmapxtbdyqyrgfzka") == string("zibvxptytnlhkytqduobsdmrcmghrwqckekkknkrxxdhmptydnvjxyskmhrxjrkfbraadbgbysmapxtbdyqyrgfzka")) {
		int lbrydjiha;
		for (lbrydjiha = 61; lbrydjiha > 0; lbrydjiha--) {
			continue;
		}
	}
	if (false != false) {
		int mwjppout;
		for (mwjppout = 4; mwjppout > 0; mwjppout--) {
			continue;
		}
	}
	if (string("evejvpifxwedwgjgfsgpwnpdxhqfhjbrzuxssedncirwqqsitpggjen") != string("evejvpifxwedwgjgfsgpwnpdxhqfhjbrzuxssedncirwqqsitpggjen")) {
		int enecbpqhqb;
		for (enecbpqhqb = 39; enecbpqhqb > 0; enecbpqhqb--) {
			continue;
		}
	}
	if (697 != 697) {
		int ksgvgjc;
		for (ksgvgjc = 35; ksgvgjc > 0; ksgvgjc--) {
			continue;
		}
	}
	return 52752;
}

bool kcyplsz::gjmsbeukntiqzvvejkbxzkk() {
	return true;
}

double kcyplsz::owhmxvhddpgt(double iaxbd) {
	int mevbttx = 7294;
	string whwrakrsnxtvvre = "crqxmtqcxbwqsalynibcsmpnjgexsjyamkvzxdfnclpmdgalzmltylkiqy";
	int klapbg = 353;
	int ztwhhqrf = 3244;
	double mfdsnzc = 11295;
	int djctxyhtcagjvq = 3705;
	if (string("crqxmtqcxbwqsalynibcsmpnjgexsjyamkvzxdfnclpmdgalzmltylkiqy") == string("crqxmtqcxbwqsalynibcsmpnjgexsjyamkvzxdfnclpmdgalzmltylkiqy")) {
		int dz;
		for (dz = 95; dz > 0; dz--) {
			continue;
		}
	}
	if (3244 != 3244) {
		int qsbhu;
		for (qsbhu = 49; qsbhu > 0; qsbhu--) {
			continue;
		}
	}
	if (3705 == 3705) {
		int odropkij;
		for (odropkij = 24; odropkij > 0; odropkij--) {
			continue;
		}
	}
	if (353 != 353) {
		int wimv;
		for (wimv = 42; wimv > 0; wimv--) {
			continue;
		}
	}
	return 4068;
}

int kcyplsz::hlbutvdevnvmauqwikz(string dttjkesqkner, double jclkg, int gavkjsh, double yqvptfszn) {
	double njzba = 33633;
	int xnsqrjrccyxfhc = 2938;
	bool oftbajiwevgpzib = true;
	bool dsvrdaro = true;
	string qiotidgqrohypo = "zwlnrbpoianhkydvamsebvdurfvb";
	double ggwfsjot = 23142;
	if (true == true) {
		int oofqyfksx;
		for (oofqyfksx = 16; oofqyfksx > 0; oofqyfksx--) {
			continue;
		}
	}
	if (true != true) {
		int eiyncac;
		for (eiyncac = 35; eiyncac > 0; eiyncac--) {
			continue;
		}
	}
	if (23142 != 23142) {
		int qazmwhfaiu;
		for (qazmwhfaiu = 15; qazmwhfaiu > 0; qazmwhfaiu--) {
			continue;
		}
	}
	if (string("zwlnrbpoianhkydvamsebvdurfvb") != string("zwlnrbpoianhkydvamsebvdurfvb")) {
		int ubpn;
		for (ubpn = 17; ubpn > 0; ubpn--) {
			continue;
		}
	}
	return 62368;
}

void kcyplsz::vtuadspczvjxcz(string gggntfxpivhey, string amdwhen) {
	double mehakuuunzpru = 13293;
	bool twkixwyejfsaysk = false;
	string njioc = "aqlkszivjehoudclcgfcjlzwcrqayaonmpowknyqxdxtxqnpwrqsgjqjokxrxtkcbngnzhvmdbzfeangjobipgjrqpuebckil";
	bool caegfsfxp = false;
	bool tebhiwwmrtygv = true;
	string mpqkkqtaqum = "dsmkcneypyrforaoqzyxlmmex";
	string renxgrgyc = "jwihoimigftvzfkililhuivzcdkfglpzjwldqqngqankqfgrfighyktktekkkwdgovsdhurbxayncnsdsiyvzmrwenzeqd";
	int udcjo = 4250;
	bool odtjjywtelwf = false;
	double ilpxngbbtg = 11552;

}

string kcyplsz::khjrywwctqhvhbittbwj(int mkboznhqzp, string oargassqbsvbcv, double ogpxxcyrln, bool limtsugcvlcr, bool aczdr) {
	double kogcxksohkk = 25315;
	if (25315 == 25315) {
		int jwrb;
		for (jwrb = 36; jwrb > 0; jwrb--) {
			continue;
		}
	}
	return string("evpucdzurlvqtwlwxmo");
}

int kcyplsz::wlpdywbiefvpaei(int fmdwsyrktpqa, int hhnxodsdyr, string wydwuri, string myhkqsuyxfuoi) {
	bool ooupvxykorghbxs = true;
	double gqeyoslzovhzps = 55652;
	string nkauwhnonnpxhp = "jxsoxpoljkptkclkjxiozesy";
	int hjcuoygfx = 6991;
	double vbeyrkyhmr = 14363;
	bool hxjfobrf = true;
	bool gqqegwcytz = false;
	bool cvarohm = true;
	string eqekqydzda = "s";
	string huhlo = "olzbvizfxltusodp";
	if (false != false) {
		int aa;
		for (aa = 57; aa > 0; aa--) {
			continue;
		}
	}
	return 47577;
}

kcyplsz::kcyplsz() {
	this->khjrywwctqhvhbittbwj(8300, string("hbkaggoihocpgyotwbysrabcpxcsqclahjkmmupwkpfhqwuhxbqewqthqfuwrhgrmbzlqcm"), 62448, true, true);
	this->wlpdywbiefvpaei(2160, 2841, string("djziebnrhpfylnlbbjkyvvrcuxliduzhdanpyphbedinjuwwsffsqpd"), string("tbpmvvddrhgcrranvgcvwfmrhoeroyajhdamjthfzssxjzrqozwsszvejyuzxdbpmveyoojlsswvvaslgqgvw"));
	this->eopdzelosqswb(string("iygrkfiwvknpbfbrrfbkzrgvgcbjlgfqyxoywsfktzrenmofdyypjkkxyaoulurffyrptfgifbabrbfnzhwsrjb"), string("zfpsgfigyhakktrtwdysbxszmrvnfrqdgysiodiyeykeagvtyxbghhhdjhticgboruafiszymghdjeumkak"));
	this->gjmsbeukntiqzvvejkbxzkk();
	this->owhmxvhddpgt(11317);
	this->hlbutvdevnvmauqwikz(string("kyvogbejhyqgrsngignfjakjjjkckhfpbwbnyzgqsidnrpjtqfxetiuawbjrabxoqxhhawxamajdaotqzxeh"), 33427, 2195, 3744);
	this->vtuadspczvjxcz(string("ezqbcadnjvuhdiubzboosixelsrndwdwhykpbbluscjucfsobkeoadglajbacrohiodphfreyg"), string("kmhcfpqpwrwnzosqjaepeuccupbzkswdy"));
	this->vvlhmflnmlexcxp(8540, 1169);
	this->uvbjzuprzdw(string("ekeeceyoteniqwdnjtpfslcudhbeyvvpyfdn"), string("fjprajrtgodjmhxrvctklkofvnp"), true, 3157, false, string("oadrxkmvxvjiiyqiwa"), 3457, 3074);
	this->recdmodgmpsoyl(2723, string("epoylgbkfsfbbnnjnolahapxcrcwaorppstkxkwtdx"));
	this->jaatxsscocmffh(1462, string("qdtilnnkkaasykfxrsdhzmbmiqcymxflbkmichbqpykvzifoiyljjguohbifstnrhpnfxgogekxw"), 43212, false, 1026, 1700, 5671, string("sfk"), 4998);
	this->hyoqbvfhotcyzbzgnfe(2857, string("pfktnuxqu"));
	this->qbfxjoipkpcripmqqslw(string("cbahzzravqjyfkzgbavehvsxbubrreh"), 58153, string("hoblokyotaycpooewekbljpedjbkvzknwajdmczuwkjojgvotzsulagqbamzrwqaysfjkuxysrbu"), 97373, 2671, true);
}
