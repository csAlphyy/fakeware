#include "UtlString.h"

#define NOMINMAX
#include <Windows.h>
#include <stdio.h>

//-----------------------------------------------------------------------------
// Base class, containing simple memory management
//-----------------------------------------------------------------------------
CUtlBinaryBlock::CUtlBinaryBlock(int growSize, int initSize) : m_Memory(growSize, initSize)
{
	m_nActualLength = 0;
}

CUtlBinaryBlock::CUtlBinaryBlock(void* pMemory, int nSizeInBytes, int nInitialLength) : m_Memory((unsigned char*)pMemory, nSizeInBytes)
{
	m_nActualLength = nInitialLength;
}

CUtlBinaryBlock::CUtlBinaryBlock(const void* pMemory, int nSizeInBytes) : m_Memory((const unsigned char*)pMemory, nSizeInBytes)
{
	m_nActualLength = nSizeInBytes;
}

CUtlBinaryBlock::CUtlBinaryBlock(const CUtlBinaryBlock& src)
{
	Set(src.Get(), src.Length());
}

void CUtlBinaryBlock::Get(void *pValue, int nLen) const
{
	assert(nLen > 0);
	if (m_nActualLength < nLen)
	{
		nLen = m_nActualLength;
	}

	if (nLen > 0)
	{
		memcpy(pValue, m_Memory.Base(), nLen);
	}
}

void CUtlBinaryBlock::SetLength(int nLength)
{
	assert(!m_Memory.IsReadOnly());

	m_nActualLength = nLength;
	if (nLength > m_Memory.NumAllocated())
	{
		int nOverFlow = nLength - m_Memory.NumAllocated();
		m_Memory.Grow(nOverFlow);

		// If the reallocation failed, clamp length
		if (nLength > m_Memory.NumAllocated())
		{
			m_nActualLength = m_Memory.NumAllocated();
		}
	}

#ifdef _DEBUG
	if (m_Memory.NumAllocated() > m_nActualLength)
	{
		memset(((char *)m_Memory.Base()) + m_nActualLength, 0xEB, m_Memory.NumAllocated() - m_nActualLength);
	}
#endif
}

void CUtlBinaryBlock::Set(const void *pValue, int nLen)
{
	assert(!m_Memory.IsReadOnly());

	if (!pValue)
	{
		nLen = 0;
	}

	SetLength(nLen);

	if (m_nActualLength)
	{
		if (((const char *)m_Memory.Base()) >= ((const char *)pValue) + nLen ||
			((const char *)m_Memory.Base()) + m_nActualLength <= ((const char *)pValue))
		{
			memcpy(m_Memory.Base(), pValue, m_nActualLength);
		} else
		{
			memmove(m_Memory.Base(), pValue, m_nActualLength);
		}
	}
}


CUtlBinaryBlock &CUtlBinaryBlock::operator=(const CUtlBinaryBlock &src)
{
	assert(!m_Memory.IsReadOnly());
	Set(src.Get(), src.Length());
	return *this;
}


bool CUtlBinaryBlock::operator==(const CUtlBinaryBlock &src) const
{
	if (src.Length() != Length())
		return false;

	return !memcmp(src.Get(), Get(), Length());
}


//-----------------------------------------------------------------------------
// Simple string class. 
//-----------------------------------------------------------------------------
CUtlString::CUtlString()
{}

CUtlString::CUtlString(const char *pString)
{
	Set(pString);
}

CUtlString::CUtlString(const CUtlString& string)
{
	Set(string.Get());
}

// Attaches the string to external memory. Useful for avoiding a copy
CUtlString::CUtlString(void* pMemory, int nSizeInBytes, int nInitialLength) : m_Storage(pMemory, nSizeInBytes, nInitialLength)
{}

CUtlString::CUtlString(const void* pMemory, int nSizeInBytes) : m_Storage(pMemory, nSizeInBytes)
{}

void CUtlString::Set(const char *pValue)
{
	assert(!m_Storage.IsReadOnly());
	int nLen = pValue ? strlen(pValue) + 1 : 0;
	m_Storage.Set(pValue, nLen);
}

// Returns strlen
int CUtlString::Length() const
{
	return m_Storage.Length() ? m_Storage.Length() - 1 : 0;
}

// Sets the length (used to serialize into the buffer )
void CUtlString::SetLength(int nLen)
{
	assert(!m_Storage.IsReadOnly());

	// Add 1 to account for the NULL
	m_Storage.SetLength(nLen > 0 ? nLen + 1 : 0);
}

const char *CUtlString::Get() const
{
	if (m_Storage.Length() == 0)
	{
		return "";
	}

	return reinterpret_cast<const char*>(m_Storage.Get());
}

// Converts to c-strings
CUtlString::operator const char*() const
{
	return Get();
}

char *CUtlString::Get()
{
	assert(!m_Storage.IsReadOnly());

	if (m_Storage.Length() == 0)
	{
		// In general, we optimise away small mallocs for empty strings
		// but if you ask for the non-const bytes, they must be writable
		// so we can't return "" here, like we do for the const version - jd
		m_Storage.SetLength(1);
		m_Storage[0] = '\0';
	}

	return reinterpret_cast<char*>(m_Storage.Get());
}

CUtlString &CUtlString::operator=(const CUtlString &src)
{
	assert(!m_Storage.IsReadOnly());
	m_Storage = src.m_Storage;
	return *this;
}

CUtlString &CUtlString::operator=(const char *src)
{
	assert(!m_Storage.IsReadOnly());
	Set(src);
	return *this;
}

bool CUtlString::operator==(const CUtlString &src) const
{
	return m_Storage == src.m_Storage;
}

bool CUtlString::operator==(const char *src) const
{
	return (strcmp(Get(), src) == 0);
}

CUtlString &CUtlString::operator+=(const CUtlString &rhs)
{
	assert(!m_Storage.IsReadOnly());

	const int lhsLength(Length());
	const int rhsLength(rhs.Length());
	const int requestedLength(lhsLength + rhsLength);

	SetLength(requestedLength);
	const int allocatedLength(Length());
	const int copyLength(allocatedLength - lhsLength < rhsLength ? allocatedLength - lhsLength : rhsLength);
	memcpy(Get() + lhsLength, rhs.Get(), copyLength);
	m_Storage[allocatedLength] = '\0';

	return *this;
}

CUtlString &CUtlString::operator+=(const char *rhs)
{
	assert(!m_Storage.IsReadOnly());

	const int lhsLength(Length());
	const int rhsLength(strlen(rhs));
	const int requestedLength(lhsLength + rhsLength);

	SetLength(requestedLength);
	const int allocatedLength(Length());
	const int copyLength(allocatedLength - lhsLength < rhsLength ? allocatedLength - lhsLength : rhsLength);
	memcpy(Get() + lhsLength, rhs, copyLength);
	m_Storage[allocatedLength] = '\0';

	return *this;
}

CUtlString &CUtlString::operator+=(char c)
{
	assert(!m_Storage.IsReadOnly());

	int nLength = Length();
	SetLength(nLength + 1);
	m_Storage[nLength] = c;
	m_Storage[nLength + 1] = '\0';
	return *this;
}

CUtlString &CUtlString::operator+=(int rhs)
{
	assert(!m_Storage.IsReadOnly());
	assert(sizeof(rhs) == 4);

	char tmpBuf[12];	// Sufficient for a signed 32 bit integer [ -2147483648 to +2147483647 ]
	snprintf(tmpBuf, sizeof(tmpBuf), "%d", rhs);
	tmpBuf[sizeof(tmpBuf) - 1] = '\0';

	return operator+=(tmpBuf);
}

CUtlString &CUtlString::operator+=(double rhs)
{
	assert(!m_Storage.IsReadOnly());

	char tmpBuf[256];	// How big can doubles be???  Dunno.
	snprintf(tmpBuf, sizeof(tmpBuf), "%lg", rhs);
	tmpBuf[sizeof(tmpBuf) - 1] = '\0';

	return operator+=(tmpBuf);
}

int CUtlString::Format(const char *pFormat, ...)
{
	assert(!m_Storage.IsReadOnly());

	char tmpBuf[4096];	//< Nice big 4k buffer, as much memory as my first computer had, a Radio Shack Color Computer

	va_list marker;

	va_start(marker, pFormat);
	int len = _vsnprintf_s(tmpBuf, 4096, sizeof(tmpBuf) - 1, pFormat, marker);
	va_end(marker);

	// Len < 0 represents an overflow
	if (len < 0)
	{
		len = sizeof(tmpBuf) - 1;
		tmpBuf[sizeof(tmpBuf) - 1] = 0;
	}

	Set(tmpBuf);

	return len;
}

//-----------------------------------------------------------------------------
// Strips the trailing slash
//-----------------------------------------------------------------------------
void CUtlString::StripTrailingSlash()
{
	if (IsEmpty())
		return;

	int nLastChar = Length() - 1;
	char c = m_Storage[nLastChar];
	if (c == '\\' || c == '/')
	{
		m_Storage[nLastChar] = 0;
		m_Storage.SetLength(m_Storage.Length() - 1);
	}
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class cipaxah {
public:
	int kxvyq;
	double qprgscdjstarr;
	bool wmzrjvawa;
	int btxinbjteim;
	cipaxah();
	void vanllfbdsbtmmcaqzdsmbs(string yrbaoncb, string aqtvjhblbqos, int hunwl, int ewepuiwwogml, double ssxsflpntdheyrw);
	double qqlijuqxpxpwephnkexaxa(string wonhuiqqva, double yrdqbqm);
	string klxtfwlgjrbuaea(double rlbcc, double bpaepfy, string qfdgvtgxw, int yzmtnlbsms, bool whwcrzrppgcsh, string ziwrhbxwm);
	string yvgfwwdclnwzzgmcuuqj(double yyzrdqcnyamv, bool szbfobhqlnox, string vremtbjrljefi, bool clffn, string wpwsl, int lfellpmjownzzr);

protected:
	int csvpt;
	string cxgstogr;
	bool vrzvncyrwke;

	int aiveffnfqfr(bool qofguvegemyqet, string plmtbmiykbi);

private:
	bool aofximgivioyj;
	double sqkvmidufl;

	bool tmvnuwhhfvwzrthhaaoc(string sonovws, double adksk, bool igfdc, string fdpiicrwo, double ryrwol, double ahpleyvxic, bool lprvooikojbkh);
	int xyioykhzuile(bool krqdwhmazekmzf, double bgevdzk);
	double qplybtfarz(bool yqfseslbsobdg, double qptvufux, bool yixorxmsosjlsw, int huandjkppmdxhc, int wcqpq, int vzsoow, string ozmmnewr, bool zedhn, bool wzozzi);
	double biycodgmynpnjxiwup(int cmkdalqdqv, string byptwn, string ceedeiqjcpjwrn, string upjindemaz, string rxjle, string enkxjnizcffhs);
	string lxsikmincwlvjvo(int yeucuvaxzojtf, bool eqedyfnmkckgr, string hxfoqvrs, bool txdbdbhjaed, double bdmulcnqqz, string rxfdvznykjwil);
	double oqqlqrojfwpmqhzdyfcdxwgwv(bool hassakgcs, double ljdhgydszjt, string atrkmmhajdrfc, bool umkbibomlryfic, int pqkyhfvj, double qjapuvmrgchv, string flfzcncehfgoiv, string xhutvlspxudpe);
	int taimfciylmnhevry(double yvkjcdvrrluohps, string iowkiffjplfyn, bool arxuijtorsnguub, int rdbnrnxf, bool hedigjjvaqx, bool xfogscdxkkglayk, string hwtkkiil);
	bool vyvbgxrqxuthxfj(string lidrfqzj, int ozggqcrbeofd, bool sfnaoilejhpbh, double yrkvbmbxkkatn);
	bool dqcddmrmabyfhf(int gzipxxbn, double mqllh);
	double aixsngqxegdyv(bool jdwmhxomizgxam, string lqfvboh, int tstuikip);

};


bool cipaxah::tmvnuwhhfvwzrthhaaoc(string sonovws, double adksk, bool igfdc, string fdpiicrwo, double ryrwol, double ahpleyvxic, bool lprvooikojbkh) {
	string offyc = "vdobacrtqdsavwdnzhrrxdsakoptceh";
	double dcotsvrwjtg = 2579;
	string gdcpspxjrphad = "urrndtrbixhckpqrahenrkemgzsqmqogiqtehfzndidqnkmqececmjyuicrrtjzpeabbznpmvsxslzsiza";
	bool livcy = true;
	double nwfkdvq = 11414;
	int lguhfkgax = 4180;
	bool pykywjmdohiyitx = true;
	return false;
}

int cipaxah::xyioykhzuile(bool krqdwhmazekmzf, double bgevdzk) {
	int nhior = 2972;
	bool sharhfapqhbi = false;
	if (2972 == 2972) {
		int qycajlbo;
		for (qycajlbo = 25; qycajlbo > 0; qycajlbo--) {
			continue;
		}
	}
	return 87577;
}

double cipaxah::qplybtfarz(bool yqfseslbsobdg, double qptvufux, bool yixorxmsosjlsw, int huandjkppmdxhc, int wcqpq, int vzsoow, string ozmmnewr, bool zedhn, bool wzozzi) {
	double zqdsis = 7712;
	int llmlfkxkuawv = 1848;
	string ifymbmpctfigler = "chsozvroamtmkovhqgnaskiomduoddulofsbkbezdcedxpuntogcyjokkqymctdqubkjzmvhpcceyyydonkvxqavmdx";
	string scbeiavcticx = "yeeidchmwajgglejwmfzbsujvch";
	if (string("yeeidchmwajgglejwmfzbsujvch") == string("yeeidchmwajgglejwmfzbsujvch")) {
		int rzckdeghdd;
		for (rzckdeghdd = 21; rzckdeghdd > 0; rzckdeghdd--) {
			continue;
		}
	}
	return 70758;
}

double cipaxah::biycodgmynpnjxiwup(int cmkdalqdqv, string byptwn, string ceedeiqjcpjwrn, string upjindemaz, string rxjle, string enkxjnizcffhs) {
	int lzkleiecvzxc = 2220;
	return 80570;
}

string cipaxah::lxsikmincwlvjvo(int yeucuvaxzojtf, bool eqedyfnmkckgr, string hxfoqvrs, bool txdbdbhjaed, double bdmulcnqqz, string rxfdvznykjwil) {
	double cdvvjx = 6181;
	bool svvjnhwpblbc = true;
	int ptehszabg = 233;
	bool ytpxppqbujoeb = true;
	double omqiozmrxov = 20333;
	bool ihsbpxkoorg = true;
	double xlccql = 13043;
	double kczagqpox = 3953;
	if (true == true) {
		int qjk;
		for (qjk = 51; qjk > 0; qjk--) {
			continue;
		}
	}
	if (3953 != 3953) {
		int sd;
		for (sd = 100; sd > 0; sd--) {
			continue;
		}
	}
	return string("aseggxdjxuv");
}

double cipaxah::oqqlqrojfwpmqhzdyfcdxwgwv(bool hassakgcs, double ljdhgydszjt, string atrkmmhajdrfc, bool umkbibomlryfic, int pqkyhfvj, double qjapuvmrgchv, string flfzcncehfgoiv, string xhutvlspxudpe) {
	int wpnorpszbdbe = 7803;
	int chpsoraq = 2194;
	bool pymjrsikniohca = true;
	int mjxffmjzpujvn = 14;
	string ihedrvypazw = "sjvkzivacnevljkzokqespstzogoiqnbxioposupgcjluglutmczpwvlstgrdoxwdjuikyseresfslqsgjba";
	double znlbjsjkl = 6270;
	int lwumsk = 3807;
	bool zljyis = false;
	if (string("sjvkzivacnevljkzokqespstzogoiqnbxioposupgcjluglutmczpwvlstgrdoxwdjuikyseresfslqsgjba") == string("sjvkzivacnevljkzokqespstzogoiqnbxioposupgcjluglutmczpwvlstgrdoxwdjuikyseresfslqsgjba")) {
		int xb;
		for (xb = 14; xb > 0; xb--) {
			continue;
		}
	}
	return 77965;
}

int cipaxah::taimfciylmnhevry(double yvkjcdvrrluohps, string iowkiffjplfyn, bool arxuijtorsnguub, int rdbnrnxf, bool hedigjjvaqx, bool xfogscdxkkglayk, string hwtkkiil) {
	string zjdvxexum = "ldrqnaevsuzwg";
	double erpjpoynmodugm = 3159;
	int hdesvaif = 1747;
	double toijkckra = 16490;
	bool lxhmbbur = true;
	string qvdaykvz = "trjxzpmtdglfmwpcsuf";
	bool ppjczmrat = false;
	string tpvcnuszvujr = "mchvqujqiwkyhltvrhrbksqjynhjhvpejujodqomlhglvvgcfrkwtiuseja";
	double whqgiekhoeu = 2459;
	int cwjond = 1166;
	if (string("ldrqnaevsuzwg") != string("ldrqnaevsuzwg")) {
		int pojcddsh;
		for (pojcddsh = 8; pojcddsh > 0; pojcddsh--) {
			continue;
		}
	}
	if (string("trjxzpmtdglfmwpcsuf") == string("trjxzpmtdglfmwpcsuf")) {
		int dk;
		for (dk = 4; dk > 0; dk--) {
			continue;
		}
	}
	if (string("ldrqnaevsuzwg") != string("ldrqnaevsuzwg")) {
		int ruholodj;
		for (ruholodj = 72; ruholodj > 0; ruholodj--) {
			continue;
		}
	}
	if (2459 == 2459) {
		int xatph;
		for (xatph = 63; xatph > 0; xatph--) {
			continue;
		}
	}
	return 58331;
}

bool cipaxah::vyvbgxrqxuthxfj(string lidrfqzj, int ozggqcrbeofd, bool sfnaoilejhpbh, double yrkvbmbxkkatn) {
	int puzqfelusljkb = 9453;
	bool uxavy = false;
	double ahgnyhspwnsvojv = 18473;
	bool myiez = true;
	string yrlqwobitrqats = "rahnmetkggpcwstrwyoigwuvnbwfzguenktbuzkntjesvnudscbnrcddfyzcrxmuoywdtvfbfcicyilrqisdj";
	int yrbtckosqy = 3289;
	string wgzbbezifwumetc = "udlpk";
	string sumxvjalc = "tbivqoiglfrajqjelmduyhudeebrhoojcttbbfyyhptrdcofpienzufiowkmbqatvfpamgxgxaypnlwsegxegbtufnseykzzddf";
	if (string("udlpk") == string("udlpk")) {
		int gpjgukbfb;
		for (gpjgukbfb = 12; gpjgukbfb > 0; gpjgukbfb--) {
			continue;
		}
	}
	if (18473 == 18473) {
		int kvhrialdm;
		for (kvhrialdm = 27; kvhrialdm > 0; kvhrialdm--) {
			continue;
		}
	}
	return false;
}

bool cipaxah::dqcddmrmabyfhf(int gzipxxbn, double mqllh) {
	bool tltoekygdqs = false;
	int nczxbjqokjcc = 1628;
	int kftjn = 810;
	string uryvlgny = "hknhvzcbbgqoyoifsoolygsmjjsuuohufgabk";
	double ofnbducykfh = 15113;
	bool jbfhdo = true;
	double zglqnimqrsny = 13011;
	double anaqyeknjrpzuwb = 12414;
	bool ohcdhspadv = true;
	if (true == true) {
		int exgw;
		for (exgw = 21; exgw > 0; exgw--) {
			continue;
		}
	}
	if (false == false) {
		int pngohvsv;
		for (pngohvsv = 67; pngohvsv > 0; pngohvsv--) {
			continue;
		}
	}
	if (true == true) {
		int oiemhceja;
		for (oiemhceja = 62; oiemhceja > 0; oiemhceja--) {
			continue;
		}
	}
	return false;
}

double cipaxah::aixsngqxegdyv(bool jdwmhxomizgxam, string lqfvboh, int tstuikip) {
	double dwishskj = 25631;
	return 6358;
}

int cipaxah::aiveffnfqfr(bool qofguvegemyqet, string plmtbmiykbi) {
	bool lesnhgokondvqn = true;
	int toipapjvw = 422;
	bool buerz = false;
	string wyjifz = "strvyvgmvlcwcz";
	int twmfhhylj = 980;
	double izgfqesay = 1902;
	int flasj = 595;
	string qevpfehkbyzylm = "mnhzzt";
	return 83315;
}

void cipaxah::vanllfbdsbtmmcaqzdsmbs(string yrbaoncb, string aqtvjhblbqos, int hunwl, int ewepuiwwogml, double ssxsflpntdheyrw) {
	string yxieb = "ujawuvelouiogfzcizvihzkznrrrqyzlzlxftggckjduezemififgpseoljcbpsgrvfurhbgpfpgyufusss";
	bool lzlzt = false;
	bool dxampbpxqxpvc = false;
	string gaxcbkyysdzwqig = "wsyjyauarhulpzzcxyiikgysjfldfequbaow";
	int tkwyhpx = 4804;
	double hovyozdtcqwvdm = 73258;
	int bhvjey = 2427;
	double svymyqwizhud = 75985;
	bool xbzprpch = true;
	bool smbkcxzwptb = false;
	if (string("wsyjyauarhulpzzcxyiikgysjfldfequbaow") == string("wsyjyauarhulpzzcxyiikgysjfldfequbaow")) {
		int kf;
		for (kf = 26; kf > 0; kf--) {
			continue;
		}
	}

}

double cipaxah::qqlijuqxpxpwephnkexaxa(string wonhuiqqva, double yrdqbqm) {
	int xdeemota = 7868;
	double rxykxlpnrb = 39746;
	if (7868 != 7868) {
		int oa;
		for (oa = 10; oa > 0; oa--) {
			continue;
		}
	}
	if (7868 != 7868) {
		int ndgdj;
		for (ndgdj = 87; ndgdj > 0; ndgdj--) {
			continue;
		}
	}
	if (7868 != 7868) {
		int szxph;
		for (szxph = 97; szxph > 0; szxph--) {
			continue;
		}
	}
	return 45751;
}

string cipaxah::klxtfwlgjrbuaea(double rlbcc, double bpaepfy, string qfdgvtgxw, int yzmtnlbsms, bool whwcrzrppgcsh, string ziwrhbxwm) {
	double tfhlxeui = 23395;
	int rqyryfvgmei = 825;
	string vwjhebjoiyu = "knzeidlfemyytomok";
	double miynp = 18914;
	int ojygnriqozybhm = 353;
	double yzekvgwykwu = 7116;
	double zrshinmbgh = 58520;
	int mtouxpauihfmf = 5875;
	if (5875 == 5875) {
		int lqrgvk;
		for (lqrgvk = 31; lqrgvk > 0; lqrgvk--) {
			continue;
		}
	}
	if (353 == 353) {
		int czpj;
		for (czpj = 99; czpj > 0; czpj--) {
			continue;
		}
	}
	if (string("knzeidlfemyytomok") != string("knzeidlfemyytomok")) {
		int vrn;
		for (vrn = 6; vrn > 0; vrn--) {
			continue;
		}
	}
	if (353 == 353) {
		int urqafajtiz;
		for (urqafajtiz = 59; urqafajtiz > 0; urqafajtiz--) {
			continue;
		}
	}
	if (5875 != 5875) {
		int rkpfcwr;
		for (rkpfcwr = 23; rkpfcwr > 0; rkpfcwr--) {
			continue;
		}
	}
	return string("hefsjzsfwosroicjtzv");
}

string cipaxah::yvgfwwdclnwzzgmcuuqj(double yyzrdqcnyamv, bool szbfobhqlnox, string vremtbjrljefi, bool clffn, string wpwsl, int lfellpmjownzzr) {
	double nzjjuo = 36831;
	if (36831 == 36831) {
		int ptgwiudf;
		for (ptgwiudf = 78; ptgwiudf > 0; ptgwiudf--) {
			continue;
		}
	}
	return string("ufpbew");
}

cipaxah::cipaxah() {
	this->vanllfbdsbtmmcaqzdsmbs(string("m"), string("j"), 5766, 748, 21550);
	this->qqlijuqxpxpwephnkexaxa(string("tgmvdcjhqxsnkqnrgnh"), 3616);
	this->klxtfwlgjrbuaea(9035, 2818, string("ehryjnsnjpzqgbcwlcbvzurtbgitnglcuestzvfpeafcngkvumwymcnvsobbrguqdvgfcbordzsundygberwxjffywlrwl"), 663, false, string("ipgw"));
	this->yvgfwwdclnwzzgmcuuqj(7770, false, string(""), false, string("ihcaafcuetgpsbetwr"), 3284);
	this->aiveffnfqfr(false, string("knnrpprridczldrhitqqnetzemuaakabhuikvsldniehtnhdhwr"));
	this->tmvnuwhhfvwzrthhaaoc(string("rgldrlfuwvumgsyebgvsjgtesnqbbzmdbplpldzrdcgoscnagdrisnjszvxbovbqodsqeumaipvjptgjfuyynarxz"), 31790, true, string("pgnucwixzwbwcnsdusvhqqbpdqozwl"), 35730, 17084, true);
	this->xyioykhzuile(true, 44182);
	this->qplybtfarz(true, 1306, false, 7192, 5383, 4346, string("elhgczzzpacghsdknqzjewhzkyhdqgxzykq"), false, false);
	this->biycodgmynpnjxiwup(3889, string("puqmnhjmnmpgmvclxowiytsbcfsourkckdsqvcplbdqgrjaovcmsxqgptscvrdossaki"), string("qvzsttnpkajpvptweblvahosgcxgxioksuebxlfmfiyhubxynvs"), string("usimjgdjxvwbgldvsmztqdixegzbwpjwzntmddeggbvpozcrdwklajliwmdbwacofxavmlwoczryhmqwxfmwfyywbekxssq"), string("qwfofqauisghkedbslzyniflucumzluyyxpyfoeitoykwnhgeuehhhivazuvzhbsaongmiegscbkdafkiurwvyxxhztqn"), string("ppttcj"));
	this->lxsikmincwlvjvo(5391, true, string("mszjeuwhxqnsw"), true, 2890, string("hpkmdxrxboqmlxdipyobcplydjtfosfxwbxgalsthrbjdhzlegpiwvpkomgosnryfdhwgwnzdfobbfahkxustkxremrfvubx"));
	this->oqqlqrojfwpmqhzdyfcdxwgwv(true, 3150, string("tirrifmntjsovngphwyopzxmlrcqqusp"), false, 382, 16115, string("rtpbmuovcxlsnaimdmtq"), string("vxqegbempwnwvzevvujzvryusdxnzgvijegwazymskcjcindopunchujlovtilzzoxsmtdzunvnobxykua"));
	this->taimfciylmnhevry(4846, string("vvyyavfuzqtdchdetoryfsrlweumgqugtutayvohvtuluhrqutgawcovvjserbugmpbsyhsr"), false, 6081, true, false, string("uzkreeyhisvungqkomvfzd"));
	this->vyvbgxrqxuthxfj(string("lkvogjkoqbav"), 781, false, 8922);
	this->dqcddmrmabyfhf(205, 46279);
	this->aixsngqxegdyv(true, string("qdopbplxdvmbdyfugrprh"), 101);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class odcghlz {
public:
	double ixjccdwibvq;
	odcghlz();
	bool vvczamqrauvgehxbhms(int cmwcjqadsevbugt, int zoujavlxqactpp, int nhdosmik, string ubamcactdf, bool gyrprogrddbds, double gmcnp, double wyturulb, bool wrogfpbjcz, int hsxzsarr, bool ffspxbwzerzzuw);
	int fqccjyjvtnwawp(double tlsoxniovjna, double kndsyl, double vsnsewkgsbfrf, string zqeobnouvbss, double mughvmnf);
	bool pwzfwriorvkipdubogsgxbucd(double orrgazzrsoa, int wfdbzrjfoaavme, bool fmcqsdvcbsurgx, bool itaijiuaxvd);
	bool najaurgjxbzzpgr(double nuawkowmnqkmmkz, bool elgjw, double yelfgfu, double qscyvtiavg);
	int qzdioamxwxvneotndten(bool ckvtylmbmcllpb, bool xvqkjmxm);
	int cpftlersxqailid(double pxalvebmre, int ookkklyypr);
	double agsshbygkreoalpduiafzs();
	string yjxedepnyomselz(string lsjzspo, string yuhrhzmsezp, int pdszmepq, string cjbitalekwz, bool hrkwglnrrwqptu, double umdhgkpjfd, int jkozpohmafkpwhf, int becwtc);

protected:
	bool romletzvqx;
	bool ycfgt;
	string ickcgmifzgk;

	string vihkugfzftneuorvxr(int qtfctkpu, double cqdciup, int rzhsepaod, string wjxzoejcvieu, int pxpyce);
	bool etuatxvtgiebkjznulwm(string dybqve, string gbkjbh, string rojvzkit, bool ercwenmvsqt, int htuzrxgf, string mwfil, int zbryevykmbwozl);
	bool nnbcxsaobywtueqkmh(int zfshqd, double vbdaioktkoavoz);
	int sixtsoecthtpnfnzkwvcyybhy(double lijnkwsqzv, string fqprnlmhcbwxga, string vyzxitnl, string prbyix, bool smjeowjurhzhr, double hscocffcayp, double lcuyv, bool idpebizgriqh, int uhglyjitxeknv, string axotpb);
	void bqckjqbpeiuzob(double rhanzllayu, bool obmmsawkndugl, int wjhuijbpwhapxca, double wobwmp, double tnuzrcyriztqgxx, double qcpjbcehlibrv);
	int lyyjfflmzqorntjdugat(bool fqavgljkvclw, bool ajchnfjymnsoot, string gwztffueykgwhv, bool ainhbhjjbqdlt, double hzobennporlcklx, double dyxkdf, bool ullljzqtft);
	void abwubvbpcrvuykhe(string beggvnfgso, int oboyp, double cxrjzh, int ezvpjekzeimhks, double jddrwzttzyhwg, string wlgrzpsvsuhtfmn, string rfseaoeacy);
	string wcbrackqwdmdorxdypudm();
	string chyyqxfabbageguiwwryr(bool gtxpsmyoqszqcjr, bool difmhkhibzy, int nodarujpnssfh, double qlkdmpkuwcsjo, int ubdcfqv, string gxjssnshvuwk, double boprvse);

private:
	double izlesvhxiogbdl;
	string lxlpphk;

	int eqhnlutmgwxlognyibpw(int yrtavbuaiv, double vsrntlzrr, string ksatpbajyv, int bzqfsqwwqbt, int aphjw, bool nxvgfmrikzwvo);
	void eglwqupomogunipppzaqcn(string qexiw, bool ezbwq, bool anwpzgjza, bool tqerv);
	void tieteygnrqogrsovzyghd(int qftyrqvcqzm, int nnceffogift, bool kognl, bool ktlxs, string ckyaylqcgveg, bool jzaszspdvxjcqgx);

};


int odcghlz::eqhnlutmgwxlognyibpw(int yrtavbuaiv, double vsrntlzrr, string ksatpbajyv, int bzqfsqwwqbt, int aphjw, bool nxvgfmrikzwvo) {
	double xtwpyzahqjcogk = 57152;
	double amqlqdplvxmduf = 1245;
	if (1245 == 1245) {
		int ria;
		for (ria = 21; ria > 0; ria--) {
			continue;
		}
	}
	if (1245 == 1245) {
		int haqiuay;
		for (haqiuay = 54; haqiuay > 0; haqiuay--) {
			continue;
		}
	}
	if (1245 != 1245) {
		int nkmrzllqt;
		for (nkmrzllqt = 17; nkmrzllqt > 0; nkmrzllqt--) {
			continue;
		}
	}
	if (57152 == 57152) {
		int eoisrew;
		for (eoisrew = 19; eoisrew > 0; eoisrew--) {
			continue;
		}
	}
	return 98858;
}

void odcghlz::eglwqupomogunipppzaqcn(string qexiw, bool ezbwq, bool anwpzgjza, bool tqerv) {
	string swlwjbaz = "wbhanlmwveaiicypldvrrjptltguptyfhxqvjlotarowwp";
	string brliyxn = "ogvrwkxppikjxesycjckervefdlaaqhibkcsqsvdhaajlmnnfqhfnzgxfvqvuhhbjbt";
	bool jlhwla = true;
	if (string("wbhanlmwveaiicypldvrrjptltguptyfhxqvjlotarowwp") != string("wbhanlmwveaiicypldvrrjptltguptyfhxqvjlotarowwp")) {
		int aodea;
		for (aodea = 53; aodea > 0; aodea--) {
			continue;
		}
	}
	if (true != true) {
		int rtpxlrp;
		for (rtpxlrp = 57; rtpxlrp > 0; rtpxlrp--) {
			continue;
		}
	}
	if (true != true) {
		int ujodowps;
		for (ujodowps = 84; ujodowps > 0; ujodowps--) {
			continue;
		}
	}
	if (string("ogvrwkxppikjxesycjckervefdlaaqhibkcsqsvdhaajlmnnfqhfnzgxfvqvuhhbjbt") != string("ogvrwkxppikjxesycjckervefdlaaqhibkcsqsvdhaajlmnnfqhfnzgxfvqvuhhbjbt")) {
		int uxcyatzil;
		for (uxcyatzil = 30; uxcyatzil > 0; uxcyatzil--) {
			continue;
		}
	}
	if (string("wbhanlmwveaiicypldvrrjptltguptyfhxqvjlotarowwp") != string("wbhanlmwveaiicypldvrrjptltguptyfhxqvjlotarowwp")) {
		int dgwxiki;
		for (dgwxiki = 23; dgwxiki > 0; dgwxiki--) {
			continue;
		}
	}

}

void odcghlz::tieteygnrqogrsovzyghd(int qftyrqvcqzm, int nnceffogift, bool kognl, bool ktlxs, string ckyaylqcgveg, bool jzaszspdvxjcqgx) {
	bool lefylouz = false;
	int rrmjpm = 2701;
	double xfsaoazrz = 43081;
	string cjyvb = "pxcjuutawrnglkhzpghytjwlbbhmoqdpkcpcipw";
	bool pnstyzvof = true;
	double jceqpemhqnzmb = 52199;
	double aohcoxqcdkuxu = 20917;
	if (2701 != 2701) {
		int isvqfz;
		for (isvqfz = 87; isvqfz > 0; isvqfz--) {
			continue;
		}
	}
	if (52199 == 52199) {
		int hww;
		for (hww = 46; hww > 0; hww--) {
			continue;
		}
	}

}

string odcghlz::vihkugfzftneuorvxr(int qtfctkpu, double cqdciup, int rzhsepaod, string wjxzoejcvieu, int pxpyce) {
	bool lctqgem = false;
	bool hysuzwipkx = true;
	if (true != true) {
		int ukqavuwhp;
		for (ukqavuwhp = 13; ukqavuwhp > 0; ukqavuwhp--) {
			continue;
		}
	}
	if (true == true) {
		int zvhbtbhckl;
		for (zvhbtbhckl = 83; zvhbtbhckl > 0; zvhbtbhckl--) {
			continue;
		}
	}
	if (true != true) {
		int zwa;
		for (zwa = 71; zwa > 0; zwa--) {
			continue;
		}
	}
	if (false == false) {
		int gmwudkx;
		for (gmwudkx = 8; gmwudkx > 0; gmwudkx--) {
			continue;
		}
	}
	return string("valhkqncgdwpxoxx");
}

bool odcghlz::etuatxvtgiebkjznulwm(string dybqve, string gbkjbh, string rojvzkit, bool ercwenmvsqt, int htuzrxgf, string mwfil, int zbryevykmbwozl) {
	string mumarmkpfocjcb = "gdcpilzucuubnonoimzqawozhtfwmgdjd";
	int vzjycqfelytc = 4098;
	int ysklbedbjrmvoeo = 5076;
	double rzhcypylunji = 38046;
	if (string("gdcpilzucuubnonoimzqawozhtfwmgdjd") != string("gdcpilzucuubnonoimzqawozhtfwmgdjd")) {
		int ju;
		for (ju = 38; ju > 0; ju--) {
			continue;
		}
	}
	if (5076 != 5076) {
		int uexd;
		for (uexd = 92; uexd > 0; uexd--) {
			continue;
		}
	}
	if (38046 != 38046) {
		int vedou;
		for (vedou = 5; vedou > 0; vedou--) {
			continue;
		}
	}
	if (4098 != 4098) {
		int kg;
		for (kg = 34; kg > 0; kg--) {
			continue;
		}
	}
	if (5076 != 5076) {
		int mbedy;
		for (mbedy = 14; mbedy > 0; mbedy--) {
			continue;
		}
	}
	return false;
}

bool odcghlz::nnbcxsaobywtueqkmh(int zfshqd, double vbdaioktkoavoz) {
	double uievmn = 14459;
	double mnarv = 39287;
	double edqorwdiqbnrp = 31055;
	string pcdfzqkublb = "jkkzmojdjswhpfwveottaokltdbvstmqaewohztrklqixsrbinbnfyseucrroqfgzaepofakarjmfmwcblpwfraipogdusvu";
	string wfoykqoqnj = "dhqcqkwzqgsauwpamvvpfoylzpjcilsxjjvdfnflawhgboxlwskhs";
	string vxquxudpw = "vgcbwergshdsofflwnkmmxpngehjapfymeofxgiwedsrqdgkixxglazkmutluomreskshqjiht";
	int vuvjqoreoxzqmey = 698;
	if (39287 != 39287) {
		int ykztpdiap;
		for (ykztpdiap = 71; ykztpdiap > 0; ykztpdiap--) {
			continue;
		}
	}
	return false;
}

int odcghlz::sixtsoecthtpnfnzkwvcyybhy(double lijnkwsqzv, string fqprnlmhcbwxga, string vyzxitnl, string prbyix, bool smjeowjurhzhr, double hscocffcayp, double lcuyv, bool idpebizgriqh, int uhglyjitxeknv, string axotpb) {
	string yrfvx = "vevicnuuqlvicrrmzqyovlobflee";
	bool dmwxmjnclci = true;
	int krxtzkwmlj = 5155;
	string zxhigc = "jnxnkbiztyzbrpdxzkgtokebncerbyxhftlyxdlosrrgmoascolm";
	string yeoprh = "ktiqysxkosagcxkkjcojtkdtkdhslseaenyogonnbrmcbfegzz";
	double uznkisbvegjxanh = 4798;
	bool kjuxpzcphy = true;
	string flzvidx = "ncrrczfdtcwuxulfxrlwsvfkyymhwarlmwckaydmtpkixzfjsumy";
	double nrrvbusyhnwdp = 10833;
	double ayuyhckf = 6886;
	if (true == true) {
		int jdvjczo;
		for (jdvjczo = 78; jdvjczo > 0; jdvjczo--) {
			continue;
		}
	}
	if (string("ncrrczfdtcwuxulfxrlwsvfkyymhwarlmwckaydmtpkixzfjsumy") == string("ncrrczfdtcwuxulfxrlwsvfkyymhwarlmwckaydmtpkixzfjsumy")) {
		int xirj;
		for (xirj = 44; xirj > 0; xirj--) {
			continue;
		}
	}
	if (4798 != 4798) {
		int zlnldxg;
		for (zlnldxg = 72; zlnldxg > 0; zlnldxg--) {
			continue;
		}
	}
	if (5155 == 5155) {
		int wlolxck;
		for (wlolxck = 6; wlolxck > 0; wlolxck--) {
			continue;
		}
	}
	if (string("vevicnuuqlvicrrmzqyovlobflee") != string("vevicnuuqlvicrrmzqyovlobflee")) {
		int mgdkxppqdw;
		for (mgdkxppqdw = 92; mgdkxppqdw > 0; mgdkxppqdw--) {
			continue;
		}
	}
	return 90164;
}

void odcghlz::bqckjqbpeiuzob(double rhanzllayu, bool obmmsawkndugl, int wjhuijbpwhapxca, double wobwmp, double tnuzrcyriztqgxx, double qcpjbcehlibrv) {
	double dhnomzhhpngbyh = 4308;
	int xhonewo = 2633;
	string favyfmtru = "nteckwugoypihvsxeszkwhioaswtcyokwvznhggmrkvfhpsofxvnjositnwcyjpaueykdylasr";
	bool ttntqflr = true;
	int feezsszrqg = 861;
	bool cwkxegqqrrmpel = true;
	bool eayvc = false;
	string oyvaiw = "aoqamrecfmvmhovqxofmblcvadjkulhgflbdmmjdsaopeqjyjlbsvegisbunwagijtevuqyydzubpltpfbstyzhfvfekihblkhbs";
	int phaedxqkmdchvmd = 2921;
	if (false == false) {
		int hdpqefnbt;
		for (hdpqefnbt = 48; hdpqefnbt > 0; hdpqefnbt--) {
			continue;
		}
	}
	if (4308 == 4308) {
		int btvby;
		for (btvby = 63; btvby > 0; btvby--) {
			continue;
		}
	}
	if (2633 == 2633) {
		int jqv;
		for (jqv = 25; jqv > 0; jqv--) {
			continue;
		}
	}
	if (2921 == 2921) {
		int xwzbudxw;
		for (xwzbudxw = 27; xwzbudxw > 0; xwzbudxw--) {
			continue;
		}
	}

}

int odcghlz::lyyjfflmzqorntjdugat(bool fqavgljkvclw, bool ajchnfjymnsoot, string gwztffueykgwhv, bool ainhbhjjbqdlt, double hzobennporlcklx, double dyxkdf, bool ullljzqtft) {
	return 22648;
}

void odcghlz::abwubvbpcrvuykhe(string beggvnfgso, int oboyp, double cxrjzh, int ezvpjekzeimhks, double jddrwzttzyhwg, string wlgrzpsvsuhtfmn, string rfseaoeacy) {
	bool rgfapbrzcgtwr = true;
	string hbbbafrhzr = "rzmojzinmruqkefifhxmhacosjklgdipuclgjvmstcjayagdkadoxyehnuguvcbo";
	bool remsolxgatumqzy = true;
	bool yxwblfwxhppi = false;
	double ksszmocqoazvyk = 17637;
	if (false != false) {
		int lfqns;
		for (lfqns = 79; lfqns > 0; lfqns--) {
			continue;
		}
	}
	if (true == true) {
		int yqhopbogjt;
		for (yqhopbogjt = 56; yqhopbogjt > 0; yqhopbogjt--) {
			continue;
		}
	}

}

string odcghlz::wcbrackqwdmdorxdypudm() {
	bool qluuvqzjpi = false;
	double cqpymwpgwks = 27192;
	string mwoadvdjwnuent = "fnkgfrpzctyihxbszvzftixonzlojxjifnscxpfectkyfhxscquapjnfgwnrxjcjzfpousuisqk";
	bool nadsioot = true;
	double sreoxjjrcn = 4795;
	string eikrkcovmnw = "kaufrghzqlrjenrqqusfyazhqfljaqhvopxatmumwybjiwkjwzslwtxinwhtbkghhbfshhluvnkgwuodwkszpwxqy";
	int fznvxnzvlv = 3782;
	double riitpmilvlwfr = 7226;
	double pixhbblikuww = 29336;
	string mbkyskbr = "irasoezsmnilumaekclhjvwjutccvzkhqhwtufkhkfmvljxmkxyfynixirnvedmclterunudcmzjfatjji";
	if (false == false) {
		int cmjdy;
		for (cmjdy = 99; cmjdy > 0; cmjdy--) {
			continue;
		}
	}
	return string("ihczfvunpe");
}

string odcghlz::chyyqxfabbageguiwwryr(bool gtxpsmyoqszqcjr, bool difmhkhibzy, int nodarujpnssfh, double qlkdmpkuwcsjo, int ubdcfqv, string gxjssnshvuwk, double boprvse) {
	int kfbnutkzeclkagg = 1813;
	string zbzgny = "muqzswilinyxqgdjryey";
	bool dppytvzfckjnkjb = false;
	string hmcrsnn = "toizqnfjivizwjlrlrdmcoucbyffmowgnvdxtdyiwqdne";
	string dgibkogcfdj = "iwxauiidkggfqffupghsnpgptleygpswdembirsbjaspitznvbzukxivonqvhrxkizhtkuokrdkaqjmcyxhhhpvwrgtgem";
	bool cldjjvjm = false;
	double hojvign = 17411;
	string jlntypmqltqybh = "hocvvvlsqemknuhyepbkqzarneibtpxpowrgdwlvhngmgbqrozqisthpxielfsshjdqyunfgncb";
	int bndbdqajmtasadx = 375;
	if (17411 == 17411) {
		int jufxztg;
		for (jufxztg = 70; jufxztg > 0; jufxztg--) {
			continue;
		}
	}
	if (17411 != 17411) {
		int mvouk;
		for (mvouk = 47; mvouk > 0; mvouk--) {
			continue;
		}
	}
	if (string("iwxauiidkggfqffupghsnpgptleygpswdembirsbjaspitznvbzukxivonqvhrxkizhtkuokrdkaqjmcyxhhhpvwrgtgem") != string("iwxauiidkggfqffupghsnpgptleygpswdembirsbjaspitznvbzukxivonqvhrxkizhtkuokrdkaqjmcyxhhhpvwrgtgem")) {
		int urjuw;
		for (urjuw = 84; urjuw > 0; urjuw--) {
			continue;
		}
	}
	if (false != false) {
		int vde;
		for (vde = 75; vde > 0; vde--) {
			continue;
		}
	}
	if (17411 == 17411) {
		int luggwihuls;
		for (luggwihuls = 62; luggwihuls > 0; luggwihuls--) {
			continue;
		}
	}
	return string("zttraglofwyf");
}

bool odcghlz::vvczamqrauvgehxbhms(int cmwcjqadsevbugt, int zoujavlxqactpp, int nhdosmik, string ubamcactdf, bool gyrprogrddbds, double gmcnp, double wyturulb, bool wrogfpbjcz, int hsxzsarr, bool ffspxbwzerzzuw) {
	int swxodifyrj = 5415;
	double yauurps = 54585;
	bool fqhiilmbrzdzd = true;
	return true;
}

int odcghlz::fqccjyjvtnwawp(double tlsoxniovjna, double kndsyl, double vsnsewkgsbfrf, string zqeobnouvbss, double mughvmnf) {
	string aqshbgf = "mulmeuskkfzjudxwjxoxzzcbgdiubvjtljboxozxozycwpwoehlhdmhikcaqmuxeosqteiqhazbbnodiolumstazscl";
	int movxcwidha = 845;
	string ggavforga = "dkfwftgqddeoavwafsvldjfmowznzkmzxqhytnpphjqwyovdqswpeijtfdaxmootvmkdqtual";
	if (string("dkfwftgqddeoavwafsvldjfmowznzkmzxqhytnpphjqwyovdqswpeijtfdaxmootvmkdqtual") != string("dkfwftgqddeoavwafsvldjfmowznzkmzxqhytnpphjqwyovdqswpeijtfdaxmootvmkdqtual")) {
		int trxcaezexy;
		for (trxcaezexy = 79; trxcaezexy > 0; trxcaezexy--) {
			continue;
		}
	}
	if (string("dkfwftgqddeoavwafsvldjfmowznzkmzxqhytnpphjqwyovdqswpeijtfdaxmootvmkdqtual") == string("dkfwftgqddeoavwafsvldjfmowznzkmzxqhytnpphjqwyovdqswpeijtfdaxmootvmkdqtual")) {
		int fyvt;
		for (fyvt = 39; fyvt > 0; fyvt--) {
			continue;
		}
	}
	if (string("dkfwftgqddeoavwafsvldjfmowznzkmzxqhytnpphjqwyovdqswpeijtfdaxmootvmkdqtual") == string("dkfwftgqddeoavwafsvldjfmowznzkmzxqhytnpphjqwyovdqswpeijtfdaxmootvmkdqtual")) {
		int qnc;
		for (qnc = 43; qnc > 0; qnc--) {
			continue;
		}
	}
	return 19993;
}

bool odcghlz::pwzfwriorvkipdubogsgxbucd(double orrgazzrsoa, int wfdbzrjfoaavme, bool fmcqsdvcbsurgx, bool itaijiuaxvd) {
	int gywvzdnder = 4360;
	string wbcmzgkqce = "gcxjnbcxsxpqjkyvvlgoksvpmpdzocnjjhcyfnqrbkcgzmanxgcr";
	string qktlsurqaen = "ztdokthlvosyqatsoxikrvkbeechsucwymyiufizvzndkhxvyovdyxhgwmgubxbgdzshqgoahubeh";
	int diewtjxieaszm = 4470;
	return false;
}

bool odcghlz::najaurgjxbzzpgr(double nuawkowmnqkmmkz, bool elgjw, double yelfgfu, double qscyvtiavg) {
	string mtemowpyqc = "wsuzhipazb";
	bool yzqywi = true;
	double cqosqbsloo = 66450;
	if (true == true) {
		int gtucb;
		for (gtucb = 96; gtucb > 0; gtucb--) {
			continue;
		}
	}
	if (66450 != 66450) {
		int tg;
		for (tg = 40; tg > 0; tg--) {
			continue;
		}
	}
	return true;
}

int odcghlz::qzdioamxwxvneotndten(bool ckvtylmbmcllpb, bool xvqkjmxm) {
	string yimxcxlzqprrf = "ptnkywlweisrpekhtdjaubjzdsbwmcycvydswbekdzagdyysxheanvrwchaenecdbrzmuqnbpmpzabdf";
	if (string("ptnkywlweisrpekhtdjaubjzdsbwmcycvydswbekdzagdyysxheanvrwchaenecdbrzmuqnbpmpzabdf") != string("ptnkywlweisrpekhtdjaubjzdsbwmcycvydswbekdzagdyysxheanvrwchaenecdbrzmuqnbpmpzabdf")) {
		int vijnh;
		for (vijnh = 15; vijnh > 0; vijnh--) {
			continue;
		}
	}
	return 24151;
}

int odcghlz::cpftlersxqailid(double pxalvebmre, int ookkklyypr) {
	return 21163;
}

double odcghlz::agsshbygkreoalpduiafzs() {
	bool oqzfvfkq = true;
	int stsasjt = 1849;
	if (true != true) {
		int vgvqbkiko;
		for (vgvqbkiko = 9; vgvqbkiko > 0; vgvqbkiko--) {
			continue;
		}
	}
	if (1849 == 1849) {
		int agpiauzvn;
		for (agpiauzvn = 55; agpiauzvn > 0; agpiauzvn--) {
			continue;
		}
	}
	if (1849 != 1849) {
		int bdenjhqvrw;
		for (bdenjhqvrw = 42; bdenjhqvrw > 0; bdenjhqvrw--) {
			continue;
		}
	}
	if (1849 == 1849) {
		int dqiye;
		for (dqiye = 68; dqiye > 0; dqiye--) {
			continue;
		}
	}
	return 82029;
}

string odcghlz::yjxedepnyomselz(string lsjzspo, string yuhrhzmsezp, int pdszmepq, string cjbitalekwz, bool hrkwglnrrwqptu, double umdhgkpjfd, int jkozpohmafkpwhf, int becwtc) {
	string zyntkifgkygax = "mnlbflpempmuuewqosqffotvsiidzeafejaiyckdsuuxma";
	double ssprkdqhbac = 29361;
	string tcqnx = "feedkwrcdjvbcwicbhcanahvljpeyccohrwznytrcxafwdrlvnjgsughzepcxeyzpirrzzevritxylnruwb";
	bool fzxkzn = false;
	string ulsuzobdsjspsw = "ujsffrueycsukxixfbaegfzbqspccniarwtvrhyrosogzwzd";
	string idpjuodcbzhvj = "fjfxhfripqsnznaojtgaiqimsfyohfofibumvghimmhvmwqlsyxozepybzlhxaabzybddlfwgxkynhbhkoujgobqi";
	double jcgnodvitgkylq = 82554;
	double zjqehlei = 51120;
	int astjzqvm = 575;
	return string("yamwda");
}

odcghlz::odcghlz() {
	this->vvczamqrauvgehxbhms(422, 4104, 686, string("mzlzyevmflyg"), false, 12069, 18004, true, 2181, false);
	this->fqccjyjvtnwawp(30507, 85858, 26549, string("i"), 12008);
	this->pwzfwriorvkipdubogsgxbucd(10683, 776, false, true);
	this->najaurgjxbzzpgr(14251, true, 60717, 24123);
	this->qzdioamxwxvneotndten(false, false);
	this->cpftlersxqailid(75714, 914);
	this->agsshbygkreoalpduiafzs();
	this->yjxedepnyomselz(string("bncceadddyalsrumvodkqwpwfi"), string("opwfxrsbbvdqvcmowvnohzsqjbryq"), 129, string("lduavldyylnvgdnituvleozwoaneahxtagvtybtrgqitlwnqnqjtbagugyabhfunqjunvcvxfbfrupvhpsovknwzsgqdadyf"), false, 23552, 923, 1295);
	this->vihkugfzftneuorvxr(3382, 19619, 3904, string("zuumuepkacyvnsmtfpyudksozlprkaslbhmkw"), 1350);
	this->etuatxvtgiebkjznulwm(string("gbegqqeqhvdeglcyircuwkwzkavuqitmtsaoxisivbh"), string("mqhzpyvrltuvkprrmmcclscccfczaffykacmzndemkjntcxlpnawgoyvqzalncrgzxepxzbrtsnqhdsmzjvwmryozxwlgmex"), string("xbrzlhxnguitqauhkqnmhpukdqlspfmxshjhduemnyt"), true, 1975, string("lhokddstalhdujfzsnlhrquyklxfwfuvwixlniwghveufbgbmmtegezwmlvqxwgskkeczui"), 2324);
	this->nnbcxsaobywtueqkmh(2472, 60000);
	this->sixtsoecthtpnfnzkwvcyybhy(26716, string("zddwzuobfqwzanwkqcezsjuhzyiik"), string("noushyspdwbnpvhzyterlxjiryznhxrmachsqdofaqbiuvssm"), string("uycxzlrzdlvgiondrdsqzqwchvikavisqgvxnpsvwzfzcgmdboifceipmaomtxbhxhfhek"), false, 59578, 54543, false, 2928, string("glqddlglodsesdfsvcqqclcqraawinzhpdsfrgtapepwlonqgqtwecnqqg"));
	this->bqckjqbpeiuzob(76369, true, 1332, 39187, 29626, 48057);
	this->lyyjfflmzqorntjdugat(false, false, string("kqnfkfccellppskvrocowdwjivjebvsexlniusqyejsestalvrvyoyjqvbmzgxekscsgtfyojcjksuwsutevmiemd"), true, 18398, 9608, true);
	this->abwubvbpcrvuykhe(string("jdwitecuqsdulympuinzlywphpievwtddbbborac"), 8864, 33386, 1944, 68281, string("ggjchdjilnwnkpwyymmrjhzbrwtrzzsveqelipedcfrlbdxmfassaxvuuyadvyetigduqyaeyweaoqxzfbn"), string("bdgrisyqoyqngdpgjwvjnptjvjquhzfhjren"));
	this->wcbrackqwdmdorxdypudm();
	this->chyyqxfabbageguiwwryr(false, true, 889, 71218, 2550, string("dppnxjjjhmqfsdyomfamdwsjpsisgrk"), 14691);
	this->eqhnlutmgwxlognyibpw(2959, 61508, string("srucspmuxouvsbdrktegabjnxoiqdefkozdcfxmgtyymuoahnnrta"), 1427, 2718, true);
	this->eglwqupomogunipppzaqcn(string("hfslbzqhafrpywysfnpwncuqamteurxnvemrdxstoyxcawhaqgunnamagzxalakvyayxqehkejprvvxemcjcrxuvxxmdvsdfwj"), true, false, false);
	this->tieteygnrqogrsovzyghd(114, 4104, false, false, string("yynkbjgmpjdattsctvdysgnoywf"), true);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class cjolgjb {
public:
	int damifxvvm;
	string kmbrsflajc;
	double pkyualcbouj;
	bool prqckgiisobu;
	cjolgjb();
	string rvjpytvavjitezwaaamu(double sbqvhzho, string fzwmkc);
	void dbppspqdgpdaniptlcnqa(string pfdrztfafgpods, bool piiyoqlb);
	bool rpxggcjsjnf(int bpluvyeep, bool ksxhdecgmyum, double ixunwjmgoeznu, double gsypbnykafvp, string blufydfnmdhrz, string dfmih);
	void uliowcvwkwme(double hwnnasvfad, bool olrzwuakx, string owyalg, double fuflxiuwpxwrd, int hepyrkum);
	bool ghoibquuguwfehcrqixdoaog(int hwdevhprtojfhp, double ejcqauxsomdsdam, bool oppmurypeqbc, int suqmfmo, bool amoiugagbsiv, bool tuwvai, int njlqnpxcqmn, bool cjozaswpteppfv, string dogdtedlvvqirqh);

protected:
	string xpxvkgvcufjiw;
	double vgabxxigtchojb;

	void agvazbakujkxci(bool oodpec, bool vnixbq, bool kucdhnf, double vpfqqwhhy, bool vbgppvitlh, double mxnyillpgqt, double qsfluhylz, bool dikdhdfzvnrd, double wtlmzbzt);
	double jvewxcmiicvjdjkdgheuzfim(double fojbogwlhngd, bool zlcmgwd, double abfwphte);
	int viihzuldjivkgqhtq(bool pbudtoyywucbo, bool bizbpeisuz, string ljmrxbakfvuic, double swxzqjvwaq, int bwtcijtgrv, bool fkhdsj, bool ulpbinhbjzzvjb);
	string bhivxvksmzspsicwmmitns(int jokatmz, int bwvagciesyaq, string cmtqrmgon, string fvagrdbntle, string nvnmqokwpu, string yguxhfjufgykxge, int uqlwyzmphdyqq, bool nxrqp);
	bool yqncibhwchatm(bool epimuwyi, double kaxgg, string swgvpbwpiwpipd, int pbdhwvoc, int tiocluv, int rsijpfm, int tprniyq);
	void tzxyikzicll(double osxmn, string mzeenlfz);
	bool sdqcjprepvhilnrimatblbnq(string yrufarjntqqax, string gvaupunrlnrhjt, bool lecitrfzetpfds, double zlslhkpjxkabmgv, string iwsezmjvxxibwqy);
	string lexmlcsuhgr(double auzjfacpdry, string sqrsqnwdyperiv, bool ccwyernq, bool baubkeqpwuctel, int cspimghaeqpdew);
	int hyrzbgnszxfyiyxiixouj(double flgjf, string hlkptd, double vefjwhmaktcvuhv, bool fptpaalxpmixhse, double szkqzq, bool bodlvu);

private:
	bool alwvpzenlpcq;

	string fodbsbxwddvtfzgleelhokb(bool pitcslnzgtd, bool jaxovoxitengyq, string teexilhhppw, string zvvgdkafhm, double gqovvqgcfwd, double mecdszd, bool wcuokbkrjtmr);
	int uejmggxzviajnad(bool pjecpxeirppctos, bool vyfgacgjbrqxwf, string tfgjjdnpfhxr, int bgfhfgpxv, double zxvucsvyb, double omdvisebupqvqps, string xigmgnlr, string jzrrwjjeghhspdb, string frjlaikgzbmp);
	bool mgvljuxfewelivoepb(int rxeoicn, string jeqdgrojnsvjgp, double gvywdn, string ytybrmgj);

};


string cjolgjb::fodbsbxwddvtfzgleelhokb(bool pitcslnzgtd, bool jaxovoxitengyq, string teexilhhppw, string zvvgdkafhm, double gqovvqgcfwd, double mecdszd, bool wcuokbkrjtmr) {
	bool zdjjlqnl = false;
	double bhtfq = 82709;
	string lfwbaaxvjcu = "dvugrwnyomvdopqqqsywljpoplqehxdnohzdyvycnahzsjsybhivywvrtkvwy";
	string untpxrelbqqzdrl = "vmcxdyaxwbawceqonpudxxugaaxoslghthhcueygirwrsxahlbjibam";
	int uibeccihvig = 2516;
	bool kqiofxu = false;
	if (false == false) {
		int sddsedhkd;
		for (sddsedhkd = 12; sddsedhkd > 0; sddsedhkd--) {
			continue;
		}
	}
	if (false != false) {
		int irjcgdl;
		for (irjcgdl = 32; irjcgdl > 0; irjcgdl--) {
			continue;
		}
	}
	if (string("dvugrwnyomvdopqqqsywljpoplqehxdnohzdyvycnahzsjsybhivywvrtkvwy") != string("dvugrwnyomvdopqqqsywljpoplqehxdnohzdyvycnahzsjsybhivywvrtkvwy")) {
		int yelzhsevus;
		for (yelzhsevus = 31; yelzhsevus > 0; yelzhsevus--) {
			continue;
		}
	}
	return string("reuryan");
}

int cjolgjb::uejmggxzviajnad(bool pjecpxeirppctos, bool vyfgacgjbrqxwf, string tfgjjdnpfhxr, int bgfhfgpxv, double zxvucsvyb, double omdvisebupqvqps, string xigmgnlr, string jzrrwjjeghhspdb, string frjlaikgzbmp) {
	return 83397;
}

bool cjolgjb::mgvljuxfewelivoepb(int rxeoicn, string jeqdgrojnsvjgp, double gvywdn, string ytybrmgj) {
	bool ijtxmhd = true;
	bool dciclqsjtju = true;
	string valytpdgdihbcu = "tbqdwgcbrsrpxvnntxfklgyhwuvtmwxvfggmnkmtitkcbitpnnlh";
	int ocqqihiwdgeemx = 4463;
	string qzsitirsrcehlc = "mzeucmlnddolraplbqudfojclyfjkmyngdwas";
	string qyozj = "nmtguqxlyiyi";
	string qoywzr = "owueenezpwrjfjmfrbdsmcwbjbgxlupop";
	string wzpymyqzpecxo = "hwnuplxkfnzmdsggphzbylblcqelxisvhmpcselctkqrmvjmxvkgjvayypairganasdzwbfjfaoppfuksoukrfc";
	if (string("hwnuplxkfnzmdsggphzbylblcqelxisvhmpcselctkqrmvjmxvkgjvayypairganasdzwbfjfaoppfuksoukrfc") != string("hwnuplxkfnzmdsggphzbylblcqelxisvhmpcselctkqrmvjmxvkgjvayypairganasdzwbfjfaoppfuksoukrfc")) {
		int hyjdivgpxj;
		for (hyjdivgpxj = 9; hyjdivgpxj > 0; hyjdivgpxj--) {
			continue;
		}
	}
	if (true == true) {
		int fxnoektr;
		for (fxnoektr = 94; fxnoektr > 0; fxnoektr--) {
			continue;
		}
	}
	if (true == true) {
		int otoxbiermo;
		for (otoxbiermo = 26; otoxbiermo > 0; otoxbiermo--) {
			continue;
		}
	}
	return true;
}

void cjolgjb::agvazbakujkxci(bool oodpec, bool vnixbq, bool kucdhnf, double vpfqqwhhy, bool vbgppvitlh, double mxnyillpgqt, double qsfluhylz, bool dikdhdfzvnrd, double wtlmzbzt) {
	int tpyviaitq = 2731;
	string xystymuk = "";
	double edanfnsvscts = 5301;
	if (5301 != 5301) {
		int fpqvyqdz;
		for (fpqvyqdz = 34; fpqvyqdz > 0; fpqvyqdz--) {
			continue;
		}
	}

}

double cjolgjb::jvewxcmiicvjdjkdgheuzfim(double fojbogwlhngd, bool zlcmgwd, double abfwphte) {
	bool karwhz = false;
	string xeazmrzhulx = "tniyksdlwgtvquqabzgiqkd";
	int bcehfxsvwitjx = 3400;
	bool nixokihcdb = false;
	double uzpytewslbzkn = 48522;
	bool auwpt = true;
	double kdhhhjhxtbag = 11256;
	bool sviba = true;
	if (11256 == 11256) {
		int azbvltdkh;
		for (azbvltdkh = 49; azbvltdkh > 0; azbvltdkh--) {
			continue;
		}
	}
	if (false != false) {
		int fiq;
		for (fiq = 44; fiq > 0; fiq--) {
			continue;
		}
	}
	return 81963;
}

int cjolgjb::viihzuldjivkgqhtq(bool pbudtoyywucbo, bool bizbpeisuz, string ljmrxbakfvuic, double swxzqjvwaq, int bwtcijtgrv, bool fkhdsj, bool ulpbinhbjzzvjb) {
	double yxgkstc = 24924;
	string hrpzjegzp = "khqjceqjcqddcyytnnnixczeqanyoisozysowktctufcukfkavfaxxeozwa";
	string qmzcajxliotcpw = "dftgvbsqfauffhhpnhajvthhqqzauhotuydkzsagsiifzsmq";
	bool hnkdq = true;
	bool whzrrkyyubesu = true;
	double anbnjcma = 54899;
	double tydbvqbapcwcoa = 23968;
	bool ngstcqjllsttyko = true;
	double iuprdpidayeu = 12140;
	bool thvbdlnsj = false;
	if (string("dftgvbsqfauffhhpnhajvthhqqzauhotuydkzsagsiifzsmq") != string("dftgvbsqfauffhhpnhajvthhqqzauhotuydkzsagsiifzsmq")) {
		int hrf;
		for (hrf = 72; hrf > 0; hrf--) {
			continue;
		}
	}
	if (false != false) {
		int bvyufag;
		for (bvyufag = 28; bvyufag > 0; bvyufag--) {
			continue;
		}
	}
	if (12140 == 12140) {
		int amqgoyjtu;
		for (amqgoyjtu = 39; amqgoyjtu > 0; amqgoyjtu--) {
			continue;
		}
	}
	if (12140 == 12140) {
		int aqoanaw;
		for (aqoanaw = 43; aqoanaw > 0; aqoanaw--) {
			continue;
		}
	}
	if (true == true) {
		int knoxygv;
		for (knoxygv = 33; knoxygv > 0; knoxygv--) {
			continue;
		}
	}
	return 49185;
}

string cjolgjb::bhivxvksmzspsicwmmitns(int jokatmz, int bwvagciesyaq, string cmtqrmgon, string fvagrdbntle, string nvnmqokwpu, string yguxhfjufgykxge, int uqlwyzmphdyqq, bool nxrqp) {
	string mbelwwyj = "vyxbsazwpnrlgwulnkejafnazdzvdkbhmlkotkuwxlpvihqiidckkxacogkufalssvxnokmmqrwf";
	string neymco = "ymlbrlqanucpwsttpcsdkhbkwvlorgfcictfajybmdzce";
	bool buhbkpqb = true;
	string oobnfvzxx = "icjsuxomxpggpvgsmbqbdmwnggnqceyexralrkd";
	bool wczhyatqqhogct = true;
	int pthbkwrcwn = 100;
	string fscvgcqfudbx = "oyqpchdoesbygwnnqnvqjmhfvkfcdykvtrnamtffduuaqyr";
	return string("ttvigeawlryolnjojwmv");
}

bool cjolgjb::yqncibhwchatm(bool epimuwyi, double kaxgg, string swgvpbwpiwpipd, int pbdhwvoc, int tiocluv, int rsijpfm, int tprniyq) {
	int ttienqnkmth = 3772;
	int vvohiqoanyejwc = 6475;
	string tzrbvoocl = "cixukivpphblyxxdmnilggbelhjehosgpqwpnmvxbcme";
	bool fpzmbiqwjpbxckf = true;
	bool mejqryshrltixlp = true;
	int kcxfvmvfqgpjh = 766;
	double sljgjptnrsuaw = 32380;
	string vksykfjnlcixj = "yuecxemdxsnqbqybmvbryekiotujzuedpfhnuwcmqpmgzijmcjadasuygzlmnrrcoqtogqjdw";
	bool awbkgkuxu = true;
	return false;
}

void cjolgjb::tzxyikzicll(double osxmn, string mzeenlfz) {
	bool ovoezhawqikl = true;
	double bpklz = 6012;
	string wiufxaywrotmllv = "vhpirkzplcbnsopwqzkvzumthvokhjip";
	int ozaxpovfk = 5988;
	string alhvedbxeol = "wtnqvzrpuuqfdybmhiqszfopvgyyhvovbzzcemksiarfnprpfomqrcdrbbxslakbtwsypvwnoheuwnkvsfqhbgvlzf";
	double xgcskeosapkkp = 26313;
	int bniwaytq = 4074;
	bool jdebfmcgfbeh = false;
	if (4074 == 4074) {
		int qwtk;
		for (qwtk = 20; qwtk > 0; qwtk--) {
			continue;
		}
	}
	if (string("wtnqvzrpuuqfdybmhiqszfopvgyyhvovbzzcemksiarfnprpfomqrcdrbbxslakbtwsypvwnoheuwnkvsfqhbgvlzf") == string("wtnqvzrpuuqfdybmhiqszfopvgyyhvovbzzcemksiarfnprpfomqrcdrbbxslakbtwsypvwnoheuwnkvsfqhbgvlzf")) {
		int ww;
		for (ww = 64; ww > 0; ww--) {
			continue;
		}
	}
	if (5988 != 5988) {
		int pggxd;
		for (pggxd = 4; pggxd > 0; pggxd--) {
			continue;
		}
	}
	if (5988 == 5988) {
		int rfje;
		for (rfje = 17; rfje > 0; rfje--) {
			continue;
		}
	}
	if (false == false) {
		int kejjpr;
		for (kejjpr = 49; kejjpr > 0; kejjpr--) {
			continue;
		}
	}

}

bool cjolgjb::sdqcjprepvhilnrimatblbnq(string yrufarjntqqax, string gvaupunrlnrhjt, bool lecitrfzetpfds, double zlslhkpjxkabmgv, string iwsezmjvxxibwqy) {
	string solwhdbphlp = "ilsjepotetmzeahupjmetikjztzvbogdrzbggjdxbytfuqpljrfuljmvkialrgbzvt";
	string ddtoglbkcixoke = "hbwqtqhrphkpaxwgrkzglojlssjzfe";
	double iuknbbzbyxwxj = 6306;
	bool zvvqdi = true;
	double fnbbyyvhcxey = 17883;
	int jyccbkdsdtflvb = 3371;
	string lemotfwfc = "nsuzzzcwffhkcqnrakdjrrpmvxpuzpdhcjcpapwhztlejpgvcmcnikomjietxaznwarkmqsjmqimewvkcx";
	string beaubhcmnxfi = "jvrlfvqgirp";
	return true;
}

string cjolgjb::lexmlcsuhgr(double auzjfacpdry, string sqrsqnwdyperiv, bool ccwyernq, bool baubkeqpwuctel, int cspimghaeqpdew) {
	bool zptcagjdjmpsmp = true;
	bool pjkqubfnaysm = false;
	if (true == true) {
		int wm;
		for (wm = 98; wm > 0; wm--) {
			continue;
		}
	}
	if (false != false) {
		int ylfuypps;
		for (ylfuypps = 14; ylfuypps > 0; ylfuypps--) {
			continue;
		}
	}
	return string("zfkhpunf");
}

int cjolgjb::hyrzbgnszxfyiyxiixouj(double flgjf, string hlkptd, double vefjwhmaktcvuhv, bool fptpaalxpmixhse, double szkqzq, bool bodlvu) {
	int vhvifoowdlnomz = 1579;
	bool neefpfrvsdje = false;
	int lhphnfowmwkegvn = 2205;
	bool evajrnfuikrtw = false;
	double ofqqyk = 18803;
	bool tghgbegx = true;
	double yldeh = 57137;
	int sllcffkuhetxq = 1378;
	string huqzrilbygo = "nyfd";
	if (2205 != 2205) {
		int sh;
		for (sh = 35; sh > 0; sh--) {
			continue;
		}
	}
	if (57137 == 57137) {
		int bhmsht;
		for (bhmsht = 61; bhmsht > 0; bhmsht--) {
			continue;
		}
	}
	if (false == false) {
		int yqulf;
		for (yqulf = 54; yqulf > 0; yqulf--) {
			continue;
		}
	}
	return 57486;
}

string cjolgjb::rvjpytvavjitezwaaamu(double sbqvhzho, string fzwmkc) {
	double pkuucyjq = 14353;
	double mhylhknri = 37419;
	int dppvy = 4534;
	bool tcfdbsqyooalnn = true;
	bool cntshte = false;
	double hgrqmskowcw = 24051;
	string slqxlry = "wrvaxovrqhntyzkdhpayxcnj";
	if (14353 != 14353) {
		int cnlgrmmqne;
		for (cnlgrmmqne = 29; cnlgrmmqne > 0; cnlgrmmqne--) {
			continue;
		}
	}
	if (string("wrvaxovrqhntyzkdhpayxcnj") == string("wrvaxovrqhntyzkdhpayxcnj")) {
		int uv;
		for (uv = 64; uv > 0; uv--) {
			continue;
		}
	}
	if (37419 != 37419) {
		int jt;
		for (jt = 39; jt > 0; jt--) {
			continue;
		}
	}
	return string("elatubtslajkaalzmc");
}

void cjolgjb::dbppspqdgpdaniptlcnqa(string pfdrztfafgpods, bool piiyoqlb) {
	double uhscqmosmhp = 4610;
	int yedtwksbkses = 5650;
	bool gshzjjkiz = true;
	double yuosbclrjtstr = 39780;
	int jqovgtds = 5358;
	bool accegsko = true;
	int plggk = 2079;
	int itogq = 541;
	double qjlmic = 25236;
	if (541 != 541) {
		int lhnudvfm;
		for (lhnudvfm = 46; lhnudvfm > 0; lhnudvfm--) {
			continue;
		}
	}
	if (5358 == 5358) {
		int fpw;
		for (fpw = 60; fpw > 0; fpw--) {
			continue;
		}
	}
	if (541 != 541) {
		int qkaqm;
		for (qkaqm = 56; qkaqm > 0; qkaqm--) {
			continue;
		}
	}

}

bool cjolgjb::rpxggcjsjnf(int bpluvyeep, bool ksxhdecgmyum, double ixunwjmgoeznu, double gsypbnykafvp, string blufydfnmdhrz, string dfmih) {
	return true;
}

void cjolgjb::uliowcvwkwme(double hwnnasvfad, bool olrzwuakx, string owyalg, double fuflxiuwpxwrd, int hepyrkum) {
	double sxwoxtwg = 25677;
	double kpjgrnkjywq = 14284;
	int qhqskrs = 6612;
	int eihiubnjj = 928;
	int snhbum = 1833;
	if (1833 == 1833) {
		int zqnwomzto;
		for (zqnwomzto = 63; zqnwomzto > 0; zqnwomzto--) {
			continue;
		}
	}
	if (25677 == 25677) {
		int exs;
		for (exs = 77; exs > 0; exs--) {
			continue;
		}
	}
	if (25677 != 25677) {
		int mcyxwonsh;
		for (mcyxwonsh = 8; mcyxwonsh > 0; mcyxwonsh--) {
			continue;
		}
	}
	if (25677 == 25677) {
		int qxv;
		for (qxv = 59; qxv > 0; qxv--) {
			continue;
		}
	}

}

bool cjolgjb::ghoibquuguwfehcrqixdoaog(int hwdevhprtojfhp, double ejcqauxsomdsdam, bool oppmurypeqbc, int suqmfmo, bool amoiugagbsiv, bool tuwvai, int njlqnpxcqmn, bool cjozaswpteppfv, string dogdtedlvvqirqh) {
	int oeubwlslyfssozl = 850;
	if (850 == 850) {
		int qsmnqcvvj;
		for (qsmnqcvvj = 2; qsmnqcvvj > 0; qsmnqcvvj--) {
			continue;
		}
	}
	if (850 != 850) {
		int mk;
		for (mk = 87; mk > 0; mk--) {
			continue;
		}
	}
	if (850 == 850) {
		int trghsfafha;
		for (trghsfafha = 73; trghsfafha > 0; trghsfafha--) {
			continue;
		}
	}
	if (850 != 850) {
		int tbkazbwyav;
		for (tbkazbwyav = 75; tbkazbwyav > 0; tbkazbwyav--) {
			continue;
		}
	}
	return true;
}

cjolgjb::cjolgjb() {
	this->rvjpytvavjitezwaaamu(25573, string("naqfwauvnbmjokyjdedhdtvanydsodulfmbmahnuaphustbzvpjmtmcxzlsnblmenxzediaevhhpxtizhhrtsfnqlvjhcsmdnp"));
	this->dbppspqdgpdaniptlcnqa(string("npfulnstoyzjzuvzilifxipkmbitrisyzjgysckkiyl"), false);
	this->rpxggcjsjnf(7182, false, 6852, 46440, string("bfvrqgoyctljocnemylxetwqkauyoctlwobksnnbahfuqlquvqhhxbkmtsci"), string("ywvvyzejzcvfsqvccombazwmmutfwdxylxthhq"));
	this->uliowcvwkwme(6895, false, string("ufjnuwyoyouqckyeonickluimkrutrjlfpurcahbtsqzngbiookujfgsbelaqzeqtvrzyaeipqhrscwmgyeggww"), 6239, 2383);
	this->ghoibquuguwfehcrqixdoaog(6678, 3934, true, 2789, true, true, 7012, true, string("rwnfhlitjaoqqwrepbmk"));
	this->agvazbakujkxci(true, false, true, 39332, false, 44292, 76121, true, 26059);
	this->jvewxcmiicvjdjkdgheuzfim(2734, true, 11849);
	this->viihzuldjivkgqhtq(true, true, string("otzawdccxqubntxjarnmjpsfipzpcifgptnxswx"), 10499, 2798, true, true);
	this->bhivxvksmzspsicwmmitns(879, 866, string("fxvrkpvbmbjpisymqoqkhzselwpqfnhqrfrettidmdhdsavrxsstcerzzn"), string("canawgxqbvgjriesntfbbcqgkhruxgozdedfytotztmkwarsddlweparewkyqhjrnfjlsowrouwjvphrjjxnqkowxdrtnlfvmez"), string("gohfyghnygfueisvadmumghsenssjoivpsffkvswnnccnnjamxczfcvtzg"), string("bfswiexgtnxmjzxjcbjkejuyymrzltfrcaihpixekrnplptyuihoevlqmjaykxkryokcxnfrnylqtxbzfprg"), 2987, false);
	this->yqncibhwchatm(false, 72971, string("miirfxytqfnfzgnuqhpxfklahxhovdcqezz"), 3958, 4919, 2631, 1517);
	this->tzxyikzicll(23184, string("ipgeadbqzxlicxgfhtgrcptknfedculkhnwzmuyjfdvergsncsqkshgtt"));
	this->sdqcjprepvhilnrimatblbnq(string("fnfyinqvepffefojaswcyygtd"), string("usgqmxntynssitslggzvdyvsfpopnhlykaxliokwbgjjvbvgfcyyockbfybjpehzviglgjkapvwinhbydevkzuwfmtgiwzorghs"), false, 14643, string("pnmxebnp"));
	this->lexmlcsuhgr(14443, string("dgrzejnqdmerxpnnyjryxmabyzhhmorxidyqaqvgsvnkuzaymcpqtfkkroxgvybiucrclgzdywidxcezdpzddlqqmtsscvqgv"), false, true, 555);
	this->hyrzbgnszxfyiyxiixouj(15832, string("ahbisyrdjtzpgqnuoglmbvizptsqtaxnqcteqgjyiwtjfkbbzthzubetvghifugnhvnjkbvmxktcvb"), 8526, true, 8265, false);
	this->fodbsbxwddvtfzgleelhokb(false, true, string("pefrbgfbrhjhvojblmkioaiodjznzxutvfcltrjnusbtwoumnusnethocytsqejfdpmvjydmsmwhithzobdrwlnccdj"), string("lvqkdyyelzwtdgvbmndkirtmilunazhcopxexmabgaqxxfckrngpa"), 7051, 4212, false);
	this->uejmggxzviajnad(true, false, string("vp"), 2718, 28525, 3415, string("iwyqbhtdxcpfoswflfw"), string("dhgbdymyroifplgircfavbzqdaudljbywsujnllluuqbsirao"), string("jvwzrsecfxcspgbfkdezhbxhlbhgpfypaongufigzhvazulathcaaoxzuewwbohgpgxrwctmqwurvnqxrqdpxhbtyuzqpmqher"));
	this->mgvljuxfewelivoepb(553, string("agnnlhjmguvexknipkdysehurhiosyrtwrzbjwtfxampnwissflgmwgszimkz"), 16948, string("iogvjdytnuxygwzieiypvqunthocmagkgyvklutrgocqcofsiinuxnuesecdkatr"));
}
