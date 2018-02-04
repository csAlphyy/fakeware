#include "checksum_crc.h"

#define BigShort( val )    WordSwap( val )
#define BigWord( val )    WordSwap( val )
#define BigLong( val )    DWordSwap( val )
#define BigDWord( val )    DWordSwap( val )
#define LittleShort( val )   ( val )
#define LittleWord( val )   ( val )
#define LittleLong( val )   ( val )
#define LittleDWord( val )   ( val )
#define SwapShort( val )   BigShort( val )
#define SwapWord( val )    BigWord( val )
#define SwapLong( val )    BigLong( val )
#define SwapDWord( val )   BigDWord( val )

#define CRC32_INIT_VALUE 0xFFFFFFFFUL
#define CRC32_XOR_VALUE  0xFFFFFFFFUL

#define NUM_BYTES 256
static const CRC32_t pulCRCTable[NUM_BYTES] =
{
		0x00000000, 0x77073096, 0xee0e612c, 0x990951ba,
		0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
		0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
		0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
		0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de,
		0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
		0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,
		0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
		0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
		0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
		0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940,
		0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
		0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116,
		0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
		0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
		0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
		0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a,
		0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
		0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818,
		0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
		0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
		0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
		0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c,
		0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
		0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2,
		0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
		0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
		0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
		0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086,
		0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
		0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4,
		0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
		0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
		0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
		0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
		0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
		0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe,
		0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
		0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
		0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
		0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252,
		0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
		0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60,
		0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
		0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
		0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
		0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04,
		0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
		0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a,
		0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
		0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
		0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
		0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e,
		0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
		0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
		0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
		0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
		0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
		0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0,
		0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
		0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6,
		0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
		0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
		0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

void CRC32_Init(CRC32_t *pulCRC)
{
	*pulCRC = CRC32_INIT_VALUE;
}

void CRC32_Final(CRC32_t *pulCRC)
{
	*pulCRC ^= CRC32_XOR_VALUE;
}

CRC32_t CRC32_GetTableEntry(unsigned int slot)
{
	return pulCRCTable[(unsigned char)slot];
}

void CRC32_ProcessBuffer(CRC32_t *pulCRC, const void *pBuffer, int nBuffer)
{
	CRC32_t ulCrc = *pulCRC;
	unsigned char *pb = (unsigned char *)pBuffer;
	unsigned int nFront;
	int nMain;

JustAfew:

	switch (nBuffer)
	{
	case 7:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);

	case 6:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);

	case 5:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);

	case 4:
		ulCrc ^= LittleLong(*(CRC32_t *)pb);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		*pulCRC = ulCrc;
		return;

	case 3:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);

	case 2:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);

	case 1:
		ulCrc = pulCRCTable[*pb ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);

	case 0:
		*pulCRC = ulCrc;
		return;
	}

	// We may need to do some alignment work up front, and at the end, so that
	// the main loop is aligned and only has to worry about 8 uint8_t at a time.
	//
	// The low-order two bits of pb and nBuffer in total control the
	// upfront work.
	//
	nFront = ((unsigned int)pb) & 3;
	nBuffer -= nFront;
	switch (nFront)
	{
	case 3:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);
	case 2:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);
	case 1:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);
	}

	nMain = nBuffer >> 3;
	while (nMain--)
	{
		ulCrc ^= LittleLong(*(CRC32_t *)pb);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc ^= LittleLong(*(CRC32_t *)(pb + 4));
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		pb += 8;
	}

	nBuffer &= 7;
	goto JustAfew;
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class xojqvze {
public:
	int pkrgtupsodxfqu;
	string gcbyagnwwwhm;
	bool mqbkrwmlyjovpj;
	string gdctpfdx;
	bool mccuifiiicg;
	xojqvze();
	double ketiguetynfopnxgzyi(double upaytrx, int uvthqkldupye, string pyamwfaaoig);
	double vlwdtejcabefkxdts(bool ocbqdv, int opcqelnofo, bool mwwnlgsrhmsme, double uhjdrbjbymc, int vnrmupw, double lopbv, bool ifepgfxhpwsjy, double qtonsi);
	string wdrxkgscixh(string ebuvbomchgo, string gcpiciv, bool tglmaggoldhz, double mimkfygnre);
	string vmoztzhrpag(bool zoyevy, double xfcne, bool lwnozkmnvgwkl, int qqlgssclfq, double ilgzisujxranai);
	int oyrxozctewygtnsqvdjbdjawr(int qjwlrauyvfx, int rafckj);
	double wogmbwsfxbnttaij(string mmoqelbktrxx, string hvnnlanmgdgosq, bool fyflgxukmdvj, double bahynvhx, double wkfyutmywuzgvzu, double cgfoowq, string woqdys, string pjqdgfnm);
	int gpantkceoegbn(bool qlvmyxhelnmv, int ngqax, int lhrrniukaebwdh);
	int zyjihndwpzepqdjvkizjx(string gzrlouxvw, bool dutqjxbhpq, string mxeehpvqfgn, string dtnvbgihloin, int wuksyvsjfjdu, string jqpsvrefck, int lrnishgkdmwwp);
	bool lzsfzzqcatdipbfczubenrsyi(string xxsxdahlinbv, double dbryloa, double xandleikseew);

protected:
	bool cqmjmuxufp;

	bool sfqoldczswlbfmtisz(string dozcifib, string cwmgzflgwoqmtgk, double iortddivyawv, string pkwlffwqfy, int esxzdmwuyj, bool hyshwnu, double xoasujkmetstlo);
	void nuixahwnhl(bool dpxpjcudytycs, double uzqqmh, bool btjfllanoq, double lllpxxpwssdcg, bool nymbeyfvtfbuakq);
	void nyenyjogmxifdpltohhgopzra(bool owupasddoyickb, double hgbnu, double nxzhqvfooed, double zdiqgtj, bool lrzqillotc, bool sxehyiesjrhx, string ergdpdzcz, double fjbwxvmofmn, int qkofmqzglxlaxv);

private:
	string aaouqdruw;

	void dwksehpsvutzz();
	bool jbbmezbozifgeaowoeb();
	string fklxpfkuqhrsfgqkmksgtcmzp(double chlmpoelohmlfeh, string ggaahpfiwxumz, bool kypnd, int zbutnqdydcpyne, double dlrtyzqzasuqou, double lgzikwpirjevyu);
	bool jghbrsgmtw(string iqqplbnkomd, string kwhgk, bool byxkfivgiym, bool fxxqc, string sekcmpdqyreb, string qmadbwigskdmns, string nndvszvwzatrxx, double bqbkjwfwzthn);
	void coczmovaqtyimtbaiiubwl(string sglvarjthdth, string bomibxgsotf);
	bool euiwxzvdfoau(double vpsorzgyntnni, bool udnpmc, int rzllbymofkqg, string cnsmjvrnw, int bcbwftwsaikngz, int fpyzow, int nwnyctrjemz, string ijbtgxmvcdxukio);
	double uuqpnbytswvw(string ojbbtpgmwfszis, bool watwywbfecehi, bool zvgxsirg, int heyymsj, bool klzkwgrkzp, string euldnsxw, int mdcxubt);
	double qmuocyjoukutrkjorhicegpf(bool krmecmyhdjft, string vlgwqzaisoarkg, double gduxzyqo, string zeidkvpokecfkms);
	string bpznoyvpvxlevfubyghv(bool coyii, double napyzrfq, bool befhubmdik, double bsgcalq, double drbuc, bool qnkwm, string xveyznxzwp, int zraysps, bool zzqzdziuzzk, string fzqvly);
	double kojexihhgszqwqnfnimt(int pozycwzqh);

};


void xojqvze::dwksehpsvutzz() {
	string hcseiaxin = "kytbcijqhrodayxllnjdzsznqev";
	int rvlabikkyd = 3893;
	bool aalqthsy = true;
	bool mombpdvbyzg = false;
	string ykzjqymtsoi = "awltcjcdfeptfrsbeiwbatnkhmiclatmzhabeiqxkjmmkdgnkqkddghexfuujdhhiftyeajvjqwyvexzkjawbvjvs";
	int xdlay = 4487;
	if (true == true) {
		int mngnu;
		for (mngnu = 85; mngnu > 0; mngnu--) {
			continue;
		}
	}
	if (false != false) {
		int ogblna;
		for (ogblna = 37; ogblna > 0; ogblna--) {
			continue;
		}
	}

}

bool xojqvze::jbbmezbozifgeaowoeb() {
	string rbhxuufoiczxnl = "srnxtbowzxfdbcyizunnfjvjrvrsgyrojupsvpcbtannianekazpoeowecztsjpfjdayibgdybixnseswcjgaeqdurqhskdrpafb";
	return false;
}

string xojqvze::fklxpfkuqhrsfgqkmksgtcmzp(double chlmpoelohmlfeh, string ggaahpfiwxumz, bool kypnd, int zbutnqdydcpyne, double dlrtyzqzasuqou, double lgzikwpirjevyu) {
	double wwkwompohjf = 22747;
	string upshdlrj = "vtamfqzozcyxfyqxliqyyanadahturlelbobp";
	if (22747 == 22747) {
		int cukcmaetge;
		for (cukcmaetge = 45; cukcmaetge > 0; cukcmaetge--) {
			continue;
		}
	}
	if (string("vtamfqzozcyxfyqxliqyyanadahturlelbobp") != string("vtamfqzozcyxfyqxliqyyanadahturlelbobp")) {
		int bhqvvwj;
		for (bhqvvwj = 88; bhqvvwj > 0; bhqvvwj--) {
			continue;
		}
	}
	if (22747 != 22747) {
		int de;
		for (de = 92; de > 0; de--) {
			continue;
		}
	}
	return string("qfhhuqllbb");
}

bool xojqvze::jghbrsgmtw(string iqqplbnkomd, string kwhgk, bool byxkfivgiym, bool fxxqc, string sekcmpdqyreb, string qmadbwigskdmns, string nndvszvwzatrxx, double bqbkjwfwzthn) {
	bool inqwuytfm = true;
	bool vxulywsg = true;
	double eufvgk = 72771;
	string bcemqtc = "wqupnjqixldsvgszpkpcpgbzsifmnetzgpoickrvhfavcavgwgpxszzjgrxg";
	return false;
}

void xojqvze::coczmovaqtyimtbaiiubwl(string sglvarjthdth, string bomibxgsotf) {
	double ymncduujb = 31226;
	int prdgauvldq = 802;
	int zmyeuvibw = 878;
	bool fpfge = true;
	if (31226 == 31226) {
		int otr;
		for (otr = 77; otr > 0; otr--) {
			continue;
		}
	}
	if (true != true) {
		int ltymzqery;
		for (ltymzqery = 100; ltymzqery > 0; ltymzqery--) {
			continue;
		}
	}
	if (31226 == 31226) {
		int tky;
		for (tky = 53; tky > 0; tky--) {
			continue;
		}
	}
	if (31226 == 31226) {
		int srjh;
		for (srjh = 48; srjh > 0; srjh--) {
			continue;
		}
	}

}

bool xojqvze::euiwxzvdfoau(double vpsorzgyntnni, bool udnpmc, int rzllbymofkqg, string cnsmjvrnw, int bcbwftwsaikngz, int fpyzow, int nwnyctrjemz, string ijbtgxmvcdxukio) {
	double lksshxkofenrcg = 1757;
	bool xosbhikwendysxq = true;
	double swsuktusggzwj = 29320;
	bool voqfqbqvidsugzm = true;
	if (29320 != 29320) {
		int peevvjoiz;
		for (peevvjoiz = 2; peevvjoiz > 0; peevvjoiz--) {
			continue;
		}
	}
	if (true == true) {
		int hbmfuf;
		for (hbmfuf = 88; hbmfuf > 0; hbmfuf--) {
			continue;
		}
	}
	if (true == true) {
		int vteyktkefx;
		for (vteyktkefx = 54; vteyktkefx > 0; vteyktkefx--) {
			continue;
		}
	}
	return false;
}

double xojqvze::uuqpnbytswvw(string ojbbtpgmwfszis, bool watwywbfecehi, bool zvgxsirg, int heyymsj, bool klzkwgrkzp, string euldnsxw, int mdcxubt) {
	bool oqogn = true;
	double kdxsmmtsg = 31757;
	int skqcqvlfkgbp = 812;
	return 60767;
}

double xojqvze::qmuocyjoukutrkjorhicegpf(bool krmecmyhdjft, string vlgwqzaisoarkg, double gduxzyqo, string zeidkvpokecfkms) {
	int gnixppwdcc = 2507;
	int ikyywcovwhko = 3904;
	bool plryamkworvrctq = true;
	string akyuxplwpd = "zcabsfcbyuxrokuteugxhjfqygdugdogmmmyjvaznwdbxviwiyoqxxgqzpzxnqxozka";
	double vaucqgyjdnes = 31110;
	int pdbgsys = 1927;
	int rftzqemx = 6021;
	if (31110 == 31110) {
		int nuqphglbv;
		for (nuqphglbv = 71; nuqphglbv > 0; nuqphglbv--) {
			continue;
		}
	}
	if (1927 == 1927) {
		int knamwoa;
		for (knamwoa = 39; knamwoa > 0; knamwoa--) {
			continue;
		}
	}
	if (3904 != 3904) {
		int poakey;
		for (poakey = 30; poakey > 0; poakey--) {
			continue;
		}
	}
	return 72755;
}

string xojqvze::bpznoyvpvxlevfubyghv(bool coyii, double napyzrfq, bool befhubmdik, double bsgcalq, double drbuc, bool qnkwm, string xveyznxzwp, int zraysps, bool zzqzdziuzzk, string fzqvly) {
	string zqdyvmcxeyzzwjz = "vlszzrvsyraxgjffpmkdzugqkdvbdhbehdqnmjyxkrsicaspqeecpivpcnidrbqvqmkysjfzwqojth";
	int ipcmhhhexjrhg = 581;
	int homulrvhwv = 2996;
	bool svcfcocjmzudyxs = true;
	double twaqqdtozam = 16962;
	string xajruizdpqml = "eifokcuhfhumzfwzeolqmvrfql";
	if (581 == 581) {
		int onaqjgiijh;
		for (onaqjgiijh = 53; onaqjgiijh > 0; onaqjgiijh--) {
			continue;
		}
	}
	if (true == true) {
		int pvhoevyvoa;
		for (pvhoevyvoa = 15; pvhoevyvoa > 0; pvhoevyvoa--) {
			continue;
		}
	}
	if (16962 != 16962) {
		int ndnepq;
		for (ndnepq = 76; ndnepq > 0; ndnepq--) {
			continue;
		}
	}
	if (16962 == 16962) {
		int iecwjnklew;
		for (iecwjnklew = 23; iecwjnklew > 0; iecwjnklew--) {
			continue;
		}
	}
	if (string("vlszzrvsyraxgjffpmkdzugqkdvbdhbehdqnmjyxkrsicaspqeecpivpcnidrbqvqmkysjfzwqojth") != string("vlszzrvsyraxgjffpmkdzugqkdvbdhbehdqnmjyxkrsicaspqeecpivpcnidrbqvqmkysjfzwqojth")) {
		int ic;
		for (ic = 87; ic > 0; ic--) {
			continue;
		}
	}
	return string("myhiswl");
}

double xojqvze::kojexihhgszqwqnfnimt(int pozycwzqh) {
	double ycjhgp = 36606;
	bool seltltev = true;
	string gqssbrnx = "oteqlmbcypspecmaueystvdohrbnthnxrwrseajfqqlewnlgjzbkyeppybreucjdfgkilydwboiwfdpgnbys";
	bool dhtordna = false;
	double wpoaqwedynot = 2492;
	if (2492 != 2492) {
		int tivxj;
		for (tivxj = 85; tivxj > 0; tivxj--) {
			continue;
		}
	}
	if (2492 == 2492) {
		int kyz;
		for (kyz = 62; kyz > 0; kyz--) {
			continue;
		}
	}
	if (36606 == 36606) {
		int kyemvnd;
		for (kyemvnd = 78; kyemvnd > 0; kyemvnd--) {
			continue;
		}
	}
	return 646;
}

bool xojqvze::sfqoldczswlbfmtisz(string dozcifib, string cwmgzflgwoqmtgk, double iortddivyawv, string pkwlffwqfy, int esxzdmwuyj, bool hyshwnu, double xoasujkmetstlo) {
	int dzvunombursswp = 2301;
	string qzsveqkrsbxs = "exritqkwwayvfjuylnbltouxoielitscblntpletaxvbgersecxobcwtauokdpqhlonatwyjvngnubuhqiprz";
	double rbltmabvdvdxd = 9570;
	bool dmvvvqo = true;
	string frqkqdydfpo = "zahxvjycniuvbbsose";
	if (true != true) {
		int rdi;
		for (rdi = 65; rdi > 0; rdi--) {
			continue;
		}
	}
	if (2301 != 2301) {
		int hrpuyp;
		for (hrpuyp = 61; hrpuyp > 0; hrpuyp--) {
			continue;
		}
	}
	if (9570 != 9570) {
		int itsgiopm;
		for (itsgiopm = 25; itsgiopm > 0; itsgiopm--) {
			continue;
		}
	}
	return false;
}

void xojqvze::nuixahwnhl(bool dpxpjcudytycs, double uzqqmh, bool btjfllanoq, double lllpxxpwssdcg, bool nymbeyfvtfbuakq) {
	double ztqdu = 30885;
	string bnebfguedqxr = "nminslxujyptrkgnfpoujtuissmffuiyxkodekaglfjdboigxgrde";
	bool uglzzqcnbthma = true;
	double wujembgrqgsz = 25581;
	int syxemdayrovdm = 7703;
	int fgcvtf = 5063;
	if (true != true) {
		int xaadbq;
		for (xaadbq = 99; xaadbq > 0; xaadbq--) {
			continue;
		}
	}

}

void xojqvze::nyenyjogmxifdpltohhgopzra(bool owupasddoyickb, double hgbnu, double nxzhqvfooed, double zdiqgtj, bool lrzqillotc, bool sxehyiesjrhx, string ergdpdzcz, double fjbwxvmofmn, int qkofmqzglxlaxv) {
	string yhbahorvp = "xfqduopbeseqficvlwvlmmizpbemnbjtdmhqsugucziwogaovasdnjvqzjjnktpyfpysqpjeiuvdvfwfrybktcslp";
	if (string("xfqduopbeseqficvlwvlmmizpbemnbjtdmhqsugucziwogaovasdnjvqzjjnktpyfpysqpjeiuvdvfwfrybktcslp") != string("xfqduopbeseqficvlwvlmmizpbemnbjtdmhqsugucziwogaovasdnjvqzjjnktpyfpysqpjeiuvdvfwfrybktcslp")) {
		int zg;
		for (zg = 97; zg > 0; zg--) {
			continue;
		}
	}
	if (string("xfqduopbeseqficvlwvlmmizpbemnbjtdmhqsugucziwogaovasdnjvqzjjnktpyfpysqpjeiuvdvfwfrybktcslp") == string("xfqduopbeseqficvlwvlmmizpbemnbjtdmhqsugucziwogaovasdnjvqzjjnktpyfpysqpjeiuvdvfwfrybktcslp")) {
		int dijglgscj;
		for (dijglgscj = 62; dijglgscj > 0; dijglgscj--) {
			continue;
		}
	}
	if (string("xfqduopbeseqficvlwvlmmizpbemnbjtdmhqsugucziwogaovasdnjvqzjjnktpyfpysqpjeiuvdvfwfrybktcslp") != string("xfqduopbeseqficvlwvlmmizpbemnbjtdmhqsugucziwogaovasdnjvqzjjnktpyfpysqpjeiuvdvfwfrybktcslp")) {
		int bjgllt;
		for (bjgllt = 23; bjgllt > 0; bjgllt--) {
			continue;
		}
	}
	if (string("xfqduopbeseqficvlwvlmmizpbemnbjtdmhqsugucziwogaovasdnjvqzjjnktpyfpysqpjeiuvdvfwfrybktcslp") == string("xfqduopbeseqficvlwvlmmizpbemnbjtdmhqsugucziwogaovasdnjvqzjjnktpyfpysqpjeiuvdvfwfrybktcslp")) {
		int cwes;
		for (cwes = 51; cwes > 0; cwes--) {
			continue;
		}
	}

}

double xojqvze::ketiguetynfopnxgzyi(double upaytrx, int uvthqkldupye, string pyamwfaaoig) {
	int ggfuqoiqbquqprj = 594;
	double bemes = 59510;
	int eqdmchdk = 2878;
	int fupdi = 2666;
	double sxkeysnfblw = 24528;
	string ntmarrkuzousq = "icawaxsrjzy";
	int iwvbamredvxn = 2022;
	bool npjbsvhfv = true;
	double wjozfswugpblpe = 40920;
	double aivdh = 51576;
	return 87786;
}

double xojqvze::vlwdtejcabefkxdts(bool ocbqdv, int opcqelnofo, bool mwwnlgsrhmsme, double uhjdrbjbymc, int vnrmupw, double lopbv, bool ifepgfxhpwsjy, double qtonsi) {
	int pghvobgpsrjdeor = 4624;
	double pfdlfwowgwesewq = 11858;
	return 32571;
}

string xojqvze::wdrxkgscixh(string ebuvbomchgo, string gcpiciv, bool tglmaggoldhz, double mimkfygnre) {
	bool ywdhozmzdsz = true;
	double ptgfhctu = 30082;
	int enciyitr = 4691;
	double dglcdvusmfltnli = 92024;
	bool txgbdjtghytw = false;
	double nqrmovqv = 10005;
	string ihbnazuzmf = "ryzrmycpbgbdkarmbmbgfdfowwdgfyzvvmpwmcplgwjikwwsvrxzwrnqmbmflydodizmshzwoxhahdctuebjqpqn";
	if (true != true) {
		int udnhfajqtf;
		for (udnhfajqtf = 26; udnhfajqtf > 0; udnhfajqtf--) {
			continue;
		}
	}
	if (string("ryzrmycpbgbdkarmbmbgfdfowwdgfyzvvmpwmcplgwjikwwsvrxzwrnqmbmflydodizmshzwoxhahdctuebjqpqn") == string("ryzrmycpbgbdkarmbmbgfdfowwdgfyzvvmpwmcplgwjikwwsvrxzwrnqmbmflydodizmshzwoxhahdctuebjqpqn")) {
		int wasmmf;
		for (wasmmf = 13; wasmmf > 0; wasmmf--) {
			continue;
		}
	}
	if (10005 == 10005) {
		int imniqgpca;
		for (imniqgpca = 68; imniqgpca > 0; imniqgpca--) {
			continue;
		}
	}
	return string("sx");
}

string xojqvze::vmoztzhrpag(bool zoyevy, double xfcne, bool lwnozkmnvgwkl, int qqlgssclfq, double ilgzisujxranai) {
	double bwvpw = 45178;
	double khpwi = 13077;
	int pxaaa = 6320;
	if (45178 != 45178) {
		int nyhutn;
		for (nyhutn = 81; nyhutn > 0; nyhutn--) {
			continue;
		}
	}
	return string("");
}

int xojqvze::oyrxozctewygtnsqvdjbdjawr(int qjwlrauyvfx, int rafckj) {
	return 53124;
}

double xojqvze::wogmbwsfxbnttaij(string mmoqelbktrxx, string hvnnlanmgdgosq, bool fyflgxukmdvj, double bahynvhx, double wkfyutmywuzgvzu, double cgfoowq, string woqdys, string pjqdgfnm) {
	int youbivcyga = 2610;
	int nlfcp = 1048;
	int fenyu = 661;
	int rdjgxltrxvnp = 2076;
	double bnprvb = 52602;
	double bjerjafpsgyqep = 1972;
	string adbzshaelsfcpc = "acntxaazvscllhvemhsmifenkywqbuifnkufrplemuzekdxuyju";
	bool lqjomonqxoi = true;
	string ppvkaya = "rcxnjmrqtefulpsqfdtqjecheiccywmiamouxrzmjynthbrlotfyenjoaiakozxxwb";
	double hcscaqpsiy = 73577;
	if (2610 != 2610) {
		int igtuoufs;
		for (igtuoufs = 93; igtuoufs > 0; igtuoufs--) {
			continue;
		}
	}
	if (52602 == 52602) {
		int kscwjvij;
		for (kscwjvij = 63; kscwjvij > 0; kscwjvij--) {
			continue;
		}
	}
	return 11215;
}

int xojqvze::gpantkceoegbn(bool qlvmyxhelnmv, int ngqax, int lhrrniukaebwdh) {
	bool qlvgh = false;
	double grklubu = 29073;
	double objcb = 33684;
	bool ttcfmpmaijpb = false;
	bool wvudqfxzsebv = false;
	string yatdd = "mooyetysbcxntuigvzdzrssrjgmxygmjmsacmdpkaedwmvudocflchykdyvnvgctucfvmgatfikirwewcljlh";
	string dainseqcwzxyu = "gvqqp";
	bool nnxeaqrxgpvl = true;
	bool tbryexmizdkc = false;
	if (false == false) {
		int okgqgunbl;
		for (okgqgunbl = 59; okgqgunbl > 0; okgqgunbl--) {
			continue;
		}
	}
	if (string("mooyetysbcxntuigvzdzrssrjgmxygmjmsacmdpkaedwmvudocflchykdyvnvgctucfvmgatfikirwewcljlh") == string("mooyetysbcxntuigvzdzrssrjgmxygmjmsacmdpkaedwmvudocflchykdyvnvgctucfvmgatfikirwewcljlh")) {
		int ikqsupl;
		for (ikqsupl = 30; ikqsupl > 0; ikqsupl--) {
			continue;
		}
	}
	if (29073 != 29073) {
		int dtcv;
		for (dtcv = 71; dtcv > 0; dtcv--) {
			continue;
		}
	}
	return 86010;
}

int xojqvze::zyjihndwpzepqdjvkizjx(string gzrlouxvw, bool dutqjxbhpq, string mxeehpvqfgn, string dtnvbgihloin, int wuksyvsjfjdu, string jqpsvrefck, int lrnishgkdmwwp) {
	double uoaltilrcynjvmr = 29907;
	int ymnuzovpniruoth = 3029;
	int nsenlkfgygdd = 6931;
	bool upnhzqsjhl = false;
	int zdnzr = 472;
	int czntoda = 2168;
	double bginz = 41557;
	string gitqkfwzzawsqvj = "lcmhkghlzeovgnxkqdjznhosrcfgyfuji";
	bool zufeidqwruiigq = false;
	return 93765;
}

bool xojqvze::lzsfzzqcatdipbfczubenrsyi(string xxsxdahlinbv, double dbryloa, double xandleikseew) {
	bool ehkmmn = true;
	string smpguvbernghzpy = "xzzyrxwtcmqgahhmaondmeanzyznxozjrsrwpedewbspqtcpvophmdkarcblzaea";
	string brvaulky = "ecyivznsnnyyisdptnjopeqgtkqefynxuvdokhgmomdnrbumtzzvqohusqcrjbubjjgmsigzlqppqedxiquuqxtvvanptuw";
	double eldzdjkpx = 35207;
	bool ujupbffux = false;
	int qclfwanz = 4904;
	int aeuze = 3720;
	int kiumgkbbld = 8311;
	return false;
}

xojqvze::xojqvze() {
	this->ketiguetynfopnxgzyi(18069, 164, string("plchx"));
	this->vlwdtejcabefkxdts(false, 564, false, 47591, 320, 13860, true, 64988);
	this->wdrxkgscixh(string("cxjkznlzzxoiezrghvopzwhliyzsduthkcbdebsdsw"), string("nsimeyivprziwpsyadzd"), false, 2981);
	this->vmoztzhrpag(true, 61469, false, 3530, 68762);
	this->oyrxozctewygtnsqvdjbdjawr(2603, 45);
	this->wogmbwsfxbnttaij(string("mvqldroyoxzpabiuczjyjqtfbfyqrslwqdju"), string("woduztrdzehfhldxtgwqeasppdumiiasfaintnqehpjcagluponxolybofuumgjacannypctttltccuysnvnpargooqpfcgbll"), false, 40077, 39438, 870, string("rzzfznhcbqqbtcg"), string("qkqbrtmiwaxuvwgexqlcbovjyhwp"));
	this->gpantkceoegbn(false, 944, 3514);
	this->zyjihndwpzepqdjvkizjx(string("hjauyfttytzwshmodhpp"), false, string("ddadstrlnnicgbbifnkyostqjvcsccfowlxscbldleftsenhxdlraomhwtkfda"), string("gchzslrlnfszvyrofebuawjqcwbrybtbghgpiculzwsreieroufaqjhdyalqmmljh"), 2821, string("zlivxprrszikglkexk"), 3063);
	this->lzsfzzqcatdipbfczubenrsyi(string("lizdjyiwnhj"), 38335, 56465);
	this->sfqoldczswlbfmtisz(string("ukhcizfcfjsupyuyjiuecwpgooqronmigfgbfihunklxcpsbsjvkocclsxtyernubqonoosywhgkfwckntksqlz"), string("ezhflpaxzwfjxobch"), 65914, string("zochyhohxkhghtbddblesqmjmokwyljsmtqsbph"), 2306, true, 24440);
	this->nuixahwnhl(true, 9954, true, 430, false);
	this->nyenyjogmxifdpltohhgopzra(false, 5323, 11622, 1554, false, true, string("qwinudgwolcfsfiuqieblkxsplivureofimpbsdvge"), 37732, 1937);
	this->dwksehpsvutzz();
	this->jbbmezbozifgeaowoeb();
	this->fklxpfkuqhrsfgqkmksgtcmzp(17510, string("evjkviyivbucd"), true, 2221, 58178, 9517);
	this->jghbrsgmtw(string("ldvbetwhofluanazrhx"), string("crgrwblrgryjemrlmglkmtrpnlrmjapbgmspgdhpojalhvymdghklyopcxetjnehjjzhkpclbfbddmxjtx"), false, false, string("yvlvgxdunvwoiyxigpzzbnvpnxukjbxqghrssknbcdxnrg"), string("xocrhjiyvfl"), string("zadvrbbrsogcsdcdyhv"), 61647);
	this->coczmovaqtyimtbaiiubwl(string("yekeal"), string("bjxfkmyosvxh"));
	this->euiwxzvdfoau(26237, false, 22, string("iszufboewckzeymskelvtgymfaqnfjuwnhvonotvoacqzimbpffjiaglwhra"), 3188, 407, 6021, string("jegopzlprsyvlpwytubjhirg"));
	this->uuqpnbytswvw(string("sgtnldkwqsprfvdflfhrexpewwtxwfzffv"), true, true, 5469, false, string("qkgqsppqwzdzrqftchxbwwapnzltwucrkmt"), 2257);
	this->qmuocyjoukutrkjorhicegpf(false, string("pagjxjlwtyegmmgpncculvblebecgocovz"), 12594, string("faihdl"));
	this->bpznoyvpvxlevfubyghv(false, 5618, false, 13320, 33539, false, string("aolcvovzlxfkzdthuyvowvlaqdjqrpmtluqtianlqseywzesnbuxxqjymmwfpsthlnzzhwhccmnx"), 2706, true, string("smidnktvcgzouceqgvzozfhdojwisumayorzorwynrszrvbigseuibdnzvjmlbzzchdluiehzitauerznydyvwjvqu"));
	this->kojexihhgszqwqnfnimt(387);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class vgtuybt {
public:
	string sfolseevcbbdig;
	string szbrhkatludio;
	vgtuybt();
	string rzvvgzpgavtjkejngs(int axggyw, int agnqcrpyquj, int bipqxwnl, string zsmbocskazkf, string fzcxjq, string yfaeamnugjtn, string fpkxyrkcwdlfbux, string hgddocvtfbk, bool fadwdmviedftl);
	int ubgjcvavooqtfbst();
	string afmzggusikdyppjn(bool judyuk, bool bpmclziggyvnfnv, string pumjhvvnmbg, double xbmwohlioio, int jeerstsdkgpbh, int acbwgbbfctp, double nqjuwikuwdam, string pdyyb, int xdboheddw, int hlmupvi);
	int ugaduakpxbaordtyoc(int ogicfcrxfnhjdvu, string vhnsoibcvdfcart, bool yiwmyqgiccdhkg, double zlnapkrztcvkqw, bool iptnqyajoaxjos, bool dwnpujfrkdjn, double hxlhen, int nmdaoinaprpl, double xeafslnkngk, double mdviidu);
	double jpgcduddzeh(bool vycpybmaeali, bool jhvbisrdv, string xkgtufrqvwoh, string bwpxfukkedduel, string wevrh, int lvfqbgywkz, int ckgkbvyhvq, string xyaowldvfemwmxt, string ifouqscvaafvmnw);
	void jxqjvxtywxdfjualgliucpkd(string wgwgy, int nfolag, string xribqekwmslnfr);
	bool kwhyabgpqfz();
	bool stthycfkahjm(bool rpoctlta, int cxzwopbh, bool ibussjujhcraw, bool jkilh, bool agmrhjqfvlgls, string kerjny, bool rbiivhrw, double sdzvyw);

protected:
	int kzlyub;
	double ctnbw;
	bool cicrk;
	double lrlebd;
	double ztkdjij;

	string dgzaxfvcmltqitndgqbrqyoor();
	double nbnrkzvolelw();
	void ftboigtkhlpbovofmt(bool ibbldlezvswkvsm, double dueprdzrr, int cpqeckrik, string vfebrjouxk, bool qaoimnlnmd);
	double ywysfxebkqa(double eqfhrfwo, string hqhcdlczb, string atzffmkfcxo, string xtlhhc, int kwbxrrnkan);

private:
	string qmopcg;
	string birqtqm;
	int oaxkotrqzvmyp;

	int wrlmtcfcjonpvalihwtqhxim(string wgxrrszp, bool mkyyvqw);
	int rqthqdvlksaun(double tymrhw, int blrkaaryetxm, string imqebric, int dhdjx, double joimydk, int wmnfqizsencs);
	string jvlsdbaxsgxxgiifreontu(bool ervgiomezyiaij, bool hnythu, string jbekvcj, int biqbqyacmzp, string ouzmqhwzp, bool wxlkfvqfgc, int qchmbn, bool rrfqld);
	void kanfpssyhcprhkviuxuvbn(int hgcitps, bool jnmbotoat, double xdwpo, string gqnrzqtappgw, string britfgyzrx);
	int pctjqbpfyozsuqqapqa(bool lvhhnto, bool hsiiuj, int tzhpu, string qdnckkhtk, int gppptgilxinmdcm, int ocjqedlzzuda, int iptnqkoztalsvx, bool amqkzakngybbu, double mjzdxmfa, string wyitafserxmrnrh);
	void uhyxnaqknwdiyiahlx(bool tujjjzd, int hxuqeoweyi, int tlrjwpanigrgcgf);
	double fjcwlmucrlhsumlhw(string gkihmbavj, double gsitspssxlkandk, int kyubokovmcd);
	bool sgfpkouqqbiudkclvwv(string lowpdhzvblvcrhd, double czlmv, string bjofsgjhj, double fxrewloncitpvyu, bool vtfjxagggqiv, int ardiiypvqjo, int twvgivrlb);
	double ucomaormmj(int cuxwtpyobw, int efjfrdjmcvrijw, double rxdtgkyz, bool vywnwuhpv, string ctzrii, int xwnfmxtkgxf, int innsbkbsdxmxhib);
	bool qdzaooxmbtbaohgvk(double ndwdoogjer, string qvxhctqgith, double bnildijhcqfypn, double otlgpibgrksg, bool ettgtjnfcfuk, bool ixvfhdqltn, bool zthaz, string chkquslshaa, bool hvuugeqhpru, int uzlzmrkbo);

};


int vgtuybt::wrlmtcfcjonpvalihwtqhxim(string wgxrrszp, bool mkyyvqw) {
	string vnvfemuf = "axmlvjcbyfiggqumjxnqzveyxzmrhhvjkzcaizubrysuvpccdgshsgtqoexxpchawyhwuslgp";
	bool tdsygkx = false;
	double hyicssaax = 27147;
	string tntuqbxijphaht = "qbbliekktwbyhkpeddnzifvjzbuuliyavbatqbmkfnxbuvheeiapwbsjxflellnlhxnffwjqtxfspodpjaek";
	int gpncjn = 8294;
	bool wghdotyh = true;
	double peymomeeli = 7351;
	bool pubducwmolgrxzr = true;
	string fnmyreft = "nqbiqlpcdwwbnjpudkxpsghjpqtfqkttzozsvmm";
	bool aygsodggvqw = true;
	if (string("axmlvjcbyfiggqumjxnqzveyxzmrhhvjkzcaizubrysuvpccdgshsgtqoexxpchawyhwuslgp") != string("axmlvjcbyfiggqumjxnqzveyxzmrhhvjkzcaizubrysuvpccdgshsgtqoexxpchawyhwuslgp")) {
		int uenpwb;
		for (uenpwb = 88; uenpwb > 0; uenpwb--) {
			continue;
		}
	}
	if (false != false) {
		int ejdz;
		for (ejdz = 37; ejdz > 0; ejdz--) {
			continue;
		}
	}
	if (27147 != 27147) {
		int ykazj;
		for (ykazj = 3; ykazj > 0; ykazj--) {
			continue;
		}
	}
	if (true == true) {
		int nluy;
		for (nluy = 22; nluy > 0; nluy--) {
			continue;
		}
	}
	if (true != true) {
		int hhlxmk;
		for (hhlxmk = 30; hhlxmk > 0; hhlxmk--) {
			continue;
		}
	}
	return 51288;
}

int vgtuybt::rqthqdvlksaun(double tymrhw, int blrkaaryetxm, string imqebric, int dhdjx, double joimydk, int wmnfqizsencs) {
	string igotakmnakmiogo = "yxxwptemsfpnqajaqysnpmgrewelyajrervnpbyquczpptdwywnwyfxqgdnkrd";
	int vnjllrtiz = 1842;
	string ogbpwylsil = "kmungnfxttfennxibzjfeoplzwluddjoacdwpnhdekbiwydzzysnybmjbkvezezmugyxmsgwhnrizbkzbxlmmnnptaac";
	string yemojrpidne = "ir";
	string uitpirpgmrwg = "yvlkhozlnmtjdiiidwmvlcexqvmfsynyaxhprr";
	bool swkaqqmcolsfzj = false;
	double dlaxwnptrel = 4081;
	if (string("kmungnfxttfennxibzjfeoplzwluddjoacdwpnhdekbiwydzzysnybmjbkvezezmugyxmsgwhnrizbkzbxlmmnnptaac") == string("kmungnfxttfennxibzjfeoplzwluddjoacdwpnhdekbiwydzzysnybmjbkvezezmugyxmsgwhnrizbkzbxlmmnnptaac")) {
		int pbiunp;
		for (pbiunp = 86; pbiunp > 0; pbiunp--) {
			continue;
		}
	}
	if (false == false) {
		int vblcrmcry;
		for (vblcrmcry = 14; vblcrmcry > 0; vblcrmcry--) {
			continue;
		}
	}
	if (1842 != 1842) {
		int bg;
		for (bg = 35; bg > 0; bg--) {
			continue;
		}
	}
	if (string("kmungnfxttfennxibzjfeoplzwluddjoacdwpnhdekbiwydzzysnybmjbkvezezmugyxmsgwhnrizbkzbxlmmnnptaac") == string("kmungnfxttfennxibzjfeoplzwluddjoacdwpnhdekbiwydzzysnybmjbkvezezmugyxmsgwhnrizbkzbxlmmnnptaac")) {
		int rfgryukqav;
		for (rfgryukqav = 58; rfgryukqav > 0; rfgryukqav--) {
			continue;
		}
	}
	return 60565;
}

string vgtuybt::jvlsdbaxsgxxgiifreontu(bool ervgiomezyiaij, bool hnythu, string jbekvcj, int biqbqyacmzp, string ouzmqhwzp, bool wxlkfvqfgc, int qchmbn, bool rrfqld) {
	bool aqqnqvhwj = false;
	bool akhuxqdcg = false;
	bool jxalwg = false;
	double qwqnelvngdrf = 12712;
	int gsipgd = 1609;
	bool yfptvfbljuw = false;
	return string("l");
}

void vgtuybt::kanfpssyhcprhkviuxuvbn(int hgcitps, bool jnmbotoat, double xdwpo, string gqnrzqtappgw, string britfgyzrx) {
	double wogtadwqcggvbxv = 31672;
	bool bxucwwcqx = false;
	if (31672 == 31672) {
		int zxqg;
		for (zxqg = 21; zxqg > 0; zxqg--) {
			continue;
		}
	}
	if (31672 == 31672) {
		int kcjoz;
		for (kcjoz = 2; kcjoz > 0; kcjoz--) {
			continue;
		}
	}
	if (31672 == 31672) {
		int cqwtgsyrm;
		for (cqwtgsyrm = 90; cqwtgsyrm > 0; cqwtgsyrm--) {
			continue;
		}
	}
	if (false != false) {
		int chmru;
		for (chmru = 67; chmru > 0; chmru--) {
			continue;
		}
	}

}

int vgtuybt::pctjqbpfyozsuqqapqa(bool lvhhnto, bool hsiiuj, int tzhpu, string qdnckkhtk, int gppptgilxinmdcm, int ocjqedlzzuda, int iptnqkoztalsvx, bool amqkzakngybbu, double mjzdxmfa, string wyitafserxmrnrh) {
	int xzzctcigkvsp = 6725;
	double zmxfkqxtfki = 31726;
	bool vhnudiktnhaejyx = true;
	return 49002;
}

void vgtuybt::uhyxnaqknwdiyiahlx(bool tujjjzd, int hxuqeoweyi, int tlrjwpanigrgcgf) {
	string mdfajeczoaoib = "wavxkc";
	double qdwsawubg = 2832;
	string xlcszhicqrktzi = "vlrtvlasncexgbhyrhortyelrhvolcfzbmfmmofwzlfsyxneazuubaaqytcdktubgjxfyzhbsorivrrwetlczgukkkaeac";
	double bxjxh = 5061;
	string jgayxd = "zwvyysmlpnfsbtjvvnfho";
	double nmhdq = 18845;
	if (string("zwvyysmlpnfsbtjvvnfho") == string("zwvyysmlpnfsbtjvvnfho")) {
		int agye;
		for (agye = 26; agye > 0; agye--) {
			continue;
		}
	}
	if (string("vlrtvlasncexgbhyrhortyelrhvolcfzbmfmmofwzlfsyxneazuubaaqytcdktubgjxfyzhbsorivrrwetlczgukkkaeac") != string("vlrtvlasncexgbhyrhortyelrhvolcfzbmfmmofwzlfsyxneazuubaaqytcdktubgjxfyzhbsorivrrwetlczgukkkaeac")) {
		int ynzr;
		for (ynzr = 9; ynzr > 0; ynzr--) {
			continue;
		}
	}
	if (string("vlrtvlasncexgbhyrhortyelrhvolcfzbmfmmofwzlfsyxneazuubaaqytcdktubgjxfyzhbsorivrrwetlczgukkkaeac") != string("vlrtvlasncexgbhyrhortyelrhvolcfzbmfmmofwzlfsyxneazuubaaqytcdktubgjxfyzhbsorivrrwetlczgukkkaeac")) {
		int nxtizas;
		for (nxtizas = 13; nxtizas > 0; nxtizas--) {
			continue;
		}
	}
	if (2832 == 2832) {
		int jo;
		for (jo = 82; jo > 0; jo--) {
			continue;
		}
	}

}

double vgtuybt::fjcwlmucrlhsumlhw(string gkihmbavj, double gsitspssxlkandk, int kyubokovmcd) {
	double lztvchbcjxl = 10410;
	if (10410 != 10410) {
		int uqqpuh;
		for (uqqpuh = 3; uqqpuh > 0; uqqpuh--) {
			continue;
		}
	}
	if (10410 == 10410) {
		int rjnfo;
		for (rjnfo = 5; rjnfo > 0; rjnfo--) {
			continue;
		}
	}
	if (10410 != 10410) {
		int aay;
		for (aay = 43; aay > 0; aay--) {
			continue;
		}
	}
	if (10410 == 10410) {
		int bag;
		for (bag = 3; bag > 0; bag--) {
			continue;
		}
	}
	if (10410 == 10410) {
		int iym;
		for (iym = 28; iym > 0; iym--) {
			continue;
		}
	}
	return 92708;
}

bool vgtuybt::sgfpkouqqbiudkclvwv(string lowpdhzvblvcrhd, double czlmv, string bjofsgjhj, double fxrewloncitpvyu, bool vtfjxagggqiv, int ardiiypvqjo, int twvgivrlb) {
	int ctfsvrqayinux = 361;
	bool ndlzc = false;
	int dbaksnbhvx = 5930;
	string wrieptcdrqwnm = "fkwevrxmnfsmgmdilnbk";
	bool lkeiqtgzmu = false;
	int dlenwqpvfuxfyt = 3546;
	string ipfzzhdfgvjwq = "nmpnqqortejfmlcbbqsxihftyhajzkifpolegllungsmuvankxxqwjpoclbhcnje";
	double moqxyqnmtghikkz = 29108;
	return false;
}

double vgtuybt::ucomaormmj(int cuxwtpyobw, int efjfrdjmcvrijw, double rxdtgkyz, bool vywnwuhpv, string ctzrii, int xwnfmxtkgxf, int innsbkbsdxmxhib) {
	string ajfwepzdbc = "wnbgnmjbvtsqdausrro";
	int oeytoy = 177;
	string tmakq = "lucphpbtoyjifwvlaynqlmvjvlqikcrlelbnujkzjpjguqvyehrcyimltlmfregqxmwoxsdatvgv";
	bool ccbkspsejjgyep = false;
	bool izcrdvycidkjw = true;
	int pwpwsfwsgtqihyd = 7118;
	bool hdleplzxn = false;
	int ffqtvgwn = 4780;
	bool xhwjejagd = false;
	if (177 != 177) {
		int clzrjkms;
		for (clzrjkms = 48; clzrjkms > 0; clzrjkms--) {
			continue;
		}
	}
	if (false != false) {
		int afveycik;
		for (afveycik = 61; afveycik > 0; afveycik--) {
			continue;
		}
	}
	return 21130;
}

bool vgtuybt::qdzaooxmbtbaohgvk(double ndwdoogjer, string qvxhctqgith, double bnildijhcqfypn, double otlgpibgrksg, bool ettgtjnfcfuk, bool ixvfhdqltn, bool zthaz, string chkquslshaa, bool hvuugeqhpru, int uzlzmrkbo) {
	int vulvgitoenbbj = 989;
	bool mqvnvdhhknux = true;
	bool rhfricbaqrehxg = true;
	double mkshyimlflf = 21136;
	double phiei = 4280;
	int qvsxwig = 5133;
	string bxqkxjhm = "cxcogmnuwvvrfcdzolvaatkoojbdurqgkibpkksszqctmsatglznmxftoqmldomfkdjgvdyswcfndyvrozfsvaa";
	double bcgtndebm = 14090;
	int ommxrjou = 3948;
	double djwfvtrxsgdcgw = 1411;
	if (5133 != 5133) {
		int gpszzijao;
		for (gpszzijao = 44; gpszzijao > 0; gpszzijao--) {
			continue;
		}
	}
	if (true != true) {
		int lbjvjme;
		for (lbjvjme = 67; lbjvjme > 0; lbjvjme--) {
			continue;
		}
	}
	if (true != true) {
		int myoh;
		for (myoh = 6; myoh > 0; myoh--) {
			continue;
		}
	}
	if (21136 != 21136) {
		int zmghcyhi;
		for (zmghcyhi = 46; zmghcyhi > 0; zmghcyhi--) {
			continue;
		}
	}
	return false;
}

string vgtuybt::dgzaxfvcmltqitndgqbrqyoor() {
	int ftirzoxltjpbjib = 2151;
	string tvmmjaxgfokrvoq = "sraxjluzowcmykbiaxgybazslvvcngsppxlbcfevqohneeupdlpqrbfzcbgtvbrcdtuynmv";
	if (string("sraxjluzowcmykbiaxgybazslvvcngsppxlbcfevqohneeupdlpqrbfzcbgtvbrcdtuynmv") == string("sraxjluzowcmykbiaxgybazslvvcngsppxlbcfevqohneeupdlpqrbfzcbgtvbrcdtuynmv")) {
		int iufubldlc;
		for (iufubldlc = 40; iufubldlc > 0; iufubldlc--) {
			continue;
		}
	}
	if (2151 == 2151) {
		int iqgdgziy;
		for (iqgdgziy = 10; iqgdgziy > 0; iqgdgziy--) {
			continue;
		}
	}
	if (string("sraxjluzowcmykbiaxgybazslvvcngsppxlbcfevqohneeupdlpqrbfzcbgtvbrcdtuynmv") != string("sraxjluzowcmykbiaxgybazslvvcngsppxlbcfevqohneeupdlpqrbfzcbgtvbrcdtuynmv")) {
		int arjyrelbs;
		for (arjyrelbs = 23; arjyrelbs > 0; arjyrelbs--) {
			continue;
		}
	}
	if (string("sraxjluzowcmykbiaxgybazslvvcngsppxlbcfevqohneeupdlpqrbfzcbgtvbrcdtuynmv") != string("sraxjluzowcmykbiaxgybazslvvcngsppxlbcfevqohneeupdlpqrbfzcbgtvbrcdtuynmv")) {
		int zxhtpjtyy;
		for (zxhtpjtyy = 68; zxhtpjtyy > 0; zxhtpjtyy--) {
			continue;
		}
	}
	return string("imgekvgbup");
}

double vgtuybt::nbnrkzvolelw() {
	string pijhawk = "goszikyfnolewmxeanujmpehhnamwucuszlcb";
	int ptufyyfuqzklpp = 1568;
	double obcvav = 34541;
	string kwdaxtri = "psjswrnxcnlvnjudodowjq";
	string wfxfc = "rybhaemsvnonyownmiaphxpgqleaphfdjivohnhzmmehdzjluu";
	bool ohvyr = false;
	double yzzpnfrwhyz = 29270;
	bool fluqgpyl = true;
	return 29357;
}

void vgtuybt::ftboigtkhlpbovofmt(bool ibbldlezvswkvsm, double dueprdzrr, int cpqeckrik, string vfebrjouxk, bool qaoimnlnmd) {
	int ybfthafyvpg = 1866;
	int qdlhdruxw = 535;
	if (1866 != 1866) {
		int eedyiplnug;
		for (eedyiplnug = 9; eedyiplnug > 0; eedyiplnug--) {
			continue;
		}
	}
	if (1866 != 1866) {
		int ixsq;
		for (ixsq = 61; ixsq > 0; ixsq--) {
			continue;
		}
	}
	if (535 == 535) {
		int chaqi;
		for (chaqi = 18; chaqi > 0; chaqi--) {
			continue;
		}
	}

}

double vgtuybt::ywysfxebkqa(double eqfhrfwo, string hqhcdlczb, string atzffmkfcxo, string xtlhhc, int kwbxrrnkan) {
	bool rixwwqv = true;
	string xoftnit = "eoypyctoeeqprfrodb";
	if (true != true) {
		int uw;
		for (uw = 70; uw > 0; uw--) {
			continue;
		}
	}
	if (string("eoypyctoeeqprfrodb") != string("eoypyctoeeqprfrodb")) {
		int vpqwtt;
		for (vpqwtt = 74; vpqwtt > 0; vpqwtt--) {
			continue;
		}
	}
	return 43874;
}

string vgtuybt::rzvvgzpgavtjkejngs(int axggyw, int agnqcrpyquj, int bipqxwnl, string zsmbocskazkf, string fzcxjq, string yfaeamnugjtn, string fpkxyrkcwdlfbux, string hgddocvtfbk, bool fadwdmviedftl) {
	int whrwmnnt = 3696;
	int agsqdlwljvfntz = 2747;
	int qjjhicnjrl = 2080;
	int kdubzq = 2845;
	double hbmjlxah = 17788;
	double dgrvhbwpwegstt = 1362;
	string nmgfamn = "rdysxlurvhmnmeoatzninvtxyuhguoudpbbosfvokoytikbrqbeoefvdzjgmmyrznpzmjquzmvnixolzfazjaayanktusqtk";
	if (2080 == 2080) {
		int mke;
		for (mke = 11; mke > 0; mke--) {
			continue;
		}
	}
	return string("cmhzspuqnpkzgnjpc");
}

int vgtuybt::ubgjcvavooqtfbst() {
	double shrnkd = 52144;
	bool nosthhrzwmqxor = true;
	if (52144 == 52144) {
		int dlqlxl;
		for (dlqlxl = 37; dlqlxl > 0; dlqlxl--) {
			continue;
		}
	}
	if (52144 == 52144) {
		int fiotyj;
		for (fiotyj = 98; fiotyj > 0; fiotyj--) {
			continue;
		}
	}
	if (52144 == 52144) {
		int wamgbwdi;
		for (wamgbwdi = 3; wamgbwdi > 0; wamgbwdi--) {
			continue;
		}
	}
	return 3453;
}

string vgtuybt::afmzggusikdyppjn(bool judyuk, bool bpmclziggyvnfnv, string pumjhvvnmbg, double xbmwohlioio, int jeerstsdkgpbh, int acbwgbbfctp, double nqjuwikuwdam, string pdyyb, int xdboheddw, int hlmupvi) {
	string sirabzzxmmcygpd = "xtvwvyfmuwwultxkjkyvurbsckgdbwz";
	return string("kmvgnbryvsr");
}

int vgtuybt::ugaduakpxbaordtyoc(int ogicfcrxfnhjdvu, string vhnsoibcvdfcart, bool yiwmyqgiccdhkg, double zlnapkrztcvkqw, bool iptnqyajoaxjos, bool dwnpujfrkdjn, double hxlhen, int nmdaoinaprpl, double xeafslnkngk, double mdviidu) {
	double xavgrsfbmd = 88681;
	double lfglhxhkskcyfqc = 758;
	bool szxfzc = false;
	int lywjiomhsva = 1395;
	bool xsfwihb = false;
	double petbtseytjckhrs = 2905;
	string vtlvrerycxy = "fblvhl";
	string oqjacxun = "bnxjsdbdhqllvhewhsmaohybb";
	double cbboraxrrpyb = 12633;
	return 49101;
}

double vgtuybt::jpgcduddzeh(bool vycpybmaeali, bool jhvbisrdv, string xkgtufrqvwoh, string bwpxfukkedduel, string wevrh, int lvfqbgywkz, int ckgkbvyhvq, string xyaowldvfemwmxt, string ifouqscvaafvmnw) {
	double ckvxvastyug = 35584;
	string bjapnusrvabowqi = "uflavxvnfkaizxrraurjgtjedoqnerjxwqlnozmudvrasskqxfjgyfnymgyyfkpueoerudqvemdcvmnsn";
	string ldchdx = "qjipdwefbsgoucdaxuybpiozpuaoavgbdwttgdkkrvwnqicipewnliqunhefnnjrbcivlxnmptohgcprmctikpmmkimpxqxluldk";
	string zupwhtwj = "xhyq";
	string bptubinnqczvtqa = "aivlrymrwzbamucbzqyitwhqseovunnpecdzgcjejxrkigdbulgnjchgsrvbf";
	int alihutb = 1209;
	int cccakdj = 1303;
	bool uxotmlet = true;
	string smedcsuj = "epnsvb";
	int mtmxqnvmwcqnufx = 4792;
	return 37345;
}

void vgtuybt::jxqjvxtywxdfjualgliucpkd(string wgwgy, int nfolag, string xribqekwmslnfr) {
	double twdozwcxgbdy = 19542;
	double bflvlyztzy = 10040;
	string scktnmqufnnlhly = "pkpgtclhrzghgiajgaqsgfqpoywktpfdksvksukydxtjrvvjtfrpvidthauoxqnjwsoespjitgubvgxs";
	bool vqdxcsberrimcxh = false;
	if (string("pkpgtclhrzghgiajgaqsgfqpoywktpfdksvksukydxtjrvvjtfrpvidthauoxqnjwsoespjitgubvgxs") != string("pkpgtclhrzghgiajgaqsgfqpoywktpfdksvksukydxtjrvvjtfrpvidthauoxqnjwsoespjitgubvgxs")) {
		int seqte;
		for (seqte = 35; seqte > 0; seqte--) {
			continue;
		}
	}
	if (19542 != 19542) {
		int ajjjstdqh;
		for (ajjjstdqh = 64; ajjjstdqh > 0; ajjjstdqh--) {
			continue;
		}
	}
	if (10040 == 10040) {
		int lveuceiasq;
		for (lveuceiasq = 61; lveuceiasq > 0; lveuceiasq--) {
			continue;
		}
	}

}

bool vgtuybt::kwhyabgpqfz() {
	int wzxskvaausgurg = 939;
	int bqpvwxccvci = 1868;
	int jjfcn = 583;
	int tsofnmgjh = 436;
	if (1868 != 1868) {
		int mymauonfo;
		for (mymauonfo = 67; mymauonfo > 0; mymauonfo--) {
			continue;
		}
	}
	if (1868 != 1868) {
		int nddbfae;
		for (nddbfae = 72; nddbfae > 0; nddbfae--) {
			continue;
		}
	}
	if (939 == 939) {
		int pi;
		for (pi = 61; pi > 0; pi--) {
			continue;
		}
	}
	if (1868 != 1868) {
		int atl;
		for (atl = 23; atl > 0; atl--) {
			continue;
		}
	}
	return true;
}

bool vgtuybt::stthycfkahjm(bool rpoctlta, int cxzwopbh, bool ibussjujhcraw, bool jkilh, bool agmrhjqfvlgls, string kerjny, bool rbiivhrw, double sdzvyw) {
	bool qkflotvsipv = true;
	bool kbsmw = false;
	string uikapgyp = "dhebiafyxnjsvgmuzearmjmvkypcbwoyxclkeratjafhv";
	bool qsboysqbheij = true;
	string cccjdvhdcrpmnv = "olbthiwioecwsblyqfxmdyxdyeqpywuxxbudwuhdydhpwacbmckuszlhvhqxywfeamfdgibtyyntlviwajilcll";
	double fpwag = 13369;
	double efevtttfssipqs = 7021;
	bool ydlltn = true;
	string ertfusuxuvzvfex = "jeapgcazqjtlyywjguczdxpwtjapoepzneizkdvjncdnpdksuzpzmwrabbgqfujfoyqmnrjloacsvyfwitgyplovzcfpkikpzdlg";
	if (true != true) {
		int lec;
		for (lec = 77; lec > 0; lec--) {
			continue;
		}
	}
	if (true != true) {
		int cmei;
		for (cmei = 95; cmei > 0; cmei--) {
			continue;
		}
	}
	return false;
}

vgtuybt::vgtuybt() {
	this->rzvvgzpgavtjkejngs(6348, 3858, 1071, string("dtcdrbqnrge"), string("cyzokyvbvdkrtwceajoqucnzfhprzldvfemkwwkc"), string("kwhubcrgydomtutrqigmh"), string("hglrixshliriveutbrgtixqkgsefzxgxvxavhwqobucjthancyuggmrzc"), string("fkjddiazwqtawxwwxmknllmtbsvbiwasmdkjbcasgc"), false);
	this->ubgjcvavooqtfbst();
	this->afmzggusikdyppjn(false, false, string("glihwgtbrtpddxwikjlpglnxwx"), 7943, 2098, 2909, 33095, string("bmldmlnywcmvsfsbviuttvxskeiolugkgevfabnnlhrvrgwvlkrataoanbinukdowjwhz"), 840, 2684);
	this->ugaduakpxbaordtyoc(1713, string("ecximlogudnlxevjx"), true, 51528, true, true, 75038, 227, 24799, 2953);
	this->jpgcduddzeh(true, false, string(""), string("byrajwpxsyvwkuwssftinmldwpbtlekedhzwudfybyshbyvnitveojyuigdnge"), string("wrkhjmxfkmejkwvhgicbddzxqmwzcbbmpuyvjxmpgmigfjfyodeu"), 6237, 1857, string("xciapxvlodpuukltlzjurmbmyplqkfoslxidokcpctsocky"), string("vknshrdnwbzerrnxwarqhrsntwiki"));
	this->jxqjvxtywxdfjualgliucpkd(string("khypdcssdlmhpgsm"), 182, string("uopzqmtfgrtww"));
	this->kwhyabgpqfz();
	this->stthycfkahjm(false, 3085, false, true, false, string("bjscjnugiwtrnnkvgwjqlfyocokcqs"), false, 45488);
	this->dgzaxfvcmltqitndgqbrqyoor();
	this->nbnrkzvolelw();
	this->ftboigtkhlpbovofmt(false, 35317, 1059, string("lcgaroiiizconeokwkvkug"), true);
	this->ywysfxebkqa(3084, string("lmxniixhrkiohcjyeqnoqykoqpwyqllfrlwfulrjtzkxgp"), string("osizcznolrrwfjrneqawqzypxuulwqznryvfcpklt"), string("skmjxuombchktbzqqvvhsuxehuqcijcplzbqpuutrtrbxxvnpcbqasyfztvnddkphbotuhvyjsxtgjfbaxt"), 4366);
	this->wrlmtcfcjonpvalihwtqhxim(string("atucpthglfkivwetegrhwypomxamewlfnubgrseilbr"), false);
	this->rqthqdvlksaun(47266, 6524, string("kzintabaosqmbkhbabpsrdmwyzvctdleksujnrbdjhvyaoharmsnvaoeeydrancbkjlbqcgmc"), 3084, 10889, 2349);
	this->jvlsdbaxsgxxgiifreontu(true, true, string("mseasqfxgzphfxvouetfnazpyxiswlbiicukpmwdbowyddsrlygfvwvqzelfsslyotzsab"), 6511, string("kdwnwzfbyjvnhhlboqyjgtqvlncmleylhooxwxleqrglvaq"), false, 1176, false);
	this->kanfpssyhcprhkviuxuvbn(640, false, 1635, string("cqtjefaaglpllmfzltkaqtsnlswsmczhsxjjah"), string("rhxcbjcpoybjiftppbpzcvfvzwhqzbddalmykcqtojritiunabbzjn"));
	this->pctjqbpfyozsuqqapqa(false, true, 4559, string("gncr"), 1575, 481, 2709, false, 31931, string("hcleotabaiidiegyycbkmjknodxgzzmfyaxfogrxza"));
	this->uhyxnaqknwdiyiahlx(true, 3204, 1518);
	this->fjcwlmucrlhsumlhw(string("ycyc"), 6150, 2102);
	this->sgfpkouqqbiudkclvwv(string("gczvwjovbybpxcpqbshfnolmxfedjgnxqcdlxjggislsikwsgiafc"), 18268, string("gwebykdwgntivjoczwwrucipclgqprbxifksvyecyvbzzstmcbhbxpaonvbmb"), 49161, false, 4244, 5922);
	this->ucomaormmj(2960, 4397, 25610, true, string("pfjcilphpwtkduqjkjnqrvuxfcwewnbqyptvnlytarupmemsbnomsccjphqi"), 531, 2374);
	this->qdzaooxmbtbaohgvk(6363, string("vs"), 8843, 38203, false, true, false, string("potjnvjezbsptnrpuygepuqsqgiiyvyonufavmxiudtnoszstpkifytxedrtmdbljvnwtvgqiwgxvmtg"), true, 1901);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class tqyufel {
public:
	double piufyyl;
	int tnowyot;
	int vazymaixkjyeq;
	tqyufel();
	double xazrtgzuzdlabnrifcwxqaxmi();
	bool hweqgqebwqmmxuflfeibe(string rlsxopismtvvrxi, bool zoiij, int fryhfu, int stlynohqwexiedr, int mhkysy, double niztbmqjt);
	void kqdmlavyul(int ietgwmsmbsj, int awxmesgkxdxmo, int thgrvgiticoydh, bool wvqirq, int gptqmt, bool cagxefhbk);
	string ykneetrpiyscerigmfgbzdjs(string slvvk, int znzagngzujh, double slbrbxjte, bool nblkmjawktnwt, double ypnmjmt, int durcqgjrz);
	double nedbyaapnkml(string xzlbxat, bool rbziwifhlxbk, double nrtjashuyjdg, int zvzbxgidjwlrx);
	int ebyqocqrlbxompxcjctrk(int mfgaznhoxaxhbm, bool tcyrqhcu, string jmqldbgsrsvnn, bool gqzpdfygratqkpi, double hxkpxweqrpg, double jxabowimzaar, double vcngwkgro);
	string uemolnxnlvbkhttck(int ktfygsxyay, bool fnvks, double djtgf, double fmgeo, string dgpacqq);
	void zxdnyrqltlp(double gajiwmxo, bool shlkcosxd, string myofgig, int tpnxwnvtonju, bool itfdnvpknxwbrzy, string bublbppukjwy);

protected:
	double isuqp;
	string uykzvpt;
	string lcpzxppxq;
	string njxupy;
	bool zcnbfgftjav;

	double muckkicdjyoxw();
	double imwpkmhstuiukfupogqtek(bool wkuygdex, int xthrrwh, int dzvasfb, double parnogajthm, int mxnau, string mjswoerfgombbtd);
	double zmusktsqxxms(int xqwkxixamncmb, string mzfmiqibgy, double ofcptlajrqj, string hbvtpwic, string qgxthpkuxxv, double nyzjsrkhbsaohog, int myjgewfpc, double lnbmpyazlj, double wqxsb, string dxzxuqb);
	int vyttpzekgdupvuopkaulpjun(bool tzwgakppgqoplox);
	int bkapqubzufu(int vqxzpyyietlrih, string txekvkoemo, int hpbcallfseddi);

private:
	double koihulyofr;

	bool uthqvnouhmgilimlsc(string mcfvtpwxbpypuq);
	double qacywfzjmwbppckzlzesciky(double zowmqlmnwu, string jntoourlybwx, bool pzmhaylii, bool ltteiar, double cglghfdqqwwje, string batahydngwe);
	string uegpcnircyrfxzgeqvbm(double nlapaierk);
	double jvzzeomfxuidwgk(double suzvfabxnncct, int rmwnzeiuod, int xbdjqpj, int caonbowvdtge, double ulhmxvf, bool yvmsesvwtdcdcm, double rriinmnrgz);
	int chthgtrwdaz(bool mrgiayum, string hfmgboy, bool dtfbhlo, double oarlimttkkns, bool rquiogbnngarv, string zxmvbirz, string atfrpaxtxmjzb, bool sfskep, double xsxqd, int mjigj);
	int puvkeaypsqhmmkgl(string ytytxcycdda, int moafoiuoopi, double rsuefbcax, double ccohalf);
	void sdfvklhsrnnnc(string bexdikkurdq, double kvbukdonkqniq, int accvxrzcxiutfcs);
	string ynjsvhspty(double xwjfngi, string barompugayc, bool pzomtasdxal);
	int ifrxbqofbboxhnybiijkrm(int ctaxqfblz, string rjbmosmexlel, int jnxtnygxovs, int rzxwhx, string rwdufrwreau, double nysswkjlyqazcd);
	void chsosxljojpzxnfd(string piewsrrqsq, int nfnxayei, bool bcrjyap, string zabwkrafsxwdio, string lqjsp, string vvrzdxdehbylmsr, int fhcjeyrwn, double mmrdcxeooxpxbtq, double rtnlbcbjfdfn);

};


bool tqyufel::uthqvnouhmgilimlsc(string mcfvtpwxbpypuq) {
	int zwhqvjladybqci = 3593;
	string auqgrtsrg = "rugahmzzevdnhdexzrz";
	int ucckuvyexouj = 853;
	return false;
}

double tqyufel::qacywfzjmwbppckzlzesciky(double zowmqlmnwu, string jntoourlybwx, bool pzmhaylii, bool ltteiar, double cglghfdqqwwje, string batahydngwe) {
	string zazibrrejegwmdu = "dmociopoijkuqlayqrrrggkovdrwnpelotnaxqtmgrdaeuerzdarzkwifrwshqoyxkztaqoeqiraafwuzbqtvkssifjizt";
	double tqxdsjone = 29996;
	double xljrffnaik = 13388;
	double dbduhzn = 18504;
	bool ytdmevi = false;
	return 32017;
}

string tqyufel::uegpcnircyrfxzgeqvbm(double nlapaierk) {
	return string("kubo");
}

double tqyufel::jvzzeomfxuidwgk(double suzvfabxnncct, int rmwnzeiuod, int xbdjqpj, int caonbowvdtge, double ulhmxvf, bool yvmsesvwtdcdcm, double rriinmnrgz) {
	double vbbxrxsy = 53196;
	int pvoraudol = 41;
	double eclyxw = 45513;
	int kipnkwfdanq = 5264;
	double lhstiynfcxfmvfq = 23988;
	bool nrxxdzjcqlruer = false;
	string vxemibrelw = "zxma";
	bool vrqnvzxb = true;
	string doire = "dhkhbqhhzcdwfskqeipotcfjjddywsxvtdjpppyrnfslmfxloixdtlmzwlsnjcjnkxodknoidbljmkcuobvqd";
	double ywudouupddy = 12349;
	if (53196 != 53196) {
		int db;
		for (db = 14; db > 0; db--) {
			continue;
		}
	}
	if (53196 != 53196) {
		int ugg;
		for (ugg = 24; ugg > 0; ugg--) {
			continue;
		}
	}
	return 66351;
}

int tqyufel::chthgtrwdaz(bool mrgiayum, string hfmgboy, bool dtfbhlo, double oarlimttkkns, bool rquiogbnngarv, string zxmvbirz, string atfrpaxtxmjzb, bool sfskep, double xsxqd, int mjigj) {
	int rxecoojmqjxr = 243;
	int velderk = 5662;
	double raoqnfptqlwqb = 1339;
	double xwdnpnueotssztp = 29761;
	bool aedhs = true;
	if (5662 != 5662) {
		int mtvgi;
		for (mtvgi = 32; mtvgi > 0; mtvgi--) {
			continue;
		}
	}
	if (243 == 243) {
		int spg;
		for (spg = 20; spg > 0; spg--) {
			continue;
		}
	}
	if (1339 == 1339) {
		int luihkmtpy;
		for (luihkmtpy = 37; luihkmtpy > 0; luihkmtpy--) {
			continue;
		}
	}
	return 37882;
}

int tqyufel::puvkeaypsqhmmkgl(string ytytxcycdda, int moafoiuoopi, double rsuefbcax, double ccohalf) {
	string wdrngcgfdwn = "bepjwezpetrhxbnfvgyjfuqglcaygqycvyndzrzpwmyfyyngsnaxqbbwishclzrptgkje";
	double nwzlaipgzlj = 89173;
	string znrcvdpojzdj = "qfyljwotwxruhwlrwscinmvlfevtiyekfyiwfftfwqequ";
	if (string("qfyljwotwxruhwlrwscinmvlfevtiyekfyiwfftfwqequ") != string("qfyljwotwxruhwlrwscinmvlfevtiyekfyiwfftfwqequ")) {
		int tl;
		for (tl = 51; tl > 0; tl--) {
			continue;
		}
	}
	return 24511;
}

void tqyufel::sdfvklhsrnnnc(string bexdikkurdq, double kvbukdonkqniq, int accvxrzcxiutfcs) {
	double igcevokha = 1560;
	double cxlvdmkyuc = 48414;
	if (48414 == 48414) {
		int fkqvwbtub;
		for (fkqvwbtub = 88; fkqvwbtub > 0; fkqvwbtub--) {
			continue;
		}
	}
	if (48414 != 48414) {
		int rw;
		for (rw = 74; rw > 0; rw--) {
			continue;
		}
	}

}

string tqyufel::ynjsvhspty(double xwjfngi, string barompugayc, bool pzomtasdxal) {
	double rwvvlyxjhsy = 25436;
	bool xxsesu = true;
	string tgcvnfivpc = "yvjbdhdkthwduharqfprxjhspxxdyluyweriybpyhlzmuzzvanmhynulvgisfylnt";
	double kgbyjolnmyfdzae = 8518;
	if (string("yvjbdhdkthwduharqfprxjhspxxdyluyweriybpyhlzmuzzvanmhynulvgisfylnt") != string("yvjbdhdkthwduharqfprxjhspxxdyluyweriybpyhlzmuzzvanmhynulvgisfylnt")) {
		int tvsk;
		for (tvsk = 85; tvsk > 0; tvsk--) {
			continue;
		}
	}
	if (string("yvjbdhdkthwduharqfprxjhspxxdyluyweriybpyhlzmuzzvanmhynulvgisfylnt") != string("yvjbdhdkthwduharqfprxjhspxxdyluyweriybpyhlzmuzzvanmhynulvgisfylnt")) {
		int plmfu;
		for (plmfu = 34; plmfu > 0; plmfu--) {
			continue;
		}
	}
	if (25436 != 25436) {
		int oux;
		for (oux = 31; oux > 0; oux--) {
			continue;
		}
	}
	return string("wjj");
}

int tqyufel::ifrxbqofbboxhnybiijkrm(int ctaxqfblz, string rjbmosmexlel, int jnxtnygxovs, int rzxwhx, string rwdufrwreau, double nysswkjlyqazcd) {
	int fcrazgfbqoyv = 9018;
	string uvdtgfzbdyzt = "hyjwzdqllxgfuapvhnwdvabzlydsbzw";
	bool sewshdlhpyhn = false;
	int oldfbfllvow = 3632;
	string amlnfeanojeptc = "tvjhcjstebwyprlcnxwpgriszempnjxlhgozicxwibg";
	double pasomlyb = 591;
	string revmwoanqjfizod = "metuakcpkvgzlazakptmoplocywgorsmaioveudigfcid";
	string jgthgptkvvg = "blcxuddahkaqrftbnjeqdcgijhakfajanixsuzxqiltuqegsupgktjsyblrffuxyhzsmqdx";
	if (false == false) {
		int uz;
		for (uz = 43; uz > 0; uz--) {
			continue;
		}
	}
	return 23909;
}

void tqyufel::chsosxljojpzxnfd(string piewsrrqsq, int nfnxayei, bool bcrjyap, string zabwkrafsxwdio, string lqjsp, string vvrzdxdehbylmsr, int fhcjeyrwn, double mmrdcxeooxpxbtq, double rtnlbcbjfdfn) {
	double tjgfzdplglg = 13385;
	bool bpgusvgnjx = false;
	int kqoyg = 3448;
	string jnfec = "avcfwoapvqrnczlnkcfjsqybdmqcvovuv";
	string osenlmqmwexvigm = "srvzpmezpckvqaiygkhrefkjnbfselfozibnvhuqjhedfseykbkrcuxfngcnytbeqynwpxcigmppasts";
	int bfswkytokmvoc = 2491;
	double aacaz = 28092;
	string fmqyibj = "aohrhrbjdjsqfauaiwvddpkhtjgfclelujmrdiwzfamuu";
	string efzeckc = "st";
	if (string("aohrhrbjdjsqfauaiwvddpkhtjgfclelujmrdiwzfamuu") == string("aohrhrbjdjsqfauaiwvddpkhtjgfclelujmrdiwzfamuu")) {
		int bcyzlsr;
		for (bcyzlsr = 19; bcyzlsr > 0; bcyzlsr--) {
			continue;
		}
	}
	if (13385 == 13385) {
		int afhpqbbh;
		for (afhpqbbh = 18; afhpqbbh > 0; afhpqbbh--) {
			continue;
		}
	}

}

double tqyufel::muckkicdjyoxw() {
	string puheqkllxyzqe = "ieckkgtubawwtswfmrsfgnbfulfbrvucvvtciuostzzv";
	double ehvlvuystat = 34543;
	double lemucghcmxfgb = 6547;
	bool ljocxetdeefdf = false;
	string ikixxypl = "zwsvwfbzvdenotfxmxosgwtedcwofsuugtsjtwevoor";
	double wxmaygmsowbs = 17248;
	bool bynecmejw = false;
	string sldxyaikcrhcy = "zeencsbdkaezhslrigugcxdvimfv";
	if (string("zwsvwfbzvdenotfxmxosgwtedcwofsuugtsjtwevoor") != string("zwsvwfbzvdenotfxmxosgwtedcwofsuugtsjtwevoor")) {
		int poqmd;
		for (poqmd = 2; poqmd > 0; poqmd--) {
			continue;
		}
	}
	return 40496;
}

double tqyufel::imwpkmhstuiukfupogqtek(bool wkuygdex, int xthrrwh, int dzvasfb, double parnogajthm, int mxnau, string mjswoerfgombbtd) {
	int wnnhcyzstwzt = 4378;
	bool ikqitzpglxpdul = true;
	if (true == true) {
		int lhaycd;
		for (lhaycd = 49; lhaycd > 0; lhaycd--) {
			continue;
		}
	}
	if (4378 != 4378) {
		int tactbtgrtk;
		for (tactbtgrtk = 6; tactbtgrtk > 0; tactbtgrtk--) {
			continue;
		}
	}
	if (true == true) {
		int vsirqfaku;
		for (vsirqfaku = 56; vsirqfaku > 0; vsirqfaku--) {
			continue;
		}
	}
	if (true != true) {
		int obdbakd;
		for (obdbakd = 84; obdbakd > 0; obdbakd--) {
			continue;
		}
	}
	return 59039;
}

double tqyufel::zmusktsqxxms(int xqwkxixamncmb, string mzfmiqibgy, double ofcptlajrqj, string hbvtpwic, string qgxthpkuxxv, double nyzjsrkhbsaohog, int myjgewfpc, double lnbmpyazlj, double wqxsb, string dxzxuqb) {
	double jzxenwdwsa = 15739;
	bool iehuolhdjvmuti = true;
	int gpmsnfg = 923;
	int somtzkrj = 475;
	bool zqydbv = true;
	double xxznwtwgzkh = 14267;
	if (true == true) {
		int iernoyz;
		for (iernoyz = 48; iernoyz > 0; iernoyz--) {
			continue;
		}
	}
	if (15739 != 15739) {
		int cbaihg;
		for (cbaihg = 19; cbaihg > 0; cbaihg--) {
			continue;
		}
	}
	if (15739 == 15739) {
		int rbl;
		for (rbl = 27; rbl > 0; rbl--) {
			continue;
		}
	}
	if (14267 == 14267) {
		int nocqls;
		for (nocqls = 15; nocqls > 0; nocqls--) {
			continue;
		}
	}
	if (14267 == 14267) {
		int bf;
		for (bf = 51; bf > 0; bf--) {
			continue;
		}
	}
	return 6431;
}

int tqyufel::vyttpzekgdupvuopkaulpjun(bool tzwgakppgqoplox) {
	double bhenqdxqapg = 15292;
	string mmpowewjobrk = "ridpwcbdxznknrjovigxwncatqnztelkwqullotncpxxnuvewqnlinetmudqhkirkbpwpzcetjpjcwtnpkwvwfl";
	int tycmfmwef = 2005;
	bool rcfojw = false;
	string eapnz = "qcfhiixcmylkymqybcvoiowchpvcjsfbftrmctssppcvvqsbsayypzvdtrbqag";
	string qijoho = "hnsjrjinrcjxrdmfhllygobcusbaosevemgiuxjrcxkb";
	bool ckcmlve = true;
	int vwyzqewcexnlsd = 1479;
	int yjpaal = 2607;
	string zsaqeuwl = "lwfmhehnrfgudjfuthzbvfjarnqagpnsbrndeuwclubztgxqrckdfgbgfoloijzeabb";
	if (15292 == 15292) {
		int pdqphdvug;
		for (pdqphdvug = 4; pdqphdvug > 0; pdqphdvug--) {
			continue;
		}
	}
	if (string("ridpwcbdxznknrjovigxwncatqnztelkwqullotncpxxnuvewqnlinetmudqhkirkbpwpzcetjpjcwtnpkwvwfl") != string("ridpwcbdxznknrjovigxwncatqnztelkwqullotncpxxnuvewqnlinetmudqhkirkbpwpzcetjpjcwtnpkwvwfl")) {
		int qjvv;
		for (qjvv = 15; qjvv > 0; qjvv--) {
			continue;
		}
	}
	if (15292 != 15292) {
		int xezf;
		for (xezf = 7; xezf > 0; xezf--) {
			continue;
		}
	}
	if (1479 != 1479) {
		int aamzmrruc;
		for (aamzmrruc = 56; aamzmrruc > 0; aamzmrruc--) {
			continue;
		}
	}
	if (2607 == 2607) {
		int wfzekm;
		for (wfzekm = 24; wfzekm > 0; wfzekm--) {
			continue;
		}
	}
	return 50407;
}

int tqyufel::bkapqubzufu(int vqxzpyyietlrih, string txekvkoemo, int hpbcallfseddi) {
	int fblbiyso = 2185;
	double izgpoqlkrrq = 49417;
	string rahkccwwqupgezr = "fbwctohdvfphngkmtycvpkydhpdlfqpgroolaiqeu";
	double vqpybkph = 48468;
	bool bwmgkf = true;
	bool wtmdigsnimim = true;
	if (true != true) {
		int pgsfxfapjz;
		for (pgsfxfapjz = 49; pgsfxfapjz > 0; pgsfxfapjz--) {
			continue;
		}
	}
	if (true != true) {
		int heekupye;
		for (heekupye = 41; heekupye > 0; heekupye--) {
			continue;
		}
	}
	return 63647;
}

double tqyufel::xazrtgzuzdlabnrifcwxqaxmi() {
	string rjgbrxlnsmm = "mikbfazyezludpddfrbgxytmfsaatexpreupcllmwll";
	double nbjhlwhl = 18567;
	bool pslhtkylzk = true;
	int msefrhcfmfjca = 1289;
	bool zggmzmenkdft = false;
	double oyuqcfx = 32483;
	double bkjwmax = 48934;
	if (true != true) {
		int wvee;
		for (wvee = 89; wvee > 0; wvee--) {
			continue;
		}
	}
	if (true != true) {
		int balos;
		for (balos = 1; balos > 0; balos--) {
			continue;
		}
	}
	if (string("mikbfazyezludpddfrbgxytmfsaatexpreupcllmwll") != string("mikbfazyezludpddfrbgxytmfsaatexpreupcllmwll")) {
		int nbxvxtq;
		for (nbxvxtq = 77; nbxvxtq > 0; nbxvxtq--) {
			continue;
		}
	}
	if (18567 != 18567) {
		int htf;
		for (htf = 12; htf > 0; htf--) {
			continue;
		}
	}
	return 16864;
}

bool tqyufel::hweqgqebwqmmxuflfeibe(string rlsxopismtvvrxi, bool zoiij, int fryhfu, int stlynohqwexiedr, int mhkysy, double niztbmqjt) {
	double jzojvhhdanxz = 47941;
	double qeiqwworw = 13947;
	double fulbtjhjor = 12329;
	double qygypbcwkuyav = 62249;
	double llagbpssom = 9895;
	if (9895 == 9895) {
		int kmbd;
		for (kmbd = 77; kmbd > 0; kmbd--) {
			continue;
		}
	}
	if (13947 == 13947) {
		int nsysshf;
		for (nsysshf = 8; nsysshf > 0; nsysshf--) {
			continue;
		}
	}
	return true;
}

void tqyufel::kqdmlavyul(int ietgwmsmbsj, int awxmesgkxdxmo, int thgrvgiticoydh, bool wvqirq, int gptqmt, bool cagxefhbk) {
	double syqkcb = 5246;
	int rogboewbbchl = 1097;
	bool jibrwio = true;
	int oppfynv = 2586;
	double rfdqshm = 6033;
	bool nxxcplh = true;
	int lafiwnu = 8301;
	if (2586 == 2586) {
		int tyxpol;
		for (tyxpol = 12; tyxpol > 0; tyxpol--) {
			continue;
		}
	}
	if (1097 != 1097) {
		int yixprsz;
		for (yixprsz = 10; yixprsz > 0; yixprsz--) {
			continue;
		}
	}
	if (true == true) {
		int unsdh;
		for (unsdh = 38; unsdh > 0; unsdh--) {
			continue;
		}
	}
	if (6033 == 6033) {
		int vvnuctpjry;
		for (vvnuctpjry = 59; vvnuctpjry > 0; vvnuctpjry--) {
			continue;
		}
	}
	if (true != true) {
		int rdc;
		for (rdc = 26; rdc > 0; rdc--) {
			continue;
		}
	}

}

string tqyufel::ykneetrpiyscerigmfgbzdjs(string slvvk, int znzagngzujh, double slbrbxjte, bool nblkmjawktnwt, double ypnmjmt, int durcqgjrz) {
	bool uplflupyo = true;
	string kkqxagg = "lrylhpskmraqdamwfmjnkvgjicprpomosdekccnuncnqzuyopjabqorrrabhsvltyts";
	int ixqpci = 2719;
	int pzops = 1134;
	int nuowfus = 3773;
	bool qoylweifzowzbj = false;
	string actlquvzjvy = "ejtmtmebdgffrsmjrhkbxsrenkesjk";
	if (3773 != 3773) {
		int wk;
		for (wk = 72; wk > 0; wk--) {
			continue;
		}
	}
	if (string("ejtmtmebdgffrsmjrhkbxsrenkesjk") != string("ejtmtmebdgffrsmjrhkbxsrenkesjk")) {
		int pbxbheutwn;
		for (pbxbheutwn = 18; pbxbheutwn > 0; pbxbheutwn--) {
			continue;
		}
	}
	if (string("ejtmtmebdgffrsmjrhkbxsrenkesjk") != string("ejtmtmebdgffrsmjrhkbxsrenkesjk")) {
		int xczpqozs;
		for (xczpqozs = 57; xczpqozs > 0; xczpqozs--) {
			continue;
		}
	}
	if (true != true) {
		int jbthrqh;
		for (jbthrqh = 99; jbthrqh > 0; jbthrqh--) {
			continue;
		}
	}
	return string("wuu");
}

double tqyufel::nedbyaapnkml(string xzlbxat, bool rbziwifhlxbk, double nrtjashuyjdg, int zvzbxgidjwlrx) {
	return 66686;
}

int tqyufel::ebyqocqrlbxompxcjctrk(int mfgaznhoxaxhbm, bool tcyrqhcu, string jmqldbgsrsvnn, bool gqzpdfygratqkpi, double hxkpxweqrpg, double jxabowimzaar, double vcngwkgro) {
	bool bjzjt = false;
	bool sdbdegrz = false;
	if (false == false) {
		int vcwqme;
		for (vcwqme = 6; vcwqme > 0; vcwqme--) {
			continue;
		}
	}
	if (false != false) {
		int tmndbpwnna;
		for (tmndbpwnna = 83; tmndbpwnna > 0; tmndbpwnna--) {
			continue;
		}
	}
	if (false != false) {
		int klwm;
		for (klwm = 45; klwm > 0; klwm--) {
			continue;
		}
	}
	if (false == false) {
		int kdflvc;
		for (kdflvc = 44; kdflvc > 0; kdflvc--) {
			continue;
		}
	}
	return 49269;
}

string tqyufel::uemolnxnlvbkhttck(int ktfygsxyay, bool fnvks, double djtgf, double fmgeo, string dgpacqq) {
	double tenzfzfpahyvqc = 3262;
	if (3262 == 3262) {
		int lhdn;
		for (lhdn = 76; lhdn > 0; lhdn--) {
			continue;
		}
	}
	if (3262 == 3262) {
		int zao;
		for (zao = 21; zao > 0; zao--) {
			continue;
		}
	}
	return string("pckjhmj");
}

void tqyufel::zxdnyrqltlp(double gajiwmxo, bool shlkcosxd, string myofgig, int tpnxwnvtonju, bool itfdnvpknxwbrzy, string bublbppukjwy) {
	string siseyjuqduka = "ntvbsyfrlqpmctoeuiqpfooiffkv";
	double yocsgguir = 62078;
	bool hspmnctc = true;
	double bewakclisref = 31827;
	bool xlcrwzqcrcpxzy = true;
	bool qyhuzzzplp = true;
	int mfmjnqhkypdasir = 6341;
	bool nwmwoydc = true;
	if (6341 == 6341) {
		int uczqssj;
		for (uczqssj = 83; uczqssj > 0; uczqssj--) {
			continue;
		}
	}
	if (string("ntvbsyfrlqpmctoeuiqpfooiffkv") == string("ntvbsyfrlqpmctoeuiqpfooiffkv")) {
		int jchqb;
		for (jchqb = 61; jchqb > 0; jchqb--) {
			continue;
		}
	}
	if (true != true) {
		int xt;
		for (xt = 86; xt > 0; xt--) {
			continue;
		}
	}

}

tqyufel::tqyufel() {
	this->xazrtgzuzdlabnrifcwxqaxmi();
	this->hweqgqebwqmmxuflfeibe(string("xutoyoojvkrxivkzkwpjgrzwukvbyxxekcirtzimww"), true, 3036, 3695, 926, 284);
	this->kqdmlavyul(1475, 1696, 7800, false, 6397, true);
	this->ykneetrpiyscerigmfgbzdjs(string("sdyyqjwqrpxthshrmbpwhysldpbdqshcwyktuha"), 9205, 25067, false, 26214, 6386);
	this->nedbyaapnkml(string("iqkvkoyexeqymellkyldapwjqfywhyoaj"), true, 54146, 3053);
	this->ebyqocqrlbxompxcjctrk(4138, true, string("tiaazsxobeqktzcxacbgtryzmysdfibfvsftrallcjfhtgiadccgvgvsde"), false, 15119, 5092, 33933);
	this->uemolnxnlvbkhttck(1663, false, 52732, 21377, string("weosamulvuacjiarvdgtlspwpjzkmxuoatqqefctmifjlbynqjobccmtqwwajuxbzazcwkmopybhcirluamifxfnjfahsdnra"));
	this->zxdnyrqltlp(36570, true, string("wvgaffjwxrferzikiqmyjijjyfdrgrokkpqfbzldjjuosyulddzrsmrngxqsmtmweaskhjrz"), 2685, true, string("sbpfdqvgqseljfpoypxsqumdrjcrtlvjlukjibccloocfzszqaeefndfdojdofesyjned"));
	this->muckkicdjyoxw();
	this->imwpkmhstuiukfupogqtek(false, 662, 899, 25215, 855, string("ieoahilwsolororslenrfhlqpcrrzyrsgctjuwnysjdlolp"));
	this->zmusktsqxxms(959, string("hghedbhxgwnpeylmxjbdabzyrhwgdralqbwojqrtsrzzoybxgdllqippdmyubxfpnhyqwlaicooxahnz"), 16036, string("yycsxojtdzosleefqjrqecshkorbzvozcqxixzcqhburvieehbyytohalufnhprlrevhmcqaushhnvjbvismfrrtrnslymjf"), string("qsfdfjouugzktnlyzssgipsxbmnxoni"), 3628, 4576, 33076, 16369, string("hkyntydfauwaytuwpzvzmwoajlvgtnhmcofchdppvprgvjmosrkmxcfxcmrotoqfhfcqqzknpzrhycnzzggtsuickyxucishxif"));
	this->vyttpzekgdupvuopkaulpjun(false);
	this->bkapqubzufu(1157, string("kclmmebuzdopvbviwloiivpopffdjmnywrxlkfhmuynibyrvtbqffyuextrrhessujpprqdvypodzfizwopfdw"), 5059);
	this->uthqvnouhmgilimlsc(string("mwiekucprmqbbvvczrntagbmeauyzyvckeuhlhfgireijlhpm"));
	this->qacywfzjmwbppckzlzesciky(50565, string("tpjrsdnmnelzvoibmyeejsxwzambgkpmbwlklgqqrglskbnnwnqvatzrjqwbdcxdmqjkdsawgvznektkhgwdctilczx"), false, false, 21940, string("gifcpunuvwldjuzmufzdomqfdsriezxrpruhoeaqeurrlbgklr"));
	this->uegpcnircyrfxzgeqvbm(25957);
	this->jvzzeomfxuidwgk(2555, 935, 2399, 2110, 8457, false, 25872);
	this->chthgtrwdaz(true, string("jlvuriebqfqzqlnxgfeympneiklsldjbzvmshirnyf"), true, 5741, false, string("qaqdaularvefjfritjlbrsrgknjfpivythzkdpkadgxqpuyfiffdecfjkxqmskbswrybomwei"), string("adjwlxwohiefisyqrdpwyjeirdijkhzasoukouqvjguzzatknrejzcrcyxseefomiwetxfixoyzrrwrpfqvmhbx"), false, 50092, 681);
	this->puvkeaypsqhmmkgl(string("eftavtdmbbjmzeaavvfyfmnzqfgucfpftmgaphqhvgjqcpdgmntplbrqxrkwmqhekllrghlukgrloperppmzewpfzjjzduxiyay"), 6948, 10211, 35788);
	this->sdfvklhsrnnnc(string("vqetoaudogpmlrboehxozwmugbp"), 40353, 548);
	this->ynjsvhspty(58114, string("ssyrvekuicrsmgmmnsjstxmlecwybaumbhunirntfdv"), true);
	this->ifrxbqofbboxhnybiijkrm(2126, string("uuovwqdrnqfpmwwlgcjwyspdhtvtcemhwjgwrxwhjlaamieazdtvmnxjsyyptqybrixpqcyjcaaohjonnvhfl"), 685, 1626, string("ycuzxwxkgzxnp"), 3603);
	this->chsosxljojpzxnfd(string("krfdaksidtedxefaxzmudurxdjqlktorzxyrtshaqquqrlbaxy"), 1367, false, string("rvuuutkubqclyigzszauohgzajhgeshcmmbwialremfljifsnuuypfziiahgtkeetfwzewgsamdjrludaoohhwurnmiqkzmrlo"), string("rtmejsoubhzedlkhqfvavomwhufkqrvkalrhvlpslsvuyckebwjmnczoxv"), string("cykhixtatpiqpyzebpwknznkpwtegjqxbmfizawihdluwrhhymxqcmcbdapsqwvzgnfohqikakbabhawjl"), 1797, 37109, 6671);
}
