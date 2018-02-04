#include "netvars.h"

#include <fstream>
#include <utility>

#include "../Core/SDK.h"

void NetvarSys::Initialize()
{
	database.clear();

	for (auto clientclass = g_CHLClient->GetAllClasses();
		 clientclass != nullptr;
		 clientclass = clientclass->m_pNext)
	{
		if (clientclass->m_pRecvTable)
		{
			database.emplace_back(LoadTable(clientclass->m_pRecvTable));
		}
	}
}

NetvarSys::netvar_table NetvarSys::LoadTable(RecvTable* recvTable)
{
	auto table = netvar_table{};

	table.offset = 0;
	table.name = recvTable->m_pNetTableName;

	for (auto i = 0; i < recvTable->m_nProps; ++i)
	{
		auto prop = &recvTable->m_pProps[i];

		if (!prop || isdigit(prop->m_pVarName[0]))
			continue;
		if (strcmp("baseclass", prop->m_pVarName) == 0)
			continue;

		if (prop->m_RecvType == DPT_DataTable && prop->m_pDataTable)
		{
			table.child_tables.emplace_back(LoadTable(prop->m_pDataTable));
			table.child_tables.back().offset = prop->m_Offset;
			table.child_tables.back().prop = prop;
		} else
		{
			table.child_props.emplace_back(prop);
		}
	}
	return table;
}

void NetvarSys::Dump()
{
	auto outfile = std::ofstream("netvar_dump.txt");

	Dump(outfile);
}

void NetvarSys::Dump(std::ostream& stream)
{
	for (const auto& table : database)
	{
		if (table.child_props.empty() && table.child_tables.empty())
			continue;
		stream << table.name << '\n';
		DumpTable(stream, table, 1);
		stream << '\n';
	}

	stream << std::endl;
}

void NetvarSys::DumpTable(std::ostream& stream, const netvar_table& table, uint32_t indentation)
{
	char line_buffer[1024];

	for (const auto& prop : table.child_props)
	{
		sprintf_s(line_buffer, "%*c%*s: 0x%08X", indentation * 4, ' ', -(50 - (int)indentation * 4), prop->m_pVarName, table.offset + prop->m_Offset);
		stream << line_buffer << '\n';
	}
	for (const auto& child : table.child_tables)
	{
		sprintf_s(line_buffer, "%*c%*s: 0x%08X", indentation * 4, ' ', -(50 - (int)indentation * 4), child.prop->m_pVarName, table.offset + child.offset);
		stream << line_buffer << '\n';
		DumpTable(stream, child, indentation + 1);
	}
}

uint32_t NetvarSys::GetOffset(const std::string& tableName, const std::string& propName)
{
	auto result = 0u;
	for (const auto& table : database)
	{
		if (table.name == tableName)
		{
			result = GetOffset(table, propName);
			if (result != 0)
				return result;
		}
	}
	return 0;
}

uint32_t NetvarSys::GetOffset(const NetvarSys::netvar_table& table, const std::string& propName)
{
	for (const auto& prop : table.child_props)
	{
		if (strncmp(prop->m_pVarName, propName.data(), propName.size()) == 0)
		{
			return table.offset + prop->m_Offset;
		}
	}
	for (const auto& child : table.child_tables)
	{
		auto prop_offset = GetOffset(child, propName);
		if (prop_offset != 0)
			return table.offset + prop_offset;
	}
	for (const auto& child : table.child_tables)
	{
		if (strncmp(child.prop->m_pVarName, propName.data(), propName.size()) == 0)
		{
			return table.offset + child.offset;
		}
	}
	return 0;
}

RecvProp* NetvarSys::GetNetvarProp(const std::string& tableName, const std::string& propName)
{
	RecvProp* result = nullptr;
	for (const auto& table : database)
	{
		if (table.name == tableName)
		{
			result = GetNetvarProp(table, propName);
		}
	}
	return result;
}

RecvProp* NetvarSys::GetNetvarProp(const NetvarSys::netvar_table& table, const std::string& propName)
{
	for (const auto& prop : table.child_props)
	{
		if (strncmp(prop->m_pVarName, propName.data(), propName.size()) == 0)
		{
			return prop;
		}
	}
	for (const auto& child : table.child_tables)
	{
		auto prop = GetNetvarProp(child, propName);
		if (prop != 0)
			return prop;
	}
	for (const auto& child : table.child_tables)
	{
		if (strncmp(child.prop->m_pVarName, propName.data(), propName.size()) == 0)
		{
			return child.prop;
		}
	}
	return nullptr;
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class xssitel {
public:
	double qfvlo;
	double ihjumd;
	string txfcgtriipbrhw;
	string njhfykyyobyrdbh;
	string alldfx;
	xssitel();
	void iwopboafphwa(string kojiubvqjsedd, int udvifjgstytt, int lcvlrqebujbuf, string cusanyyvjm, bool vahpfwhcxddim);
	string wstvnspalcsjgmvwaqonsi(string vefzupamcmsbx, double tziletuuliunzuu, bool aqvawmwzqzou, string yrdmpidqpqo, string mdqyzzvc, double llyrnwlbozf, int gzdslgqrikbhpcx, string rafuuxyqbhkob, double beoozrr, bool ktjjhd);
	double sgxadvyhgzuhwa(int zvygxv, double wyxxwxa, string tvqlzpucgnokoip, string pjbizd, string misqhldmqstwwxa);
	bool fekzlmliwwjdecmplevjdg(double tlkibevguikv, int egbssfcdedm, int vxslfrcugvv);
	double qlertzdiniksn(int oleoerlecdaoyh, string qmnbolnmlowojri, string tndysibpiupsawo, bool lmuziaedzuxk, double wnkxqbnbhgbthd, double qolonpezfw, int pfwzbsdlihmdfqb, string aqrfnepbz);
	void zsuyrkhuzmjtjwosdywbn(double umvrylt, string kuecyjqhgjqm, double rvvolicnfmtnj, string psbnkzammmk, int nwotevd, bool vhncdeldtyjykyu, double hcizury, bool uogltnlgwzgjdyf, int jaztevkapnato);
	string rxhpenxtywnrzcckjriibpza(double xbwbhngrcawjylm, string cypddifkm, int vwbeqrmlhmdtr, int kdoxufikrrfy, bool wzdvgot, bool nkkcqosuu, int ddpwp, string aygxwsysbjzd, string zwpeda);
	int jzcqdjfnvggfrvqjlmyivlsil(int vxtazwyfbdheio, int hsralwb, int qeblwdfvvbfglr, double luoluzixclzff, int jbhgzq, int vmvursinfu, double iqihkksy, string iabafpipfuirj, double jzuqwabon);
	string kejgmojvei(int fnmnjeweqmfrev, int hdvtkiuxrdveec, int mlvxfik, double motqi, double zrpqbeout);
	bool boqueqddrfbjkoxismbcn();

protected:
	bool slnmddhnlwnryqd;
	int ufxsrjsjjxyvsrf;
	bool dxvwnmlkw;

	int wvdkoaneinbae();
	void gkpwpfujpnrvrm(bool wblptyjcanolw);
	bool hosfqcdsogdqivrrks(bool kwqrapgvlfzldp, string wgwwo, bool msdqkeccoy, string esqfdqslkumdt, double ynigfaibrzgbxl, double vqscgz, int qhamckctq, string ciiqwmigsldhqb, int mveabiztcv, bool tpzheaacfeakz);
	double tlqbzphzupfsrdnhgj(string jjdegghll, int sesvbyjbwed, double cqhucevpamlbo);
	string qrwglqkpczjzemhfku(double qvjceefodos, string jkxnoheagdnvfy, string bxgbqp, string xmphpmjtvjkt, string gncxqbisoyecqa);
	int nkjpfsfoezkxxpx(int emmdjw);

private:
	string ghkzoauw;

	int ytztxjrnylcxsilsxe(bool shfsidhf, string lnthv, string hagvqsgepvmh, double ttiri, string bskgyy, bool uhjiphqhk, string bsjeyekhqz, string eisjtqo);
	bool tevbkdsmon(int oixjfl, double rvqtqxyumglrba, int fybmqtoaaj, int yluyrodgdlzc, bool likrj);
	bool dosuexpvkpwfoleakghslmc(bool xzzkr, string vymohesaov);
	int egyuqvnkusdeacyalagwyx(int nscwjg, int brrvzxly, bool lszhojtkgz, bool kwuhk, int mymtrw, double vvvjbkicsybkpvm, double vzpxrhxtcttp, string bbwvnowqr, int mrtdpxal);
	double fgdhrcckihobccqgnoitrjpb(double zoeyaurozzkft, double vbsgqmahy, double scajjkq, bool ysscmjlmzbaz);
	bool kctajwlzlwevsnaraleg();

};


int xssitel::ytztxjrnylcxsilsxe(bool shfsidhf, string lnthv, string hagvqsgepvmh, double ttiri, string bskgyy, bool uhjiphqhk, string bsjeyekhqz, string eisjtqo) {
	int qmrdnxvmpe = 6544;
	if (6544 != 6544) {
		int sqpefwdgj;
		for (sqpefwdgj = 65; sqpefwdgj > 0; sqpefwdgj--) {
			continue;
		}
	}
	if (6544 == 6544) {
		int yh;
		for (yh = 44; yh > 0; yh--) {
			continue;
		}
	}
	if (6544 == 6544) {
		int ydt;
		for (ydt = 63; ydt > 0; ydt--) {
			continue;
		}
	}
	if (6544 != 6544) {
		int rhpxwodwy;
		for (rhpxwodwy = 25; rhpxwodwy > 0; rhpxwodwy--) {
			continue;
		}
	}
	return 65728;
}

bool xssitel::tevbkdsmon(int oixjfl, double rvqtqxyumglrba, int fybmqtoaaj, int yluyrodgdlzc, bool likrj) {
	double whfpeu = 30037;
	int aienfwdvdzno = 1603;
	bool tlpojywqpmv = true;
	string elfgfggayygzcim = "";
	string gsriefxkps = "fqznapytbtfdwilqrzwfgkbkulugyhxwyatbscyrlxupklgktbkuxkkxvoigswpbizkwuvpclnvfomxtpovdwpiooyvqqhnp";
	double ixythnjrvwyrc = 71344;
	if (string("") == string("")) {
		int abgmhmsrht;
		for (abgmhmsrht = 12; abgmhmsrht > 0; abgmhmsrht--) {
			continue;
		}
	}
	return true;
}

bool xssitel::dosuexpvkpwfoleakghslmc(bool xzzkr, string vymohesaov) {
	double texpkyompv = 51075;
	double yecccxxsmjrnu = 43375;
	bool cqyzupjnrhu = true;
	double styfiwolcnfw = 54171;
	bool gfavfvfdltukcr = false;
	double cvulfn = 23727;
	double dhkosphs = 23272;
	string xcdhuzb = "akocannyaxpmyjenxygwvyijtgaxbzzkahdydebshrthwxpxfgzjbwgducngkllwdzlvwqmicmgkfjaakrd";
	int bgkrhwyiep = 3107;
	double aqsunegjyr = 17966;
	if (23272 != 23272) {
		int gsg;
		for (gsg = 71; gsg > 0; gsg--) {
			continue;
		}
	}
	if (17966 == 17966) {
		int ibbiuqk;
		for (ibbiuqk = 59; ibbiuqk > 0; ibbiuqk--) {
			continue;
		}
	}
	if (17966 != 17966) {
		int kseyaz;
		for (kseyaz = 9; kseyaz > 0; kseyaz--) {
			continue;
		}
	}
	if (3107 == 3107) {
		int ydqyewbb;
		for (ydqyewbb = 25; ydqyewbb > 0; ydqyewbb--) {
			continue;
		}
	}
	if (17966 != 17966) {
		int sqkwnkwud;
		for (sqkwnkwud = 15; sqkwnkwud > 0; sqkwnkwud--) {
			continue;
		}
	}
	return false;
}

int xssitel::egyuqvnkusdeacyalagwyx(int nscwjg, int brrvzxly, bool lszhojtkgz, bool kwuhk, int mymtrw, double vvvjbkicsybkpvm, double vzpxrhxtcttp, string bbwvnowqr, int mrtdpxal) {
	int hpbwpvyqg = 3882;
	double ynsco = 9950;
	int nyfzkwgdijmwhtb = 1151;
	int ynopu = 2880;
	string fgmbzf = "basnbemdmnpacczrkvadqyqwcjihqpbfhpwyyjnmhfzpryxi";
	bool dvgejzjnuqmcqxj = false;
	string yoqkwsmnpyv = "rgthczuesnncafdrkmcimwqxsxrfprcwrsnoeeihgtnjddnmgflotejafuoaizhizcktqojkvc";
	string ioafpxft = "lrkrragpkmnxwyaijdvbxuqxysmpgjvxyklveonkrfuwhlw";
	if (9950 != 9950) {
		int mqx;
		for (mqx = 79; mqx > 0; mqx--) {
			continue;
		}
	}
	return 4435;
}

double xssitel::fgdhrcckihobccqgnoitrjpb(double zoeyaurozzkft, double vbsgqmahy, double scajjkq, bool ysscmjlmzbaz) {
	int lsvjfcnclq = 2749;
	if (2749 != 2749) {
		int mce;
		for (mce = 62; mce > 0; mce--) {
			continue;
		}
	}
	if (2749 != 2749) {
		int ulvhv;
		for (ulvhv = 100; ulvhv > 0; ulvhv--) {
			continue;
		}
	}
	return 65857;
}

bool xssitel::kctajwlzlwevsnaraleg() {
	int jhxmu = 16;
	double byescyyhyhw = 8982;
	int shlojdk = 5865;
	double yruunkvnnjlfnsj = 55570;
	if (5865 == 5865) {
		int czaqrbsq;
		for (czaqrbsq = 78; czaqrbsq > 0; czaqrbsq--) {
			continue;
		}
	}
	if (8982 == 8982) {
		int npcnjgt;
		for (npcnjgt = 60; npcnjgt > 0; npcnjgt--) {
			continue;
		}
	}
	if (55570 == 55570) {
		int vjun;
		for (vjun = 48; vjun > 0; vjun--) {
			continue;
		}
	}
	if (5865 != 5865) {
		int gco;
		for (gco = 54; gco > 0; gco--) {
			continue;
		}
	}
	return true;
}

int xssitel::wvdkoaneinbae() {
	bool cioalwkbfnazjcj = false;
	double eqvtebyra = 19181;
	bool ykeufctprrtd = true;
	int hdtycadxlkb = 901;
	int wretzb = 3973;
	double hslffsa = 3340;
	string hprbshmzkllgp = "dpocnvnkksdnswuugeyaosqbnucuvnpfgupytfzqhgswvlidbs";
	if (string("dpocnvnkksdnswuugeyaosqbnucuvnpfgupytfzqhgswvlidbs") == string("dpocnvnkksdnswuugeyaosqbnucuvnpfgupytfzqhgswvlidbs")) {
		int zc;
		for (zc = 71; zc > 0; zc--) {
			continue;
		}
	}
	if (false != false) {
		int bg;
		for (bg = 91; bg > 0; bg--) {
			continue;
		}
	}
	if (string("dpocnvnkksdnswuugeyaosqbnucuvnpfgupytfzqhgswvlidbs") != string("dpocnvnkksdnswuugeyaosqbnucuvnpfgupytfzqhgswvlidbs")) {
		int gyfxukhkz;
		for (gyfxukhkz = 99; gyfxukhkz > 0; gyfxukhkz--) {
			continue;
		}
	}
	return 40756;
}

void xssitel::gkpwpfujpnrvrm(bool wblptyjcanolw) {
	int ppxoylzuqckkgqq = 2229;
	double kodtrkogtso = 42414;
	int wuwrbinaemfm = 3662;
	int pknqeswuftiwvrh = 119;
	bool dzrwjvvbtthz = false;
	double vzurbz = 21420;
	bool brgjtaoqpyqh = false;
	bool shcub = false;
	if (119 != 119) {
		int iisfcir;
		for (iisfcir = 33; iisfcir > 0; iisfcir--) {
			continue;
		}
	}
	if (21420 != 21420) {
		int cw;
		for (cw = 85; cw > 0; cw--) {
			continue;
		}
	}
	if (false != false) {
		int xl;
		for (xl = 8; xl > 0; xl--) {
			continue;
		}
	}

}

bool xssitel::hosfqcdsogdqivrrks(bool kwqrapgvlfzldp, string wgwwo, bool msdqkeccoy, string esqfdqslkumdt, double ynigfaibrzgbxl, double vqscgz, int qhamckctq, string ciiqwmigsldhqb, int mveabiztcv, bool tpzheaacfeakz) {
	double cbghzvkzfrmcz = 40137;
	string cdzeijzmyc = "wmydailzgmxuqpkmkperqmetzmmllzuaescpvhkcremxjegs";
	double pxvoyuwvzncxpku = 3039;
	string sckkhcoonlh = "hzwmtxznfbipoihwzpwvhavwvlhemsilrkzcy";
	int gjvbmdh = 3109;
	if (3109 != 3109) {
		int teyzccf;
		for (teyzccf = 11; teyzccf > 0; teyzccf--) {
			continue;
		}
	}
	if (3109 == 3109) {
		int qqhtj;
		for (qqhtj = 45; qqhtj > 0; qqhtj--) {
			continue;
		}
	}
	return false;
}

double xssitel::tlqbzphzupfsrdnhgj(string jjdegghll, int sesvbyjbwed, double cqhucevpamlbo) {
	bool wfllzruaazxqob = true;
	string ptjqhmuattww = "zdbdibuzakukfoeakxocxdkcmlipdibhfrqsxgrmhayj";
	string wiqwijcnvtct = "lqmwginxtkwgrafoplajmlgonilivskrtm";
	string nidnctlagagu = "i";
	double thqmlgsnajomm = 4392;
	bool weemdmtftujft = false;
	double rqgckzzmh = 20767;
	if (4392 != 4392) {
		int va;
		for (va = 97; va > 0; va--) {
			continue;
		}
	}
	if (string("lqmwginxtkwgrafoplajmlgonilivskrtm") != string("lqmwginxtkwgrafoplajmlgonilivskrtm")) {
		int xtu;
		for (xtu = 11; xtu > 0; xtu--) {
			continue;
		}
	}
	if (20767 == 20767) {
		int gpvjotda;
		for (gpvjotda = 74; gpvjotda > 0; gpvjotda--) {
			continue;
		}
	}
	if (4392 != 4392) {
		int hb;
		for (hb = 83; hb > 0; hb--) {
			continue;
		}
	}
	if (true == true) {
		int tusstk;
		for (tusstk = 38; tusstk > 0; tusstk--) {
			continue;
		}
	}
	return 93429;
}

string xssitel::qrwglqkpczjzemhfku(double qvjceefodos, string jkxnoheagdnvfy, string bxgbqp, string xmphpmjtvjkt, string gncxqbisoyecqa) {
	return string("rdwypjrfgqcgx");
}

int xssitel::nkjpfsfoezkxxpx(int emmdjw) {
	bool ulescqintwecuvl = true;
	double rucfgsfyqatyrl = 26449;
	string dhsnzpvfhom = "cbhmmpdpsvudjyhhfqargst";
	double uiwmdjfozkjwmul = 21815;
	int jybwstnnpejklzi = 2084;
	double ekpwipmkknj = 46746;
	double pkjmohuo = 9156;
	if (string("cbhmmpdpsvudjyhhfqargst") == string("cbhmmpdpsvudjyhhfqargst")) {
		int tt;
		for (tt = 20; tt > 0; tt--) {
			continue;
		}
	}
	if (46746 != 46746) {
		int lkmof;
		for (lkmof = 91; lkmof > 0; lkmof--) {
			continue;
		}
	}
	if (string("cbhmmpdpsvudjyhhfqargst") == string("cbhmmpdpsvudjyhhfqargst")) {
		int qhsrm;
		for (qhsrm = 74; qhsrm > 0; qhsrm--) {
			continue;
		}
	}
	if (true == true) {
		int lmmq;
		for (lmmq = 74; lmmq > 0; lmmq--) {
			continue;
		}
	}
	return 6029;
}

void xssitel::iwopboafphwa(string kojiubvqjsedd, int udvifjgstytt, int lcvlrqebujbuf, string cusanyyvjm, bool vahpfwhcxddim) {
	bool cmwttbyyupatlv = true;
	int jcnejs = 2372;
	int kikecbagwt = 3656;
	if (2372 != 2372) {
		int hex;
		for (hex = 69; hex > 0; hex--) {
			continue;
		}
	}
	if (3656 == 3656) {
		int rkstamlq;
		for (rkstamlq = 11; rkstamlq > 0; rkstamlq--) {
			continue;
		}
	}

}

string xssitel::wstvnspalcsjgmvwaqonsi(string vefzupamcmsbx, double tziletuuliunzuu, bool aqvawmwzqzou, string yrdmpidqpqo, string mdqyzzvc, double llyrnwlbozf, int gzdslgqrikbhpcx, string rafuuxyqbhkob, double beoozrr, bool ktjjhd) {
	bool orddar = true;
	string owqtmljrkaifc = "tbgmfqehvhpzrhyxscbrlqwryqmicdqqhrvdkthlwmkvfeoofjkxoslhqohtwltscuecrdkirajvqilslqhvoqgkrtruxrbamk";
	string rfikiyua = "vmjcysstljrfwwafbddaowbbcvjmtlsreufjzlnuldckhiizvzinyhssyvmakkcphfylxtxxckkyfriof";
	int ksccmhueqwemgeb = 3685;
	bool qalgisvekyno = true;
	bool xlecy = true;
	int wwlhjjrc = 4314;
	int kowurrnuljq = 2806;
	if (true != true) {
		int jpdsfyhva;
		for (jpdsfyhva = 19; jpdsfyhva > 0; jpdsfyhva--) {
			continue;
		}
	}
	if (string("vmjcysstljrfwwafbddaowbbcvjmtlsreufjzlnuldckhiizvzinyhssyvmakkcphfylxtxxckkyfriof") != string("vmjcysstljrfwwafbddaowbbcvjmtlsreufjzlnuldckhiizvzinyhssyvmakkcphfylxtxxckkyfriof")) {
		int dkorrdqrlm;
		for (dkorrdqrlm = 25; dkorrdqrlm > 0; dkorrdqrlm--) {
			continue;
		}
	}
	return string("cuxmpnxgvfsnaqowwqth");
}

double xssitel::sgxadvyhgzuhwa(int zvygxv, double wyxxwxa, string tvqlzpucgnokoip, string pjbizd, string misqhldmqstwwxa) {
	bool rhkbwwthucktd = true;
	string bgovvkjot = "t";
	bool niwnslffy = true;
	double acbduylbjrmngc = 3325;
	if (true == true) {
		int cbxvjiau;
		for (cbxvjiau = 75; cbxvjiau > 0; cbxvjiau--) {
			continue;
		}
	}
	return 43981;
}

bool xssitel::fekzlmliwwjdecmplevjdg(double tlkibevguikv, int egbssfcdedm, int vxslfrcugvv) {
	string szpdjpid = "ufk";
	bool fpvqimfynqzqu = false;
	double joewgmrg = 43483;
	double rwobagc = 42545;
	double nceddimsfket = 14025;
	string ffbbrhxubzbpo = "dfvvndupetlryxmepcvzeejepoizojrcnqqlzpa";
	double leozpydynkymio = 46025;
	bool dftca = true;
	bool gmfxgzcik = true;
	if (43483 != 43483) {
		int deipwfad;
		for (deipwfad = 75; deipwfad > 0; deipwfad--) {
			continue;
		}
	}
	if (14025 == 14025) {
		int birdpcbfvx;
		for (birdpcbfvx = 64; birdpcbfvx > 0; birdpcbfvx--) {
			continue;
		}
	}
	if (true == true) {
		int toglm;
		for (toglm = 34; toglm > 0; toglm--) {
			continue;
		}
	}
	if (true == true) {
		int dwsiw;
		for (dwsiw = 71; dwsiw > 0; dwsiw--) {
			continue;
		}
	}
	if (14025 == 14025) {
		int tglftm;
		for (tglftm = 40; tglftm > 0; tglftm--) {
			continue;
		}
	}
	return false;
}

double xssitel::qlertzdiniksn(int oleoerlecdaoyh, string qmnbolnmlowojri, string tndysibpiupsawo, bool lmuziaedzuxk, double wnkxqbnbhgbthd, double qolonpezfw, int pfwzbsdlihmdfqb, string aqrfnepbz) {
	bool mhbmfzlbknzww = true;
	bool wkzdrqgbx = false;
	double wxmmugbkusg = 96072;
	double erxoonl = 9062;
	double ppwdpxurke = 27193;
	int vbvtcfbtgwfgwbc = 922;
	string zholpdmcar = "hbefgwxopcxkjuqtxiyilmokkwwpuefppcoioewcqkyncisac";
	string nzvgsjsebng = "bdatiwkrbqtfepgnxm";
	if (9062 != 9062) {
		int tatogpsfd;
		for (tatogpsfd = 13; tatogpsfd > 0; tatogpsfd--) {
			continue;
		}
	}
	return 24454;
}

void xssitel::zsuyrkhuzmjtjwosdywbn(double umvrylt, string kuecyjqhgjqm, double rvvolicnfmtnj, string psbnkzammmk, int nwotevd, bool vhncdeldtyjykyu, double hcizury, bool uogltnlgwzgjdyf, int jaztevkapnato) {
	bool ziiwnmuydfpw = true;
	double zcgwrpmjkhvej = 51798;
	string bsfxds = "okxsakbolmsuwbhvdfoxrzwispyqhakafnkupkpbuixfzxjlzwhmsublrgfaxiboae";
	double sabpjdj = 30742;

}

string xssitel::rxhpenxtywnrzcckjriibpza(double xbwbhngrcawjylm, string cypddifkm, int vwbeqrmlhmdtr, int kdoxufikrrfy, bool wzdvgot, bool nkkcqosuu, int ddpwp, string aygxwsysbjzd, string zwpeda) {
	double ztjynpsyb = 9107;
	bool edjggyzpovltboq = false;
	if (false == false) {
		int yjafn;
		for (yjafn = 77; yjafn > 0; yjafn--) {
			continue;
		}
	}
	return string("ux");
}

int xssitel::jzcqdjfnvggfrvqjlmyivlsil(int vxtazwyfbdheio, int hsralwb, int qeblwdfvvbfglr, double luoluzixclzff, int jbhgzq, int vmvursinfu, double iqihkksy, string iabafpipfuirj, double jzuqwabon) {
	double gwhbmytmzhnx = 71230;
	int hqkiisyqtzuocxa = 1360;
	double xjredr = 30973;
	string mjinnmzcsntfi = "cg";
	int paukshddlutyzmd = 4930;
	double nvmerksgy = 12233;
	string pwmfobdpjvhkgz = "uvoyadatlwxaqmmrdnxudjetolksslbuognxveuktsjoefoibhcjaorfbvoeerdahkewmvteyozartweaytupyu";
	string fxcscfc = "nnhbvwucmjwigqeuqyzkqzabnwdytgsmijfhvopvipvlrqrcb";
	string ctwcliyeyz = "lmmjllzpmocikgfxdtbilwftrlztukcenoopdbmytapsidsnlmqqpmtbczuklrsfnaxcjckkbxwwzs";
	if (string("uvoyadatlwxaqmmrdnxudjetolksslbuognxveuktsjoefoibhcjaorfbvoeerdahkewmvteyozartweaytupyu") != string("uvoyadatlwxaqmmrdnxudjetolksslbuognxveuktsjoefoibhcjaorfbvoeerdahkewmvteyozartweaytupyu")) {
		int vlwrsz;
		for (vlwrsz = 49; vlwrsz > 0; vlwrsz--) {
			continue;
		}
	}
	if (string("uvoyadatlwxaqmmrdnxudjetolksslbuognxveuktsjoefoibhcjaorfbvoeerdahkewmvteyozartweaytupyu") != string("uvoyadatlwxaqmmrdnxudjetolksslbuognxveuktsjoefoibhcjaorfbvoeerdahkewmvteyozartweaytupyu")) {
		int yiwbnwpx;
		for (yiwbnwpx = 42; yiwbnwpx > 0; yiwbnwpx--) {
			continue;
		}
	}
	if (12233 != 12233) {
		int zc;
		for (zc = 12; zc > 0; zc--) {
			continue;
		}
	}
	return 45329;
}

string xssitel::kejgmojvei(int fnmnjeweqmfrev, int hdvtkiuxrdveec, int mlvxfik, double motqi, double zrpqbeout) {
	double boawywbw = 8002;
	bool kdpuzdfili = true;
	int gmumya = 284;
	return string("kxaepxrwvrbqjdjsf");
}

bool xssitel::boqueqddrfbjkoxismbcn() {
	double kryhglybbnfcad = 16861;
	string utxqfkzlvpoq = "b";
	bool dmemfjihm = true;
	int flzkpcolnnav = 2667;
	if (true != true) {
		int fsoky;
		for (fsoky = 75; fsoky > 0; fsoky--) {
			continue;
		}
	}
	if (true == true) {
		int thvkowoob;
		for (thvkowoob = 11; thvkowoob > 0; thvkowoob--) {
			continue;
		}
	}
	if (string("b") != string("b")) {
		int cowtewk;
		for (cowtewk = 2; cowtewk > 0; cowtewk--) {
			continue;
		}
	}
	return true;
}

xssitel::xssitel() {
	this->iwopboafphwa(string("zlsuftbmjanpzorzugzzs"), 6478, 2771, string("vytsoefuxzsmlqgbcuaprctzauyzbgrqwsytmzjxczwfnzruxjmzvlwsssxazqjliefthfxpnfuax"), true);
	this->wstvnspalcsjgmvwaqonsi(string("kkqmlhelruzprwyjijxifnsvqigncozvbeapnquxyiap"), 2749, false, string("sekptpbspzhotghsreifezcgzmfamoxnsqwkoxpxpgaxdlujraidyllnnhcfvigslbcprgpnhrubsyxounzw"), string("rleqlmztguhamacve"), 3, 1050, string("zlkuqlticytjtjdrdjzseuidjkaposey"), 9849, true);
	this->sgxadvyhgzuhwa(2004, 8295, string("yqbxdphbtbfrqyoceyzqwlhydgyjshtiedtyowzemkfimrprsmmhlmzrowrqgohiycxnafqtpwjgkxibtaazprvpfqeycrtrowb"), string(""), string("zpeimyaxtztjqtlbzuerqjwhlkpbueb"));
	this->fekzlmliwwjdecmplevjdg(11977, 1454, 3978);
	this->qlertzdiniksn(2822, string("vbmbwyxyidsrbsdfiiafshjzdt"), string("cpftkzdie"), true, 33671, 24448, 1444, string("ugdzleqombwtygnaefrkejuljmkwoqxfatnzhc"));
	this->zsuyrkhuzmjtjwosdywbn(3136, string("fjvprypnusorynmkycwrvkdxceuwpavwvqquhpfyeyeaskufpkznrysn"), 1910, string("fdrkctlyiijbblotcubpvrsfyazlupbjwsvfda"), 1318, false, 23518, false, 1445);
	this->rxhpenxtywnrzcckjriibpza(12421, string("cahvqmejzl"), 5592, 7742, false, true, 1850, string("rmgdealsqzeqjaqwwxwbducwpqysspxtqvuejqdkjplcvgicmwphbbmiijkhmnzmffby"), string("vt"));
	this->jzcqdjfnvggfrvqjlmyivlsil(5807, 7065, 1268, 13649, 1002, 4504, 23868, string("rqgjrkbbqdybcpmdcryqfxkeyg"), 19224);
	this->kejgmojvei(2385, 2550, 156, 71799, 36998);
	this->boqueqddrfbjkoxismbcn();
	this->wvdkoaneinbae();
	this->gkpwpfujpnrvrm(true);
	this->hosfqcdsogdqivrrks(false, string("lxvpruchvnwat"), false, string("jeezlwdhpvjxrucijqrpnjnevv"), 19346, 43519, 3570, string("anwmoaqlhfmotfjyhhyopjwpdcqxlhqaejdksnh"), 2710, false);
	this->tlqbzphzupfsrdnhgj(string("vcsuymlbykhxiw"), 2229, 53663);
	this->qrwglqkpczjzemhfku(40010, string("bxgul"), string("boqdavkrmuvfhhyuvp"), string("xtojcdmedczapwvumlkxxfutzdsxwhvrcdfzuqhgtengjwowzechphraayyyflecxkyjzqvsrmrrykltkd"), string("mpmgzauaesryeyiceqlldnepccfeqmupnqsbk"));
	this->nkjpfsfoezkxxpx(2208);
	this->ytztxjrnylcxsilsxe(false, string("tkjrnbctghzodsdqqgkcnsxusmrnfvttcjhlkwrmodtmqxuldqxkgkyhamlghpjelxrwyrwkjrkjyoldoigva"), string("bbnousdhrsvzht"), 16769, string("pxgmgeqxjveqmafnkrxidalcvpbwblospwjivkvqtgrtrsfkokwciiiboisoadppmhwphhszgemxnt"), true, string("hajeymycxdurpfhwdexiwsvfpkrsfkisdzeccagre"), string("vlspdzunjgkniqeyjghndvfdgp"));
	this->tevbkdsmon(3243, 78907, 3358, 2889, true);
	this->dosuexpvkpwfoleakghslmc(false, string("owcuzgwkefresdkztqzioiiavtzfpwitvxtklcrhqzkmvarytuqmb"));
	this->egyuqvnkusdeacyalagwyx(8419, 796, true, false, 4029, 7365, 62066, string("vfnyvzkixwtclidhvwrdwtad"), 695);
	this->fgdhrcckihobccqgnoitrjpb(23518, 11410, 8287, false);
	this->kctajwlzlwevsnaraleg();
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class fnyskvz {
public:
	string dyfejq;
	bool poiefhzhgx;
	string zicnaqsg;
	bool twmgcckouaxcevs;
	int jeugcatdwnaos;
	fnyskvz();
	double sfbvfuhmzhdry(int kbeuq, string ezvfdnovia, int akhurwjevfyevlz, double abpvxszsvxccx, int seimoiizmfxc, int fxffisldyicpvm, double wevjimyhdr, double ageqxpwcosqfbys);
	string jjivvwgboaappg(string uizxaxcllbuxtlk);
	string mpjualgtlcrffvpsuohn(string kibswwfnwjkps, string afobgt, int hvzyxltx, string ddgxeolngwnseg, double rovpqkky);
	int xsbxqyjbrctvmgmh(double gnzvijqdxfwjmhy, bool elwxh, int vxatuoryzmyfai, int rrmqyhnlolpi, bool pcymmg, double psksponz, bool pwnqdprqmmutp, double qcuhrs);
	bool pmejtesmsxkdjrl(int kstmzbzxeqilpc, bool yuglievkeqv, double uimusc, int rtnmauzh, double otjceftcoecicw, string nwrlfbbc, int lriugvxsjyomm, string ahtvvmstywt);
	int gffgrenyvnwax(int jcffehvgoi, string pxbbqb, bool wxpsmof, double vjsysjes);
	string sffvxvqybzylxihpzmm(int bwkvqsw, double dzquronnqb, bool gpkdrxkrvzf);
	double toouqzruohpjhgrkvdbjirwg(string menimeiosby, double mogectsp, int avwmhcvgirbrf, double psyiaqydiu, bool iwpfodqhaoac, bool jyrqnnnrqfbpg, string rfqccpezjfvresy, string nwhudsbsxbpbvo, string ohfssbd);
	double tmijpdvnxsb(string rxnpbddytokqbg, string qzagqzby, string wynlyegvas, string sxkltunykj, string lpnxv, int jnczer);
	void ddlalzynpsrsraglpd(bool aopioxxwyxrh);

protected:
	int nxjchvqnaigzowi;
	bool gqbiokdmvzdh;
	string luoiqtp;
	string rvloket;

	double yzetenifixvorbsgah();
	void blhaipuwiustgonyyfizgnt(int oseijg, int isyozoe, double bbqsecojqegotwj, double jcvqqujjrw, int vavcqegyzjzlsp);
	int rdvjmqmlrkqcvvcheoh(bool civihwp, int rbpqnhiqcfeoo, bool opidoocbg, string nblwxgpzn, string tdgprybceidefrc);
	string ghsvyzylbygphoylumdpzl(int abqhtbjpkuofody, bool poketfgvo, int eefyvarmzsycs, double crkjmemhrk, bool wddaqtcpvixubyw, bool gplpyojixufvb);
	string jnznrjiszq(bool soqhro, int qeoxuzrw);
	bool mxsphdftmaopxbock(string ywomkzxu, double itybdvgwpgaeewa, double mflhggkpbzrtp, double bdylsadjmugctjo, double uhqyltsu, bool nlkuijysbymtg, double zhjevibykgmuz, int yghheymajsqdwz);
	int yziewyzaldmqvdpfivcudhn(bool rxvvpnovj, int qwjnivd);
	int xjruncxbwrmsqmmpjkjexzbdi(bool qlhtfilw, bool mbmkrix, int eexjmdzftffgs, int lloje);

private:
	double mktzytum;

	double fpoztfzfxdplu(string qvdwbjuuumqwoo, double kdkmybw);
	double uxidhdknfhhvwylymfvhnjqvs(double oidwybfchsi, double upgsbmmeekv, bool dpjvp, double ejppirtbrsolxiq, string kdotpmwwa, double rtszhbzugc, string dhdqiiccsmqfiv, bool tspttscbryup, string itowwngoygic);
	string ylqrvmeqvqmqqgogm(int nhercggrva, string hsioddgrzluyrxd, bool ojxdlhgh, string blcuduytf);
	string itmevsabtvlyktsfhzpw(bool emwpsolqxljna, double oeucfuhwbe);
	int dgethljajiztq();
	int yjhwiymnauyxcbzzaso(int luaojgehdcy, string xokwbh, double rzmwcwrfoxtfj, bool rzyfqnrxvfr, bool qhxggoqmnqcremq, double hbxdw, double tkxfq, double xaodbsgnon, bool iuvei);
	int dqrxyiroylljmbgllr(int cayjtxfztq, int kqlwb, double xqkydyso, int uofzj);
	int xknavyqxzmsqtzrzozjwk(int jfuldk, bool gndgp, int jrumi, double dyclmrlg, bool fgwvy, int ybehsrzvzxq, double uspmtuflpfss, int rnilaxsllgjwaj, double bdnwvm, string vtqbl);
	string qpjvykqyalxgeofug();
	bool eekrikztizrhrova(string duddovmnkl, double vrmfcxcmqtc, double stxafqesnoyqer, bool ktkzntf, int fxpzd, double wdbtcet, string qffsuhjytuttg);

};


double fnyskvz::fpoztfzfxdplu(string qvdwbjuuumqwoo, double kdkmybw) {
	int bqxxazofpippofq = 2094;
	int jyzwniiyozdq = 1571;
	bool gvgszkb = true;
	string zyjjkrymlzfjxlw = "vroaxkuiqgrkzpfprrcbtwdrrxhwoestfdqiordpdsozjhaegwvuccbtlzsgtnbcllgx";
	int aimuetcyc = 326;
	bool ybmow = false;
	double camgvstxos = 62964;
	string hemmfojt = "aswkulblwrpdhqgfsqkpmjpjbkcccwfcbvnuybrveybxmxgvrjcdbjrhi";
	string ppusipkwegccw = "ufid";
	if (true == true) {
		int gzzcqlhml;
		for (gzzcqlhml = 27; gzzcqlhml > 0; gzzcqlhml--) {
			continue;
		}
	}
	if (326 != 326) {
		int hxr;
		for (hxr = 55; hxr > 0; hxr--) {
			continue;
		}
	}
	if (string("ufid") != string("ufid")) {
		int ucoglt;
		for (ucoglt = 8; ucoglt > 0; ucoglt--) {
			continue;
		}
	}
	if (string("ufid") == string("ufid")) {
		int qaeonhvq;
		for (qaeonhvq = 79; qaeonhvq > 0; qaeonhvq--) {
			continue;
		}
	}
	return 38279;
}

double fnyskvz::uxidhdknfhhvwylymfvhnjqvs(double oidwybfchsi, double upgsbmmeekv, bool dpjvp, double ejppirtbrsolxiq, string kdotpmwwa, double rtszhbzugc, string dhdqiiccsmqfiv, bool tspttscbryup, string itowwngoygic) {
	int wfswfgtfhaa = 820;
	string uuniujeqglxy = "yys";
	bool jomvcfg = false;
	if (false == false) {
		int wji;
		for (wji = 75; wji > 0; wji--) {
			continue;
		}
	}
	if (820 != 820) {
		int itrant;
		for (itrant = 97; itrant > 0; itrant--) {
			continue;
		}
	}
	if (string("yys") != string("yys")) {
		int gacn;
		for (gacn = 30; gacn > 0; gacn--) {
			continue;
		}
	}
	return 83149;
}

string fnyskvz::ylqrvmeqvqmqqgogm(int nhercggrva, string hsioddgrzluyrxd, bool ojxdlhgh, string blcuduytf) {
	string ggbdduyaqztx = "lixrdytqujsytfacocgvevsho";
	bool iuwaaiceyv = false;
	string flntm = "cdewyjxowjynskufglamvkynlowvsasxwrxsmkcboeygiaxyzvwxbkskotbgklafiaowlpghbuzwfmeswuqkq";
	if (false == false) {
		int sym;
		for (sym = 52; sym > 0; sym--) {
			continue;
		}
	}
	if (false == false) {
		int gc;
		for (gc = 5; gc > 0; gc--) {
			continue;
		}
	}
	if (string("cdewyjxowjynskufglamvkynlowvsasxwrxsmkcboeygiaxyzvwxbkskotbgklafiaowlpghbuzwfmeswuqkq") == string("cdewyjxowjynskufglamvkynlowvsasxwrxsmkcboeygiaxyzvwxbkskotbgklafiaowlpghbuzwfmeswuqkq")) {
		int ugp;
		for (ugp = 48; ugp > 0; ugp--) {
			continue;
		}
	}
	if (string("cdewyjxowjynskufglamvkynlowvsasxwrxsmkcboeygiaxyzvwxbkskotbgklafiaowlpghbuzwfmeswuqkq") != string("cdewyjxowjynskufglamvkynlowvsasxwrxsmkcboeygiaxyzvwxbkskotbgklafiaowlpghbuzwfmeswuqkq")) {
		int agc;
		for (agc = 30; agc > 0; agc--) {
			continue;
		}
	}
	if (false == false) {
		int yedxcukc;
		for (yedxcukc = 100; yedxcukc > 0; yedxcukc--) {
			continue;
		}
	}
	return string("tspxlkpnkmzjzzj");
}

string fnyskvz::itmevsabtvlyktsfhzpw(bool emwpsolqxljna, double oeucfuhwbe) {
	bool dqvtisstww = false;
	int pavcwijlzr = 4550;
	if (4550 != 4550) {
		int wexmuwqsg;
		for (wexmuwqsg = 16; wexmuwqsg > 0; wexmuwqsg--) {
			continue;
		}
	}
	return string("xflemmsvizwlokpfekty");
}

int fnyskvz::dgethljajiztq() {
	double zvzcwiypqw = 14598;
	if (14598 == 14598) {
		int cctbxtzg;
		for (cctbxtzg = 40; cctbxtzg > 0; cctbxtzg--) {
			continue;
		}
	}
	return 11936;
}

int fnyskvz::yjhwiymnauyxcbzzaso(int luaojgehdcy, string xokwbh, double rzmwcwrfoxtfj, bool rzyfqnrxvfr, bool qhxggoqmnqcremq, double hbxdw, double tkxfq, double xaodbsgnon, bool iuvei) {
	double mivjtrhkehgx = 52351;
	bool oxzia = true;
	if (52351 == 52351) {
		int usogvoefl;
		for (usogvoefl = 20; usogvoefl > 0; usogvoefl--) {
			continue;
		}
	}
	if (52351 == 52351) {
		int bo;
		for (bo = 87; bo > 0; bo--) {
			continue;
		}
	}
	if (true == true) {
		int tvspbx;
		for (tvspbx = 84; tvspbx > 0; tvspbx--) {
			continue;
		}
	}
	if (true != true) {
		int ylkdhd;
		for (ylkdhd = 85; ylkdhd > 0; ylkdhd--) {
			continue;
		}
	}
	if (true == true) {
		int ysvoqjkddi;
		for (ysvoqjkddi = 5; ysvoqjkddi > 0; ysvoqjkddi--) {
			continue;
		}
	}
	return 27491;
}

int fnyskvz::dqrxyiroylljmbgllr(int cayjtxfztq, int kqlwb, double xqkydyso, int uofzj) {
	bool oyuawnwokianabd = true;
	int spqguyskwxhc = 1600;
	int aioglxoncze = 5437;
	int qienze = 3840;
	bool szshaukq = false;
	bool hsjoskmbywupdwr = false;
	if (true != true) {
		int kthpoeakf;
		for (kthpoeakf = 44; kthpoeakf > 0; kthpoeakf--) {
			continue;
		}
	}
	return 5978;
}

int fnyskvz::xknavyqxzmsqtzrzozjwk(int jfuldk, bool gndgp, int jrumi, double dyclmrlg, bool fgwvy, int ybehsrzvzxq, double uspmtuflpfss, int rnilaxsllgjwaj, double bdnwvm, string vtqbl) {
	return 19654;
}

string fnyskvz::qpjvykqyalxgeofug() {
	bool caccnpfzfuueprs = true;
	bool ozihdhzhibqd = false;
	bool dxzgvujejssy = false;
	string smkkmvhjcyh = "islj";
	string ivsccbbyx = "nogfrkamcmrgzborgfmumumbwxuwkouxbgnwzcfzkcglrvunrgfoknfazgqdndbovkhcsjdkgpusvkyeepmklyvdwctztc";
	int kdzmgufdzlxrifc = 2061;
	bool qgnjfqcxk = false;
	bool jgafmdcwauvj = true;
	bool rxjjpgabedakx = true;
	int zdxdurnae = 5891;
	if (false == false) {
		int mucxzfppzo;
		for (mucxzfppzo = 46; mucxzfppzo > 0; mucxzfppzo--) {
			continue;
		}
	}
	if (2061 != 2061) {
		int awlgtnib;
		for (awlgtnib = 48; awlgtnib > 0; awlgtnib--) {
			continue;
		}
	}
	if (true == true) {
		int dj;
		for (dj = 77; dj > 0; dj--) {
			continue;
		}
	}
	if (true != true) {
		int tesz;
		for (tesz = 33; tesz > 0; tesz--) {
			continue;
		}
	}
	if (5891 != 5891) {
		int vyw;
		for (vyw = 31; vyw > 0; vyw--) {
			continue;
		}
	}
	return string("");
}

bool fnyskvz::eekrikztizrhrova(string duddovmnkl, double vrmfcxcmqtc, double stxafqesnoyqer, bool ktkzntf, int fxpzd, double wdbtcet, string qffsuhjytuttg) {
	int bvierxrme = 1872;
	string pupntdbzdetl = "rwdbhyxkcgge";
	double barfplnalvfebzi = 44931;
	if (string("rwdbhyxkcgge") == string("rwdbhyxkcgge")) {
		int tvmap;
		for (tvmap = 18; tvmap > 0; tvmap--) {
			continue;
		}
	}
	if (string("rwdbhyxkcgge") != string("rwdbhyxkcgge")) {
		int ixfmhxo;
		for (ixfmhxo = 46; ixfmhxo > 0; ixfmhxo--) {
			continue;
		}
	}
	if (string("rwdbhyxkcgge") == string("rwdbhyxkcgge")) {
		int qyioyzvhy;
		for (qyioyzvhy = 10; qyioyzvhy > 0; qyioyzvhy--) {
			continue;
		}
	}
	if (string("rwdbhyxkcgge") == string("rwdbhyxkcgge")) {
		int vvbchf;
		for (vvbchf = 29; vvbchf > 0; vvbchf--) {
			continue;
		}
	}
	return true;
}

double fnyskvz::yzetenifixvorbsgah() {
	return 77879;
}

void fnyskvz::blhaipuwiustgonyyfizgnt(int oseijg, int isyozoe, double bbqsecojqegotwj, double jcvqqujjrw, int vavcqegyzjzlsp) {
	string iwlkgmqloagt = "dmexrw";
	bool jbvpkbsjqg = false;
	if (string("dmexrw") == string("dmexrw")) {
		int gj;
		for (gj = 67; gj > 0; gj--) {
			continue;
		}
	}
	if (string("dmexrw") != string("dmexrw")) {
		int ahqabliwqj;
		for (ahqabliwqj = 46; ahqabliwqj > 0; ahqabliwqj--) {
			continue;
		}
	}
	if (false == false) {
		int mbbqc;
		for (mbbqc = 77; mbbqc > 0; mbbqc--) {
			continue;
		}
	}

}

int fnyskvz::rdvjmqmlrkqcvvcheoh(bool civihwp, int rbpqnhiqcfeoo, bool opidoocbg, string nblwxgpzn, string tdgprybceidefrc) {
	int xfzxdgyi = 519;
	int qtbueotsxkreucz = 406;
	string gbssbznalrgc = "wukjpkyybfwlhqhbedmdewvchipnrshwwxewiogap";
	int sjedtnphjjt = 1925;
	bool pmxqsgjzbup = false;
	bool pekxbzsadqpgus = true;
	if (406 != 406) {
		int wx;
		for (wx = 36; wx > 0; wx--) {
			continue;
		}
	}
	if (519 == 519) {
		int ij;
		for (ij = 42; ij > 0; ij--) {
			continue;
		}
	}
	if (string("wukjpkyybfwlhqhbedmdewvchipnrshwwxewiogap") == string("wukjpkyybfwlhqhbedmdewvchipnrshwwxewiogap")) {
		int ov;
		for (ov = 19; ov > 0; ov--) {
			continue;
		}
	}
	if (1925 != 1925) {
		int nsf;
		for (nsf = 9; nsf > 0; nsf--) {
			continue;
		}
	}
	return 13339;
}

string fnyskvz::ghsvyzylbygphoylumdpzl(int abqhtbjpkuofody, bool poketfgvo, int eefyvarmzsycs, double crkjmemhrk, bool wddaqtcpvixubyw, bool gplpyojixufvb) {
	string yvrqytpiofozqnq = "tpzfeiavnzeztddrhvdhacdvmthgqfmamxkjvasfjpikdarmxmwmyulrjtydscpvqzdagcmqakmkdhfikyrwryvzrmchmkuohnet";
	return string("ioeocfldayepihd");
}

string fnyskvz::jnznrjiszq(bool soqhro, int qeoxuzrw) {
	string qphddvnwunryadd = "bmjddjihpoylgqipeodvdvnnnxputzejimwtpjwuipieonwunqqudtygukfltzopfcmtnfciaetgmrdfafqwfwwy";
	int yrqhcbrcdhw = 4396;
	double vjouamj = 6814;
	int jreapaflyoroi = 553;
	string tksxkrhb = "axyviogxfqfvwjrcbqzudmdishohlblyvwuuyib";
	int hknmal = 1365;
	if (6814 != 6814) {
		int olxejek;
		for (olxejek = 66; olxejek > 0; olxejek--) {
			continue;
		}
	}
	if (string("bmjddjihpoylgqipeodvdvnnnxputzejimwtpjwuipieonwunqqudtygukfltzopfcmtnfciaetgmrdfafqwfwwy") != string("bmjddjihpoylgqipeodvdvnnnxputzejimwtpjwuipieonwunqqudtygukfltzopfcmtnfciaetgmrdfafqwfwwy")) {
		int xlag;
		for (xlag = 12; xlag > 0; xlag--) {
			continue;
		}
	}
	if (string("axyviogxfqfvwjrcbqzudmdishohlblyvwuuyib") != string("axyviogxfqfvwjrcbqzudmdishohlblyvwuuyib")) {
		int vffxaqm;
		for (vffxaqm = 99; vffxaqm > 0; vffxaqm--) {
			continue;
		}
	}
	return string("tkp");
}

bool fnyskvz::mxsphdftmaopxbock(string ywomkzxu, double itybdvgwpgaeewa, double mflhggkpbzrtp, double bdylsadjmugctjo, double uhqyltsu, bool nlkuijysbymtg, double zhjevibykgmuz, int yghheymajsqdwz) {
	string kpvdcmwbge = "pgleirvucoelkqxnzozvllmaqgchazfukohuhwdfzkjlc";
	double mrvwctz = 1149;
	bool qpqnkemwl = false;
	int ngtkf = 4331;
	string cnpebmh = "ddytxfqq";
	bool jdqdw = false;
	bool etmqrvzgwq = false;
	if (false != false) {
		int lignwe;
		for (lignwe = 52; lignwe > 0; lignwe--) {
			continue;
		}
	}
	if (false != false) {
		int mo;
		for (mo = 71; mo > 0; mo--) {
			continue;
		}
	}
	if (false == false) {
		int djix;
		for (djix = 75; djix > 0; djix--) {
			continue;
		}
	}
	if (false != false) {
		int hkpyl;
		for (hkpyl = 39; hkpyl > 0; hkpyl--) {
			continue;
		}
	}
	if (false == false) {
		int hukshbvzt;
		for (hukshbvzt = 100; hukshbvzt > 0; hukshbvzt--) {
			continue;
		}
	}
	return false;
}

int fnyskvz::yziewyzaldmqvdpfivcudhn(bool rxvvpnovj, int qwjnivd) {
	double lihjrrgcr = 4239;
	string aztdlr = "ezbqxhgybnykgejohtqzyqwfgo";
	if (4239 != 4239) {
		int ulegikjwmo;
		for (ulegikjwmo = 71; ulegikjwmo > 0; ulegikjwmo--) {
			continue;
		}
	}
	if (4239 == 4239) {
		int zyblkuge;
		for (zyblkuge = 47; zyblkuge > 0; zyblkuge--) {
			continue;
		}
	}
	if (4239 != 4239) {
		int vxvuezsxoc;
		for (vxvuezsxoc = 90; vxvuezsxoc > 0; vxvuezsxoc--) {
			continue;
		}
	}
	return 61915;
}

int fnyskvz::xjruncxbwrmsqmmpjkjexzbdi(bool qlhtfilw, bool mbmkrix, int eexjmdzftffgs, int lloje) {
	int avhhugeeya = 190;
	bool nqezx = true;
	if (190 != 190) {
		int gubvkgb;
		for (gubvkgb = 61; gubvkgb > 0; gubvkgb--) {
			continue;
		}
	}
	return 78902;
}

double fnyskvz::sfbvfuhmzhdry(int kbeuq, string ezvfdnovia, int akhurwjevfyevlz, double abpvxszsvxccx, int seimoiizmfxc, int fxffisldyicpvm, double wevjimyhdr, double ageqxpwcosqfbys) {
	int afadhknu = 742;
	int sfuui = 7953;
	int liegvymw = 4112;
	bool zuouuofuqm = false;
	bool rdftjbobk = false;
	bool kdqfshdgqsgiyea = false;
	double vwjhcezzdx = 39404;
	if (false == false) {
		int rbuqgden;
		for (rbuqgden = 1; rbuqgden > 0; rbuqgden--) {
			continue;
		}
	}
	if (7953 != 7953) {
		int smdgefx;
		for (smdgefx = 18; smdgefx > 0; smdgefx--) {
			continue;
		}
	}
	if (4112 == 4112) {
		int yuykhkxvc;
		for (yuykhkxvc = 62; yuykhkxvc > 0; yuykhkxvc--) {
			continue;
		}
	}
	return 49772;
}

string fnyskvz::jjivvwgboaappg(string uizxaxcllbuxtlk) {
	return string("yrtmbvrssmtwcvzam");
}

string fnyskvz::mpjualgtlcrffvpsuohn(string kibswwfnwjkps, string afobgt, int hvzyxltx, string ddgxeolngwnseg, double rovpqkky) {
	string equqph = "oomkrdovzvxoyuvarogjq";
	double xmgfnqg = 34881;
	int pwdktzupgswdsiv = 2532;
	string bexjyihz = "hwoyolatkjskihrxnsognrgbwcdemkymmpbywltgxhwhndecaztahmxzaannztnttxzlzbpkdxuibgtgqjwwgrmpq";
	bool xdvuvykc = true;
	int uxrbsycnqxjszio = 4432;
	string separqif = "dhzyagontgyqzopoluaatsgfbhwhazkzdqfkkfxputjgjktorssmkbx";
	if (4432 != 4432) {
		int cohyzf;
		for (cohyzf = 11; cohyzf > 0; cohyzf--) {
			continue;
		}
	}
	if (2532 != 2532) {
		int dcrckvxv;
		for (dcrckvxv = 3; dcrckvxv > 0; dcrckvxv--) {
			continue;
		}
	}
	if (string("dhzyagontgyqzopoluaatsgfbhwhazkzdqfkkfxputjgjktorssmkbx") != string("dhzyagontgyqzopoluaatsgfbhwhazkzdqfkkfxputjgjktorssmkbx")) {
		int qfnighenw;
		for (qfnighenw = 9; qfnighenw > 0; qfnighenw--) {
			continue;
		}
	}
	return string("ywrbnqmovx");
}

int fnyskvz::xsbxqyjbrctvmgmh(double gnzvijqdxfwjmhy, bool elwxh, int vxatuoryzmyfai, int rrmqyhnlolpi, bool pcymmg, double psksponz, bool pwnqdprqmmutp, double qcuhrs) {
	return 5514;
}

bool fnyskvz::pmejtesmsxkdjrl(int kstmzbzxeqilpc, bool yuglievkeqv, double uimusc, int rtnmauzh, double otjceftcoecicw, string nwrlfbbc, int lriugvxsjyomm, string ahtvvmstywt) {
	double ycnovqwgmiu = 32290;
	int gvyrehyrbleg = 5226;
	int zdgkpq = 526;
	double dconssodpfayhvz = 22742;
	double omcvmfwutz = 31669;
	string ayliadklve = "lgqawdjwbmafyjynsntxfeocqaewszjgcwzeeljepkdfquccmbppkvjujvisujke";
	if (5226 != 5226) {
		int ijjjob;
		for (ijjjob = 32; ijjjob > 0; ijjjob--) {
			continue;
		}
	}
	if (32290 != 32290) {
		int xnevmgpphf;
		for (xnevmgpphf = 51; xnevmgpphf > 0; xnevmgpphf--) {
			continue;
		}
	}
	if (31669 != 31669) {
		int vnyp;
		for (vnyp = 41; vnyp > 0; vnyp--) {
			continue;
		}
	}
	return false;
}

int fnyskvz::gffgrenyvnwax(int jcffehvgoi, string pxbbqb, bool wxpsmof, double vjsysjes) {
	double lphikyrwm = 10412;
	double ijhqpvq = 4840;
	int rfadb = 5374;
	string ptjjlohrwyw = "slvxxidmrpubmfompfcrbznyxfwkzqpybywbrhqskfiwbtckbjixmcitjpvultpguyluf";
	double kcrmczijklowq = 13279;
	double qnkaybx = 10650;
	bool dissaaadfc = true;
	int nmebfzn = 659;
	bool cwsluzxsanjo = true;
	if (true != true) {
		int ltj;
		for (ltj = 10; ltj > 0; ltj--) {
			continue;
		}
	}
	if (4840 != 4840) {
		int gapmffs;
		for (gapmffs = 7; gapmffs > 0; gapmffs--) {
			continue;
		}
	}
	if (10650 == 10650) {
		int ztrkcpjwa;
		for (ztrkcpjwa = 86; ztrkcpjwa > 0; ztrkcpjwa--) {
			continue;
		}
	}
	if (5374 != 5374) {
		int vwxp;
		for (vwxp = 0; vwxp > 0; vwxp--) {
			continue;
		}
	}
	if (true == true) {
		int yczrkkqet;
		for (yczrkkqet = 21; yczrkkqet > 0; yczrkkqet--) {
			continue;
		}
	}
	return 62639;
}

string fnyskvz::sffvxvqybzylxihpzmm(int bwkvqsw, double dzquronnqb, bool gpkdrxkrvzf) {
	double cjigymh = 29070;
	bool cjwoiv = true;
	bool dbvrllnecr = true;
	double ihftdnfgi = 80575;
	bool uxrljnhare = false;
	int jjkiepxge = 3510;
	if (true != true) {
		int lazt;
		for (lazt = 44; lazt > 0; lazt--) {
			continue;
		}
	}
	if (true == true) {
		int nxxam;
		for (nxxam = 0; nxxam > 0; nxxam--) {
			continue;
		}
	}
	if (true != true) {
		int kurz;
		for (kurz = 30; kurz > 0; kurz--) {
			continue;
		}
	}
	if (true == true) {
		int gz;
		for (gz = 32; gz > 0; gz--) {
			continue;
		}
	}
	return string("coke");
}

double fnyskvz::toouqzruohpjhgrkvdbjirwg(string menimeiosby, double mogectsp, int avwmhcvgirbrf, double psyiaqydiu, bool iwpfodqhaoac, bool jyrqnnnrqfbpg, string rfqccpezjfvresy, string nwhudsbsxbpbvo, string ohfssbd) {
	bool rctxbb = false;
	bool zdsqjvufnj = true;
	string rtrdae = "qpmmwdjwvtdifsocuvyperecadxmkdjxuabxtfxefseeerzpodqgbznyqwxijxblggopvcgftokqyfnhslbvxzrhs";
	string mcxfxzqygcz = "blivkfqsehvmkyxeeooetfkhcobmgcdzmhf";
	string xhfzdysbswg = "hyzmikaoqpusrcwhgdiljiegadjvmwuh";
	bool ktqipm = true;
	double mqmqa = 25458;
	string mrkvixygrnbfe = "pwapgpqtqsoydkxaamleixekpfyazhbbsyyehecrgahbiktvnuilqzzoiaovodohxllvjnxmyuytrqlepdrjzyapssvttoezwixm";
	int xzhfzmorwgwjzz = 1655;
	bool agdwdinwpicdtoq = false;
	if (25458 != 25458) {
		int ra;
		for (ra = 50; ra > 0; ra--) {
			continue;
		}
	}
	if (true == true) {
		int amtgajym;
		for (amtgajym = 67; amtgajym > 0; amtgajym--) {
			continue;
		}
	}
	return 81197;
}

double fnyskvz::tmijpdvnxsb(string rxnpbddytokqbg, string qzagqzby, string wynlyegvas, string sxkltunykj, string lpnxv, int jnczer) {
	string skuhensi = "nzwqfgzytlgnkntgequbxqgtqgupdsvhfngqthqfpekudvdxryfcevkwbqsmrxxp";
	string jbaqye = "oauueyejfhehyynqnikpcidivjjbdzf";
	int wjhwkocnnwzud = 55;
	string gkhesjnbqypgaor = "emtu";
	double gzlrwagyedblaz = 11386;
	int evfxgrp = 3225;
	int qokcgigxgdljd = 2181;
	double wbcivfxckwbhwpd = 67482;
	int nivqlvouxyywv = 6533;
	string fixxtpxtiyumd = "xrufdctosyvtbioryfrqbtjfuqohabavmowgijrdgoakdopavvuhxinnsdmjguaqpkxqhysvkblnzzdugkko";
	if (string("nzwqfgzytlgnkntgequbxqgtqgupdsvhfngqthqfpekudvdxryfcevkwbqsmrxxp") != string("nzwqfgzytlgnkntgequbxqgtqgupdsvhfngqthqfpekudvdxryfcevkwbqsmrxxp")) {
		int lslo;
		for (lslo = 66; lslo > 0; lslo--) {
			continue;
		}
	}
	if (11386 != 11386) {
		int tgkkcof;
		for (tgkkcof = 8; tgkkcof > 0; tgkkcof--) {
			continue;
		}
	}
	return 83825;
}

void fnyskvz::ddlalzynpsrsraglpd(bool aopioxxwyxrh) {
	bool yaartheyh = true;
	string qizzrbk = "ulcetprelvwccjmzuygspqsufeknrhyahpnovaszcwzsrfuesrutv";
	int itieuheouiciu = 5977;
	double lxvtmwjwxwncp = 78694;
	string nndiugjhlivanhz = "qeuunaagozqbrxqfeeeryiwmvhfrknaeicctgfdpqoekhrwqpfuvvpnsgeckhgzqdloimimbcbznxlmdsv";
	int dgtdywn = 6307;
	string jgubbgaoc = "qpcwasnkuslflhcugjfcmcopofwtveioohjkfswjhlvlvsudkvpgnorw";
	bool mgmhv = false;
	int erwztpaple = 2272;

}

fnyskvz::fnyskvz() {
	this->sfbvfuhmzhdry(202, string("sprpkbhjlv"), 411, 30457, 2036, 3971, 39070, 22116);
	this->jjivvwgboaappg(string("jlspyhqjndfcgkhymjafsibkxjkwyzdkqtyjotqeykpquxcmbkydejb"));
	this->mpjualgtlcrffvpsuohn(string("ihvvisgzmhtpqfwdpopfsqpfttbdhic"), string("gpuuxompixmczdovutjsczuuzumjhslolwqellafexhsxasnzixthgfdihxpmqohovflvdggehogdu"), 4209, string("gqliyxlxvscramjxbfunbocoklmlyjqruzywxhkamdnoxogxzmgkxprczerxxpbvaseeysoglnbc"), 11837);
	this->xsbxqyjbrctvmgmh(10919, false, 499, 1982, true, 83488, false, 37582);
	this->pmejtesmsxkdjrl(2390, true, 42915, 3730, 7686, string("tominxsgsmryoikzewxknspifmmvjgeuetkwecqphqsdhpsajuk"), 51, string("qzjijwbbitbfhadcigwfzangohjdcpwgdntmmecuxrfmlqzzo"));
	this->gffgrenyvnwax(2675, string("odclnnssjoxfzy"), false, 62692);
	this->sffvxvqybzylxihpzmm(3576, 18140, false);
	this->toouqzruohpjhgrkvdbjirwg(string("cihrtiwtgmwxspwhmzwfltldp"), 34728, 3251, 6846, true, false, string("lamabpklvwjxcvincafuhvzzzbuclryajjcvrwlzojwowbjvcfxmshcyiitjvoeaforimflmemmhlvymeefkoysbymdwevadycov"), string("kcxuxfvlonimwxkitgsmkkfjber"), string("xxtymkzcmybvjaoggvfszjksbddxerdcfs"));
	this->tmijpdvnxsb(string("ndbjbfpbgioyakgfopnqiuduxlyuvmmpgnqtjhtnhmthjrtjyieepdwen"), string("iidqpowaxtzuqitarherhyeybhniuazqkliqqbjihjxrblejitropnoqnijgpzpxdqmmlrnhsobsqohgc"), string("jhvsajolshayqtncxeflxgwccfdnxnpoopjfhwl"), string("dyroctrkiyhyjuypcuhutyfnzuaoksmviyruacb"), string("amxswmyxwljjrjvcskcknzrywnketjmcrkjeqqhmswlinwtciznqppikqusjegyjatpzoxysqfvfjzwdz"), 6693);
	this->ddlalzynpsrsraglpd(false);
	this->yzetenifixvorbsgah();
	this->blhaipuwiustgonyyfizgnt(1282, 2439, 6871, 37589, 1235);
	this->rdvjmqmlrkqcvvcheoh(false, 1248, true, string("zdrgwchyvnzfydbydykyolzmjtwzkuxwiczmsomswdburmqcqtcdmgj"), string("uayogjbsmuuimjlmixorphyxirrozqgudttflzhpygghcgbobvhslgxctxxtqsytbfaixscxxch"));
	this->ghsvyzylbygphoylumdpzl(3184, false, 338, 41734, false, false);
	this->jnznrjiszq(true, 975);
	this->mxsphdftmaopxbock(string("wlyxwqzsfqacueqoojrvpkbssgqvpwbiiufdqcxzmvyuumoevtgnjpenfmcbwodzemwjvtkjyfmphttsfplivjfvzgpyyr"), 4212, 14087, 2938, 6974, true, 11387, 382);
	this->yziewyzaldmqvdpfivcudhn(true, 4950);
	this->xjruncxbwrmsqmmpjkjexzbdi(false, false, 2373, 4486);
	this->fpoztfzfxdplu(string("osonbwkmfwucmsvtoaamffkmfmbmrodssuhc"), 26699);
	this->uxidhdknfhhvwylymfvhnjqvs(14724, 96458, true, 3173, string("kyiztroehbbnbdyyzgsnibftprjumzmupmjkcuafadjrdcftcgchotvhwxageyexacrwxbpsoekvtcx"), 11506, string("dypqqqcaactttwftzuktsjflajqolnbzrgzsdrslxjchwscbbcpdbsiikemfnk"), true, string("ztkdpwaggcfaqtpsltfbhimhnikegluxftycjedyfwuumtqzxmjdjmzhumdmjcymanemezbubtfxhuanffliqgkjhppq"));
	this->ylqrvmeqvqmqqgogm(615, string("axuwygvtkckrexinjkptpdqasmgxrgakgufkghnuydeerkgycukanizjjvticucszfducvmluupvetcqvpypcvrux"), true, string("vzjxsvfeqpbtklujyzflvcdktpvhzsesagstpamdxlnqpeocfkpfeusgsyigjiyjyajhfjyst"));
	this->itmevsabtvlyktsfhzpw(false, 29973);
	this->dgethljajiztq();
	this->yjhwiymnauyxcbzzaso(2948, string("kgotbbwuzckxchxjmayfihr"), 68384, true, false, 24850, 1002, 26356, false);
	this->dqrxyiroylljmbgllr(6111, 4191, 58246, 1276);
	this->xknavyqxzmsqtzrzozjwk(2743, true, 8456, 68546, false, 633, 6474, 379, 20031, string("dyvdlccwolvpe"));
	this->qpjvykqyalxgeofug();
	this->eekrikztizrhrova(string("nnwernkjnawgezadliyszqvfdfcxuaiaredtvzcjranxzqepkvqljfbzljfwhrjcpfeltcbnzvrbryheourofnzvxvoapsm"), 6121, 9526, false, 6308, 53609, string("aslztmrbwovczbacynongikshwmaeuagwuwbevexbqmvhmsydyamhzknjwividqwyrdhosvevcpywzyydritdmqhekys"));
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class zwsmcqq {
public:
	int pizlujh;
	int ggkfhfrdp;
	zwsmcqq();
	double aoiurgeemrbuw();
	string zazlhojefywcif(int cdxfelbduq, bool bmfhixpc, string ylymxbh, string yhafbf, double abzrottlgqc, double bvjzby);
	double ewbuadklnrbdax(bool tqgsffgngvag, int vwerqlfhbv, bool qiyaucb, string mjoiwhicaoy, string pxrxnool);
	void tlqvhcfyde(string hbrhgcjyzoonndk, string vuxyle, string wtjszcq, string hyasxnuzljchud);
	bool oovkmcgmfqzs(string hypcvyiiur, double cxofdnnnooncy, string sxhbqksixlk, int sjtjadvx, double wlqtacfh);
	bool hcjfhrbukxn(double obmbntdmj, double vsuluumsz, bool ntcqk, string ynddtuyvpgzfcwt, int wehgi);

protected:
	string uempvwxobw;
	bool kadmhceefejns;

	string ktcpjoeyhxkjzlpr(string gghcscilpq, int edbkibtep, int lkfmikowj, int fpggb, string xzfqhzwfkpkdmnb, string psfcyjwojfpogad, string fsmqaqfwjehzvbb, string ezjvanlbypkm);
	string rszznmmaucdiuuovqntpxdek(bool hvsbn, int sllnfqomdvdu, int sgckgenaqditdc, int aadnxcb, double yavwbohqezavjx, bool tifcpxvqbtqgkgu, int hhzeapy, string blqargsiinlsxrd, bool qxuhxh);
	double lopwipfmiytqxmhlbbch(string xbgigoyzra, bool iqjkbbqjnwabmr, double hsqsmueh, bool ewajq);
	string vlsqssfuxmlzykj(string anwhzcntxxakmz, int xjibc, string puqsbihb, double epvldloau, bool vvljlno);
	bool jtscjwfcekhffg(int lybxjkhkfvdqudy, int oaolqn);
	bool epkavrkjyicgyq(bool utkwjsmbtoqzll, int ihemxtberfqfeqv, string xvydndsuoixfvf, int hqvyhhaa, int svxciqgbnrgc, int fjfvupgrrtrusy, bool vxvcmul, double cfmnkp);
	int hmibfqtrpppfjmgoarjste(string rxyky, double bdxwyl, double ztvrzucmq);
	double hcclanafnoawutzeintry();
	void ybixswkodehsapvkjgiclien(bool snfvqcwookzrokf, bool griyjuawbigmaey, double lpozcxxkspf, int snpkwhjc, bool vfzlh, double zuomagxgvw, int qfjetldu, double zkytjzdjvf, double uaiovbryfgrw);

private:
	int cspffrccivrvkh;
	string isbvrqog;
	double lkcvhvud;
	bool bqdwvkymwyeohxz;

	double vbcmicdeoxhdnmcfm(bool lwsuwj, bool ifxigz, bool mjdlvymendgnxif, double ggngqhyhymc, string bvpzm, int arpfslvfqbklac, bool qulyedzcwpfsdrh, string vpjaushit, string ikdirgzhsvyzozd);
	void bhkcrhetiel(double oshogxskwiik, double cfaiipfqzecyb, string hjicvdh, string dryeqdewn);
	double qphpxlpwia(int cvfkuaf, int wuscfrp, bool zumqnd, string iomhbkqjz, string vjeubivmadoxoak, string bexkqnhgjwemy, bool qvinnr, string khyjjivzdxrwfq, bool xyaotqr, string pomyhrtlq);
	string cufxuocxzccyxugso(string aqacpkqsgfcdbb, string dzucoscn, double iqplpegb, string jhblsghetyyl, double ekoleuttdd, int zlcqb, double rrlfeqqcmfpop);
	void dpfufgzqyxqoprlhmjdkivtm(bool qtfavfvlpsoyz);
	int dnoaxybbjtmt(int vyulx, string ittqxgimnpyaqlv, string cvdmbrz, bool isiqwvjkt, string zfbziicneawcfqs, int bhsxny, double szbcauzptcfkwif, string gegvrmgfyvnkp, string varkipnjnjc, int vyltgiiwaowwpg);

};


double zwsmcqq::vbcmicdeoxhdnmcfm(bool lwsuwj, bool ifxigz, bool mjdlvymendgnxif, double ggngqhyhymc, string bvpzm, int arpfslvfqbklac, bool qulyedzcwpfsdrh, string vpjaushit, string ikdirgzhsvyzozd) {
	int qjqdmrarapgapg = 440;
	string lntvpk = "syvmjuabpfxlphvlyemotjtioliykohqccyypl";
	if (string("syvmjuabpfxlphvlyemotjtioliykohqccyypl") == string("syvmjuabpfxlphvlyemotjtioliykohqccyypl")) {
		int vhnccplfq;
		for (vhnccplfq = 54; vhnccplfq > 0; vhnccplfq--) {
			continue;
		}
	}
	if (string("syvmjuabpfxlphvlyemotjtioliykohqccyypl") != string("syvmjuabpfxlphvlyemotjtioliykohqccyypl")) {
		int sdyhotjwdw;
		for (sdyhotjwdw = 73; sdyhotjwdw > 0; sdyhotjwdw--) {
			continue;
		}
	}
	if (string("syvmjuabpfxlphvlyemotjtioliykohqccyypl") == string("syvmjuabpfxlphvlyemotjtioliykohqccyypl")) {
		int upuyaefinv;
		for (upuyaefinv = 21; upuyaefinv > 0; upuyaefinv--) {
			continue;
		}
	}
	if (string("syvmjuabpfxlphvlyemotjtioliykohqccyypl") == string("syvmjuabpfxlphvlyemotjtioliykohqccyypl")) {
		int xenvhobc;
		for (xenvhobc = 59; xenvhobc > 0; xenvhobc--) {
			continue;
		}
	}
	if (string("syvmjuabpfxlphvlyemotjtioliykohqccyypl") == string("syvmjuabpfxlphvlyemotjtioliykohqccyypl")) {
		int sylfkj;
		for (sylfkj = 97; sylfkj > 0; sylfkj--) {
			continue;
		}
	}
	return 68994;
}

void zwsmcqq::bhkcrhetiel(double oshogxskwiik, double cfaiipfqzecyb, string hjicvdh, string dryeqdewn) {
	double usudgpudikvx = 32118;
	string mcttadmi = "tftxoqzvhiyovbylsayxgasojwnizjjpunfdvegdfqiasrhfqzoaukfazykkredruwkxtiuwffrdxmhvozwpkeybearweepp";
	int rfvok = 3097;
	bool zchdlsws = false;
	if (32118 == 32118) {
		int ocbfnt;
		for (ocbfnt = 35; ocbfnt > 0; ocbfnt--) {
			continue;
		}
	}
	if (3097 == 3097) {
		int ap;
		for (ap = 76; ap > 0; ap--) {
			continue;
		}
	}
	if (3097 != 3097) {
		int zd;
		for (zd = 87; zd > 0; zd--) {
			continue;
		}
	}
	if (string("tftxoqzvhiyovbylsayxgasojwnizjjpunfdvegdfqiasrhfqzoaukfazykkredruwkxtiuwffrdxmhvozwpkeybearweepp") != string("tftxoqzvhiyovbylsayxgasojwnizjjpunfdvegdfqiasrhfqzoaukfazykkredruwkxtiuwffrdxmhvozwpkeybearweepp")) {
		int lavxtyqxgq;
		for (lavxtyqxgq = 36; lavxtyqxgq > 0; lavxtyqxgq--) {
			continue;
		}
	}
	if (false != false) {
		int ge;
		for (ge = 24; ge > 0; ge--) {
			continue;
		}
	}

}

double zwsmcqq::qphpxlpwia(int cvfkuaf, int wuscfrp, bool zumqnd, string iomhbkqjz, string vjeubivmadoxoak, string bexkqnhgjwemy, bool qvinnr, string khyjjivzdxrwfq, bool xyaotqr, string pomyhrtlq) {
	string icoza = "wbojtwtvxbwimiqvvurlysytprytpaytszogphfxlsjaglpafniytmgsxewmmg";
	int mgtwggxu = 4122;
	string patbopc = "eskgcsuwplzuzgqchncno";
	int podxgclgpqoc = 1997;
	int ljkjsp = 375;
	double ewngudrugygks = 14637;
	double bcmqsige = 42676;
	double vyypjkfqc = 43194;
	string phhckiolzuzepvd = "wbwshlaurw";
	return 6520;
}

string zwsmcqq::cufxuocxzccyxugso(string aqacpkqsgfcdbb, string dzucoscn, double iqplpegb, string jhblsghetyyl, double ekoleuttdd, int zlcqb, double rrlfeqqcmfpop) {
	string ilfmafltndzte = "opndqjekpurllroecvmkuatnpvx";
	string squpq = "urqwc";
	bool bboxpmipuclh = false;
	int jelbenl = 1198;
	if (1198 != 1198) {
		int akp;
		for (akp = 10; akp > 0; akp--) {
			continue;
		}
	}
	return string("zxmmfxmkwpsuldkk");
}

void zwsmcqq::dpfufgzqyxqoprlhmjdkivtm(bool qtfavfvlpsoyz) {
	int gxqjvbyeyvtdee = 4237;
	double tlxzgug = 43239;
	string remnu = "yydaohlqmrpbmbwmzsfl";
	double rsncpqiebstrgz = 55371;
	int japypcxvw = 9902;
	int omzcqetgocqb = 3098;
	string usuhbwlha = "iehmccyaxyakfceoqhnszajjjwldwspwfnlbjzwdhllxubibifuwssojeemrycvexjbcpvpcjplvmy";
	if (string("yydaohlqmrpbmbwmzsfl") != string("yydaohlqmrpbmbwmzsfl")) {
		int nuojb;
		for (nuojb = 84; nuojb > 0; nuojb--) {
			continue;
		}
	}

}

int zwsmcqq::dnoaxybbjtmt(int vyulx, string ittqxgimnpyaqlv, string cvdmbrz, bool isiqwvjkt, string zfbziicneawcfqs, int bhsxny, double szbcauzptcfkwif, string gegvrmgfyvnkp, string varkipnjnjc, int vyltgiiwaowwpg) {
	double bcyzemqibhlhyw = 11768;
	int xsdtuniikeyh = 1655;
	string mwtbmkajgloh = "fmsxwfhgbspvwuzobjiqtivptgtjohqlangunmnesjtyjaafddwzuxzauazzprdldwfaxunpqphubebjmblpmrxveickxfcqjxda";
	double gfogx = 10031;
	bool qtsgjz = true;
	double qdywunnlfwpyb = 3410;
	double cblkbf = 19498;
	bool pvzqnxjcmmp = false;
	if (10031 != 10031) {
		int mlua;
		for (mlua = 77; mlua > 0; mlua--) {
			continue;
		}
	}
	if (false != false) {
		int ccthzy;
		for (ccthzy = 49; ccthzy > 0; ccthzy--) {
			continue;
		}
	}
	if (11768 == 11768) {
		int lwmra;
		for (lwmra = 53; lwmra > 0; lwmra--) {
			continue;
		}
	}
	if (3410 != 3410) {
		int beup;
		for (beup = 65; beup > 0; beup--) {
			continue;
		}
	}
	return 37674;
}

string zwsmcqq::ktcpjoeyhxkjzlpr(string gghcscilpq, int edbkibtep, int lkfmikowj, int fpggb, string xzfqhzwfkpkdmnb, string psfcyjwojfpogad, string fsmqaqfwjehzvbb, string ezjvanlbypkm) {
	string cfzxlddrvrecp = "lhqsneosaspnsfpteamfunmemfnueqtkbpfiwkehowgkagghpxjoufvnnzhejosi";
	double akaautfjxpe = 17304;
	bool jaboudnrxzmux = true;
	bool chbzcke = true;
	string ozsmvv = "zpmwgzzgidqbqnftbszwdivkvegliwecindtyxeraikhvakhleezqmtvtuxezzzzhuylqcucmzdcnsfxlubtcvm";
	int qjzpwrlhaijsn = 6375;
	double rjevdgvrvvgu = 7472;
	int whngreatprwy = 5747;
	return string("bxclooxiwrp");
}

string zwsmcqq::rszznmmaucdiuuovqntpxdek(bool hvsbn, int sllnfqomdvdu, int sgckgenaqditdc, int aadnxcb, double yavwbohqezavjx, bool tifcpxvqbtqgkgu, int hhzeapy, string blqargsiinlsxrd, bool qxuhxh) {
	double otizz = 3093;
	string cmhnucxweydmd = "qzicjjcfvuabobutqpshtzwoapjubotizohpizsxeitvwsnxpitbqkojworaleovdc";
	double lvjvnngfxjzxkl = 52844;
	int tfwqmdiofhoqdcr = 988;
	double tvcidmkzhm = 38986;
	bool ulgjtiqqubmx = true;
	double eqhzvgntcjt = 11557;
	if (string("qzicjjcfvuabobutqpshtzwoapjubotizohpizsxeitvwsnxpitbqkojworaleovdc") != string("qzicjjcfvuabobutqpshtzwoapjubotizohpizsxeitvwsnxpitbqkojworaleovdc")) {
		int erisjzonr;
		for (erisjzonr = 30; erisjzonr > 0; erisjzonr--) {
			continue;
		}
	}
	if (52844 == 52844) {
		int slx;
		for (slx = 31; slx > 0; slx--) {
			continue;
		}
	}
	return string("ucjm");
}

double zwsmcqq::lopwipfmiytqxmhlbbch(string xbgigoyzra, bool iqjkbbqjnwabmr, double hsqsmueh, bool ewajq) {
	int xklyupwbtkfvtzv = 162;
	int xovlkywftb = 3086;
	int jtrezaolhbuyjju = 976;
	if (976 == 976) {
		int oxlngpvmt;
		for (oxlngpvmt = 22; oxlngpvmt > 0; oxlngpvmt--) {
			continue;
		}
	}
	if (3086 == 3086) {
		int yhlbskpd;
		for (yhlbskpd = 7; yhlbskpd > 0; yhlbskpd--) {
			continue;
		}
	}
	if (162 != 162) {
		int ivxkyonv;
		for (ivxkyonv = 19; ivxkyonv > 0; ivxkyonv--) {
			continue;
		}
	}
	if (3086 == 3086) {
		int ihs;
		for (ihs = 14; ihs > 0; ihs--) {
			continue;
		}
	}
	return 64011;
}

string zwsmcqq::vlsqssfuxmlzykj(string anwhzcntxxakmz, int xjibc, string puqsbihb, double epvldloau, bool vvljlno) {
	bool flqczu = true;
	if (true != true) {
		int sag;
		for (sag = 55; sag > 0; sag--) {
			continue;
		}
	}
	if (true == true) {
		int nd;
		for (nd = 32; nd > 0; nd--) {
			continue;
		}
	}
	return string("qesvckylp");
}

bool zwsmcqq::jtscjwfcekhffg(int lybxjkhkfvdqudy, int oaolqn) {
	string gtdzpzodwrmyx = "ywnkalxlkgjlydtkjrlkaufyhhrhxlfitxzybbizbwnxtzvft";
	bool fwvikc = false;
	bool bywlosvkiyfndo = true;
	bool xkztbkbfk = false;
	bool jbtisdofzgmjey = true;
	int illcfpmtek = 3182;
	int fhinzswj = 6;
	return true;
}

bool zwsmcqq::epkavrkjyicgyq(bool utkwjsmbtoqzll, int ihemxtberfqfeqv, string xvydndsuoixfvf, int hqvyhhaa, int svxciqgbnrgc, int fjfvupgrrtrusy, bool vxvcmul, double cfmnkp) {
	bool kudtfgowyag = false;
	int qauyjnvonrdiqu = 3395;
	int syzvmukdiymmbjz = 4146;
	int dmkuyqvzdwdwlzf = 4366;
	bool ccbgmofljqyxcoz = true;
	if (4366 == 4366) {
		int aoavqbypo;
		for (aoavqbypo = 33; aoavqbypo > 0; aoavqbypo--) {
			continue;
		}
	}
	if (true != true) {
		int nhfounyc;
		for (nhfounyc = 47; nhfounyc > 0; nhfounyc--) {
			continue;
		}
	}
	if (4366 == 4366) {
		int hfscfl;
		for (hfscfl = 16; hfscfl > 0; hfscfl--) {
			continue;
		}
	}
	if (false != false) {
		int zv;
		for (zv = 47; zv > 0; zv--) {
			continue;
		}
	}
	if (4146 == 4146) {
		int lggxufbpqz;
		for (lggxufbpqz = 72; lggxufbpqz > 0; lggxufbpqz--) {
			continue;
		}
	}
	return true;
}

int zwsmcqq::hmibfqtrpppfjmgoarjste(string rxyky, double bdxwyl, double ztvrzucmq) {
	bool hfbshypxsrosah = true;
	return 18062;
}

double zwsmcqq::hcclanafnoawutzeintry() {
	string kscsjbsjrkzsgz = "hvxmlyaxdqvvljwuuoobgmyunzesjctvzxvm";
	bool czlal = true;
	string qgeeqznuuvlryqq = "hqyteupphqxxgsswmrxuwwrbcporfdwrkpdplysdqsabbwdttbsffrzffibvzlt";
	string uwbukpa = "moxmrsgvsnzgvhutuxvsyilxtwbquwdpckostcdozhntmgmqoxfdpdsjhxf";
	int kygtojsac = 2442;
	if (2442 != 2442) {
		int vsllxuqr;
		for (vsllxuqr = 39; vsllxuqr > 0; vsllxuqr--) {
			continue;
		}
	}
	if (string("moxmrsgvsnzgvhutuxvsyilxtwbquwdpckostcdozhntmgmqoxfdpdsjhxf") == string("moxmrsgvsnzgvhutuxvsyilxtwbquwdpckostcdozhntmgmqoxfdpdsjhxf")) {
		int bnpuo;
		for (bnpuo = 60; bnpuo > 0; bnpuo--) {
			continue;
		}
	}
	if (string("moxmrsgvsnzgvhutuxvsyilxtwbquwdpckostcdozhntmgmqoxfdpdsjhxf") != string("moxmrsgvsnzgvhutuxvsyilxtwbquwdpckostcdozhntmgmqoxfdpdsjhxf")) {
		int zjzbzovtwd;
		for (zjzbzovtwd = 34; zjzbzovtwd > 0; zjzbzovtwd--) {
			continue;
		}
	}
	if (string("hvxmlyaxdqvvljwuuoobgmyunzesjctvzxvm") != string("hvxmlyaxdqvvljwuuoobgmyunzesjctvzxvm")) {
		int xnzacwt;
		for (xnzacwt = 58; xnzacwt > 0; xnzacwt--) {
			continue;
		}
	}
	return 49664;
}

void zwsmcqq::ybixswkodehsapvkjgiclien(bool snfvqcwookzrokf, bool griyjuawbigmaey, double lpozcxxkspf, int snpkwhjc, bool vfzlh, double zuomagxgvw, int qfjetldu, double zkytjzdjvf, double uaiovbryfgrw) {
	int puttqzohbjhhmmr = 888;
	string xvdvpmfilge = "zresnkojebznqnuxmvxxmfyfetwysdfknjqpedmu";
	int uoihxrvzulhac = 5881;
	if (888 == 888) {
		int erskak;
		for (erskak = 77; erskak > 0; erskak--) {
			continue;
		}
	}
	if (5881 != 5881) {
		int zookxqrjz;
		for (zookxqrjz = 72; zookxqrjz > 0; zookxqrjz--) {
			continue;
		}
	}
	if (string("zresnkojebznqnuxmvxxmfyfetwysdfknjqpedmu") == string("zresnkojebznqnuxmvxxmfyfetwysdfknjqpedmu")) {
		int ykmioaxqho;
		for (ykmioaxqho = 80; ykmioaxqho > 0; ykmioaxqho--) {
			continue;
		}
	}

}

double zwsmcqq::aoiurgeemrbuw() {
	return 8909;
}

string zwsmcqq::zazlhojefywcif(int cdxfelbduq, bool bmfhixpc, string ylymxbh, string yhafbf, double abzrottlgqc, double bvjzby) {
	bool jdvjlgzyplmnvts = false;
	bool trpfegaaj = true;
	bool pfrhdlrpz = true;
	int rtygdsd = 5085;
	int sxgxpegie = 6881;
	string veqzlnk = "oumnyguulekrvildxsabpudesrtyrlvpmggtaonwhgvvtjyfy";
	return string("qhelyodeu");
}

double zwsmcqq::ewbuadklnrbdax(bool tqgsffgngvag, int vwerqlfhbv, bool qiyaucb, string mjoiwhicaoy, string pxrxnool) {
	bool dinanuzwd = true;
	bool qfywmgmufiov = false;
	double mhowkeoabmbo = 50744;
	double jxveqbgyrbphh = 50624;
	bool qlcjst = true;
	int mvxymoijviupw = 546;
	double gobmflo = 33342;
	if (true == true) {
		int yqfynktosq;
		for (yqfynktosq = 11; yqfynktosq > 0; yqfynktosq--) {
			continue;
		}
	}
	if (546 == 546) {
		int kfp;
		for (kfp = 28; kfp > 0; kfp--) {
			continue;
		}
	}
	return 18927;
}

void zwsmcqq::tlqvhcfyde(string hbrhgcjyzoonndk, string vuxyle, string wtjszcq, string hyasxnuzljchud) {
	bool nlcrtohvxs = true;
	if (true != true) {
		int ucsmlbkg;
		for (ucsmlbkg = 69; ucsmlbkg > 0; ucsmlbkg--) {
			continue;
		}
	}

}

bool zwsmcqq::oovkmcgmfqzs(string hypcvyiiur, double cxofdnnnooncy, string sxhbqksixlk, int sjtjadvx, double wlqtacfh) {
	string ghzlijcqkv = "rtmregkyghpiffhjneltxjhcnqbzctscoscoickiffuulbbdbtymjuyqipagqkqsntmtiuyvtab";
	bool rqjrkz = false;
	double legoeiawjdb = 25753;
	double edphamncc = 5266;
	string phprezx = "wllmzlzlpbeyxmdrieuttcgtachlbnxgicjewqsrfsvsopjffdguj";
	bool bdzjrlbjdkiby = false;
	int ixxkapahoulmb = 529;
	string fdtntd = "bhengkhwazlntocvvqoynz";
	int fdbvmwy = 1272;
	if (25753 == 25753) {
		int rk;
		for (rk = 50; rk > 0; rk--) {
			continue;
		}
	}
	return true;
}

bool zwsmcqq::hcjfhrbukxn(double obmbntdmj, double vsuluumsz, bool ntcqk, string ynddtuyvpgzfcwt, int wehgi) {
	return false;
}

zwsmcqq::zwsmcqq() {
	this->aoiurgeemrbuw();
	this->zazlhojefywcif(1866, false, string("nkwnjl"), string("pwwbjzqvyrzfgtlfkgpxtsztiynutxrk"), 16127, 69633);
	this->ewbuadklnrbdax(false, 6883, false, string("mibdrvf"), string("hxfalsmfqszbxtrwhijkasjtnhhwhbhxzahpuipqptdmblcexsezvdcd"));
	this->tlqvhcfyde(string("gcyoeqtzgzqxyzehnjukjgtxxnklzusgflemcgauljdovslugpapjmxgviybzveixijyitfuy"), string("ggvwryrdaxgchsxanxvqzujzdnunianssvcxgqazrkkhpulclknbsephekrmhpctvjkmqibqxc"), string("nzpmgtpaxndzxnivcprhtkvztidxueudihpylwrsqoos"), string("ulcofdydjuwphpiieviwnidupsshmfwpagxdcwlwblynvrxbnnbomhkozkpwupzimxgkhbgguqgloqqppnkoubmnokvs"));
	this->oovkmcgmfqzs(string("oyophcozzqegylojboznszddrkixgmwgwjngwctskvhkwydmygihkzstuomenkwdszkvkfoyrqbvmnxunrlauliljpivdejeka"), 7763, string("mwocdgdhbrdigmltjwgmqqytqdawucqncvbnybkziuegcjwmqhjqktcpj"), 429, 41216);
	this->hcjfhrbukxn(72222, 41376, false, string("bgqinhenkfsbmozgvpxr"), 8701);
	this->ktcpjoeyhxkjzlpr(string("yosekpvvjejirhiygarfhprbtzyqnhmglukkuwukxssdzqnuqzlhqzgtgigvbmudd"), 2030, 535, 129, string("czixcyl"), string("uscamjslsnndndhofylywenxpqmcpotwmugqsdfcsuooeearqlgyypfwwzxffrqgxpxodlverztjgjflfhoyxxm"), string("gebwucpkehcdshcgrsthyvaahrksuxlutqibjxadebgkqovd"), string("fihugdtcyeustgdtsnztwnpcddgalepaiflrnlfiaqsxfuutzidakwwfuisfbsiyssgkgyteeokovzpjfsciczajqugwbl"));
	this->rszznmmaucdiuuovqntpxdek(false, 1537, 6113, 824, 60544, false, 1487, string("xeommthvlqhbjyygjveqypjslerbhppxzkoziujozdccxsomjlhzbtbvngkvhhholxxbwkiakyn"), true);
	this->lopwipfmiytqxmhlbbch(string("bxmuyvgfqzceq"), false, 78650, false);
	this->vlsqssfuxmlzykj(string("nijetezuzyqoirfzjnjajyyhrkbdbvkcwgtixxztkubctouthuvqqloiagrrneyhbxnetklqeg"), 4374, string("tvbiqvkeyxygxmmssgibtjbjlpvosepd"), 12529, false);
	this->jtscjwfcekhffg(1234, 2330);
	this->epkavrkjyicgyq(true, 807, string("ulxlwcwdgmksjqcullhubsjzat"), 2014, 508, 177, true, 51137);
	this->hmibfqtrpppfjmgoarjste(string("go"), 60773, 18731);
	this->hcclanafnoawutzeintry();
	this->ybixswkodehsapvkjgiclien(true, false, 15061, 718, true, 76317, 3793, 55786, 20321);
	this->vbcmicdeoxhdnmcfm(false, false, false, 7699, string("jibnhpjsryboduwwxlvgaeowuzmwvnupvvplpadkbyvvlaxdabwocjelvulakhdgosfxikpplgnykquewvtykeavobwaaxnnh"), 1058, false, string("paxewdzxfwuirrpivqmglnueuqlkglatzmdddsbcraqdodesbkjasrkqpsabinufdeulnklsf"), string("omguvzeefninfgcirtfunzoaxfpijhsqqyyxiibtvmffqkbwbagqmurjriekkxrfpqv"));
	this->bhkcrhetiel(15744, 54559, string("jdvsartvvklpbwfzwrnyrmfnbzcwbdjkdtvtwdqdswqqliimflqdwmecwsoltljbdxjiytox"), string("gnfbtcdjdlogadwfkimsvuxlnyxkhrxqt"));
	this->qphpxlpwia(2886, 758, false, string("wztoxjrcadjxgspiblvufwpyhycwvcowbepflkijvhghekkrpdmkqnzjwwogxfhsaljaxdhwyizf"), string("g"), string("clzeryj"), false, string("cnooflthepxrnqbvlyrfngfkpnymqityvvletifye"), true, string("tsfvcdwfnnfvtohzdrqskccd"));
	this->cufxuocxzccyxugso(string("nscjqebpsivshdile"), string("ypq"), 43391, string("inqkodcrwlob"), 5230, 2918, 16263);
	this->dpfufgzqyxqoprlhmjdkivtm(true);
	this->dnoaxybbjtmt(7348, string("vjbevayfvtatttnlmnpluneiqkxbnvpjdrxnlvkyj"), string("skrnkjdmoeclvjcmvseoezsmbs"), false, string("hntaawprumiarougrjdivtnhdfwfoajhpmpeuauponevjtoatnuzisyhvctsmizfhjzdhktkqpizwiivfhutbpsqakqubtsjuk"), 3658, 30175, string("fbinpvvhhkavwlscyaxyarcypyfauhtftkyoahjoprjpbvxxmnuwkqhztoxxaukfmfjwvhwttuwzclaqijrtofei"), string("pjledcvex"), 1124);
}
