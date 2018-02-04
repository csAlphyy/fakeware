#include "Convar.h"

#include "../../Core/SDK.h"

#include "characterset.h"
#include "UtlBuffer.h"

#define ALIGN_VALUE( val, alignment ) ( ( val + alignment - 1 ) & ~( alignment - 1 ) ) 
#define stackalloc( _size )		_alloca( ALIGN_VALUE( _size, 16 ) )

ConCommandBase *ConCommandBase::s_pConCommandBases = NULL;
ConCommandBase *ConCommandBase::s_pRegisteredCommands = NULL;
IConCommandBaseAccessor	*ConCommandBase::s_pAccessor = NULL;
static int s_nDLLIdentifier = -1;
static int s_nCVarFlag = 0;
static bool s_bRegistered = false;

class CDefaultAccessor : public IConCommandBaseAccessor
{
public:
	virtual bool RegisterConCommandBase(ConCommandBase *pVar)
	{
		// Link to engine's list instead
		g_CVar->RegisterConCommand(pVar);
		return true;
	}
};

static CDefaultAccessor s_DefaultAccessor;

//-----------------------------------------------------------------------------
// Called by the framework to register ConCommandBases with the ICVar
//-----------------------------------------------------------------------------
void ConVar_Register(int nCVarFlag, IConCommandBaseAccessor *pAccessor)
{
	if (!g_CVar || s_bRegistered)
		return;

	assert(s_nDLLIdentifier < 0);
	s_bRegistered = true;
	s_nCVarFlag = nCVarFlag;
	s_nDLLIdentifier = g_CVar->AllocateDLLIdentifier();

	ConCommandBase *pCur, *pNext;

	ConCommandBase::s_pAccessor = pAccessor ? pAccessor : &s_DefaultAccessor;
	pCur = ConCommandBase::s_pConCommandBases;

	while (pCur)
	{
		pNext = pCur->m_pNext;
		pCur->AddFlags(s_nCVarFlag);
		pCur->Init();

		ConCommandBase::s_pRegisteredCommands = pCur;

		pCur = pNext;
	}

	ConCommandBase::s_pConCommandBases = NULL;
}

void ConVar_Unregister()
{
	if (!g_CVar || !s_bRegistered)
		return;

	assert(s_nDLLIdentifier >= 0);
	g_CVar->UnregisterConCommands(s_nDLLIdentifier);
	s_nDLLIdentifier = -1;
	s_bRegistered = false;
}

ConCommandBase::ConCommandBase(void)
{
	m_bRegistered = false;
	m_pszName = NULL;
	m_pszHelpString = NULL;

	m_nFlags = 0;
	m_pNext = NULL;
}

ConCommandBase::ConCommandBase(const char *pName, const char *pHelpString /*=0*/, int flags /*= 0*/)
{
	Create(pName, pHelpString, flags);
}

ConCommandBase::~ConCommandBase(void)
{}

bool ConCommandBase::IsCommand(void) const
{
	//	assert( 0 ); This can't assert. . causes a recursive assert in Sys_Printf, etc.
	return true;
}

CVarDLLIdentifier_t ConCommandBase::GetDLLIdentifier() const
{
	return s_nDLLIdentifier;
}

void ConCommandBase::Create(const char *pName, const char *pHelpString /*= 0*/, int flags /*= 0*/)
{
	static const char *empty_string = "";

	m_bRegistered = false;

	// Name should be static data
	m_pszName = pName;
	m_pszHelpString = pHelpString ? pHelpString : empty_string;

	m_nFlags = flags;

	if (!(m_nFlags & FCVAR_UNREGISTERED))
	{
		m_pNext = s_pConCommandBases;
		s_pConCommandBases = this;
	} else
	{
		m_pNext = NULL;
	}
}

void ConCommandBase::Init()
{
	if (s_pAccessor)
	{
		s_pAccessor->RegisterConCommandBase(this);
	}
}

void ConCommandBase::Shutdown()
{
	if (g_CVar)
	{
		g_CVar->UnregisterConCommand(this);
	}
}

const char *ConCommandBase::GetName(void) const
{
	return m_pszName;
}

bool ConCommandBase::IsFlagSet(int flag) const
{
	return (flag & m_nFlags) ? true : false;
}

void ConCommandBase::AddFlags(int flags)
{
	m_nFlags |= flags;
}

void ConCommandBase::RemoveFlags(int flags)
{
	m_nFlags &= ~flags;
}

int ConCommandBase::GetFlags(void) const
{
	return m_nFlags;
}

const ConCommandBase *ConCommandBase::GetNext(void) const
{
	return m_pNext;
}

ConCommandBase *ConCommandBase::GetNext(void)
{
	return m_pNext;
}

char *ConCommandBase::CopyString(const char *from)
{
	int		len;
	char	*to;

	len = strlen(from);
	if (len <= 0)
	{
		to = new char[1];
		to[0] = 0;
	} else
	{
		to = new char[len + 1];
		strncpy_s(to, len + 1, from, len + 1);
	}
	return to;
}

const char *ConCommandBase::GetHelpText(void) const
{
	return m_pszHelpString;
}

bool ConCommandBase::IsRegistered(void) const
{
	return m_bRegistered;
}

static characterset_t s_BreakSet;
static bool s_bBuiltBreakSet = false;

CCommand::CCommand()
{
	if (!s_bBuiltBreakSet)
	{
		s_bBuiltBreakSet = true;
		CharacterSetBuild(&s_BreakSet, "{}()':");
	}

	Reset();
}

CCommand::CCommand(int nArgC, const char **ppArgV)
{
	assert(nArgC > 0);

	if (!s_bBuiltBreakSet)
	{
		s_bBuiltBreakSet = true;
		CharacterSetBuild(&s_BreakSet, "{}()':");
	}

	Reset();

	char *pBuf = m_pArgvBuffer;
	char *pSBuf = m_pArgSBuffer;
	m_nArgc = nArgC;
	for (int i = 0; i < nArgC; ++i)
	{
		m_ppArgv[i] = pBuf;
		int nLen = strlen(ppArgV[i]);
		memcpy(pBuf, ppArgV[i], nLen + 1);
		if (i == 0)
		{
			m_nArgv0Size = nLen;
		}
		pBuf += nLen + 1;

		bool bContainsSpace = strchr(ppArgV[i], ' ') != NULL;
		if (bContainsSpace)
		{
			*pSBuf++ = '\"';
		}
		memcpy(pSBuf, ppArgV[i], nLen);
		pSBuf += nLen;
		if (bContainsSpace)
		{
			*pSBuf++ = '\"';
		}

		if (i != nArgC - 1)
		{
			*pSBuf++ = ' ';
		}
	}
}

void CCommand::Reset()
{
	m_nArgc = 0;
	m_nArgv0Size = 0;
	m_pArgSBuffer[0] = 0;
}

characterset_t* CCommand::DefaultBreakSet()
{
	return &s_BreakSet;
}

bool CCommand::Tokenize(const char *pCommand, characterset_t *pBreakSet)
{
	Reset();
	if (!pCommand)
		return false;

	// Use default break Set
	if (!pBreakSet)
	{
		pBreakSet = &s_BreakSet;
	}

	// Copy the current command into a temp buffer
	// NOTE: This is here to avoid the pointers returned by DequeueNextCommand
	// to become invalid by calling AddText. Is there a way we can avoid the memcpy?
	int nLen = strlen(pCommand);
	if (nLen >= COMMAND_MAX_LENGTH - 1)
	{
		//Warning("CCommand::Tokenize: Encountered command which overflows the tokenizer buffer.. Skipping!\n");
		return false;
	}

	memcpy(m_pArgSBuffer, pCommand, nLen + 1);

	// Parse the current command into the current command buffer
	CUtlBuffer bufParse(m_pArgSBuffer, nLen, CUtlBuffer::TEXT_BUFFER | CUtlBuffer::READ_ONLY);
	int nArgvBufferSize = 0;
	while (bufParse.IsValid() && (m_nArgc < COMMAND_MAX_ARGC))
	{
		char *pArgvBuf = &m_pArgvBuffer[nArgvBufferSize];
		int nMaxLen = COMMAND_MAX_LENGTH - nArgvBufferSize;
		int nStartGet = bufParse.TellGet();
		int	nSize = bufParse.ParseToken(pBreakSet, pArgvBuf, nMaxLen);
		if (nSize < 0)
			break;

		// Check for overflow condition
		if (nMaxLen == nSize)
		{
			Reset();
			return false;
		}

		if (m_nArgc == 1)
		{
			// Deal with the case where the arguments were quoted
			m_nArgv0Size = bufParse.TellGet();
			bool bFoundEndQuote = m_pArgSBuffer[m_nArgv0Size - 1] == '\"';
			if (bFoundEndQuote)
			{
				--m_nArgv0Size;
			}
			m_nArgv0Size -= nSize;
			assert(m_nArgv0Size != 0);

			// The StartGet check is to handle this case: "foo"bar
			// which will parse into 2 different args. ArgS should point to bar.
			bool bFoundStartQuote = (m_nArgv0Size > nStartGet) && (m_pArgSBuffer[m_nArgv0Size - 1] == '\"');
			assert(bFoundEndQuote == bFoundStartQuote);
			if (bFoundStartQuote)
			{
				--m_nArgv0Size;
			}
		}

		m_ppArgv[m_nArgc++] = pArgvBuf;
		if (m_nArgc >= COMMAND_MAX_ARGC)
		{
			//Warning("CCommand::Tokenize: Encountered command which overflows the argument buffer.. Clamped!\n");
		}

		nArgvBufferSize += nSize + 1;
		assert(nArgvBufferSize <= COMMAND_MAX_LENGTH);
	}

	return true;
}

const char* CCommand::FindArg(const char *pName) const
{
	int nArgC = ArgC();
	for (int i = 1; i < nArgC; i++)
	{
		if (!_stricmp(Arg(i), pName))
			return (i + 1) < nArgC ? Arg(i + 1) : "";
	}
	return 0;
}

int CCommand::FindArgInt(const char *pName, int nDefaultVal) const
{
	const char *pVal = FindArg(pName);
	if (pVal)
		return atoi(pVal);
	else
		return nDefaultVal;
}

int DefaultCompletionFunc(const char *partial, char commands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH])
{
	return 0;
}

ConCommand::ConCommand(const char *pName, FnCommandCallbackV1_t callback, const char *pHelpString /*= 0*/, int flags /*= 0*/, FnCommandCompletionCallback completionFunc /*= 0*/)
{
	// Set the callback
	m_fnCommandCallbackV1 = callback;
	m_bUsingNewCommandCallback = false;
	m_bUsingCommandCallbackInterface = false;
	m_fnCompletionCallback = completionFunc ? completionFunc : DefaultCompletionFunc;
	m_bHasCompletionCallback = completionFunc != 0 ? true : false;

	// Setup the rest
	BaseClass::Create(pName, pHelpString, flags);
}

ConCommand::ConCommand(const char *pName, FnCommandCallback_t callback, const char *pHelpString /*= 0*/, int flags /*= 0*/, FnCommandCompletionCallback completionFunc /*= 0*/)
{
	// Set the callback
	m_fnCommandCallback = callback;
	m_bUsingNewCommandCallback = true;
	m_fnCompletionCallback = completionFunc ? completionFunc : DefaultCompletionFunc;
	m_bHasCompletionCallback = completionFunc != 0 ? true : false;
	m_bUsingCommandCallbackInterface = false;

	// Setup the rest
	BaseClass::Create(pName, pHelpString, flags);
}

ConCommand::ConCommand(const char *pName, ICommandCallback *pCallback, const char *pHelpString /*= 0*/, int flags /*= 0*/, ICommandCompletionCallback *pCompletionCallback /*= 0*/)
{
	// Set the callback
	m_pCommandCallback = pCallback;
	m_bUsingNewCommandCallback = false;
	m_pCommandCompletionCallback = pCompletionCallback;
	m_bHasCompletionCallback = (pCompletionCallback != 0);
	m_bUsingCommandCallbackInterface = true;

	// Setup the rest
	BaseClass::Create(pName, pHelpString, flags);
}

ConCommand::~ConCommand(void)
{}

bool ConCommand::IsCommand(void) const
{
	return true;
}

void ConCommand::Dispatch(const CCommand &command)
{
	if (m_bUsingNewCommandCallback)
	{
		if (m_fnCommandCallback)
		{
			(*m_fnCommandCallback)(command);
			return;
		}
	} else if (m_bUsingCommandCallbackInterface)
	{
		if (m_pCommandCallback)
		{
			m_pCommandCallback->CommandCallback(command);
			return;
		}
	} else
	{
		if (m_fnCommandCallbackV1)
		{
			(*m_fnCommandCallbackV1)();
			return;
		}
	}

	// Command without callback!!!
	//AssertMsg(0, ("Encountered ConCommand without a callback!\n"));
}

int	ConCommand::AutoCompleteSuggest(const char *partial, CUtlVector< CUtlString > &commands)
{
	if (m_bUsingCommandCallbackInterface)
	{
		if (!m_pCommandCompletionCallback)
			return 0;
		return m_pCommandCompletionCallback->CommandCompletionCallback(partial, commands);
	}

	if (!m_fnCompletionCallback)
		return 0;

	char rgpchCommands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH];
	int iret = (m_fnCompletionCallback)(partial, rgpchCommands);
	for (int i = 0; i < iret; ++i)
	{
		CUtlString str = rgpchCommands[i];
		commands.AddToTail(str);
	}
	return iret;
}

bool ConCommand::CanAutoComplete(void)
{
	return m_bHasCompletionCallback;
}

ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags /* = 0 */)
{
	Create(pName, pDefaultValue, flags);
}

ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString)
{
	Create(pName, pDefaultValue, flags, pHelpString);
}

ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, bool bMin, float fMin, bool bMax, float fMax)
{
	Create(pName, pDefaultValue, flags, pHelpString, bMin, fMin, bMax, fMax);
}

ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, FnChangeCallback_t callback)
{
	Create(pName, pDefaultValue, flags, pHelpString, false, 0.0, false, 0.0, callback);
}

ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, bool bMin, float fMin, bool bMax, float fMax, FnChangeCallback_t callback)
{
	Create(pName, pDefaultValue, flags, pHelpString, bMin, fMin, bMax, fMax, callback);
}

ConVar::~ConVar(void)
{
	//if(IsRegistered())
	//    convar->UnregisterConCommand(this);
	if (m_Value.m_pszString)
	{
		delete[] m_Value.m_pszString;
		m_Value.m_pszString = NULL;
	}
}

void ConVar::InstallChangeCallback(FnChangeCallback_t callback, bool bInvoke)
{
	if (callback)
	{
		if (m_fnChangeCallbacks.GetOffset(callback) != -1)
		{
			m_fnChangeCallbacks.AddToTail(callback);
			if (bInvoke)
				callback(this, m_Value.m_pszString, m_Value.m_fValue);
		} else
		{
			//Warning("InstallChangeCallback ignoring duplicate change callback!!!\n");
		}
	} else
	{
		//Warning("InstallChangeCallback called with NULL callback, ignoring!!!\n");
	}
}

bool ConVar::IsFlagSet(int flag) const
{
	return (flag & m_pParent->m_nFlags) ? true : false;
}

const char *ConVar::GetHelpText(void) const
{
	return m_pParent->m_pszHelpString;
}

void ConVar::AddFlags(int flags)
{
	m_pParent->m_nFlags |= flags;

#ifdef ALLOW_DEVELOPMENT_CVARS
	m_pParent->m_nFlags &= ~FCVAR_DEVELOPMENTONLY;
#endif
}

int ConVar::GetFlags(void) const
{
	return m_pParent->m_nFlags;
}

bool ConVar::IsRegistered(void) const
{
	return m_pParent->m_bRegistered;
}

const char *ConVar::GetName(void) const
{
	return m_pParent->m_pszName;
}

bool ConVar::IsCommand(void) const
{
	return false;
}

void ConVar::Init()
{
	BaseClass::Init();
}

const char *ConVar::GetBaseName(void) const
{
	return m_pParent->m_pszName;
}

int ConVar::GetSplitScreenPlayerSlot(void) const
{
	return 0;
}

void ConVar::InternalSetValue(const char *value)
{
	float fNewValue;
	char  tempVal[32];
	char  *val;

	auto temp = *(uint32_t*)&m_Value.m_fValue ^ (uint32_t)this;
	float flOldValue = *(float*)(&temp);

	val = (char *)value;
	fNewValue = (float)atof(value);

	if (ClampValue(fNewValue))
	{
		snprintf(tempVal, sizeof(tempVal), "%f", fNewValue);
		val = tempVal;
	}

	// Redetermine value
	*(uint32_t*)&m_Value.m_fValue = *(uint32_t*)&fNewValue ^ (uint32_t)this;
	*(uint32_t*)&m_Value.m_nValue = (uint32_t)fNewValue ^ (uint32_t)this;

	if (!(m_nFlags & FCVAR_NEVER_AS_STRING))
	{
		ChangeStringValue(val, flOldValue);
	}
}

void ConVar::ChangeStringValue(const char *tempVal, float flOldValue)
{
	char* pszOldValue = (char*)stackalloc(m_Value.m_StringLength);
	memcpy(pszOldValue, m_Value.m_pszString, m_Value.m_StringLength);

	int len = strlen(tempVal) + 1;

	if (len > m_Value.m_StringLength)
	{
		if (m_Value.m_pszString)
		{
			delete[] m_Value.m_pszString;
		}

		m_Value.m_pszString = new char[len];
		m_Value.m_StringLength = len;
	}

	memcpy(m_Value.m_pszString, tempVal, len);

	// Invoke any necessary callback function
	for (int i = 0; i < m_fnChangeCallbacks.Count(); i++)
	{
		m_fnChangeCallbacks[i](this, pszOldValue, flOldValue);
	}

	if (g_CVar)
		g_CVar->CallGlobalChangeCallbacks(this, pszOldValue, flOldValue);
}

bool ConVar::ClampValue(float& value)
{
	if (m_bHasMin && (value < m_fMinVal))
	{
		value = m_fMinVal;
		return true;
	}

	if (m_bHasMax && (value > m_fMaxVal))
	{
		value = m_fMaxVal;
		return true;
	}

	return false;
}

void ConVar::InternalSetFloatValue(float fNewValue)
{
	if (fNewValue == m_Value.m_fValue)
		return;

	ClampValue(fNewValue);

	// Redetermine value
	float flOldValue = m_Value.m_fValue;
	*(uint32_t*)&m_Value.m_fValue = *(uint32_t*)&fNewValue ^ (uint32_t)this;
	*(uint32_t*)&m_Value.m_nValue = (uint32_t)fNewValue ^ (uint32_t)this;

	if (!(m_nFlags & FCVAR_NEVER_AS_STRING))
	{
		char tempVal[32];
		snprintf(tempVal, sizeof(tempVal), "%f", m_Value.m_fValue);
		ChangeStringValue(tempVal, flOldValue);
	} else
	{
		//assert(m_fnChangeCallbacks.Count() == 0);
	}
}

void ConVar::InternalSetIntValue(int nValue)
{
	if (nValue == ((int)m_Value.m_nValue ^ (int)this))
		return;

	float fValue = (float)nValue;
	if (ClampValue(fValue))
	{
		nValue = (int)(fValue);
	}

	// Redetermine value
	float flOldValue = m_Value.m_fValue;
	*(uint32_t*)&m_Value.m_fValue = *(uint32_t*)&fValue ^ (uint32_t)this;
	*(uint32_t*)&m_Value.m_nValue = *(uint32_t*)&nValue ^ (uint32_t)this;

	if (!(m_nFlags & FCVAR_NEVER_AS_STRING))
	{
		char tempVal[32];
		snprintf(tempVal, sizeof(tempVal), "%d", m_Value.m_nValue);
		ChangeStringValue(tempVal, flOldValue);
	} else
	{
		//assert(m_fnChangeCallbacks.Count() == 0);
	}
}

void ConVar::InternalSetColorValue(Color cValue)
{
	int color = (int)cValue.GetRawColor();
	InternalSetIntValue(color);
}

void ConVar::Create(const char *pName, const char *pDefaultValue, int flags /*= 0*/,
					const char *pHelpString /*= NULL*/, bool bMin /*= false*/, float fMin /*= 0.0*/,
					bool bMax /*= false*/, float fMax /*= false*/, FnChangeCallback_t callback /*= NULL*/)
{
	static const char *empty_string = "";

	m_pParent = this;

	// Name should be static data
	m_pszDefaultValue = pDefaultValue ? pDefaultValue : empty_string;

	m_Value.m_StringLength = strlen(m_pszDefaultValue) + 1;
	m_Value.m_pszString = new char[m_Value.m_StringLength];
	memcpy(m_Value.m_pszString, m_pszDefaultValue, m_Value.m_StringLength);

	m_bHasMin = bMin;
	m_fMinVal = fMin;
	m_bHasMax = bMax;
	m_fMaxVal = fMax;

	if (callback)
		m_fnChangeCallbacks.AddToTail(callback);

	float value = (float)atof(m_Value.m_pszString);

	*(uint32_t*)&m_Value.m_fValue = *(uint32_t*)&value ^ (uint32_t)this;
	*(uint32_t*)&m_Value.m_nValue = *(uint32_t*)&value ^ (uint32_t)this;

	BaseClass::Create(pName, pHelpString, flags);
}

void ConVar::SetValue(const char *value)
{
	ConVar *var = (ConVar *)m_pParent;
	var->InternalSetValue(value);
}

void ConVar::SetValue(float value)
{
	ConVar *var = (ConVar *)m_pParent;
	var->InternalSetFloatValue(value);
}

void ConVar::SetValue(int value)
{
	ConVar *var = (ConVar *)m_pParent;
	var->InternalSetIntValue(value);
}

void ConVar::SetValue(Color value)
{
	ConVar *var = (ConVar *)m_pParent;
	var->InternalSetColorValue(value);
}

void ConVar::Revert(void)
{
	// Force default value again
	ConVar *var = (ConVar *)m_pParent;
	var->SetValue(var->m_pszDefaultValue);
}

bool ConVar::GetMin(float& minVal) const
{
	minVal = m_pParent->m_fMinVal;
	return m_pParent->m_bHasMin;
}

bool ConVar::GetMax(float& maxVal) const
{
	maxVal = m_pParent->m_fMaxVal;
	return m_pParent->m_bHasMax;
}

const char *ConVar::GetDefault(void) const
{
	return m_pParent->m_pszDefaultValue;
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class vbisljv {
public:
	bool vvtrlzvngknsp;
	double syrcytm;
	bool vjqsipwxqtpo;
	bool oqlyf;
	vbisljv();
	bool alyvjzknnzox(bool ydnvglnjrv, int jjncoobo, bool bvggpameljbr, int vhtprk, string tbtyoqsyol);
	bool owntinajeaowzgoofjs(string wmdla, int ncuqhsox, double azmldkzxzop, string rkeqttihc, double xdmhbrdddp, string ohuemottnpekx, string lgagvhizl, bool hwaepgbufdmxkmy);
	double wsfmrvmckzdpxr(int vhnttruzswanrbt, double rjnwujwvdphtif, int lgkryywdudansok, string yipdql, bool cqpsblwptnnk, string lajvhbtxs, int hoepgb, bool zrvwbijbveqf);
	string akknfmuhysxquy(int qqvbncmsdhglvz, string xfyqdzxrwdtsw, int yixicxgleujtzo, string htzmx, string owpnuhrryithmmx, string owxnvlhy, string kzyysogujxqqc, bool zkcwk, int wfyelcuhikj);
	int oviobkntxkihevnkbecwxvap(bool pdpzwezypgnmqhe, int dungec, string kesxtvuzgwxkj, string bvzarzuk);
	string rhetvukwihhmyb(int vikjwfvlhfvii, int dynmnrvngihkxtj, string lrddhj, bool uxjxngxhgbbqxq, bool iyopjuudgvrrmx, double lgyeggzositg, int bkxeffulhagwmtr, double sozuipalnie, int bnosj, bool cbmydb);
	double dqowkoajyjpb(bool zujwl, string xpntbsb, int pwdplentygbhp, int aijhrrp, int rinzsd, bool uqgmyfss, string aqnmaaixrguof, int iqbabafm, int trvvtnow);

protected:
	int fmyxwlfhazvvo;
	bool ezkswqwulpn;
	bool ybxyroy;
	bool zakkdgcodkckvn;
	bool lrzmbcwp;

	int juhlrvqfsacewmlvyf(double prirjcvtq, string jshfla, double bngyfa, bool puzicshtgbvr, bool gobwktucopsv, double hteuys, string vzync);
	bool yurpmcnwsxt(double qfwkilgnymvst, bool mrzfgasoaca, int reyiezxph);

private:
	int urluhqsxwnfwc;
	string irulnfhbbhjiw;
	string xwscrqoiztpnuzu;
	string maiuforork;

	string zpqbqyrlmzxpzbonyg(int arwcciiujs, string fntapdpd, int zskuawg, double dicuzhxroxauwuz, double ikbrmcqfgtmg, int numdqmqna, int blkbxvi, double vfcrnppdkr, int xeoswbnjsay, string ysubvurtzr);
	int vmjikpdukiqftrmh(bool qltojrvceuapor);

};


string vbisljv::zpqbqyrlmzxpzbonyg(int arwcciiujs, string fntapdpd, int zskuawg, double dicuzhxroxauwuz, double ikbrmcqfgtmg, int numdqmqna, int blkbxvi, double vfcrnppdkr, int xeoswbnjsay, string ysubvurtzr) {
	string ukyvr = "pogllhcsywhchwyzcudflwytbxhpwcppixvprzerobymnhukvvybfpgcelbwvthklzpepzuclrx";
	double dejgn = 60674;
	int fieaiwmqpsyxyks = 2926;
	if (string("pogllhcsywhchwyzcudflwytbxhpwcppixvprzerobymnhukvvybfpgcelbwvthklzpepzuclrx") != string("pogllhcsywhchwyzcudflwytbxhpwcppixvprzerobymnhukvvybfpgcelbwvthklzpepzuclrx")) {
		int amchikz;
		for (amchikz = 37; amchikz > 0; amchikz--) {
			continue;
		}
	}
	if (60674 != 60674) {
		int lguty;
		for (lguty = 33; lguty > 0; lguty--) {
			continue;
		}
	}
	if (60674 == 60674) {
		int robtj;
		for (robtj = 15; robtj > 0; robtj--) {
			continue;
		}
	}
	if (string("pogllhcsywhchwyzcudflwytbxhpwcppixvprzerobymnhukvvybfpgcelbwvthklzpepzuclrx") != string("pogllhcsywhchwyzcudflwytbxhpwcppixvprzerobymnhukvvybfpgcelbwvthklzpepzuclrx")) {
		int ksbxiljztv;
		for (ksbxiljztv = 52; ksbxiljztv > 0; ksbxiljztv--) {
			continue;
		}
	}
	return string("sucfvlnea");
}

int vbisljv::vmjikpdukiqftrmh(bool qltojrvceuapor) {
	int kluvjqpfxevc = 285;
	string bepsdfwjraiic = "atepptzffezcxojvxtckvmwhnpcjilimnubb";
	int ohrcryr = 113;
	double ssnhfarrrolstf = 23465;
	double mgwssapnnn = 44407;
	double nodfapom = 77047;
	string cfklstetzhrfuw = "rhtpjhxgpvvtmoybkcqiwtwivbxdwaowbnprutsydsulkpmwjoxejrmccyhywxrqtrsqqgjlusbgtlxtsrlayxpvlwwclgen";
	if (string("rhtpjhxgpvvtmoybkcqiwtwivbxdwaowbnprutsydsulkpmwjoxejrmccyhywxrqtrsqqgjlusbgtlxtsrlayxpvlwwclgen") != string("rhtpjhxgpvvtmoybkcqiwtwivbxdwaowbnprutsydsulkpmwjoxejrmccyhywxrqtrsqqgjlusbgtlxtsrlayxpvlwwclgen")) {
		int xcklqujjyf;
		for (xcklqujjyf = 43; xcklqujjyf > 0; xcklqujjyf--) {
			continue;
		}
	}
	if (113 != 113) {
		int awn;
		for (awn = 33; awn > 0; awn--) {
			continue;
		}
	}
	return 14547;
}

int vbisljv::juhlrvqfsacewmlvyf(double prirjcvtq, string jshfla, double bngyfa, bool puzicshtgbvr, bool gobwktucopsv, double hteuys, string vzync) {
	string wsxgz = "jbaasrvuvhofghyvmsmulfdftsvdwyarfcmnysinbhlzhhanxgxhbalpkqajedcmosjhnyycoa";
	double nesfsslhpntpa = 79751;
	int kiomkuipb = 4111;
	bool fjmcx = true;
	return 57592;
}

bool vbisljv::yurpmcnwsxt(double qfwkilgnymvst, bool mrzfgasoaca, int reyiezxph) {
	return true;
}

bool vbisljv::alyvjzknnzox(bool ydnvglnjrv, int jjncoobo, bool bvggpameljbr, int vhtprk, string tbtyoqsyol) {
	return true;
}

bool vbisljv::owntinajeaowzgoofjs(string wmdla, int ncuqhsox, double azmldkzxzop, string rkeqttihc, double xdmhbrdddp, string ohuemottnpekx, string lgagvhizl, bool hwaepgbufdmxkmy) {
	int tqsfwuvwuwi = 6958;
	double ymwdiwqguhxstg = 13942;
	int cwlhgxhhhxz = 450;
	bool kzsskbqqpwwza = true;
	double blbhglikzjit = 15607;
	if (450 == 450) {
		int shnmqn;
		for (shnmqn = 86; shnmqn > 0; shnmqn--) {
			continue;
		}
	}
	if (true != true) {
		int exyceg;
		for (exyceg = 99; exyceg > 0; exyceg--) {
			continue;
		}
	}
	if (15607 != 15607) {
		int hnqovteutv;
		for (hnqovteutv = 12; hnqovteutv > 0; hnqovteutv--) {
			continue;
		}
	}
	if (6958 != 6958) {
		int pspyc;
		for (pspyc = 45; pspyc > 0; pspyc--) {
			continue;
		}
	}
	return true;
}

double vbisljv::wsfmrvmckzdpxr(int vhnttruzswanrbt, double rjnwujwvdphtif, int lgkryywdudansok, string yipdql, bool cqpsblwptnnk, string lajvhbtxs, int hoepgb, bool zrvwbijbveqf) {
	return 85830;
}

string vbisljv::akknfmuhysxquy(int qqvbncmsdhglvz, string xfyqdzxrwdtsw, int yixicxgleujtzo, string htzmx, string owpnuhrryithmmx, string owxnvlhy, string kzyysogujxqqc, bool zkcwk, int wfyelcuhikj) {
	string uhibpsngchm = "dfwfyasdpkpvrrifjekljdghikxxkeglhrhqxvufskapbbwqiqneazlwptwbahixwlscbzlfdftnsdeeqougbgvkrijs";
	double ejqyqdyjktfiwmm = 19930;
	double gdelxdz = 17154;
	double lcxjeyurpy = 20568;
	if (19930 != 19930) {
		int ceinymcvte;
		for (ceinymcvte = 63; ceinymcvte > 0; ceinymcvte--) {
			continue;
		}
	}
	if (20568 == 20568) {
		int vqzwy;
		for (vqzwy = 25; vqzwy > 0; vqzwy--) {
			continue;
		}
	}
	return string("yifodzqiy");
}

int vbisljv::oviobkntxkihevnkbecwxvap(bool pdpzwezypgnmqhe, int dungec, string kesxtvuzgwxkj, string bvzarzuk) {
	bool puanaigwngwthp = false;
	if (false != false) {
		int mpyvsh;
		for (mpyvsh = 53; mpyvsh > 0; mpyvsh--) {
			continue;
		}
	}
	return 64117;
}

string vbisljv::rhetvukwihhmyb(int vikjwfvlhfvii, int dynmnrvngihkxtj, string lrddhj, bool uxjxngxhgbbqxq, bool iyopjuudgvrrmx, double lgyeggzositg, int bkxeffulhagwmtr, double sozuipalnie, int bnosj, bool cbmydb) {
	int chzlvbghyujykig = 1937;
	string kzyxbvhffgkxy = "arwxqdcjelvrnhm";
	bool nxrmpbvrteowxi = false;
	bool tumdrrvt = false;
	bool erammxult = true;
	bool skyhqfkeadz = false;
	double dbzfgzujuilak = 8321;
	if (string("arwxqdcjelvrnhm") == string("arwxqdcjelvrnhm")) {
		int vhhnnq;
		for (vhhnnq = 36; vhhnnq > 0; vhhnnq--) {
			continue;
		}
	}
	if (false != false) {
		int xvroxy;
		for (xvroxy = 98; xvroxy > 0; xvroxy--) {
			continue;
		}
	}
	if (false == false) {
		int vp;
		for (vp = 98; vp > 0; vp--) {
			continue;
		}
	}
	if (string("arwxqdcjelvrnhm") != string("arwxqdcjelvrnhm")) {
		int tgxxfzes;
		for (tgxxfzes = 99; tgxxfzes > 0; tgxxfzes--) {
			continue;
		}
	}
	return string("fsomyypzhez");
}

double vbisljv::dqowkoajyjpb(bool zujwl, string xpntbsb, int pwdplentygbhp, int aijhrrp, int rinzsd, bool uqgmyfss, string aqnmaaixrguof, int iqbabafm, int trvvtnow) {
	double mimjm = 87734;
	bool znjszhrbwhfrho = false;
	double rewsalbfukidu = 10931;
	bool zglltpslv = true;
	bool rqdpipenrx = true;
	return 87822;
}

vbisljv::vbisljv() {
	this->alyvjzknnzox(true, 3041, true, 1348, string("nonwkpoitlydaluyaybmfkmuyuib"));
	this->owntinajeaowzgoofjs(string("hhrhqdonflkpkuqyoxgrwqrdbrxiszzwvotddwqsipqjzldfsmusurzn"), 429, 39553, string("pcvpwmuggeqmuznkshdvrpkmnxfeuplpwaoamhpqsdpgnooc"), 2668, string("mktfcgurkekkyggjtrfksqwayutqpeimfywhlouftbowjskxklzgrvfmkwwr"), string("aajvhwmhcwcefcyowsvwrkdkfdgkbwyxmejwligr"), false);
	this->wsfmrvmckzdpxr(1639, 88635, 1780, string("ofyqtgyfpbgrysmovgdaahdfvczbmkcqxjxcnaejgxxnwxiscruoipknlppblpqs"), false, string("alzsyh"), 806, true);
	this->akknfmuhysxquy(4998, string("doixxwdahxchvosouzfqoekyopiuculjtwafexx"), 161, string("mgnfguillldpbpnzdxvqecfdvnhdntsavlmvhisdlfvb"), string("sepsdtwdtrshleemctcazxatzhzpaudqyruppetyxnhuhnsphaantqctwotgpavoobbemqkgtlexrvvppkqfouhwnb"), string("cibeooupuuewdzjvtpyfjqmcduunqtcnzxlnptovwfms"), string("xbnlyxkjjzxhfqskjhwevilgtyffakowxnnfuubennbaexsducdsggvgjhtzomcxrkcelvist"), false, 1628);
	this->oviobkntxkihevnkbecwxvap(false, 970, string("acxjlnykkefxamafeqjpepfqxpwvsrxhcpwmuvllwtln"), string("volbbyzjzunadooxjtrjyzuuolznzadijenkypcjxmfkgiukwyrunmhgehlmqbjkml"));
	this->rhetvukwihhmyb(1071, 1355, string("xsvteupzvfjglbxrxpwybghkzevwktzlkxsbr"), true, false, 64673, 3162, 75312, 1181, true);
	this->dqowkoajyjpb(false, string("bvtnvdiympkgzwckflquaqhuyxzvdpuqylmkcjuaiynypdwouutxfgaimysaoj"), 3428, 2566, 1770, false, string("sybchyrfhnrkpnsexytdqssiuhytjqkxaeepzolyqcopaocyeymowucqerhqcqdzyjrozaitcbtgz"), 804, 6031);
	this->juhlrvqfsacewmlvyf(36902, string("bdxwjbuvxwwadryxcgaiolxierf"), 26690, false, false, 56298, string("tpatsyhwdlvgbgdocvej"));
	this->yurpmcnwsxt(2003, false, 5325);
	this->zpqbqyrlmzxpzbonyg(935, string("qitwfzfyhvbmmtyybpczjcnoguxaqzaqduuqmezvjs"), 191, 16779, 3792, 4337, 323, 22162, 3143, string("i"));
	this->vmjikpdukiqftrmh(true);
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ewxbdpj {
public:
	double bawfugaiw;
	string vuoksznk;
	bool mvsojvqirvio;
	ewxbdpj();
	bool wztqsqujnzbasynnxqwyaz(int znatc, double ekqcivediyzrf, bool ywuozqnqouwhfys, bool qkjtmpqcyplil, int supyxgfrkqj, double cikpuomyi, string skbazcqatjpdss);
	int xnpsvzujimouwpo(string bfvalhvp, double nhkdwau, double oljakdcku, string seliavpnfdj, int bspysm);
	bool buqxiwqvskqhanfin(bool tgiehbeigcj);
	string emdjdvvhhuettovqqtfodu(int cblushursod);
	double silbuhtgqodzjqx(string xguskephsvfuh, bool kutfpukghzcie, string fddpiewer, double vziyqwnuc, bool cbjuoxsob, bool fihcqjxoid, int ygbkrqcovsc, int pgalh, string gxfshgz, string kbmstkgdykjruu);
	void ayleerwvanurpyuvo(double nazweu, double nsfbnk, bool nmfswysv, int ywjueazu, int ttqjyhvh, int dyelvk, string jklezdiol, string mrfpmuaglo, double ghjkxnis, double jqexi);
	double hjtvtnvtcpchgfeuzsbpenst(string woakizishys);
	bool efjeqqjzmpnqxmadki(string dhqdalie, bool elouc, bool xjygcoenpw, int jcjgo);
	void iscpdspdeixcqtbhvxmroqt(int vrfpcbfghrfsoo, string wgrihjgc, string uykmjqloxfmhqa, string qeiuputmih, bool hlruj);

protected:
	double mrbftwq;
	double zvrrwnskiuuly;
	bool whqkjrolsnkno;

	void gnolpoxwlojth(double ceipnqklpxbu, double mqzhwgv, string onwzdsqherrtxbk, string yzmvpdnkzgzp, string uispexuneaoozo);
	void xywhlwbcfsvbtazfzxubvh(double wxslxwcqht);
	string kkzcbguqyqakbiuc(bool qswymfopb, double bweolyinezyorxy, double togwfqvgpelndu, int dnihdntuqiei, bool qoqazttpggh, int adboiq, bool mwbshzdzllaei);
	int sckfjaqzojxg(string wnvhwwzhr, bool clvhnaaclupurb, double sguqhqowzwh, bool cfggjnvgl, string phmmkpxj, int nfehxphidspjd, double zsodeep, bool lnmes, string mdlpheeurkm);

private:
	string geowlaz;
	string ufxaif;
	int ywjqvysjxxzq;
	bool kyxvtvg;

	void xgxoymwpgrkhvjwpsafoj(bool hzyrujqjeb, bool rerjpq);
	bool oifplgxevyxnmlp();
	double rvqqxdqxgncvpkatugjrb(string gbswdz, bool gvrgurfbftx, string wwahoo, string vmwdevntd, string jwumyz, double mobcjdbzrwpgl, string wsxbwhlbcncuvme, int edfzcwa, string qmfnfjjg);

};


void ewxbdpj::xgxoymwpgrkhvjwpsafoj(bool hzyrujqjeb, bool rerjpq) {
	string qrgchxszup = "kvvoviriflrrdpjyaqviapimplxbfcifpactkaglzcscghfftmxhxxfrvtpljxhsjkbjtmmapjemryqoyysqozrwyvel";
	double msfnlsg = 36523;
	bool gorducxrlt = false;
	bool gghoylirgzmqc = false;
	int fpeuuih = 8494;
	double wclyiykdescovq = 24473;
	bool ntezitlhz = true;
	bool qssaqczdzovfgm = true;
	string vqklejfwzyngkh = "kfbcglngoszhgjpfacdabjbyttfioyvlhdoiistiquldwxtydmladovtlvi";
	bool bcazrfkpvs = false;
	if (true == true) {
		int gwbwwaic;
		for (gwbwwaic = 20; gwbwwaic > 0; gwbwwaic--) {
			continue;
		}
	}
	if (36523 != 36523) {
		int qyfehuuyvg;
		for (qyfehuuyvg = 9; qyfehuuyvg > 0; qyfehuuyvg--) {
			continue;
		}
	}
	if (true != true) {
		int cjeq;
		for (cjeq = 60; cjeq > 0; cjeq--) {
			continue;
		}
	}
	if (true == true) {
		int gadoksdgjz;
		for (gadoksdgjz = 20; gadoksdgjz > 0; gadoksdgjz--) {
			continue;
		}
	}

}

bool ewxbdpj::oifplgxevyxnmlp() {
	bool ldjxuoqp = false;
	string mmcyxzcgbkrfag = "itlyujligknrxvgtbjfojqawqmabseeglugbnbsmdiswduuzghdijm";
	string zsismrbl = "gqbcxdwktjvoyejqrukwspdidrkbcqongfhginlbvsy";
	double ycwpvelypv = 1431;
	bool ysytrjlxds = true;
	int sdghnsko = 482;
	double rqbhg = 88625;
	int rrrvxas = 4148;
	if (4148 == 4148) {
		int gdkoj;
		for (gdkoj = 28; gdkoj > 0; gdkoj--) {
			continue;
		}
	}
	if (4148 != 4148) {
		int aebiyszqq;
		for (aebiyszqq = 49; aebiyszqq > 0; aebiyszqq--) {
			continue;
		}
	}
	if (false != false) {
		int yoyj;
		for (yoyj = 41; yoyj > 0; yoyj--) {
			continue;
		}
	}
	return false;
}

double ewxbdpj::rvqqxdqxgncvpkatugjrb(string gbswdz, bool gvrgurfbftx, string wwahoo, string vmwdevntd, string jwumyz, double mobcjdbzrwpgl, string wsxbwhlbcncuvme, int edfzcwa, string qmfnfjjg) {
	bool gieom = false;
	int qzcmtwtla = 489;
	if (false == false) {
		int swmxbbm;
		for (swmxbbm = 2; swmxbbm > 0; swmxbbm--) {
			continue;
		}
	}
	if (false != false) {
		int jshqxu;
		for (jshqxu = 88; jshqxu > 0; jshqxu--) {
			continue;
		}
	}
	if (false == false) {
		int wrgo;
		for (wrgo = 20; wrgo > 0; wrgo--) {
			continue;
		}
	}
	if (489 == 489) {
		int hc;
		for (hc = 41; hc > 0; hc--) {
			continue;
		}
	}
	if (false != false) {
		int ijecnddpt;
		for (ijecnddpt = 81; ijecnddpt > 0; ijecnddpt--) {
			continue;
		}
	}
	return 54370;
}

void ewxbdpj::gnolpoxwlojth(double ceipnqklpxbu, double mqzhwgv, string onwzdsqherrtxbk, string yzmvpdnkzgzp, string uispexuneaoozo) {
	int vqoeqhj = 1717;
	string ptwhrxsooey = "ozkjwiwondixuvipockxnvdwggrvesqrifqnlshvndahcxnrwfgsyenhiqhgwboksaavllkhcttnzh";
	int gzxiwac = 5971;
	bool ehyxbuwih = true;
	string mmzffsomr = "ejrfedukylcfendnqyhpudrurwpkuamxosbrx";
	int nczlruiadyfsxx = 0;
	if (true == true) {
		int mly;
		for (mly = 35; mly > 0; mly--) {
			continue;
		}
	}

}

void ewxbdpj::xywhlwbcfsvbtazfzxubvh(double wxslxwcqht) {
	string cmbtkofb = "iufkpcnazjsggaqsumlaitoegzklbtgcmhrvkqhuymdaiwtolfpqaqzgsuguglkpfv";
	bool yicmrvngqeu = true;
	double loscsszwfrlcbqt = 31747;
	int yhgkkgpdotbt = 6351;
	bool bikeevmqb = false;
	string zqqdqrahilppnmu = "cebsxnsjunudtuckzptriofqawbtbrkhkldzdhbfmlso";
	string wdtqssgtszt = "sylxprxrb";
	string nvqeyvkk = "cuggxgmuxulnjojfqgluecsusxewkjhwjlxmnsvlduqaaoibmdfqzclewtgbbyvovjfmm";
	if (string("cebsxnsjunudtuckzptriofqawbtbrkhkldzdhbfmlso") != string("cebsxnsjunudtuckzptriofqawbtbrkhkldzdhbfmlso")) {
		int ivuyf;
		for (ivuyf = 6; ivuyf > 0; ivuyf--) {
			continue;
		}
	}
	if (string("cebsxnsjunudtuckzptriofqawbtbrkhkldzdhbfmlso") == string("cebsxnsjunudtuckzptriofqawbtbrkhkldzdhbfmlso")) {
		int fhrw;
		for (fhrw = 22; fhrw > 0; fhrw--) {
			continue;
		}
	}

}

string ewxbdpj::kkzcbguqyqakbiuc(bool qswymfopb, double bweolyinezyorxy, double togwfqvgpelndu, int dnihdntuqiei, bool qoqazttpggh, int adboiq, bool mwbshzdzllaei) {
	bool yizictcd = true;
	double gzbmxkaxq = 73117;
	double jpaiuir = 33184;
	int cwfyxrjgmxzujjq = 1531;
	double kshmhiyzhibht = 6890;
	int lzwdivocvlk = 3211;
	string degua = "gtxahfciwjbkgredugltnqsdaadsfmfmggazacrktbubfrwfxza";
	string wxzrcnrmhrlsl = "sittuglyfugztcoqtxbnuimdcswavqnjbebyzxzkutkmjsimmmcusyy";
	return string("qauzcrxbcct");
}

int ewxbdpj::sckfjaqzojxg(string wnvhwwzhr, bool clvhnaaclupurb, double sguqhqowzwh, bool cfggjnvgl, string phmmkpxj, int nfehxphidspjd, double zsodeep, bool lnmes, string mdlpheeurkm) {
	string oqaihzn = "eznboiyezzbyheigbczfztq";
	if (string("eznboiyezzbyheigbczfztq") != string("eznboiyezzbyheigbczfztq")) {
		int vrmscabqto;
		for (vrmscabqto = 69; vrmscabqto > 0; vrmscabqto--) {
			continue;
		}
	}
	if (string("eznboiyezzbyheigbczfztq") != string("eznboiyezzbyheigbczfztq")) {
		int tdj;
		for (tdj = 21; tdj > 0; tdj--) {
			continue;
		}
	}
	return 1940;
}

bool ewxbdpj::wztqsqujnzbasynnxqwyaz(int znatc, double ekqcivediyzrf, bool ywuozqnqouwhfys, bool qkjtmpqcyplil, int supyxgfrkqj, double cikpuomyi, string skbazcqatjpdss) {
	string nlqcmjx = "aegizyurkjbbbqiwtynarau";
	string qwtckoechorbxh = "qbgkpgnvwuivaoopnaqndnegurhaepbmduonenfitsxnxrfaoly";
	int eljlbqokjj = 2399;
	double eogndiu = 24815;
	bool ywgpvcj = false;
	if (string("aegizyurkjbbbqiwtynarau") != string("aegizyurkjbbbqiwtynarau")) {
		int cvowyarz;
		for (cvowyarz = 88; cvowyarz > 0; cvowyarz--) {
			continue;
		}
	}
	return true;
}

int ewxbdpj::xnpsvzujimouwpo(string bfvalhvp, double nhkdwau, double oljakdcku, string seliavpnfdj, int bspysm) {
	string qxdwiubjrzytmg = "vgroeaixxvhkupribedbzgindoghiwlzevfysdiokwvigdmplonwfadyhzacedrqt";
	double sojnvuolxdttnkz = 5761;
	int wlyfkbv = 878;
	return 6493;
}

bool ewxbdpj::buqxiwqvskqhanfin(bool tgiehbeigcj) {
	int vdcgweghykggsbd = 298;
	double ttgiibuo = 14039;
	int qwlsfinppww = 337;
	int wdqsadnbliphna = 4705;
	int cggloghslubumiv = 950;
	if (950 != 950) {
		int vbpevwsy;
		for (vbpevwsy = 42; vbpevwsy > 0; vbpevwsy--) {
			continue;
		}
	}
	return false;
}

string ewxbdpj::emdjdvvhhuettovqqtfodu(int cblushursod) {
	int ezewdae = 3840;
	string hehvbjucckrxtx = "fudqyydcoeuvnpcjpsrsujwnlclrrtwbucfjwsikzxhazbzybohhivqfdmwckpdwx";
	if (3840 == 3840) {
		int ojht;
		for (ojht = 84; ojht > 0; ojht--) {
			continue;
		}
	}
	if (3840 == 3840) {
		int immnngw;
		for (immnngw = 60; immnngw > 0; immnngw--) {
			continue;
		}
	}
	if (string("fudqyydcoeuvnpcjpsrsujwnlclrrtwbucfjwsikzxhazbzybohhivqfdmwckpdwx") == string("fudqyydcoeuvnpcjpsrsujwnlclrrtwbucfjwsikzxhazbzybohhivqfdmwckpdwx")) {
		int pewu;
		for (pewu = 100; pewu > 0; pewu--) {
			continue;
		}
	}
	if (3840 != 3840) {
		int scusmq;
		for (scusmq = 47; scusmq > 0; scusmq--) {
			continue;
		}
	}
	return string("yqbsrfjgedyopkvo");
}

double ewxbdpj::silbuhtgqodzjqx(string xguskephsvfuh, bool kutfpukghzcie, string fddpiewer, double vziyqwnuc, bool cbjuoxsob, bool fihcqjxoid, int ygbkrqcovsc, int pgalh, string gxfshgz, string kbmstkgdykjruu) {
	return 18229;
}

void ewxbdpj::ayleerwvanurpyuvo(double nazweu, double nsfbnk, bool nmfswysv, int ywjueazu, int ttqjyhvh, int dyelvk, string jklezdiol, string mrfpmuaglo, double ghjkxnis, double jqexi) {
	string ervrc = "qbewjoadterhuimrbpptkkausxoyjokammxbtfhesalyorqhkmahmtrappbnkwh";
	bool eiwxpabuh = false;
	bool luquiioidncx = false;
	int vdrflvjxzlmxqvn = 4583;
	int itsoc = 663;
	string mcjlxllq = "ijuxikbruiusxbclauvdcrjraztayicwoidckbsstvgtfl";
	double lptvelsisrbqkc = 16868;
	string kncinylg = "cpgaivcxnlamraeuuxerhckysngelsqsdnepqibcwdrzntssfg";
	bool hmhidooquhzwh = true;
	int axksflodqeilntz = 55;
	if (55 != 55) {
		int kciumpzdmv;
		for (kciumpzdmv = 95; kciumpzdmv > 0; kciumpzdmv--) {
			continue;
		}
	}

}

double ewxbdpj::hjtvtnvtcpchgfeuzsbpenst(string woakizishys) {
	return 24707;
}

bool ewxbdpj::efjeqqjzmpnqxmadki(string dhqdalie, bool elouc, bool xjygcoenpw, int jcjgo) {
	string ndrvtdv = "xvcdanzovgahcmgnffikjohseaixngpdsxhzhfqpmgd";
	bool pmnjnugptn = true;
	if (true == true) {
		int ttdd;
		for (ttdd = 95; ttdd > 0; ttdd--) {
			continue;
		}
	}
	return true;
}

void ewxbdpj::iscpdspdeixcqtbhvxmroqt(int vrfpcbfghrfsoo, string wgrihjgc, string uykmjqloxfmhqa, string qeiuputmih, bool hlruj) {
	bool mraitudywquysr = true;
	double iuqvfotqbpz = 57293;

}

ewxbdpj::ewxbdpj() {
	this->wztqsqujnzbasynnxqwyaz(6212, 43142, false, true, 3022, 7930, string("vl"));
	this->xnpsvzujimouwpo(string("tiugspuacokgmcuwkjcasawtujfjmlbfitatgwthmdfohzfdyqzxutplbhnqwovlrcpwqayzqcfq"), 8098, 43064, string("ghojpfpbwnfryszgzeckfqexkjprpipfoilrdklrejfeczxvvhgqusehsxbvuogrishdnmjtmjzxfvjcqdkbqgpywkjj"), 2564);
	this->buqxiwqvskqhanfin(true);
	this->emdjdvvhhuettovqqtfodu(7479);
	this->silbuhtgqodzjqx(string("uzapkggzmmbmwltdodvwbuldemwdlqwqslcoubyudnxoulplgjdqvxja"), false, string("gkmoxkzzjsmvzkmwbwmonuye"), 43500, true, false, 4842, 5500, string("llhsrumofjhmfadkcsmvdxkeoqptwswoldxtbvpbsw"), string("roueydiuvhzknxsacensextgvwneraqerubovebycullaznnldnevihr"));
	this->ayleerwvanurpyuvo(2361, 2861, true, 1746, 2504, 143, string("zfbpcnmzqtzhjjhmclgdmwkfzykitnfgpsjqjtlvrcdiyqdmowvaeouenowilitirepgwatgzapnajwxmfmrntzp"), string("lzbgwkdfvpyjodaudmufiemmgnqneuxyjwtewiwfshzwd"), 1292, 7709);
	this->hjtvtnvtcpchgfeuzsbpenst(string("mcslvchaobj"));
	this->efjeqqjzmpnqxmadki(string("ave"), false, false, 5419);
	this->iscpdspdeixcqtbhvxmroqt(1358, string("dacrtfpzamtknsoopiqfuampjjgrnnxeyglchevvrxqklfkuusjg"), string("tpaddqzbktrlrtiltxyzdsmyzmsurenznncrm"), string("srvqaqaqomijtljqqgldefisbazw"), true);
	this->gnolpoxwlojth(8687, 70740, string("cweybfnpvpfbtisovymiymwiuiqvitpqdhjusayvwrovnkxdgkctclumjyaruejrhauslwdgiqmqm"), string("vxewmsnvlyiwusqqtolokiiiiksgstrbodcdmwriryklkjnidlrulgqrtvionmdapqiqthzlopbxzmmreidrrqijexzi"), string("dxrudihmsyvhkpezvwgcfrvpygnnxxpekqczonjppgw"));
	this->xywhlwbcfsvbtazfzxubvh(40789);
	this->kkzcbguqyqakbiuc(true, 8501, 580, 3827, true, 3559, true);
	this->sckfjaqzojxg(string("hzzpssmkvcxnpxdunuoprifcpxezkpxrdknrnifwmrl"), false, 6211, true, string("tjxomrmeplzdjhvguckrcciitiodhbphgdoeuvtpmzmfmiyyoxyvfdjjaxvrdwyjd"), 4021, 8001, true, string("ywmubakjflmhhzonxhzjoxncpehxfegpotoeuesqerhjoekxupqmpxzgztdpneow"));
	this->xgxoymwpgrkhvjwpsafoj(true, true);
	this->oifplgxevyxnmlp();
	this->rvqqxdqxgncvpkatugjrb(string("msickavhksibmngsoyccpqlmupmwruhgfsvwdmxhraqqaciuuqbagqkianmkpeycmiuncbuuoy"), false, string("pvojjljytyldfrpggoadcsdccnnmjbctmidyogwrkrsyfistgymwgjmkz"), string("gmbpwmbparheawfjednphwfzbpylplwmknopevwp"), string("ajrobanskusumkeratwylzpinpadlmzvfrjcpzceaavqicqtwsfwpvdsazgyaetzxqayofdpwelhxemeeld"), 47233, string("ijqckgtwlhhellxekulupwbzxihyttknhkyamylaipcnfzqgbixanpmwepdipekxugtaamoblzumdmwikjjqcsyhgd"), 756, string("wsyq"));
}
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class mnlurqi {
public:
	int uolteex;
	bool ffhpj;
	mnlurqi();
	double bmqsnwwvptklewszgjwvpb(string gwnaqwwtp, double ircrj, bool mlxha, double zaitdlayl, int udygyvovovr, bool gtypry, double pxotnvhodc, int uinse, bool jebmra);
	string yylrfstprwmkp(double wjhjrgubbqa, double ffejimzayonzvi, int oqdeab, string djvdgwwhoogq, int yetbuwybcfldq);

protected:
	string rtrixweqygu;
	bool dxzfxo;

	double yhntllbozmrjmot(double fyjfap);
	int xcbgvwtyfuxaxhwrwmx(double mglfkjwr, bool lnkguwu, int lhkmtgmqg, int brtwaasbvytvgr, int veplopayiefgay, string ahsqksy);

private:
	bool ndvegfw;

	string ypnpydyvsebulhraqqmgq(string yaiwjbtodkol, double mggiweobpdojqxj, string eknwcmy, string cvkjtdynzps, string bvrgckspppkl, double cxynmlioh);
	string dzpatkjyvomzummq(double lelnw, int xiqcrqchug, double xvgfwt, int grtwfxuf, string kgaiclatlifphf, double ifpiqaex, double trzqcbxortq, int rjwcda, string pgmtwf);
	bool nvamfogyqiwhwxhsnmgdjdrp(string temlzwy, double zpktjhkpq, bool hapopnnkwqf);
	int mpwthaqnegrczn(string jjxqxfkr, bool fnyjhylyitjl, bool gwbjnh, double xcsftzsle);

};


string mnlurqi::ypnpydyvsebulhraqqmgq(string yaiwjbtodkol, double mggiweobpdojqxj, string eknwcmy, string cvkjtdynzps, string bvrgckspppkl, double cxynmlioh) {
	string akoebwkrbnllcyk = "glyhrfsnkzagufzbnrulxzlfksffbjbcxqjhcibqibwibtiuccmzamretfllpewbvbowqciayuawojmdokxyufst";
	string etktmmgurte = "vfteoxvnbgqqbqymkobbfsunnvjnaljlbnzszgvakccxcajss";
	bool nvpuinj = true;
	double impilc = 24050;
	double znmscop = 16647;
	int hojotmnxrckajnn = 43;
	if (string("glyhrfsnkzagufzbnrulxzlfksffbjbcxqjhcibqibwibtiuccmzamretfllpewbvbowqciayuawojmdokxyufst") == string("glyhrfsnkzagufzbnrulxzlfksffbjbcxqjhcibqibwibtiuccmzamretfllpewbvbowqciayuawojmdokxyufst")) {
		int muvt;
		for (muvt = 69; muvt > 0; muvt--) {
			continue;
		}
	}
	if (string("vfteoxvnbgqqbqymkobbfsunnvjnaljlbnzszgvakccxcajss") != string("vfteoxvnbgqqbqymkobbfsunnvjnaljlbnzszgvakccxcajss")) {
		int hp;
		for (hp = 31; hp > 0; hp--) {
			continue;
		}
	}
	if (16647 == 16647) {
		int pshl;
		for (pshl = 66; pshl > 0; pshl--) {
			continue;
		}
	}
	return string("tigezynqulddsaicbssb");
}

string mnlurqi::dzpatkjyvomzummq(double lelnw, int xiqcrqchug, double xvgfwt, int grtwfxuf, string kgaiclatlifphf, double ifpiqaex, double trzqcbxortq, int rjwcda, string pgmtwf) {
	string vsqawrdl = "hlpjhtgplcxhshufdxzgxvfbvgyafiovumjczmhmoxncotl";
	string mulwcye = "lzamrmervdpoupzpvhffjajwgmmvqajtoauwuodbpewxszcvyjogxpbxbwjoqmejoyczbngiauydngibxdrstewqgbpzgfg";
	double jzdtzxpj = 5119;
	if (5119 != 5119) {
		int dkpk;
		for (dkpk = 23; dkpk > 0; dkpk--) {
			continue;
		}
	}
	if (5119 == 5119) {
		int oygknst;
		for (oygknst = 43; oygknst > 0; oygknst--) {
			continue;
		}
	}
	if (string("lzamrmervdpoupzpvhffjajwgmmvqajtoauwuodbpewxszcvyjogxpbxbwjoqmejoyczbngiauydngibxdrstewqgbpzgfg") != string("lzamrmervdpoupzpvhffjajwgmmvqajtoauwuodbpewxszcvyjogxpbxbwjoqmejoyczbngiauydngibxdrstewqgbpzgfg")) {
		int igvzbf;
		for (igvzbf = 70; igvzbf > 0; igvzbf--) {
			continue;
		}
	}
	if (5119 == 5119) {
		int inb;
		for (inb = 25; inb > 0; inb--) {
			continue;
		}
	}
	return string("xomfxyygdecgflozg");
}

bool mnlurqi::nvamfogyqiwhwxhsnmgdjdrp(string temlzwy, double zpktjhkpq, bool hapopnnkwqf) {
	int irzvjhnedfoejn = 2247;
	bool airyxlosrknaqes = false;
	bool ymtyncvgntszupv = true;
	int ovxydlnb = 398;
	bool jslpx = true;
	int nuguvdtb = 3268;
	double fjmfvnbsb = 32890;
	if (true == true) {
		int kq;
		for (kq = 44; kq > 0; kq--) {
			continue;
		}
	}
	if (32890 != 32890) {
		int ecwpmwndjz;
		for (ecwpmwndjz = 28; ecwpmwndjz > 0; ecwpmwndjz--) {
			continue;
		}
	}
	if (true == true) {
		int hqstzn;
		for (hqstzn = 75; hqstzn > 0; hqstzn--) {
			continue;
		}
	}
	if (32890 != 32890) {
		int cehxnhc;
		for (cehxnhc = 18; cehxnhc > 0; cehxnhc--) {
			continue;
		}
	}
	if (true == true) {
		int uur;
		for (uur = 52; uur > 0; uur--) {
			continue;
		}
	}
	return true;
}

int mnlurqi::mpwthaqnegrczn(string jjxqxfkr, bool fnyjhylyitjl, bool gwbjnh, double xcsftzsle) {
	string rgyoszzhysrk = "xejklxwjesshacintkjnidkyryfodgaxswwtnmxxkftjecfscniwpnzxewuouzaubympendghsfxmoonxs";
	bool sszzt = false;
	int ojknhvnpvpolcpf = 672;
	string blxabvntdnreyt = "nhbcvoehuoxhbaeoilqktbynymykbyxiwnuwncgwzgjzxprysqhnjsvgwzmxmfhlfcicnaapjzyvmvok";
	bool osdpsijxxh = true;
	bool gtkqihdrggy = false;
	double pocqvuftxhsj = 4730;
	string nobsmxkwhgajrcf = "cgfxrisgfnumbuoxohlgjvrzpkol";
	if (string("nhbcvoehuoxhbaeoilqktbynymykbyxiwnuwncgwzgjzxprysqhnjsvgwzmxmfhlfcicnaapjzyvmvok") != string("nhbcvoehuoxhbaeoilqktbynymykbyxiwnuwncgwzgjzxprysqhnjsvgwzmxmfhlfcicnaapjzyvmvok")) {
		int urimob;
		for (urimob = 96; urimob > 0; urimob--) {
			continue;
		}
	}
	if (string("xejklxwjesshacintkjnidkyryfodgaxswwtnmxxkftjecfscniwpnzxewuouzaubympendghsfxmoonxs") == string("xejklxwjesshacintkjnidkyryfodgaxswwtnmxxkftjecfscniwpnzxewuouzaubympendghsfxmoonxs")) {
		int alxvgwjyln;
		for (alxvgwjyln = 24; alxvgwjyln > 0; alxvgwjyln--) {
			continue;
		}
	}
	if (string("nhbcvoehuoxhbaeoilqktbynymykbyxiwnuwncgwzgjzxprysqhnjsvgwzmxmfhlfcicnaapjzyvmvok") != string("nhbcvoehuoxhbaeoilqktbynymykbyxiwnuwncgwzgjzxprysqhnjsvgwzmxmfhlfcicnaapjzyvmvok")) {
		int ywqekcyxr;
		for (ywqekcyxr = 7; ywqekcyxr > 0; ywqekcyxr--) {
			continue;
		}
	}
	return 93609;
}

double mnlurqi::yhntllbozmrjmot(double fyjfap) {
	bool pvomwvbu = true;
	int bhkyh = 1358;
	double mmwqavlcui = 29178;
	double zyxbuwztl = 9257;
	string hpcwmbwrn = "gdmvyxflkfstooyvlyydxgqnkmrywhgpivydtcrhfyantryfhtfczaosryemebsgz";
	string jijopvbx = "xntplowouqhouejygmemoornzawtvlcotqm";
	if (9257 != 9257) {
		int pw;
		for (pw = 85; pw > 0; pw--) {
			continue;
		}
	}
	if (true == true) {
		int bkhj;
		for (bkhj = 36; bkhj > 0; bkhj--) {
			continue;
		}
	}
	if (9257 != 9257) {
		int km;
		for (km = 54; km > 0; km--) {
			continue;
		}
	}
	if (9257 != 9257) {
		int lphc;
		for (lphc = 68; lphc > 0; lphc--) {
			continue;
		}
	}
	return 73144;
}

int mnlurqi::xcbgvwtyfuxaxhwrwmx(double mglfkjwr, bool lnkguwu, int lhkmtgmqg, int brtwaasbvytvgr, int veplopayiefgay, string ahsqksy) {
	string uanverec = "jjuivqrcjrokigpipuyizzhymefgyu";
	int jynbm = 2640;
	bool kacfngpn = false;
	int bjfhmvtwrj = 1557;
	if (2640 != 2640) {
		int nogeqmpsi;
		for (nogeqmpsi = 14; nogeqmpsi > 0; nogeqmpsi--) {
			continue;
		}
	}
	if (2640 != 2640) {
		int irarhr;
		for (irarhr = 14; irarhr > 0; irarhr--) {
			continue;
		}
	}
	if (1557 == 1557) {
		int xcehosuo;
		for (xcehosuo = 53; xcehosuo > 0; xcehosuo--) {
			continue;
		}
	}
	if (false != false) {
		int jgvfua;
		for (jgvfua = 12; jgvfua > 0; jgvfua--) {
			continue;
		}
	}
	if (string("jjuivqrcjrokigpipuyizzhymefgyu") == string("jjuivqrcjrokigpipuyizzhymefgyu")) {
		int pseqnn;
		for (pseqnn = 59; pseqnn > 0; pseqnn--) {
			continue;
		}
	}
	return 76457;
}

double mnlurqi::bmqsnwwvptklewszgjwvpb(string gwnaqwwtp, double ircrj, bool mlxha, double zaitdlayl, int udygyvovovr, bool gtypry, double pxotnvhodc, int uinse, bool jebmra) {
	bool bkunprglb = true;
	string yfzhjhy = "eayctvaxzvdznfqfftmmqscxqjsxskmdjbfpoqqlefjimperelcynltv";
	string cgbjuyhgrrac = "jelyqbiwpnyelvhabhdhmmrrntvfvoqudqwytofpahakahuwygoknwxwm";
	if (string("jelyqbiwpnyelvhabhdhmmrrntvfvoqudqwytofpahakahuwygoknwxwm") == string("jelyqbiwpnyelvhabhdhmmrrntvfvoqudqwytofpahakahuwygoknwxwm")) {
		int zzdxf;
		for (zzdxf = 87; zzdxf > 0; zzdxf--) {
			continue;
		}
	}
	if (string("eayctvaxzvdznfqfftmmqscxqjsxskmdjbfpoqqlefjimperelcynltv") != string("eayctvaxzvdznfqfftmmqscxqjsxskmdjbfpoqqlefjimperelcynltv")) {
		int qlqlau;
		for (qlqlau = 18; qlqlau > 0; qlqlau--) {
			continue;
		}
	}
	if (true != true) {
		int jhz;
		for (jhz = 93; jhz > 0; jhz--) {
			continue;
		}
	}
	if (string("eayctvaxzvdznfqfftmmqscxqjsxskmdjbfpoqqlefjimperelcynltv") == string("eayctvaxzvdznfqfftmmqscxqjsxskmdjbfpoqqlefjimperelcynltv")) {
		int np;
		for (np = 57; np > 0; np--) {
			continue;
		}
	}
	return 96014;
}

string mnlurqi::yylrfstprwmkp(double wjhjrgubbqa, double ffejimzayonzvi, int oqdeab, string djvdgwwhoogq, int yetbuwybcfldq) {
	return string("qlhfwell");
}

mnlurqi::mnlurqi() {
	this->bmqsnwwvptklewszgjwvpb(string("qpcgxnstaauohxhqpvqetxttakysoufaufzrjmodograzaqibnvcxnbzpnabquxwxyxxisklycashtayfhsvbulmgcncvhe"), 10107, false, 64301, 6105, true, 3508, 4528, true);
	this->yylrfstprwmkp(35233, 60204, 689, string("yrujpwfyoijaoppuaxzvbkoidavtexqcvdffureneopujlpjrgoesbdzgshouelauaaxzzojvvswffqntomucnlfqkdmsamqen"), 3779);
	this->yhntllbozmrjmot(36895);
	this->xcbgvwtyfuxaxhwrwmx(35665, true, 780, 1557, 2413, string("xanpimllvoxtvymthzmjngocskftllufnpqemobdwhxtaacidytjsoymvddtwjncjufrgpiebtsfaylqo"));
	this->ypnpydyvsebulhraqqmgq(string("rstpfuzkbrtfpdozbkgrqi"), 41705, string("tnigpaihheiwophzkfygotwgqusrnqcmxbhgorpgunqfvlqyiadnjwnfifagkwtptjheazmdhpcilbyquskjwcozsndsllszp"), string("aemqsvxwbysiaghvxsrnxyjrorukpkgdjvhgfaraevorjmckhbkerqbojznqngituwgjogujzbqup"), string("dbxzvkg"), 349);
	this->dzpatkjyvomzummq(27045, 3779, 46289, 3172, string("wqufoywqlxvcrtfmeftdtpfgkmhoqohslgucmzihhubsjcyikawvdwuysmkcnkntszumxhchukr"), 66003, 38590, 2196, string("yvvcguosuqpxtygwnikndqaynykxvzjiwdnsjaztzopgvbbhpeoljvtlkilnjhcibuvfvssgrqfgrfecwh"));
	this->nvamfogyqiwhwxhsnmgdjdrp(string("uqmuuobemrsxgfgqjejemjuxgefwcsfmpyzhlvzrwpaawpboxppnaqwklwjsovhtpgoirspframkpivaidfsuzwmrr"), 16765, true);
	this->mpwthaqnegrczn(string("znfvrdlxsxynxunerfjmclksxkqixbwfcrrrjvi"), false, false, 40693);
}
