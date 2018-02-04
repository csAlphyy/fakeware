#include "VMatrix.h"

//-----------------------------------------------------------------------------
// VMatrix inlines.
//-----------------------------------------------------------------------------
inline VMatrix::VMatrix()
{}

inline VMatrix::VMatrix(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33)
{
	Init(
		m00, m01, m02, m03,
		m10, m11, m12, m13,
		m20, m21, m22, m23,
		m30, m31, m32, m33
	);
}


inline VMatrix::VMatrix(const matrix3x4_t& matrix3x4)
{
	Init(matrix3x4);
}


//-----------------------------------------------------------------------------
// Creates a matrix where the X axis = forward
// the Y axis = left, and the Z axis = up
//-----------------------------------------------------------------------------
inline VMatrix::VMatrix(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis)
{
	Init(
		xAxis.x, yAxis.x, zAxis.x, 0.0f,
		xAxis.y, yAxis.y, zAxis.y, 0.0f,
		xAxis.z, yAxis.z, zAxis.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}


inline void VMatrix::Init(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33
)
{
	m[0][0] = m00;
	m[0][1] = m01;
	m[0][2] = m02;
	m[0][3] = m03;

	m[1][0] = m10;
	m[1][1] = m11;
	m[1][2] = m12;
	m[1][3] = m13;

	m[2][0] = m20;
	m[2][1] = m21;
	m[2][2] = m22;
	m[2][3] = m23;

	m[3][0] = m30;
	m[3][1] = m31;
	m[3][2] = m32;
	m[3][3] = m33;
}


//-----------------------------------------------------------------------------
// Initialize from a 3x4
//-----------------------------------------------------------------------------
inline void VMatrix::Init(const matrix3x4_t& matrix3x4)
{
	memcpy(m, matrix3x4.Base(), sizeof(matrix3x4_t));

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;
}

//-----------------------------------------------------------------------------
// Vector3DMultiplyPosition treats src2 as if it's a point (adds the translation)
//-----------------------------------------------------------------------------
// NJS: src2 is passed in as a full vector rather than a reference to prevent the need
// for 2 branches and a potential copy in the body.  (ie, handling the case when the src2
// reference is the same as the dst reference ).
inline void Vector3DMultiplyPosition(const VMatrix& src1, const Vector& src2, Vector& dst)
{
	dst[0] = src1[0][0] * src2.x + src1[0][1] * src2.y + src1[0][2] * src2.z + src1[0][3];
	dst[1] = src1[1][0] * src2.x + src1[1][1] * src2.y + src1[1][2] * src2.z + src1[1][3];
	dst[2] = src1[2][0] * src2.x + src1[2][1] * src2.y + src1[2][2] * src2.z + src1[2][3];
}

//-----------------------------------------------------------------------------
// Methods related to the basis vectors of the matrix
//-----------------------------------------------------------------------------

inline Vector VMatrix::GetForward() const
{
	return Vector(m[0][0], m[1][0], m[2][0]);
}

inline Vector VMatrix::GetLeft() const
{
	return Vector(m[0][1], m[1][1], m[2][1]);
}

inline Vector VMatrix::GetUp() const
{
	return Vector(m[0][2], m[1][2], m[2][2]);
}

inline void VMatrix::SetForward(const Vector &vForward)
{
	m[0][0] = vForward.x;
	m[1][0] = vForward.y;
	m[2][0] = vForward.z;
}

inline void VMatrix::SetLeft(const Vector &vLeft)
{
	m[0][1] = vLeft.x;
	m[1][1] = vLeft.y;
	m[2][1] = vLeft.z;
}

inline void VMatrix::SetUp(const Vector &vUp)
{
	m[0][2] = vUp.x;
	m[1][2] = vUp.y;
	m[2][2] = vUp.z;
}

inline void VMatrix::GetBasisVectors(Vector &vForward, Vector &vLeft, Vector &vUp) const
{
	vForward.Init(m[0][0], m[1][0], m[2][0]);
	vLeft.Init(m[0][1], m[1][1], m[2][1]);
	vUp.Init(m[0][2], m[1][2], m[2][2]);
}

inline void VMatrix::SetBasisVectors(const Vector &vForward, const Vector &vLeft, const Vector &vUp)
{
	SetForward(vForward);
	SetLeft(vLeft);
	SetUp(vUp);
}


//-----------------------------------------------------------------------------
// Methods related to the translation component of the matrix
//-----------------------------------------------------------------------------

inline Vector VMatrix::GetTranslation() const
{
	return Vector(m[0][3], m[1][3], m[2][3]);
}

inline Vector& VMatrix::GetTranslation(Vector &vTrans) const
{
	vTrans.x = m[0][3];
	vTrans.y = m[1][3];
	vTrans.z = m[2][3];
	return vTrans;
}

inline void VMatrix::SetTranslation(const Vector &vTrans)
{
	m[0][3] = vTrans.x;
	m[1][3] = vTrans.y;
	m[2][3] = vTrans.z;
}


//-----------------------------------------------------------------------------
// appply translation to this matrix in the input space
//-----------------------------------------------------------------------------
inline void VMatrix::PreTranslate(const Vector &vTrans)
{
	Vector tmp;
	Vector3DMultiplyPosition(*this, vTrans, tmp);
	m[0][3] = tmp.x;
	m[1][3] = tmp.y;
	m[2][3] = tmp.z;
}


//-----------------------------------------------------------------------------
// appply translation to this matrix in the output space
//-----------------------------------------------------------------------------
inline void VMatrix::PostTranslate(const Vector &vTrans)
{
	m[0][3] += vTrans.x;
	m[1][3] += vTrans.y;
	m[2][3] += vTrans.z;
}

inline const matrix3x4_t& VMatrix::As3x4() const
{
	return *((const matrix3x4_t*)this);
}

inline matrix3x4_t& VMatrix::As3x4()
{
	return *((matrix3x4_t*)this);
}

inline void VMatrix::CopyFrom3x4(const matrix3x4_t &m3x4)
{
	memcpy(m, m3x4.Base(), sizeof(matrix3x4_t));
	m[3][0] = m[3][1] = m[3][2] = 0;
	m[3][3] = 1;
}

inline void VMatrix::Set3x4(matrix3x4_t& matrix3x4) const
{
	memcpy(matrix3x4.Base(), m, sizeof(matrix3x4_t));
}


//-----------------------------------------------------------------------------
// Matrix Math operations
//-----------------------------------------------------------------------------
inline const VMatrix& VMatrix::operator+=(const VMatrix &other)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] += other.m[i][j];
		}
	}

	return *this;
}

inline VMatrix VMatrix::operator+(const VMatrix &other) const
{
	VMatrix ret;
	for (int i = 0; i < 16; i++)
	{
		((float*)ret.m)[i] = ((float*)m)[i] + ((float*)other.m)[i];
	}
	return ret;
}

inline VMatrix VMatrix::operator-(const VMatrix &other) const
{
	VMatrix ret;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ret.m[i][j] = m[i][j] - other.m[i][j];
		}
	}

	return ret;
}

inline VMatrix VMatrix::operator-() const
{
	VMatrix ret;
	for (int i = 0; i < 16; i++)
	{
		((float*)ret.m)[i] = -((float*)m)[i];
	}
	return ret;
}

//-----------------------------------------------------------------------------
// Vector transformation
//-----------------------------------------------------------------------------


inline Vector VMatrix::operator*(const Vector &vVec) const
{
	Vector vRet;
	vRet.x = m[0][0] * vVec.x + m[0][1] * vVec.y + m[0][2] * vVec.z + m[0][3];
	vRet.y = m[1][0] * vVec.x + m[1][1] * vVec.y + m[1][2] * vVec.z + m[1][3];
	vRet.z = m[2][0] * vVec.x + m[2][1] * vVec.y + m[2][2] * vVec.z + m[2][3];

	return vRet;
}

inline Vector VMatrix::VMul4x3(const Vector &vVec) const
{
	Vector vResult;
	Vector3DMultiplyPosition(*this, vVec, vResult);
	return vResult;
}


inline Vector VMatrix::VMul4x3Transpose(const Vector &vVec) const
{
	Vector tmp = vVec;
	tmp.x -= m[0][3];
	tmp.y -= m[1][3];
	tmp.z -= m[2][3];

	return Vector(
		m[0][0] * tmp.x + m[1][0] * tmp.y + m[2][0] * tmp.z,
		m[0][1] * tmp.x + m[1][1] * tmp.y + m[2][1] * tmp.z,
		m[0][2] * tmp.x + m[1][2] * tmp.y + m[2][2] * tmp.z
	);
}

inline Vector VMatrix::VMul3x3(const Vector &vVec) const
{
	return Vector(
		m[0][0] * vVec.x + m[0][1] * vVec.y + m[0][2] * vVec.z,
		m[1][0] * vVec.x + m[1][1] * vVec.y + m[1][2] * vVec.z,
		m[2][0] * vVec.x + m[2][1] * vVec.y + m[2][2] * vVec.z
	);
}

inline Vector VMatrix::VMul3x3Transpose(const Vector &vVec) const
{
	return Vector(
		m[0][0] * vVec.x + m[1][0] * vVec.y + m[2][0] * vVec.z,
		m[0][1] * vVec.x + m[1][1] * vVec.y + m[2][1] * vVec.z,
		m[0][2] * vVec.x + m[1][2] * vVec.y + m[2][2] * vVec.z
	);
}


inline void VMatrix::V3Mul(const Vector &vIn, Vector &vOut) const
{
	float rw;

	rw = 1.0f / (m[3][0] * vIn.x + m[3][1] * vIn.y + m[3][2] * vIn.z + m[3][3]);
	vOut.x = (m[0][0] * vIn.x + m[0][1] * vIn.y + m[0][2] * vIn.z + m[0][3]) * rw;
	vOut.y = (m[1][0] * vIn.x + m[1][1] * vIn.y + m[1][2] * vIn.z + m[1][3]) * rw;
	vOut.z = (m[2][0] * vIn.x + m[2][1] * vIn.y + m[2][2] * vIn.z + m[2][3]) * rw;
}

//-----------------------------------------------------------------------------
// Other random stuff
//-----------------------------------------------------------------------------
inline void VMatrix::Identity()
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}


inline bool VMatrix::IsIdentity() const
{
	return
		m[0][0] == 1.0f && m[0][1] == 0.0f && m[0][2] == 0.0f && m[0][3] == 0.0f &&
		m[1][0] == 0.0f && m[1][1] == 1.0f && m[1][2] == 0.0f && m[1][3] == 0.0f &&
		m[2][0] == 0.0f && m[2][1] == 0.0f && m[2][2] == 1.0f && m[2][3] == 0.0f &&
		m[3][0] == 0.0f && m[3][1] == 0.0f && m[3][2] == 0.0f && m[3][3] == 1.0f;
}

inline Vector VMatrix::ApplyRotation(const Vector &vVec) const
{
	return VMul3x3(vVec);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class dnnuwih {
public:
	double tcwjkflksmh;
	dnnuwih();
	void dpoztegzsrulisdmchbbjsed(string mqwttiiotgcaom, double dfmrfqdegqyda, string ihmltvvulqsir, int hiprscdat, double sqqooqevefnj, int hddaeagbw, int baghja, int evkysafj, int ztnakevp);
	bool froronvdcjpjz(bool snnsoto, bool ckrsmfe, double rzhwnfh, double yknuqvwihkhi, int xexsoeldjwzoggx, int dodmscyzcoedo, bool napjgldpvevg);
	bool dgaccvvupffdaxr();
	bool ozoobfzcxjawvdodapgmi(double lpdgxambkgrzk, double zaydbkxxipfyrom, int beqjrd, double ujjszcfsst, string csiadepdfi, int zkche, bool cbytkfngiecsm, bool bazqurna);

protected:
	double ybyzophvzuf;
	double lwwtoch;

	string edubdvpyznllvai(bool bsjnveenkqk, int yszgushaepqre);
	bool wiihtplnsmbzoe(bool jgldmumgyhhpoh, int nmrjb, string nstbmdrffx, string trhwypr, double mgtuausecgoymw, string pfrbp, double kntowfbufdzo);
	string kguihukkackyzicsyfttbvu(int iilhxqrfcutcj, int grczgmylxsgsz, int invamgnceo, double gziroja, int cygtkcf, string uzpbxowuu, string opkfurchucizqq, double ydirjlwfpfibgjm, int gwnosqmctsa, int tvoczytinxqnwoa);
	string dhoraxekkzkh(bool jcpkc, bool vjimquadw, int exojwnmpdsq, bool zkxxxpfwidpmy, bool toilum, string pfonhlmnot, double mofzvjktxhepbdl, double ythobmvruamkn, string kjotkpsjzyiwup);
	bool puxzpreoqp();
	int iivxhlvgwuchsdewvmjdrlv(bool xwmetfjyzzim, bool jrxdnjucdbsy, string rrlypfsjimgutx, double edvjq, int nzgmkead);

private:
	bool dzascqwuk;
	bool jbafgfekz;
	double vwzzrn;

	bool ftaqpbggkdmsoxzbh(string xjvnistyfxp);
	void keqwbdnkdzmsta(int uutgttp, double qepjffkzqnctac, bool bjpcwe, string sxggfpraurlrm, int dlljimiyqabo);
	string yqbbwgqziktrkrjcjug(int aclivwpdn, int gpbpeqvexn, int zujlmkqlchvva, double qngeqtd, string animcjyuvjasp, bool fazxaqrnmrvcye, int wjlyudrcsfkikqk, string vjutwr, int qzracminiqjxq);
	string cbffvmaggvfpygihzwvcfceaa(bool bddvicwpn);
	int silbmakpitg(double prpfikbqloit, string fqpsfjbklqrb, string qrfrsfe, int puvfs, int xafjftqu, int gwnbfbrpgrc, string qwzdqkzxg, double xhabmtbpaig);
	bool mqzvnhtixozpycinov();
	bool hwtynlglbwivnknvipgogan(bool cbatqll, bool llfikp, int iifkmrcrvlljt, bool fsymnkveuyrb, bool laftfdhh, bool cuyks, int lgfogoultuyval);
	bool xoafhlykevdcmlfkroifhjne(string fsxmpmsphbrt);
	bool vrrskrjfmpyymisihnm(double xmpzvv, int vjhydyfuaurk, double cusebxhkdcwnkv, int dcymj, int ydzzroyjx, string xigeagmohjsa, string uxmxgceaorkdj, int ovmmv);
	string wwpplnsjkaczqndesl(string ltebv, string ugjsqsnsoik);

};


bool dnnuwih::ftaqpbggkdmsoxzbh(string xjvnistyfxp) {
	bool bmzzovs = true;
	bool dblcraffryglq = true;
	string qkosrt = "vlbsnxcybzrhlypningojjwjadauinmwporfczkkejnyquyq";
	string cwahdnwftvv = "ublhjclapiu";
	double phwfd = 52213;
	bool impckxyglcfvo = false;
	bool tsovxmczux = true;
	return false;
}

void dnnuwih::keqwbdnkdzmsta(int uutgttp, double qepjffkzqnctac, bool bjpcwe, string sxggfpraurlrm, int dlljimiyqabo) {
	int ueajzqmhd = 2087;
	bool ziimyzsdidoreo = true;
	double kjiohtcreqoe = 37054;
	int dyrwrqqe = 1968;
	double pdcfusrny = 4358;
	double sgyzj = 77322;
	if (1968 != 1968) {
		int jiaxlnhceg;
		for (jiaxlnhceg = 84; jiaxlnhceg > 0; jiaxlnhceg--) {
			continue;
		}
	}

}

string dnnuwih::yqbbwgqziktrkrjcjug(int aclivwpdn, int gpbpeqvexn, int zujlmkqlchvva, double qngeqtd, string animcjyuvjasp, bool fazxaqrnmrvcye, int wjlyudrcsfkikqk, string vjutwr, int qzracminiqjxq) {
	int qffonnxzohkaug = 2371;
	double uwqsy = 25912;
	int npdhstckeithuad = 1711;
	string ezcmoqv = "iqqrjnxxzgingfvkmwhyiotvmfonyjcsbwxwuqqqumdoixieouwzjqrnon";
	int xbdyznemsrtnk = 1343;
	string upzvw = "jafrjun";
	bool kfohoi = false;
	int gqgfzrqinysn = 3691;
	int ndzlgipkrd = 958;
	if (1711 != 1711) {
		int mos;
		for (mos = 31; mos > 0; mos--) {
			continue;
		}
	}
	if (string("jafrjun") == string("jafrjun")) {
		int lyqlrgwd;
		for (lyqlrgwd = 84; lyqlrgwd > 0; lyqlrgwd--) {
			continue;
		}
	}
	if (false == false) {
		int buutlhoi;
		for (buutlhoi = 50; buutlhoi > 0; buutlhoi--) {
			continue;
		}
	}
	if (1711 != 1711) {
		int xmshq;
		for (xmshq = 37; xmshq > 0; xmshq--) {
			continue;
		}
	}
	if (string("jafrjun") == string("jafrjun")) {
		int bsx;
		for (bsx = 60; bsx > 0; bsx--) {
			continue;
		}
	}
	return string("xbvwccweqkgz");
}

string dnnuwih::cbffvmaggvfpygihzwvcfceaa(bool bddvicwpn) {
	bool gmntzussj = false;
	if (false != false) {
		int grmhgf;
		for (grmhgf = 81; grmhgf > 0; grmhgf--) {
			continue;
		}
	}
	return string("aft");
}

int dnnuwih::silbmakpitg(double prpfikbqloit, string fqpsfjbklqrb, string qrfrsfe, int puvfs, int xafjftqu, int gwnbfbrpgrc, string qwzdqkzxg, double xhabmtbpaig) {
	int wdzipzg = 8546;
	string zwaiplrpylrsj = "emcabjvopdmqzjawoxcitadisnojsyksnfkwgkcceffuhjumijuymbtjhughsfoooukkvkfo";
	double myuugualeoqk = 48879;
	double knvts = 8497;
	if (8546 != 8546) {
		int ktcm;
		for (ktcm = 43; ktcm > 0; ktcm--) {
			continue;
		}
	}
	if (8546 == 8546) {
		int yygf;
		for (yygf = 77; yygf > 0; yygf--) {
			continue;
		}
	}
	if (8546 != 8546) {
		int kdjkwmv;
		for (kdjkwmv = 10; kdjkwmv > 0; kdjkwmv--) {
			continue;
		}
	}
	if (8546 != 8546) {
		int qemtbfhz;
		for (qemtbfhz = 5; qemtbfhz > 0; qemtbfhz--) {
			continue;
		}
	}
	if (string("emcabjvopdmqzjawoxcitadisnojsyksnfkwgkcceffuhjumijuymbtjhughsfoooukkvkfo") == string("emcabjvopdmqzjawoxcitadisnojsyksnfkwgkcceffuhjumijuymbtjhughsfoooukkvkfo")) {
		int fddrcvo;
		for (fddrcvo = 4; fddrcvo > 0; fddrcvo--) {
			continue;
		}
	}
	return 40947;
}

bool dnnuwih::mqzvnhtixozpycinov() {
	return false;
}

bool dnnuwih::hwtynlglbwivnknvipgogan(bool cbatqll, bool llfikp, int iifkmrcrvlljt, bool fsymnkveuyrb, bool laftfdhh, bool cuyks, int lgfogoultuyval) {
	string ahslxtvjdc = "kxfrhsovmmzesyxvsbokgjnzskduzxgaaowxztkaimjdrgpwmcwppkkiynyhraenxtlwfaurpiasawwdhgjpoorojqqh";
	bool crxbeiywceohn = true;
	bool jnkphl = false;
	string azmgxb = "ekqlclapjwkaerpohscuksrsuv";
	bool mpyparmfrcuh = false;
	string lkdxlbwqwr = "rctkz";
	if (string("ekqlclapjwkaerpohscuksrsuv") == string("ekqlclapjwkaerpohscuksrsuv")) {
		int uicfxnzg;
		for (uicfxnzg = 2; uicfxnzg > 0; uicfxnzg--) {
			continue;
		}
	}
	if (false == false) {
		int lxklwni;
		for (lxklwni = 58; lxklwni > 0; lxklwni--) {
			continue;
		}
	}
	if (string("ekqlclapjwkaerpohscuksrsuv") != string("ekqlclapjwkaerpohscuksrsuv")) {
		int ekcpr;
		for (ekcpr = 57; ekcpr > 0; ekcpr--) {
			continue;
		}
	}
	return true;
}

bool dnnuwih::xoafhlykevdcmlfkroifhjne(string fsxmpmsphbrt) {
	double gbhylwboeljl = 27075;
	string wegqokrpg = "vyfukkfoslzwueypqvyvnnottzduvphvbekyppejapyuxegcptyiithovipqjulgyjqdz";
	int nzhcmdhkncu = 2736;
	string vmyzhlzcwftyo = "jrymjejezqspypnfltxdcepymvpaotjolqwslph";
	double ydjoxa = 10565;
	string ikdog = "fihjchatexuydrmtpchpaeqeewtivzzkeewmsinfffjcluminnhhiyadgykpjvqrribrmzsdnaldlziynitgbzbms";
	bool wrxlgkfcobras = false;
	double ylfgg = 668;
	if (10565 == 10565) {
		int dxogw;
		for (dxogw = 24; dxogw > 0; dxogw--) {
			continue;
		}
	}
	if (string("jrymjejezqspypnfltxdcepymvpaotjolqwslph") == string("jrymjejezqspypnfltxdcepymvpaotjolqwslph")) {
		int kmzhcuisf;
		for (kmzhcuisf = 90; kmzhcuisf > 0; kmzhcuisf--) {
			continue;
		}
	}
	if (string("vyfukkfoslzwueypqvyvnnottzduvphvbekyppejapyuxegcptyiithovipqjulgyjqdz") == string("vyfukkfoslzwueypqvyvnnottzduvphvbekyppejapyuxegcptyiithovipqjulgyjqdz")) {
		int ryjvssqyax;
		for (ryjvssqyax = 30; ryjvssqyax > 0; ryjvssqyax--) {
			continue;
		}
	}
	if (false != false) {
		int bwc;
		for (bwc = 39; bwc > 0; bwc--) {
			continue;
		}
	}
	if (2736 != 2736) {
		int pdkga;
		for (pdkga = 37; pdkga > 0; pdkga--) {
			continue;
		}
	}
	return true;
}

bool dnnuwih::vrrskrjfmpyymisihnm(double xmpzvv, int vjhydyfuaurk, double cusebxhkdcwnkv, int dcymj, int ydzzroyjx, string xigeagmohjsa, string uxmxgceaorkdj, int ovmmv) {
	string uueyirnl = "lblhdhyhsnurnvskzdzekrcqatdmtnxhestehwarzgy";
	string rmmhkuj = "xroqiksbtcvnwrfozkddfldtbwzocwwovtyxghehpnlgmmmluyxcjrmd";
	bool iyzhpcgpnmpci = true;
	return false;
}

string dnnuwih::wwpplnsjkaczqndesl(string ltebv, string ugjsqsnsoik) {
	bool lpxwycu = true;
	int olgowwmebjs = 320;
	int gvrntszrr = 429;
	bool ikfgql = false;
	double frglragxnkac = 70959;
	return string("pnp");
}

string dnnuwih::edubdvpyznllvai(bool bsjnveenkqk, int yszgushaepqre) {
	int vrhvbumbqfelb = 6161;
	string pakcocg = "mzwzbhlzpgrppcesntwtqpkfhpuzrhatrmchgojejjzwyebddkdtcznzuie";
	bool ogxlutzjopf = true;
	double ilmsupttoemfvl = 5084;
	int vcwuqsulyoshp = 8751;
	string wvfjjbywpj = "auisjnbjutoizhawdvqsrxjvzxatvrpegdkimrrn";
	bool iuraa = true;
	string rdpkuhiqyeqga = "uttkyhrcbdmrkfohwxkehkfxgjlxeirmyyftmulsgtvap";
	int mfghxmmvnnurcp = 626;
	if (true != true) {
		int dlkzkqg;
		for (dlkzkqg = 53; dlkzkqg > 0; dlkzkqg--) {
			continue;
		}
	}
	return string("hevmngsdlpzqk");
}

bool dnnuwih::wiihtplnsmbzoe(bool jgldmumgyhhpoh, int nmrjb, string nstbmdrffx, string trhwypr, double mgtuausecgoymw, string pfrbp, double kntowfbufdzo) {
	int vmyaxozxdsx = 388;
	string agnibklq = "vcklgluxaguurbliohaorpmvg";
	double oyvzmvsce = 9041;
	double vcltyvwmxppoa = 14677;
	int pmvycgsewfvof = 3344;
	bool xnpumgzkkno = false;
	return false;
}

string dnnuwih::kguihukkackyzicsyfttbvu(int iilhxqrfcutcj, int grczgmylxsgsz, int invamgnceo, double gziroja, int cygtkcf, string uzpbxowuu, string opkfurchucizqq, double ydirjlwfpfibgjm, int gwnosqmctsa, int tvoczytinxqnwoa) {
	string jxfdgxdpqgkhnq = "gvwmuknnujstskcdueovbyqljcpzsddbbxqcogdsjxhhayengsljpzoo";
	int zfvcbacxdfgvgez = 3336;
	double mbnqsyq = 5148;
	int pxcizf = 4345;
	double xxtpmstivhdh = 5444;
	if (4345 == 4345) {
		int vzlcfcict;
		for (vzlcfcict = 97; vzlcfcict > 0; vzlcfcict--) {
			continue;
		}
	}
	if (5148 != 5148) {
		int mczngcriu;
		for (mczngcriu = 68; mczngcriu > 0; mczngcriu--) {
			continue;
		}
	}
	if (string("gvwmuknnujstskcdueovbyqljcpzsddbbxqcogdsjxhhayengsljpzoo") == string("gvwmuknnujstskcdueovbyqljcpzsddbbxqcogdsjxhhayengsljpzoo")) {
		int wv;
		for (wv = 1; wv > 0; wv--) {
			continue;
		}
	}
	if (4345 == 4345) {
		int wxi;
		for (wxi = 60; wxi > 0; wxi--) {
			continue;
		}
	}
	if (string("gvwmuknnujstskcdueovbyqljcpzsddbbxqcogdsjxhhayengsljpzoo") != string("gvwmuknnujstskcdueovbyqljcpzsddbbxqcogdsjxhhayengsljpzoo")) {
		int jkak;
		for (jkak = 94; jkak > 0; jkak--) {
			continue;
		}
	}
	return string("");
}

string dnnuwih::dhoraxekkzkh(bool jcpkc, bool vjimquadw, int exojwnmpdsq, bool zkxxxpfwidpmy, bool toilum, string pfonhlmnot, double mofzvjktxhepbdl, double ythobmvruamkn, string kjotkpsjzyiwup) {
	bool espzvhan = false;
	int cbbntqftia = 1277;
	if (1277 == 1277) {
		int kkcz;
		for (kkcz = 88; kkcz > 0; kkcz--) {
			continue;
		}
	}
	if (false != false) {
		int kbaqolnumj;
		for (kbaqolnumj = 65; kbaqolnumj > 0; kbaqolnumj--) {
			continue;
		}
	}
	if (false == false) {
		int bw;
		for (bw = 39; bw > 0; bw--) {
			continue;
		}
	}
	return string("kwowazpwedpqbri");
}

bool dnnuwih::puxzpreoqp() {
	bool wlrsvfvvpguln = true;
	int gphohnpjz = 784;
	double hrevxtifms = 20638;
	string fmnozklqo = "llxhmjrbxdtflwcdqywdsnpyecarlcwmwmojubxcwekypwugxtrgmrgiugvvltkaarezaeovcouy";
	int ornqelwlyn = 1210;
	string waczqxzci = "foawvoffofzsiukdbjchrexdjxawvnpgtkmzmygzxsvtkhihfccpuxfqasctyvmsjmfjmqfqmj";
	string hnowswjf = "bxpowfyjpsyyfxpgvphcoekmyzcyavvquwjegdyvmsrvepl";
	if (string("bxpowfyjpsyyfxpgvphcoekmyzcyavvquwjegdyvmsrvepl") == string("bxpowfyjpsyyfxpgvphcoekmyzcyavvquwjegdyvmsrvepl")) {
		int dnrd;
		for (dnrd = 22; dnrd > 0; dnrd--) {
			continue;
		}
	}
	if (string("foawvoffofzsiukdbjchrexdjxawvnpgtkmzmygzxsvtkhihfccpuxfqasctyvmsjmfjmqfqmj") != string("foawvoffofzsiukdbjchrexdjxawvnpgtkmzmygzxsvtkhihfccpuxfqasctyvmsjmfjmqfqmj")) {
		int zhocvwnblk;
		for (zhocvwnblk = 9; zhocvwnblk > 0; zhocvwnblk--) {
			continue;
		}
	}
	if (784 != 784) {
		int qy;
		for (qy = 9; qy > 0; qy--) {
			continue;
		}
	}
	if (784 == 784) {
		int gnt;
		for (gnt = 76; gnt > 0; gnt--) {
			continue;
		}
	}
	if (string("foawvoffofzsiukdbjchrexdjxawvnpgtkmzmygzxsvtkhihfccpuxfqasctyvmsjmfjmqfqmj") != string("foawvoffofzsiukdbjchrexdjxawvnpgtkmzmygzxsvtkhihfccpuxfqasctyvmsjmfjmqfqmj")) {
		int ol;
		for (ol = 28; ol > 0; ol--) {
			continue;
		}
	}
	return true;
}

int dnnuwih::iivxhlvgwuchsdewvmjdrlv(bool xwmetfjyzzim, bool jrxdnjucdbsy, string rrlypfsjimgutx, double edvjq, int nzgmkead) {
	double nstfwzuu = 38304;
	double tcvpkbslizpd = 9842;
	double lixfkohspfvtvnx = 45593;
	double jsnrpoqdvphd = 35106;
	if (9842 == 9842) {
		int wh;
		for (wh = 97; wh > 0; wh--) {
			continue;
		}
	}
	if (45593 != 45593) {
		int jqgzoow;
		for (jqgzoow = 27; jqgzoow > 0; jqgzoow--) {
			continue;
		}
	}
	if (45593 == 45593) {
		int fzwamnl;
		for (fzwamnl = 88; fzwamnl > 0; fzwamnl--) {
			continue;
		}
	}
	if (35106 != 35106) {
		int tgfnjlvdfc;
		for (tgfnjlvdfc = 3; tgfnjlvdfc > 0; tgfnjlvdfc--) {
			continue;
		}
	}
	if (45593 != 45593) {
		int fsxh;
		for (fsxh = 12; fsxh > 0; fsxh--) {
			continue;
		}
	}
	return 90893;
}

void dnnuwih::dpoztegzsrulisdmchbbjsed(string mqwttiiotgcaom, double dfmrfqdegqyda, string ihmltvvulqsir, int hiprscdat, double sqqooqevefnj, int hddaeagbw, int baghja, int evkysafj, int ztnakevp) {
	string vbyiohq = "iiwihdxxdbwclmkeffqadlwygcmpbrkazcwisoigmzelktpjnacebgntu";
	double shfnlglhhugdnt = 15141;
	bool dbcbrmmvfod = true;
	if (15141 != 15141) {
		int bp;
		for (bp = 50; bp > 0; bp--) {
			continue;
		}
	}

}

bool dnnuwih::froronvdcjpjz(bool snnsoto, bool ckrsmfe, double rzhwnfh, double yknuqvwihkhi, int xexsoeldjwzoggx, int dodmscyzcoedo, bool napjgldpvevg) {
	bool zmxusuksobdty = false;
	int wpnwstblqjj = 3466;
	string ocqecvhjg = "geppuqoy";
	double ppkkmoyhtgbwgfm = 10856;
	double owuktjwcdhlafk = 34051;
	string ymqqrskaglv = "cxzdlucpekkildxmxbpgjxrshvovmjcnqutvmntusfydoxcwrpcokyzirsxokl";
	bool vhgnwu = false;
	double qsvgqkwoelx = 4183;
	bool bbjifyhkucizbv = false;
	if (10856 != 10856) {
		int rhafvzlez;
		for (rhafvzlez = 19; rhafvzlez > 0; rhafvzlez--) {
			continue;
		}
	}
	return true;
}

bool dnnuwih::dgaccvvupffdaxr() {
	int onwdcrc = 1896;
	int ogkimoneapr = 6405;
	double qxabsmctom = 11183;
	int ddbirmtfwztrd = 4303;
	if (1896 != 1896) {
		int nwhnsc;
		for (nwhnsc = 89; nwhnsc > 0; nwhnsc--) {
			continue;
		}
	}
	if (4303 == 4303) {
		int nwd;
		for (nwd = 42; nwd > 0; nwd--) {
			continue;
		}
	}
	if (1896 == 1896) {
		int apjvi;
		for (apjvi = 23; apjvi > 0; apjvi--) {
			continue;
		}
	}
	return false;
}

bool dnnuwih::ozoobfzcxjawvdodapgmi(double lpdgxambkgrzk, double zaydbkxxipfyrom, int beqjrd, double ujjszcfsst, string csiadepdfi, int zkche, bool cbytkfngiecsm, bool bazqurna) {
	int vyacqz = 3336;
	bool czeugekl = false;
	if (3336 == 3336) {
		int qqslgl;
		for (qqslgl = 43; qqslgl > 0; qqslgl--) {
			continue;
		}
	}
	if (3336 == 3336) {
		int ehpsjfzcx;
		for (ehpsjfzcx = 54; ehpsjfzcx > 0; ehpsjfzcx--) {
			continue;
		}
	}
	if (false != false) {
		int kbyuczueu;
		for (kbyuczueu = 30; kbyuczueu > 0; kbyuczueu--) {
			continue;
		}
	}
	if (3336 != 3336) {
		int ecqp;
		for (ecqp = 86; ecqp > 0; ecqp--) {
			continue;
		}
	}
	return true;
}

dnnuwih::dnnuwih() {
	this->dpoztegzsrulisdmchbbjsed(string("shdrn"), 64877, string("rzlkkkwlwhjygcxfhubwzdouodfsqshnksmfocwgujrhmhznxxeujy"), 1918, 7915, 2682, 385, 1184, 706);
	this->froronvdcjpjz(true, false, 21556, 32051, 1442, 3576, true);
	this->dgaccvvupffdaxr();
	this->ozoobfzcxjawvdodapgmi(5132, 47454, 3472, 11365, string("oiuedybjlyrwagfrkcwzkghaymkekdf"), 3108, true, true);
	this->edubdvpyznllvai(false, 7751);
	this->wiihtplnsmbzoe(false, 259, string("lhonfucowzwnjjdp"), string("oliwttycejnsmibxhqcoafvfxhpaooeknmxuquswiejmkutmaoqsjrngoygicssndkaaqrvhgexorantihqnbutvnxkfifax"), 476, string("nrnhhobacjawv"), 4101);
	this->kguihukkackyzicsyfttbvu(2939, 4545, 1162, 40005, 4831, string("fxxmxwpqwahj"), string("rnsiyayjzslfkupcqfreeewwocbhbxkwdlgsthchkpmnzszdfxqqgglqy"), 67541, 923, 4185);
	this->dhoraxekkzkh(true, true, 1847, false, false, string("ocjpaexwdktvlhpmomosaqhgkdylbclztpzqwpsgunxxnvnxscglcocckzgplbnxkvcp"), 40536, 43911, string("izfqhlzwtldnvcnxbzoszrdqgatkgpmdvkodpqlumeqlmeekruoouqcdnjl"));
	this->puxzpreoqp();
	this->iivxhlvgwuchsdewvmjdrlv(false, false, string("cmeomhxlodxvunuqqshatuaidtpaeuueglrwerpfpoc"), 43258, 6877);
	this->ftaqpbggkdmsoxzbh(string("ubmweamqqjikhudyow"));
	this->keqwbdnkdzmsta(1613, 12845, false, string("ytjyocqpgrymnxvixtzerlyjjfgsazmmofetqqwisggpuvnxhgbpevbfrnjscjbrhsdwvmjkmrxdgtt"), 1300);
	this->yqbbwgqziktrkrjcjug(2724, 3681, 555, 30531, string("iziyedxjxjqvltjzqrfmmaekywojhsaibjioybazhoayuadmfdwbnviphipfzxtzhmpwgqumaieuridkldyuklgpmofqcbze"), true, 7828, string("jxvfhbrxympzewhuhjhoywrcjaolpswkikdebvutpmazlomaapqpxajamfjmvfnshikxzpuxgkxhe"), 3104);
	this->cbffvmaggvfpygihzwvcfceaa(true);
	this->silbmakpitg(32426, string("ufnsuzjyjyqrkhrncwsnojhgaqahpjkujtskvqojdvtbkxoapch"), string("getjnteikevymcfcsufqbjzpamhvwethngbabklvsqvysmefvqcavdnagsdyhdeypotmyjg"), 1789, 2237, 745, string("hfnufozunpykuwqufkzopsbmobmfirotehptyuoiybypafsixhezhkv"), 30401);
	this->mqzvnhtixozpycinov();
	this->hwtynlglbwivnknvipgogan(false, true, 1626, false, false, true, 1613);
	this->xoafhlykevdcmlfkroifhjne(string("hwrovtwbxxcarlnaxhxbqzyeajpypybfqexdpxmbfuwatiyjwfkgwqhztrbmbifbkptubxmcqdrqdaqeeeqtgynb"));
	this->vrrskrjfmpyymisihnm(4554, 198, 29607, 2927, 5748, string("vrgqwdnjwuclioghgaqyhinuhdak"), string("cmrhjrylavlnwuqgwasnxupegrptzeitqrxmdmtbvwvftldbol"), 1473);
	this->wwpplnsjkaczqndesl(string("bqfsramspnmytgng"), string("tufxtdhkxjieberhbarsvpmzspwmtmmdbogouv"));
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ntuxmaq {
public:
	bool aqagqorawymtxb;
	double yfuosg;
	string fxrhikyjejoae;
	ntuxmaq();
	double pdcmxplohxqhtr(int brkzgyr, string fxuobh, bool wnrlbfzsectp, int qnznsgpnrmzee);
	bool qizmtcmcxaqgyzyr(bool raajlqy, double adymkv, double csjggevubyeg, int dmfgauxlbkqnm, int lbtqwhzohv);
	string ahmpxcxrst(bool tpytrejqkmq, bool iqwavzoyyb, bool qjdmxax, double ftjlp);
	void xluicvhkmmjgqjb(bool iyudl, int ikkli, int wbqnxt, int aupkmcj, int aancxrnqisqb, int wzlabse, int pemzdyxn);
	double xaxlkodootccjedh(double tjsmdzaq);
	void shwvgduyvnuhw(int jthkaydjphpu, string xavpijaganwdn);
	int kjfrikzabshtehbguiud(string mbujy, double eqvjiqishrym);
	void hwebgfzytqz(int dychgkrame, double heblyneikpe, string xhqdakxgnpmb, double tqtnpmkxbyanslx, bool cirxpbufcaevtyp, bool yalslxruc, string ghfphlcutnjyx, double xkcxidglotcjupd, double lprhuhhv, int kllcdnj);
	void bokguvfdyliwpusthd(int hinrgy, bool pgdni, int pfllz, string slcvxnf, bool ibbxgbbpvjvbufe, double tqhuzdfhvnaoo, string xrtzfoqaonihxpa);

protected:
	string eoahofhufag;
	int mzawhzihzule;
	double iizxhhpvovixs;

	void pqenwqjepssbaausnafdobf(int zifjlciwqayz, bool iogqdgcywojerce, double xhpakhaqeyp, double qpgydbjjkzqeo, bool uhtedjzptyxzqwe, int nrwqrrh, double qilicn);
	double wxvcvwksiwwelpwpagjaxqj(string oevefubkvfc, bool hrusvrk, string aangpc, string ewxirigepduxs);
	bool zhmtxgdqgxioyegajtlvik(string jmtmoepegjscxmz, int ldgfpis, string jgjeecnvxh);
	double fadwfvnikpgzrdcyishqji(string jjcaz, double ildpnv, bool jlvkrrmijxxc, bool ogxrfbvgekotlok, int nviwao, int fnsqzdvfsesqpeg, string mmlgap);
	double tfpyioixlsb(double huscif);
	void elaosywnnnbhegmtdkwpf(double mehgaacwpmsdpo, bool veskxyrfoupbs, double vvzkowlfqi, string pagcwykpvckc, double gakfkwvmrdzvi, double dzhzkecfv);
	void tfgfzjwxinzinvzboopg(string uzmktfykgip, bool qtkzaej, int arwvajgiktq, string oqpfpdg, bool mcfudpbbtdi, double ertcctsgb, double eaqdtutfcctkl, bool dxsjoljb);
	int fsmkoutwdw(bool xqnrrsvtluuzmtu);
	void wnpzschikzwzwjthchmqcsn(bool qqiktqnx, double vilykwrwhp, double zeoibsm, string jazfegffytio, string hihdmiyuuq, string elenhwgqk);

private:
	bool cqtgae;

	string jrblbrbunkfuxgtucc(bool tcwdtrhjblz, double kewwnkrugdpq, int mznesmp, int amazsduorr, double swwjmyplbddexi, int dryxeqksr, int jgjjafqytwqsb, string spbeu);
	int mqjrdojnbdavum(int ydwzyzzxjx, double niwyqzovwblfqco, double htdzyxfmucguvic, string ggfitmub, double subtzsufpl, bool cmzhxgjtyafqbn, bool mljntugtg, string hrumconddujo, bool vjhhvqydbdkgl);
	string srnntabuonsjexhfhneux(int ufguhsxr);
	bool dzkampgtutqrcjdplk();
	double vdjwvaqkcvpyhdb();
	bool pksraunskedgucg(double qhhbnyvbq, double fzkmijbk, bool tjtmwtpidssat, int gievtn, bool lseyoxokmgj, bool qmbhfb, bool onssjtokvqaudj);
	bool apgofcofpxlprgotumllri(double ccytj, int hfweepqop, double ukvrg, bool ppcaepxuqs, string svuxf, string omtpileojvbo, int vrcbijcb, double tkfwxlxvhyovbdo, int pxodu, bool hjamxrtpnxifgtq);

};


string ntuxmaq::jrblbrbunkfuxgtucc(bool tcwdtrhjblz, double kewwnkrugdpq, int mznesmp, int amazsduorr, double swwjmyplbddexi, int dryxeqksr, int jgjjafqytwqsb, string spbeu) {
	double vhnkzujqa = 10475;
	if (10475 == 10475) {
		int xgbvip;
		for (xgbvip = 33; xgbvip > 0; xgbvip--) {
			continue;
		}
	}
	if (10475 != 10475) {
		int cjzmadcna;
		for (cjzmadcna = 59; cjzmadcna > 0; cjzmadcna--) {
			continue;
		}
	}
	if (10475 != 10475) {
		int nzwthj;
		for (nzwthj = 90; nzwthj > 0; nzwthj--) {
			continue;
		}
	}
	return string("eicxdwf");
}

int ntuxmaq::mqjrdojnbdavum(int ydwzyzzxjx, double niwyqzovwblfqco, double htdzyxfmucguvic, string ggfitmub, double subtzsufpl, bool cmzhxgjtyafqbn, bool mljntugtg, string hrumconddujo, bool vjhhvqydbdkgl) {
	string flsfukmubosei = "lveaizgtzgkhoe";
	int yzmlhdxfcvqlcpq = 37;
	bool qoifdjbogh = false;
	bool crlzzy = false;
	bool kauzsa = false;
	bool dbfqkhozf = false;
	if (false == false) {
		int zbuucuebyp;
		for (zbuucuebyp = 22; zbuucuebyp > 0; zbuucuebyp--) {
			continue;
		}
	}
	if (false != false) {
		int vininyfv;
		for (vininyfv = 37; vininyfv > 0; vininyfv--) {
			continue;
		}
	}
	if (false != false) {
		int ulz;
		for (ulz = 95; ulz > 0; ulz--) {
			continue;
		}
	}
	if (false != false) {
		int ujiexjno;
		for (ujiexjno = 1; ujiexjno > 0; ujiexjno--) {
			continue;
		}
	}
	if (string("lveaizgtzgkhoe") == string("lveaizgtzgkhoe")) {
		int snmi;
		for (snmi = 42; snmi > 0; snmi--) {
			continue;
		}
	}
	return 80575;
}

string ntuxmaq::srnntabuonsjexhfhneux(int ufguhsxr) {
	bool lvddldzubylrmec = true;
	bool zgzkkkleh = false;
	if (false != false) {
		int zdduprpuvu;
		for (zdduprpuvu = 81; zdduprpuvu > 0; zdduprpuvu--) {
			continue;
		}
	}
	if (true == true) {
		int veza;
		for (veza = 100; veza > 0; veza--) {
			continue;
		}
	}
	return string("eqsvi");
}

bool ntuxmaq::dzkampgtutqrcjdplk() {
	string vwvdjsgjg = "qjchuopwpapacdmveeqbktbomciocqnrvdkmymtxwmqtihestidipwnjcvqlgzzn";
	bool uyprmpgqzdr = true;
	string khcdtbssfiuy = "q";
	double axlbnv = 14113;
	double djhag = 30228;
	double wotzxbal = 13582;
	bool gronk = true;
	bool ickvknzrljyo = true;
	double febieqtcf = 14977;
	if (string("qjchuopwpapacdmveeqbktbomciocqnrvdkmymtxwmqtihestidipwnjcvqlgzzn") != string("qjchuopwpapacdmveeqbktbomciocqnrvdkmymtxwmqtihestidipwnjcvqlgzzn")) {
		int icnmklrj;
		for (icnmklrj = 17; icnmklrj > 0; icnmklrj--) {
			continue;
		}
	}
	if (13582 != 13582) {
		int wlabekvoc;
		for (wlabekvoc = 29; wlabekvoc > 0; wlabekvoc--) {
			continue;
		}
	}
	if (14977 != 14977) {
		int sxxksf;
		for (sxxksf = 0; sxxksf > 0; sxxksf--) {
			continue;
		}
	}
	if (13582 == 13582) {
		int djcutj;
		for (djcutj = 24; djcutj > 0; djcutj--) {
			continue;
		}
	}
	if (30228 != 30228) {
		int fmsapf;
		for (fmsapf = 34; fmsapf > 0; fmsapf--) {
			continue;
		}
	}
	return true;
}

double ntuxmaq::vdjwvaqkcvpyhdb() {
	bool gqxlanf = false;
	int ehawrhjqdtv = 207;
	if (207 == 207) {
		int oppc;
		for (oppc = 61; oppc > 0; oppc--) {
			continue;
		}
	}
	if (false == false) {
		int gv;
		for (gv = 49; gv > 0; gv--) {
			continue;
		}
	}
	if (207 != 207) {
		int wveoyjk;
		for (wveoyjk = 5; wveoyjk > 0; wveoyjk--) {
			continue;
		}
	}
	if (false != false) {
		int roaootfa;
		for (roaootfa = 26; roaootfa > 0; roaootfa--) {
			continue;
		}
	}
	if (false != false) {
		int qyhhozfo;
		for (qyhhozfo = 6; qyhhozfo > 0; qyhhozfo--) {
			continue;
		}
	}
	return 21607;
}

bool ntuxmaq::pksraunskedgucg(double qhhbnyvbq, double fzkmijbk, bool tjtmwtpidssat, int gievtn, bool lseyoxokmgj, bool qmbhfb, bool onssjtokvqaudj) {
	string yawrpfpwz = "htoicdjkbiocndnkmjtnocdg";
	string tfgnxyndqgi = "xbhoypxfowpbuoszvfnrdqrxqghvxnlolptymxnkmwmbvifs";
	string sbrzmd = "vjgobnbubbrmxywkiacdjjynovpcltctgaexoumkjqggtykm";
	bool brmipvgpaf = true;
	if (string("xbhoypxfowpbuoszvfnrdqrxqghvxnlolptymxnkmwmbvifs") != string("xbhoypxfowpbuoszvfnrdqrxqghvxnlolptymxnkmwmbvifs")) {
		int yhaeanzfjz;
		for (yhaeanzfjz = 52; yhaeanzfjz > 0; yhaeanzfjz--) {
			continue;
		}
	}
	if (string("htoicdjkbiocndnkmjtnocdg") != string("htoicdjkbiocndnkmjtnocdg")) {
		int cjermfcjj;
		for (cjermfcjj = 3; cjermfcjj > 0; cjermfcjj--) {
			continue;
		}
	}
	if (string("htoicdjkbiocndnkmjtnocdg") == string("htoicdjkbiocndnkmjtnocdg")) {
		int jxjplov;
		for (jxjplov = 24; jxjplov > 0; jxjplov--) {
			continue;
		}
	}
	if (string("htoicdjkbiocndnkmjtnocdg") == string("htoicdjkbiocndnkmjtnocdg")) {
		int pdryo;
		for (pdryo = 32; pdryo > 0; pdryo--) {
			continue;
		}
	}
	return false;
}

bool ntuxmaq::apgofcofpxlprgotumllri(double ccytj, int hfweepqop, double ukvrg, bool ppcaepxuqs, string svuxf, string omtpileojvbo, int vrcbijcb, double tkfwxlxvhyovbdo, int pxodu, bool hjamxrtpnxifgtq) {
	bool qrpijyqbo = true;
	bool ghpyhxdxcmvjy = false;
	string nmdyzupxqgli = "dubajymezvmscoiqoljirwbvvwyhcikwc";
	bool avhoagpjmg = false;
	bool erkurvecnx = false;
	if (string("dubajymezvmscoiqoljirwbvvwyhcikwc") == string("dubajymezvmscoiqoljirwbvvwyhcikwc")) {
		int ylnnflelmt;
		for (ylnnflelmt = 76; ylnnflelmt > 0; ylnnflelmt--) {
			continue;
		}
	}
	if (string("dubajymezvmscoiqoljirwbvvwyhcikwc") != string("dubajymezvmscoiqoljirwbvvwyhcikwc")) {
		int wkcfgyx;
		for (wkcfgyx = 33; wkcfgyx > 0; wkcfgyx--) {
			continue;
		}
	}
	return false;
}

void ntuxmaq::pqenwqjepssbaausnafdobf(int zifjlciwqayz, bool iogqdgcywojerce, double xhpakhaqeyp, double qpgydbjjkzqeo, bool uhtedjzptyxzqwe, int nrwqrrh, double qilicn) {
	bool ycxxarezsguluzb = false;
	bool epyqrfr = false;

}

double ntuxmaq::wxvcvwksiwwelpwpagjaxqj(string oevefubkvfc, bool hrusvrk, string aangpc, string ewxirigepduxs) {
	int gnkdvyxd = 3537;
	double bsutulkxiooye = 9623;
	int plwlqjpnzvzdpm = 1493;
	double oocnmpgac = 16536;
	double ptyolrverq = 27221;
	string gaahxiqfznbpyb = "fjcampgheabahbskhsboiykzpoqyuqotknkw";
	return 22891;
}

bool ntuxmaq::zhmtxgdqgxioyegajtlvik(string jmtmoepegjscxmz, int ldgfpis, string jgjeecnvxh) {
	int bpoaaoooqikg = 420;
	bool xlhvscvay = true;
	int qktjzjhehhzruz = 1240;
	int txqgql = 4195;
	double nxjlu = 31351;
	int vcdcieibdh = 1941;
	int gltlb = 1398;
	double sblqumbbpfopsu = 10997;
	int damqsbxztopy = 2631;
	if (1398 == 1398) {
		int wkg;
		for (wkg = 70; wkg > 0; wkg--) {
			continue;
		}
	}
	if (10997 != 10997) {
		int nzgn;
		for (nzgn = 53; nzgn > 0; nzgn--) {
			continue;
		}
	}
	return true;
}

double ntuxmaq::fadwfvnikpgzrdcyishqji(string jjcaz, double ildpnv, bool jlvkrrmijxxc, bool ogxrfbvgekotlok, int nviwao, int fnsqzdvfsesqpeg, string mmlgap) {
	string xccrvhhqzwdd = "rprijlizx";
	string ofcobn = "iqcnbiktzamsyvula";
	double gqfno = 38542;
	bool ryqxsacnhybbtwc = false;
	double wbuqz = 20073;
	double vxfskjbslucrk = 2289;
	int domisryfw = 864;
	double ssszkxkxprrxjp = 73540;
	string xxunzyee = "vrteahcnxjzsaiugdqznhwpasbtssfesxsocwzinvhciwfedtolswoipgbpjowfevozliahdnvsfnzfnhqx";
	return 72971;
}

double ntuxmaq::tfpyioixlsb(double huscif) {
	return 75978;
}

void ntuxmaq::elaosywnnnbhegmtdkwpf(double mehgaacwpmsdpo, bool veskxyrfoupbs, double vvzkowlfqi, string pagcwykpvckc, double gakfkwvmrdzvi, double dzhzkecfv) {

}

void ntuxmaq::tfgfzjwxinzinvzboopg(string uzmktfykgip, bool qtkzaej, int arwvajgiktq, string oqpfpdg, bool mcfudpbbtdi, double ertcctsgb, double eaqdtutfcctkl, bool dxsjoljb) {
	double fnjlbfwkiuh = 1121;
	string gnyzrhkc = "ruqyzzoktznpagdhgq";
	int rrhef = 3031;
	int rnefvedi = 2594;
	double phhbjegcck = 8664;
	string heoxyysj = "iaoqobtjdxppcxsenigdkffhemoepzlnefswewtouaedcgnxhbdqwzpvxvifbrxtyzwatzytdiaeefvefhcrqqzqb";
	int fxrkumtqvkbbli = 1963;
	int znojchkotzlpg = 2158;
	string yqjrevlezdmno = "yqlewsqerqjwpdficovivrfkabxqufebcprdlpfbahegiyzmdiztj";
	double zaaos = 11775;
	if (string("yqlewsqerqjwpdficovivrfkabxqufebcprdlpfbahegiyzmdiztj") == string("yqlewsqerqjwpdficovivrfkabxqufebcprdlpfbahegiyzmdiztj")) {
		int mvitwahv;
		for (mvitwahv = 80; mvitwahv > 0; mvitwahv--) {
			continue;
		}
	}
	if (1121 == 1121) {
		int opzp;
		for (opzp = 9; opzp > 0; opzp--) {
			continue;
		}
	}
	if (8664 != 8664) {
		int mzaictjvw;
		for (mzaictjvw = 66; mzaictjvw > 0; mzaictjvw--) {
			continue;
		}
	}

}

int ntuxmaq::fsmkoutwdw(bool xqnrrsvtluuzmtu) {
	int fnikzpozfprzk = 884;
	int ndovppaqnqycfkj = 418;
	bool tlqletfe = false;
	string zwdetsuplxbx = "gfjartcqsxbtteldbszszftzpyunoislgkjvsp";
	if (884 == 884) {
		int vaadcpa;
		for (vaadcpa = 79; vaadcpa > 0; vaadcpa--) {
			continue;
		}
	}
	if (418 == 418) {
		int dbdm;
		for (dbdm = 4; dbdm > 0; dbdm--) {
			continue;
		}
	}
	if (418 != 418) {
		int qnc;
		for (qnc = 29; qnc > 0; qnc--) {
			continue;
		}
	}
	if (884 == 884) {
		int zquysmxy;
		for (zquysmxy = 46; zquysmxy > 0; zquysmxy--) {
			continue;
		}
	}
	if (418 != 418) {
		int ihbuslfyta;
		for (ihbuslfyta = 67; ihbuslfyta > 0; ihbuslfyta--) {
			continue;
		}
	}
	return 28767;
}

void ntuxmaq::wnpzschikzwzwjthchmqcsn(bool qqiktqnx, double vilykwrwhp, double zeoibsm, string jazfegffytio, string hihdmiyuuq, string elenhwgqk) {
	int qvvua = 359;
	bool fqetwnhqwrkiyu = true;
	int zpcpavq = 4848;
	double irepg = 8127;
	if (4848 != 4848) {
		int mfrkq;
		for (mfrkq = 56; mfrkq > 0; mfrkq--) {
			continue;
		}
	}
	if (4848 != 4848) {
		int olyplia;
		for (olyplia = 22; olyplia > 0; olyplia--) {
			continue;
		}
	}
	if (359 == 359) {
		int bshhjns;
		for (bshhjns = 95; bshhjns > 0; bshhjns--) {
			continue;
		}
	}
	if (4848 == 4848) {
		int br;
		for (br = 59; br > 0; br--) {
			continue;
		}
	}
	if (8127 == 8127) {
		int hkbfgifamx;
		for (hkbfgifamx = 58; hkbfgifamx > 0; hkbfgifamx--) {
			continue;
		}
	}

}

double ntuxmaq::pdcmxplohxqhtr(int brkzgyr, string fxuobh, bool wnrlbfzsectp, int qnznsgpnrmzee) {
	double qwonaeruexyhnjy = 49762;
	int aejgh = 1005;
	double zegvvaurhjjzd = 10680;
	string erjmapagjkmyopl = "nxeesnbjdvknsqzssqzuhsmq";
	bool zbnzmhqiy = false;
	string gwmjarkl = "zdwtwxfrjftcmfvwpqzhyvyafftjazdyjzvctmexsguvvidlbpsfomvspbropmbgfhyblbshwqrsyovczonxolpxjfad";
	string uchjhlzswhevsxq = "sptystokmypcrvujqqcjxiafkifwkytfiglzgpulegmnlwmaabohmslyghmpucukrppjykwbznfgyhrx";
	bool etdbig = false;
	int ggxxgkc = 7592;
	if (1005 == 1005) {
		int az;
		for (az = 82; az > 0; az--) {
			continue;
		}
	}
	if (1005 == 1005) {
		int xnak;
		for (xnak = 100; xnak > 0; xnak--) {
			continue;
		}
	}
	if (7592 != 7592) {
		int hhktbbgxgc;
		for (hhktbbgxgc = 96; hhktbbgxgc > 0; hhktbbgxgc--) {
			continue;
		}
	}
	return 43425;
}

bool ntuxmaq::qizmtcmcxaqgyzyr(bool raajlqy, double adymkv, double csjggevubyeg, int dmfgauxlbkqnm, int lbtqwhzohv) {
	string fmgilvamenlxud = "qekyxskmojvinkpdfrpnzgxksmjyveudqvwsqcfdwvdikzwssljnnwcqf";
	double yfaxj = 5755;
	if (5755 == 5755) {
		int pmmfikw;
		for (pmmfikw = 26; pmmfikw > 0; pmmfikw--) {
			continue;
		}
	}
	if (string("qekyxskmojvinkpdfrpnzgxksmjyveudqvwsqcfdwvdikzwssljnnwcqf") != string("qekyxskmojvinkpdfrpnzgxksmjyveudqvwsqcfdwvdikzwssljnnwcqf")) {
		int lalegclp;
		for (lalegclp = 32; lalegclp > 0; lalegclp--) {
			continue;
		}
	}
	if (5755 == 5755) {
		int jmzzz;
		for (jmzzz = 39; jmzzz > 0; jmzzz--) {
			continue;
		}
	}
	if (string("qekyxskmojvinkpdfrpnzgxksmjyveudqvwsqcfdwvdikzwssljnnwcqf") != string("qekyxskmojvinkpdfrpnzgxksmjyveudqvwsqcfdwvdikzwssljnnwcqf")) {
		int fkhnd;
		for (fkhnd = 33; fkhnd > 0; fkhnd--) {
			continue;
		}
	}
	return false;
}

string ntuxmaq::ahmpxcxrst(bool tpytrejqkmq, bool iqwavzoyyb, bool qjdmxax, double ftjlp) {
	bool attjzypq = true;
	string amcoipxngio = "pfyabau";
	double qtninxqn = 56068;
	bool givgn = false;
	int csnzls = 1654;
	string jknkuzzhlina = "zfqdtycxgsolgcsdvqhqsnjrq";
	string wlbre = "ddcdrqxeygbzsbdwmlbiiwlmickohjn";
	string odalwvlennhg = "oovabrvymslnbygsnbkyoatpgllbmjxmeqejxxgokbelbnbtpjcsjfmdsijosgumsixxxzdtlyuhyehipd";
	bool ftcufipicqbfgi = true;
	if (string("pfyabau") != string("pfyabau")) {
		int rh;
		for (rh = 74; rh > 0; rh--) {
			continue;
		}
	}
	return string("k");
}

void ntuxmaq::xluicvhkmmjgqjb(bool iyudl, int ikkli, int wbqnxt, int aupkmcj, int aancxrnqisqb, int wzlabse, int pemzdyxn) {

}

double ntuxmaq::xaxlkodootccjedh(double tjsmdzaq) {
	string ksddx = "";
	if (string("") == string("")) {
		int ixohsombky;
		for (ixohsombky = 96; ixohsombky > 0; ixohsombky--) {
			continue;
		}
	}
	if (string("") == string("")) {
		int fyxrunv;
		for (fyxrunv = 21; fyxrunv > 0; fyxrunv--) {
			continue;
		}
	}
	if (string("") != string("")) {
		int ow;
		for (ow = 88; ow > 0; ow--) {
			continue;
		}
	}
	return 31367;
}

void ntuxmaq::shwvgduyvnuhw(int jthkaydjphpu, string xavpijaganwdn) {
	bool xnzummrnaoyssc = true;
	bool sbzdqpwiincph = true;
	string mdjkvomxybvjvfr = "nnkckgquavlqybzdnerqotchjjikxhjfhyulxzdiaimuvwwyygiclwumzmuqbjzejqup";
	double diluenbyfsfc = 27340;
	string qfbjkoo = "jrkutqundggwjhecacfwqnekrgwlspgnlnlqohftiqqcntclnjrlapluoibganykktovbtqvdzknuusdsd";
	int aoaqspn = 4969;
	if (4969 != 4969) {
		int kby;
		for (kby = 25; kby > 0; kby--) {
			continue;
		}
	}
	if (27340 != 27340) {
		int xhcndakxxh;
		for (xhcndakxxh = 15; xhcndakxxh > 0; xhcndakxxh--) {
			continue;
		}
	}
	if (true == true) {
		int liqxpyilg;
		for (liqxpyilg = 77; liqxpyilg > 0; liqxpyilg--) {
			continue;
		}
	}
	if (27340 != 27340) {
		int uykuambv;
		for (uykuambv = 17; uykuambv > 0; uykuambv--) {
			continue;
		}
	}
	if (true != true) {
		int qwzcbtg;
		for (qwzcbtg = 80; qwzcbtg > 0; qwzcbtg--) {
			continue;
		}
	}

}

int ntuxmaq::kjfrikzabshtehbguiud(string mbujy, double eqvjiqishrym) {
	double uxdjpjkikjfr = 14717;
	int ptesj = 6579;
	bool psmpt = false;
	double jfdvqxqscbzm = 1898;
	double edxhixejfktmshn = 10827;
	double yqxtplubl = 1518;
	string wbqeudwwjquhuug = "jqstcdqrzohptgqd";
	string ufzvkleg = "vuwhtjvodkqspcwgsbldahhwnbpk";
	string cdgxgbsw = "bmhipkydsgccdkshtbapjdajmngkwugybulaffyrsulyyywozujbbcvnsfjwvhsvuqr";
	int uvcxrtz = 3093;
	if (10827 == 10827) {
		int xjxdmcg;
		for (xjxdmcg = 99; xjxdmcg > 0; xjxdmcg--) {
			continue;
		}
	}
	if (1518 == 1518) {
		int nn;
		for (nn = 23; nn > 0; nn--) {
			continue;
		}
	}
	return 93343;
}

void ntuxmaq::hwebgfzytqz(int dychgkrame, double heblyneikpe, string xhqdakxgnpmb, double tqtnpmkxbyanslx, bool cirxpbufcaevtyp, bool yalslxruc, string ghfphlcutnjyx, double xkcxidglotcjupd, double lprhuhhv, int kllcdnj) {
	string otwesnrnw = "phbezrappnovwfwqu";
	double pvdekmx = 2120;
	double xhlflidpdmuf = 78950;
	int lnwchumxl = 1476;
	bool vzahtyjggrxiap = false;
	bool vwyef = true;
	string xwouoljsejfgnvx = "zpopyxbabgttkpnnszntwlegkhxjpclrrlsuoytzjjsmlwycibotdpll";
	if (string("zpopyxbabgttkpnnszntwlegkhxjpclrrlsuoytzjjsmlwycibotdpll") != string("zpopyxbabgttkpnnszntwlegkhxjpclrrlsuoytzjjsmlwycibotdpll")) {
		int ufq;
		for (ufq = 86; ufq > 0; ufq--) {
			continue;
		}
	}
	if (string("zpopyxbabgttkpnnszntwlegkhxjpclrrlsuoytzjjsmlwycibotdpll") != string("zpopyxbabgttkpnnszntwlegkhxjpclrrlsuoytzjjsmlwycibotdpll")) {
		int bktc;
		for (bktc = 63; bktc > 0; bktc--) {
			continue;
		}
	}

}

void ntuxmaq::bokguvfdyliwpusthd(int hinrgy, bool pgdni, int pfllz, string slcvxnf, bool ibbxgbbpvjvbufe, double tqhuzdfhvnaoo, string xrtzfoqaonihxpa) {
	string qmxmeqlrfkn = "txawbjuvaoopryajnudmgwxfppvncijfmrcavztbryaumpaxbpbkbkbylbvfqihhcmcmceepeawqmgdgottsvqurdlozaebkwq";
	double pukab = 12445;
	bool ukvtvfabeqnczm = true;
	bool xoupfvrmmelt = false;
	string qjstvyihediidjg = "eianfnllfcpalmulgrfyxmadwerulgcprhdyfuqqiupsjcbcmdcmodvswojgkvbude";
	bool gcenmtozsaw = true;
	double hbycnjvs = 44385;
	bool dyudxzn = false;
	if (string("txawbjuvaoopryajnudmgwxfppvncijfmrcavztbryaumpaxbpbkbkbylbvfqihhcmcmceepeawqmgdgottsvqurdlozaebkwq") == string("txawbjuvaoopryajnudmgwxfppvncijfmrcavztbryaumpaxbpbkbkbylbvfqihhcmcmceepeawqmgdgottsvqurdlozaebkwq")) {
		int oirgrgm;
		for (oirgrgm = 9; oirgrgm > 0; oirgrgm--) {
			continue;
		}
	}
	if (12445 != 12445) {
		int bcwrqixxr;
		for (bcwrqixxr = 100; bcwrqixxr > 0; bcwrqixxr--) {
			continue;
		}
	}

}

ntuxmaq::ntuxmaq() {
	this->pdcmxplohxqhtr(8773, string("momjnjllatmjh"), true, 3437);
	this->qizmtcmcxaqgyzyr(true, 67464, 41829, 63, 741);
	this->ahmpxcxrst(false, false, false, 16943);
	this->xluicvhkmmjgqjb(false, 2786, 3989, 397, 8936, 3047, 4608);
	this->xaxlkodootccjedh(15177);
	this->shwvgduyvnuhw(2361, string("hlxjkamyhejeqgvryrvbzfmajcfaimawhapwigphqswhcqarklexynavttgicmpvtgkgofnzassjehlbipendqxsv"));
	this->kjfrikzabshtehbguiud(string("xcuwdatlxznxdtcnfawmvtllzgonguqwq"), 24121);
	this->hwebgfzytqz(1042, 7232, string("y"), 57212, false, true, string("aucqgdwkdoncojfqmnevwfuadxsbimfhzdzserbaeywzlnnsznalamuqqqhxqz"), 10187, 2170, 2037);
	this->bokguvfdyliwpusthd(6183, true, 2831, string("kaqwavdjbxuylamrvmhpkhxzaanopjrnvscozwbzkhyddnrszxjcjeugkiuealxwyyeahfenlusujljrctcmhxaaqta"), false, 27596, string("rtoeepgfrroggpjojvcumca"));
	this->pqenwqjepssbaausnafdobf(1763, true, 11800, 31378, false, 630, 14112);
	this->wxvcvwksiwwelpwpagjaxqj(string("kshupcsdjqnzsstxmcrautbiwxugdxxbx"), false, string("hwirrazmprgdwfqowndrdvhhow"), string("qxrsmvibquueyn"));
	this->zhmtxgdqgxioyegajtlvik(string("mgyajldmrekhazomqondnlqogntipqycthvgnuhcueogqcah"), 2148, string("cgalttoz"));
	this->fadwfvnikpgzrdcyishqji(string("ibuvyiqeytqbmefhoffotfibduczyrgwoyyybphusrynyfnbtxljnbmsqipbbiunwwfsyegksjiypfz"), 13979, false, true, 549, 796, string("wloglddspgrhigguebwuscelzwtjvlbofykirvvdoseemghhdzaokcyyr"));
	this->tfpyioixlsb(3604);
	this->elaosywnnnbhegmtdkwpf(11742, true, 97453, string("dxzdyhckmyivtnvuordkhdsigyu"), 27024, 45940);
	this->tfgfzjwxinzinvzboopg(string("lckcphxcyffltcmdnyngglzstynyqzsizmofjtilclazfynszaikhbfyqfuoznxhbvxmunylvsunerqeh"), false, 428, string("acdlumjkbkvjrcatcijskemceqvmesrpswqwgmuxtwczahiqupdzkbljcxppmhpsurgmjysnmmvtkguthqwdnnagakaqrv"), false, 24235, 32084, false);
	this->fsmkoutwdw(true);
	this->wnpzschikzwzwjthchmqcsn(false, 81944, 42244, string("kuqjlsu"), string("dubvgestcqjbybyozatnzdwzfyhlszpcfmedgoqgzqyuaewdlwugkerurvgnezfuvnsnyfhumoytrvruzcurzxbqtuiapiavdhf"), string("zvocliikcnkkzyunqgwpbyovxdywcnwrcvlymrhvwpnsjxhxh"));
	this->jrblbrbunkfuxgtucc(true, 45547, 289, 1970, 32805, 46, 1022, string("pjlbbsnkmoaejzvxhjrwxpygzccfybnikbddktlhrohx"));
	this->mqjrdojnbdavum(5239, 2623, 20137, string("txfrrhrkiigyrqgyzwoxvldthchvkznczmgynzntloalvsxtuavozxzrccnmesmmqfbgroepuvpvmildcqvgnnuvecfgvyvqnob"), 14039, false, true, string("xowslqdfyfmnurplzazjsestisdhbklpnkbvjgcsfhjrkztizpnrvpsdlyklhgcmzgreyatctyvjhhnukk"), false);
	this->srnntabuonsjexhfhneux(3671);
	this->dzkampgtutqrcjdplk();
	this->vdjwvaqkcvpyhdb();
	this->pksraunskedgucg(20145, 60274, false, 4622, true, false, true);
	this->apgofcofpxlprgotumllri(14515, 1298, 56294, true, string("yioybxybxkiajmdigzfstqmywwibgjgacptvxxyxywxiseiqjcgdcznchblkvemagznxmdjyopahmuwovunamm"), string("jfwwwttwyenwuyjcwesiqlqsalrfokwhvgalgyrfusupdljjjpbkxjatcydlkvswpggghfqdoggiatzlc"), 854, 30087, 377, false);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class huelacu {
public:
	double dmrix;
	int ngpmydnvu;
	string hykuvaqek;
	string zqmstesn;
	int mbgbhhtf;
	huelacu();
	void xalnrtlnchprugh();

protected:
	double vmfllcxrbkzm;
	bool yykifmyv;
	bool pweju;
	string cmgmxc;

	bool raolkjuuihbqqefdthct(string ffaym, string inpdq);

private:
	int rkzgpzg;
	string aruhamfnkqv;
	double djnzapqjritgsf;

	string evtwactmnjcj();
	bool pfbxqsitjoxpvtktpsvu(bool mtvtwzpwhzoxxao, int czvdrmljdk, string ramyqzwcbk, bool pyobrwcugjtb, string mydawwjmximpwx, string ripjti);
	string ebfvxykebxpdiniphvydfwrxi(bool ljhqjzyuqdihici, string namaferezoyqbuc, bool wpubgm, string mlmwuc);
	string iowvjwlxlolpzp(string jtlzxoe, string vomfxhndymcgmpq, double xnjdxb, double civxc, int vbxexovpeagh);
	bool qjabksfkcxqbj();
	bool aqjoahgbcy(int zhnuq, double aubhnt, int sowfmqiuq, double rbsqtucxcje, string ironjhxfrqn, string vwejhayoevextz);
	void bhmvubcladspzyhusx(int zoolkqdgt, double wqfxhwovbyx, bool xgwcxin);
	string dfhivzyabmiujts(bool wgxbnae, bool azyatabcpkvc, string qzhekrewrq, double qkxmssf);

};


string huelacu::evtwactmnjcj() {
	bool cvrvwnjtnvi = true;
	int rgvzixdfvxnglgq = 2473;
	bool ofdgpdy = false;
	int ytsjziqsgqgc = 1338;
	string dzbpzomuvij = "ntsbryafojykvlpnlkvbiwuoqpkznw";
	int fewufhkitu = 519;
	int iixsmjod = 786;
	if (519 == 519) {
		int fcpcftk;
		for (fcpcftk = 31; fcpcftk > 0; fcpcftk--) {
			continue;
		}
	}
	if (2473 != 2473) {
		int hcyo;
		for (hcyo = 1; hcyo > 0; hcyo--) {
			continue;
		}
	}
	if (1338 != 1338) {
		int lrmw;
		for (lrmw = 5; lrmw > 0; lrmw--) {
			continue;
		}
	}
	return string("vsmzipnqdnlwjaja");
}

bool huelacu::pfbxqsitjoxpvtktpsvu(bool mtvtwzpwhzoxxao, int czvdrmljdk, string ramyqzwcbk, bool pyobrwcugjtb, string mydawwjmximpwx, string ripjti) {
	int jwghrgrliqai = 1322;
	int iqyash = 2200;
	int cgbdlbzn = 5367;
	double cnlmwnlseaxfupe = 58408;
	int vjgzayjdhxr = 8731;
	bool hixqzpf = false;
	bool yiustfrln = false;
	if (false != false) {
		int wolgalmnfq;
		for (wolgalmnfq = 57; wolgalmnfq > 0; wolgalmnfq--) {
			continue;
		}
	}
	if (58408 == 58408) {
		int zyaly;
		for (zyaly = 5; zyaly > 0; zyaly--) {
			continue;
		}
	}
	if (2200 == 2200) {
		int muccuqc;
		for (muccuqc = 10; muccuqc > 0; muccuqc--) {
			continue;
		}
	}
	if (false == false) {
		int fffvw;
		for (fffvw = 48; fffvw > 0; fffvw--) {
			continue;
		}
	}
	return false;
}

string huelacu::ebfvxykebxpdiniphvydfwrxi(bool ljhqjzyuqdihici, string namaferezoyqbuc, bool wpubgm, string mlmwuc) {
	string dwooovdqfqowpyl = "hegyltpqdekylwtlkhrpzbsdzhoxupapwviennlpsxmwhgxatzehootdauugrrqhumf";
	string sulszubj = "kdubivzdsrrnzdaqviiqhjroeprczvgvguprjpxvbzhhimlluawsujgfag";
	bool btlndbu = false;
	double junwjuphsn = 37060;
	int iwpssksv = 6645;
	bool ysjxsvlqhvok = false;
	if (string("kdubivzdsrrnzdaqviiqhjroeprczvgvguprjpxvbzhhimlluawsujgfag") != string("kdubivzdsrrnzdaqviiqhjroeprczvgvguprjpxvbzhhimlluawsujgfag")) {
		int njix;
		for (njix = 27; njix > 0; njix--) {
			continue;
		}
	}
	return string("nprebvjrnfdjkmfmzh");
}

string huelacu::iowvjwlxlolpzp(string jtlzxoe, string vomfxhndymcgmpq, double xnjdxb, double civxc, int vbxexovpeagh) {
	bool dihzxfairga = false;
	int dyvuacztjeh = 7054;
	string uwtazwr = "guooe";
	double ryhmprevtfzalm = 36860;
	int wukzucjbeoyqwhx = 266;
	bool itosgc = true;
	double sxsgcmdrxrkug = 42373;
	if (string("guooe") != string("guooe")) {
		int dlypmhpv;
		for (dlypmhpv = 65; dlypmhpv > 0; dlypmhpv--) {
			continue;
		}
	}
	if (42373 == 42373) {
		int hrgp;
		for (hrgp = 63; hrgp > 0; hrgp--) {
			continue;
		}
	}
	if (false == false) {
		int qdmqrrchi;
		for (qdmqrrchi = 100; qdmqrrchi > 0; qdmqrrchi--) {
			continue;
		}
	}
	return string("wmouhwhqgh");
}

bool huelacu::qjabksfkcxqbj() {
	bool qivrgfyd = true;
	if (true == true) {
		int jxkmqot;
		for (jxkmqot = 88; jxkmqot > 0; jxkmqot--) {
			continue;
		}
	}
	if (true == true) {
		int grleej;
		for (grleej = 71; grleej > 0; grleej--) {
			continue;
		}
	}
	if (true != true) {
		int yhwjewakd;
		for (yhwjewakd = 90; yhwjewakd > 0; yhwjewakd--) {
			continue;
		}
	}
	if (true == true) {
		int gykibu;
		for (gykibu = 82; gykibu > 0; gykibu--) {
			continue;
		}
	}
	return true;
}

bool huelacu::aqjoahgbcy(int zhnuq, double aubhnt, int sowfmqiuq, double rbsqtucxcje, string ironjhxfrqn, string vwejhayoevextz) {
	return false;
}

void huelacu::bhmvubcladspzyhusx(int zoolkqdgt, double wqfxhwovbyx, bool xgwcxin) {
	bool fwnvqwlsh = true;
	double pmzkwus = 46676;
	string ylyddivcaotvir = "seypnyiylmolkrowgusvghypqrteuwrebnkripxipqrpwqjnmlibclerwrfftfpyzitzfuqadftgvhsqjdwcbh";
	int pytunhbt = 4504;
	bool vquxsddzlxhrmd = true;
	bool aqhromszvxns = false;

}

string huelacu::dfhivzyabmiujts(bool wgxbnae, bool azyatabcpkvc, string qzhekrewrq, double qkxmssf) {
	bool gcveqlatjks = true;
	int iwmkvrzszabc = 1886;
	int mgcuhzggdcv = 7600;
	if (7600 != 7600) {
		int scwyzspaky;
		for (scwyzspaky = 85; scwyzspaky > 0; scwyzspaky--) {
			continue;
		}
	}
	if (1886 != 1886) {
		int piecyuk;
		for (piecyuk = 73; piecyuk > 0; piecyuk--) {
			continue;
		}
	}
	if (1886 == 1886) {
		int ozqrqrrsvk;
		for (ozqrqrrsvk = 85; ozqrqrrsvk > 0; ozqrqrrsvk--) {
			continue;
		}
	}
	if (7600 == 7600) {
		int gafi;
		for (gafi = 88; gafi > 0; gafi--) {
			continue;
		}
	}
	return string("o");
}

bool huelacu::raolkjuuihbqqefdthct(string ffaym, string inpdq) {
	bool hthlahrrtk = true;
	int uckveeozla = 5313;
	double exfeyaw = 25556;
	double kaputjflstretcu = 7800;
	string juvrlezslm = "fqyjkyntnnurpry";
	bool lazdvbwj = false;
	bool aweobktpmq = true;
	if (25556 == 25556) {
		int ldav;
		for (ldav = 77; ldav > 0; ldav--) {
			continue;
		}
	}
	if (7800 == 7800) {
		int ljajsyrly;
		for (ljajsyrly = 71; ljajsyrly > 0; ljajsyrly--) {
			continue;
		}
	}
	return false;
}

void huelacu::xalnrtlnchprugh() {

}

huelacu::huelacu() {
	this->xalnrtlnchprugh();
	this->raolkjuuihbqqefdthct(string("glcvqjzcizjbokysqhnuzortmkehzumzgtkkulkhslfnmdbpwoguccdx"), string("tyyglykunmywjodvmzcqptieljsyugkzbqsdswilimtogfnigjjsmoxepoyytiarsfzsgbfkrasixdvgfoyqezdoaverrz"));
	this->evtwactmnjcj();
	this->pfbxqsitjoxpvtktpsvu(false, 1115, string("zdcyefpzjgxebriwayicgcosahhjpcdzzqxgjjwpugkhtabgjmauwoetwzxsjrwgxjmicdeohszu"), true, string("gzajlmojtgxiyfvtg"), string("ekdjwtsznokwyncfalzfeuzrmscznllzeslnjyubzbaschvgjqrsvpndgvsipvvkvoxenjtbn"));
	this->ebfvxykebxpdiniphvydfwrxi(false, string("yienhsboehokekkynntrzcsxrdbzdilqwzvkpwktyqmtixkufnifl"), false, string("chzymrkrmkignwiuypdopkpvjhgllrkuxgygbuezzkqjkmhiqoyijdptjdhdhwkpxfy"));
	this->iowvjwlxlolpzp(string("kuswd"), string("rjdn"), 62921, 76377, 7139);
	this->qjabksfkcxqbj();
	this->aqjoahgbcy(2136, 22915, 3829, 3797, string("jswhblsjqajcvzikwhtrklrnffbigvwjf"), string("uvesseahgmlechbqbojjgbkgzemaovjmasnezscjajiwrtsfobqoiysdupoqldvprgvphdhrxjgbwucedyfepdeslf"));
	this->bhmvubcladspzyhusx(3957, 15547, false);
	this->dfhivzyabmiujts(false, false, string("exksqfxjtucdtldunlyjwaizaevpqiaycmihvwtrxudvhsihgosywwgu"), 8406);
}
