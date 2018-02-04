//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// $Header: $
// $NoKeywords: $
//
// Serialization buffer
//===========================================================================//

#pragma warning (disable : 4514)

#include "UtlBuffer.h"
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>
#include "characterset.h"

const char* V_strnchr(const char* pStr, char c, int n)
{
	char const* pLetter = pStr;
	char const* pLast = pStr + n;

	// Check the entire string
	while ((pLetter < pLast) && (*pLetter != 0))
	{
		if (*pLetter == c)
			return pLetter;
		++pLetter;
	}
	return NULL;
}
//-----------------------------------------------------------------------------
// Finds a string in another string with a case insensitive test w/ length validation
//-----------------------------------------------------------------------------
char const* V_strnistr(char const* pStr, char const* pSearch, int n)
{
	if (!pStr || !pSearch)
		return 0;

	char const* pLetter = pStr;

	// Check the entire string
	while (*pLetter != 0)
	{
		if (n <= 0)
			return 0;

		// Skip over non-matches
		if (tolower(*pLetter) == tolower(*pSearch))
		{
			int n1 = n - 1;

			// Check for match
			char const* pMatch = pLetter + 1;
			char const* pTest = pSearch + 1;
			while (*pTest != 0)
			{
				if (n1 <= 0)
					return 0;

				// We've run off the end; don't bother.
				if (*pMatch == 0)
					return 0;

				if (tolower(*pMatch) != tolower(*pTest))
					break;

				++pMatch;
				++pTest;
				--n1;
			}

			// Found a match!
			if (*pTest == 0)
				return pLetter;
		}

		++pLetter;
		--n;
	}

	return 0;
}
//-----------------------------------------------------------------------------
// Character conversions for C strings
//-----------------------------------------------------------------------------
class CUtlCStringConversion : public CUtlCharConversion
{
public:
	CUtlCStringConversion(char nEscapeChar, const char *pDelimiter, int nCount, ConversionArray_t *pArray);

	// Finds a conversion for the passed-in string, returns length
	virtual char FindConversion(const char *pString, int *pLength);

private:
	char m_pConversion[255];
};


//-----------------------------------------------------------------------------
// Character conversions for no-escape sequence strings
//-----------------------------------------------------------------------------
class CUtlNoEscConversion : public CUtlCharConversion
{
public:
	CUtlNoEscConversion(char nEscapeChar, const char *pDelimiter, int nCount, ConversionArray_t *pArray) :
		CUtlCharConversion(nEscapeChar, pDelimiter, nCount, pArray)
	{}

	// Finds a conversion for the passed-in string, returns length
	virtual char FindConversion(const char *pString, int *pLength) { *pLength = 0; return 0; }
};


//-----------------------------------------------------------------------------
// List of character conversions
//-----------------------------------------------------------------------------
BEGIN_CUSTOM_CHAR_CONVERSION(CUtlCStringConversion, s_StringCharConversion, "\"", '\\')
{
	'\n', "n"
},
{ '\t', "t" },
{ '\v', "v" },
{ '\b', "b" },
{ '\r', "r" },
{ '\f', "f" },
{ '\a', "a" },
{ '\\', "\\" },
{ '\?', "\?" },
{ '\'', "\'" },
{ '\"', "\"" },
END_CUSTOM_CHAR_CONVERSION(CUtlCStringConversion, s_StringCharConversion, "\"", '\\');

	CUtlCharConversion *GetCStringCharConversion()
	{
		return &s_StringCharConversion;
	}

	BEGIN_CUSTOM_CHAR_CONVERSION(CUtlNoEscConversion, s_NoEscConversion, "\"", 0x7F)
	{
		0x7F, ""
	},
		END_CUSTOM_CHAR_CONVERSION(CUtlNoEscConversion, s_NoEscConversion, "\"", 0x7F);

		CUtlCharConversion *GetNoEscCharConversion()
		{
			return &s_NoEscConversion;
		}


		//-----------------------------------------------------------------------------
		// Constructor
		//-----------------------------------------------------------------------------
		CUtlCStringConversion::CUtlCStringConversion(char nEscapeChar, const char *pDelimiter, int nCount, ConversionArray_t *pArray) :
			CUtlCharConversion(nEscapeChar, pDelimiter, nCount, pArray)
		{
			memset(m_pConversion, 0x0, sizeof(m_pConversion));
			for (int i = 0; i < nCount; ++i)
			{
				m_pConversion[pArray[i].m_pReplacementString[0]] = pArray[i].m_nActualChar;
			}
		}

		// Finds a conversion for the passed-in string, returns length
		char CUtlCStringConversion::FindConversion(const char *pString, int *pLength)
		{
			char c = m_pConversion[pString[0]];
			*pLength = (c != '\0') ? 1 : 0;
			return c;
		}



		//-----------------------------------------------------------------------------
		// Constructor
		//-----------------------------------------------------------------------------
		CUtlCharConversion::CUtlCharConversion(char nEscapeChar, const char *pDelimiter, int nCount, ConversionArray_t *pArray)
		{
			m_nEscapeChar = nEscapeChar;
			m_pDelimiter = pDelimiter;
			m_nCount = nCount;
			m_nDelimiterLength = strlen(pDelimiter);
			m_nMaxConversionLength = 0;

			memset(m_pReplacements, 0, sizeof(m_pReplacements));

			for (int i = 0; i < nCount; ++i)
			{
				m_pList[i] = pArray[i].m_nActualChar;
				ConversionInfo_t &info = m_pReplacements[m_pList[i]];
				assert(info.m_pReplacementString == 0);
				info.m_pReplacementString = pArray[i].m_pReplacementString;
				info.m_nLength = strlen(info.m_pReplacementString);
				if (info.m_nLength > m_nMaxConversionLength)
				{
					m_nMaxConversionLength = info.m_nLength;
				}
			}
		}


		//-----------------------------------------------------------------------------
		// Escape character + delimiter
		//-----------------------------------------------------------------------------
		char CUtlCharConversion::GetEscapeChar() const
		{
			return m_nEscapeChar;
		}

		const char *CUtlCharConversion::GetDelimiter() const
		{
			return m_pDelimiter;
		}

		int CUtlCharConversion::GetDelimiterLength() const
		{
			return m_nDelimiterLength;
		}


		//-----------------------------------------------------------------------------
		// Constructor
		//-----------------------------------------------------------------------------
		const char *CUtlCharConversion::GetConversionString(char c) const
		{
			return m_pReplacements[c].m_pReplacementString;
		}

		int CUtlCharConversion::GetConversionLength(char c) const
		{
			return m_pReplacements[c].m_nLength;
		}

		int CUtlCharConversion::MaxConversionLength() const
		{
			return m_nMaxConversionLength;
		}


		//-----------------------------------------------------------------------------
		// Finds a conversion for the passed-in string, returns length
		//-----------------------------------------------------------------------------
		char CUtlCharConversion::FindConversion(const char *pString, int *pLength)
		{
			for (int i = 0; i < m_nCount; ++i)
			{
				if (!strcmp(pString, m_pReplacements[m_pList[i]].m_pReplacementString))
				{
					*pLength = m_pReplacements[m_pList[i]].m_nLength;
					return m_pList[i];
				}
			}

			*pLength = 0;
			return '\0';
		}


		//-----------------------------------------------------------------------------
		// constructors
		//-----------------------------------------------------------------------------
		CUtlBuffer::CUtlBuffer(int growSize, int initSize, int nFlags) :
			m_Memory(growSize, initSize), m_Error(0)
		{
			m_Get = 0;
			m_Put = 0;
			m_nTab = 0;
			m_nOffset = 0;
			m_Flags = (unsigned char)nFlags;
			if ((initSize != 0) && !IsReadOnly())
			{
				m_nMaxPut = -1;
				AddNullTermination();
			} else
			{
				m_nMaxPut = 0;
			}
			SetOverflowFuncs(&CUtlBuffer::GetOverflow, &CUtlBuffer::PutOverflow);
		}

		CUtlBuffer::CUtlBuffer(const void *pBuffer, int nSize, int nFlags) :
			m_Memory((unsigned char*)pBuffer, nSize), m_Error(0)
		{
			assert(nSize != 0);

			m_Get = 0;
			m_Put = 0;
			m_nTab = 0;
			m_nOffset = 0;
			m_Flags = (unsigned char)nFlags;
			if (IsReadOnly())
			{
				m_nMaxPut = nSize;
			} else
			{
				m_nMaxPut = -1;
				AddNullTermination();
			}
			SetOverflowFuncs(&CUtlBuffer::GetOverflow, &CUtlBuffer::PutOverflow);
		}


		//-----------------------------------------------------------------------------
		// Modifies the buffer to be binary or text; Blows away the buffer and the CONTAINS_CRLF value. 
		//-----------------------------------------------------------------------------
		void CUtlBuffer::SetBufferType(bool bIsText, bool bContainsCRLF)
		{
#ifdef _DEBUG
			// If the buffer is empty, there is no opportunity for this stuff to fail
			if (TellMaxPut() != 0)
			{
				if (IsText())
				{
					if (bIsText)
					{
						assert(ContainsCRLF() == bContainsCRLF);
					} else
					{
						assert(ContainsCRLF());
					}
				} else
				{
					if (bIsText)
					{
						assert(bContainsCRLF);
					}
				}
			}
#endif

			if (bIsText)
			{
				m_Flags |= TEXT_BUFFER;
			} else
			{
				m_Flags &= ~TEXT_BUFFER;
			}
			if (bContainsCRLF)
			{
				m_Flags |= CONTAINS_CRLF;
			} else
			{
				m_Flags &= ~CONTAINS_CRLF;
			}
		}


		//-----------------------------------------------------------------------------
		// Attaches the buffer to external memory....
		//-----------------------------------------------------------------------------
		void CUtlBuffer::SetExternalBuffer(void* pMemory, int nSize, int nInitialPut, int nFlags)
		{
			m_Memory.SetExternalBuffer((unsigned char*)pMemory, nSize);

			// Reset all indices; we just changed memory
			m_Get = 0;
			m_Put = nInitialPut;
			m_nTab = 0;
			m_Error = 0;
			m_nOffset = 0;
			m_Flags = (unsigned char)nFlags;
			m_nMaxPut = -1;
			AddNullTermination();
		}

		//-----------------------------------------------------------------------------
		// Assumes an external buffer but manages its deletion
		//-----------------------------------------------------------------------------
		void CUtlBuffer::AssumeMemory(void *pMemory, int nSize, int nInitialPut, int nFlags)
		{
			m_Memory.AssumeMemory((unsigned char*)pMemory, nSize);

			// Reset all indices; we just changed memory
			m_Get = 0;
			m_Put = nInitialPut;
			m_nTab = 0;
			m_Error = 0;
			m_nOffset = 0;
			m_Flags = (unsigned char)nFlags;
			m_nMaxPut = -1;
			AddNullTermination();
		}

		//-----------------------------------------------------------------------------
		// Makes sure we've got at least this much memory
		//-----------------------------------------------------------------------------
		void CUtlBuffer::EnsureCapacity(int num)
		{
			// Add one extra for the null termination
			num += 1;
			if (m_Memory.IsExternallyAllocated())
			{
				if (IsGrowable() && (m_Memory.NumAllocated() < num))
				{
					m_Memory.ConvertToGrowableMemory(0);
				} else
				{
					num -= 1;
				}
			}

			m_Memory.EnsureCapacity(num);
		}


		//-----------------------------------------------------------------------------
		// Base Get method from which all others derive
		//-----------------------------------------------------------------------------
		void CUtlBuffer::Get(void* pMem, int size)
		{
			if (CheckGet(size))
			{
				memcpy(pMem, &m_Memory[m_Get - m_nOffset], size);
				m_Get += size;
			}
		}


		//-----------------------------------------------------------------------------
		// This will Get at least 1 uint8_t and up to nSize bytes. 
		// It will return the number of bytes actually read.
		//-----------------------------------------------------------------------------
		int CUtlBuffer::GetUpTo(void *pMem, int nSize)
		{
			if (CheckArbitraryPeekGet(0, nSize))
			{
				memcpy(pMem, &m_Memory[m_Get - m_nOffset], nSize);
				m_Get += nSize;
				return nSize;
			}
			return 0;
		}


		//-----------------------------------------------------------------------------
		// Eats whitespace
		//-----------------------------------------------------------------------------
		void CUtlBuffer::EatWhiteSpace()
		{
			if (IsText() && IsValid())
			{
				while (CheckGet(sizeof(char)))
				{
					if (!isspace(*(const unsigned char*)PeekGet()))
						break;
					m_Get += sizeof(char);
				}
			}
		}


		//-----------------------------------------------------------------------------
		// Eats C++ style comments
		//-----------------------------------------------------------------------------
		bool CUtlBuffer::EatCPPComment()
		{
			if (IsText() && IsValid())
			{
				// If we don't have a a c++ style comment next, we're done
				const char *pPeek = (const char *)PeekGet(2 * sizeof(char), 0);
				if (!pPeek || (pPeek[0] != '/') || (pPeek[1] != '/'))
					return false;

				// Deal with c++ style comments
				m_Get += 2;

				// read complete line
				for (char c = GetChar(); IsValid(); c = GetChar())
				{
					if (c == '\n')
						break;
				}
				return true;
			}
			return false;
		}


		//-----------------------------------------------------------------------------
		// Peeks how much whitespace to eat
		//-----------------------------------------------------------------------------
		int CUtlBuffer::PeekWhiteSpace(int nOffset)
		{
			if (!IsText() || !IsValid())
				return 0;

			while (CheckPeekGet(nOffset, sizeof(char)))
			{
				if (!isspace(*(unsigned char*)PeekGet(nOffset)))
					break;
				nOffset += sizeof(char);
			}

			return nOffset;
		}


		//-----------------------------------------------------------------------------
		// Peek size of sting to come, check memory bound
		//-----------------------------------------------------------------------------
		int	CUtlBuffer::PeekStringLength()
		{
			if (!IsValid())
				return 0;

			// Eat preceeding whitespace
			int nOffset = 0;
			if (IsText())
			{
				nOffset = PeekWhiteSpace(nOffset);
			}

			int nStartingOffset = nOffset;

			do
			{
				int nPeekAmount = 128;

				// NOTE: Add 1 for the terminating zero!
				if (!CheckArbitraryPeekGet(nOffset, nPeekAmount))
				{
					if (nOffset == nStartingOffset)
						return 0;
					return nOffset - nStartingOffset + 1;
				}

				const char *pTest = (const char *)PeekGet(nOffset);

				if (!IsText())
				{
					for (int i = 0; i < nPeekAmount; ++i)
					{
						// The +1 here is so we eat the terminating 0
						if (pTest[i] == 0)
							return (i + nOffset - nStartingOffset + 1);
					}
				} else
				{
					for (int i = 0; i < nPeekAmount; ++i)
					{
						// The +1 here is so we eat the terminating 0
						if (isspace((unsigned char)pTest[i]) || (pTest[i] == 0))
							return (i + nOffset - nStartingOffset + 1);
					}
				}

				nOffset += nPeekAmount;

			} while (true);
		}


		//-----------------------------------------------------------------------------
		// Peek size of line to come, check memory bound
		//-----------------------------------------------------------------------------
		int	CUtlBuffer::PeekLineLength()
		{
			if (!IsValid())
				return 0;

			int nOffset = 0;
			int nStartingOffset = nOffset;

			do
			{
				int nPeekAmount = 128;

				// NOTE: Add 1 for the terminating zero!
				if (!CheckArbitraryPeekGet(nOffset, nPeekAmount))
				{
					if (nOffset == nStartingOffset)
						return 0;
					return nOffset - nStartingOffset + 1;
				}

				const char *pTest = (const char *)PeekGet(nOffset);

				for (int i = 0; i < nPeekAmount; ++i)
				{
					// The +2 here is so we eat the terminating '\n' and 0
					if (pTest[i] == '\n' || pTest[i] == '\r')
						return (i + nOffset - nStartingOffset + 2);
					// The +1 here is so we eat the terminating 0
					if (pTest[i] == 0)
						return (i + nOffset - nStartingOffset + 1);
				}

				nOffset += nPeekAmount;

			} while (true);
		}


		//-----------------------------------------------------------------------------
		// Does the next bytes of the buffer match a pattern?
		//-----------------------------------------------------------------------------
		bool CUtlBuffer::PeekStringMatch(int nOffset, const char *pString, int nLen)
		{
			if (!CheckPeekGet(nOffset, nLen))
				return false;
			return !strncmp((const char*)PeekGet(nOffset), pString, nLen);
		}


		//-----------------------------------------------------------------------------
		// This version of PeekStringLength converts \" to \\ and " to \, etc.
		// It also reads a " at the beginning and end of the string
		//-----------------------------------------------------------------------------
		int CUtlBuffer::PeekDelimitedStringLength(CUtlCharConversion *pConv, bool bActualSize)
		{
			if (!IsText() || !pConv)
				return PeekStringLength();

			// Eat preceeding whitespace
			int nOffset = 0;
			if (IsText())
			{
				nOffset = PeekWhiteSpace(nOffset);
			}

			if (!PeekStringMatch(nOffset, pConv->GetDelimiter(), pConv->GetDelimiterLength()))
				return 0;

			// Try to read ending ", but don't accept \"
			int nActualStart = nOffset;
			nOffset += pConv->GetDelimiterLength();
			int nLen = 1;	// Starts at 1 for the '\0' termination

			do
			{
				if (PeekStringMatch(nOffset, pConv->GetDelimiter(), pConv->GetDelimiterLength()))
					break;

				if (!CheckPeekGet(nOffset, 1))
					break;

				char c = *(const char*)PeekGet(nOffset);
				++nLen;
				++nOffset;
				if (c == pConv->GetEscapeChar())
				{
					int nLength = pConv->MaxConversionLength();
					if (!CheckArbitraryPeekGet(nOffset, nLength))
						break;

					pConv->FindConversion((const char*)PeekGet(nOffset), &nLength);
					nOffset += nLength;
				}
			} while (true);

			return bActualSize ? nLen : nOffset - nActualStart + pConv->GetDelimiterLength() + 1;
		}


		//-----------------------------------------------------------------------------
		// Reads a null-terminated string
		//-----------------------------------------------------------------------------
		void CUtlBuffer::GetString(char* pString, int nMaxChars)
		{
			if (!IsValid())
			{
				*pString = 0;
				return;
			}

			if (nMaxChars == 0)
			{
				nMaxChars = INT_MAX;
			}

			// Remember, this *includes* the null character
			// It will be 0, however, if the buffer is empty.
			int nLen = PeekStringLength();

			if (IsText())
			{
				EatWhiteSpace();
			}

			if (nLen == 0)
			{
				*pString = 0;
				m_Error |= GET_OVERFLOW;
				return;
			}

			// Strip off the terminating NULL
			if (nLen <= nMaxChars)
			{
				Get(pString, nLen - 1);
				pString[nLen - 1] = 0;
			} else
			{
				Get(pString, nMaxChars - 1);
				pString[nMaxChars - 1] = 0;
				SeekGet(SEEK_CURRENT, nLen - 1 - nMaxChars);
			}

			// Read the terminating NULL in binary formats
			if (!IsText())
			{
				assert(GetChar() == 0);
			}
		}


		//-----------------------------------------------------------------------------
		// Reads up to and including the first \n
		//-----------------------------------------------------------------------------
		void CUtlBuffer::GetLine(char* pLine, int nMaxChars)
		{
			assert(IsText() && !ContainsCRLF());

			if (!IsValid())
			{
				*pLine = 0;
				return;
			}

			if (nMaxChars == 0)
			{
				nMaxChars = INT_MAX;
			}

			// Remember, this *includes* the null character
			// It will be 0, however, if the buffer is empty.
			int nLen = PeekLineLength();
			if (nLen == 0)
			{
				*pLine = 0;
				m_Error |= GET_OVERFLOW;
				return;
			}

			// Strip off the terminating NULL
			if (nLen <= nMaxChars)
			{
				Get(pLine, nLen - 1);
				pLine[nLen - 1] = 0;
			} else
			{
				Get(pLine, nMaxChars - 1);
				pLine[nMaxChars - 1] = 0;
				SeekGet(SEEK_CURRENT, nLen - 1 - nMaxChars);
			}
		}


		//-----------------------------------------------------------------------------
		// This version of GetString converts \ to \\ and " to \", etc.
		// It also places " at the beginning and end of the string
		//-----------------------------------------------------------------------------
		char CUtlBuffer::GetDelimitedCharInternal(CUtlCharConversion *pConv)
		{
			char c = GetChar();
			if (c == pConv->GetEscapeChar())
			{
				int nLength = pConv->MaxConversionLength();
				if (!CheckArbitraryPeekGet(0, nLength))
					return '\0';

				c = pConv->FindConversion((const char *)PeekGet(), &nLength);
				SeekGet(SEEK_CURRENT, nLength);
			}

			return c;
		}

		char CUtlBuffer::GetDelimitedChar(CUtlCharConversion *pConv)
		{
			if (!IsText() || !pConv)
				return GetChar();
			return GetDelimitedCharInternal(pConv);
		}

		void CUtlBuffer::GetDelimitedString(CUtlCharConversion *pConv, char *pString, int nMaxChars)
		{
			if (!IsText() || !pConv)
			{
				GetString(pString, nMaxChars);
				return;
			}

			if (!IsValid())
			{
				*pString = 0;
				return;
			}

			if (nMaxChars == 0)
			{
				nMaxChars = INT_MAX;
			}

			EatWhiteSpace();
			if (!PeekStringMatch(0, pConv->GetDelimiter(), pConv->GetDelimiterLength()))
				return;

			// Pull off the starting delimiter
			SeekGet(SEEK_CURRENT, pConv->GetDelimiterLength());

			int nRead = 0;
			while (IsValid())
			{
				if (PeekStringMatch(0, pConv->GetDelimiter(), pConv->GetDelimiterLength()))
				{
					SeekGet(SEEK_CURRENT, pConv->GetDelimiterLength());
					break;
				}

				char c = GetDelimitedCharInternal(pConv);

				if (nRead < nMaxChars)
				{
					pString[nRead] = c;
					++nRead;
				}
			}

			if (nRead >= nMaxChars)
			{
				nRead = nMaxChars - 1;
			}
			pString[nRead] = '\0';
		}


		//-----------------------------------------------------------------------------
		// Checks if a Get is ok
		//-----------------------------------------------------------------------------
		bool CUtlBuffer::CheckGet(int nSize)
		{
			if (m_Error & GET_OVERFLOW)
				return false;

			if (TellMaxPut() < m_Get + nSize)
			{
				m_Error |= GET_OVERFLOW;
				return false;
			}

			if ((m_Get < m_nOffset) || (m_Memory.NumAllocated() < m_Get - m_nOffset + nSize))
			{
				if (!OnGetOverflow(nSize))
				{
					m_Error |= GET_OVERFLOW;
					return false;
				}
			}

			return true;
		}


		//-----------------------------------------------------------------------------
		// Checks if a peek Get is ok
		//-----------------------------------------------------------------------------
		bool CUtlBuffer::CheckPeekGet(int nOffset, int nSize)
		{
			if (m_Error & GET_OVERFLOW)
				return false;

			// Checking for peek can't Set the overflow flag
			bool bOk = CheckGet(nOffset + nSize);
			m_Error &= ~GET_OVERFLOW;
			return bOk;
		}


		//-----------------------------------------------------------------------------
		// Call this to peek arbitrarily long into memory. It doesn't fail unless
		// it can't read *anything* new
		//-----------------------------------------------------------------------------
		bool CUtlBuffer::CheckArbitraryPeekGet(int nOffset, int &nIncrement)
		{
			if (TellGet() + nOffset >= TellMaxPut())
			{
				nIncrement = 0;
				return false;
			}

			if (TellGet() + nOffset + nIncrement > TellMaxPut())
			{
				nIncrement = TellMaxPut() - TellGet() - nOffset;
			}

			// NOTE: CheckPeekGet could modify TellMaxPut for streaming files
			// We have to call TellMaxPut again here
			CheckPeekGet(nOffset, nIncrement);
			int nMaxGet = TellMaxPut() - TellGet();
			if (nMaxGet < nIncrement)
			{
				nIncrement = nMaxGet;
			}
			return (nIncrement != 0);
		}


		//-----------------------------------------------------------------------------
		// Peek part of the butt
		//-----------------------------------------------------------------------------
		const void* CUtlBuffer::PeekGet(int nMaxSize, int nOffset)
		{
			if (!CheckPeekGet(nOffset, nMaxSize))
				return NULL;
			return &m_Memory[m_Get + nOffset - m_nOffset];
		}


		//-----------------------------------------------------------------------------
		// Change where I'm reading
		//-----------------------------------------------------------------------------
		void CUtlBuffer::SeekGet(SeekType_t type, int offset)
		{
			switch (type)
			{
			case SEEK_HEAD:
				m_Get = offset;
				break;

			case SEEK_CURRENT:
				m_Get += offset;
				break;

			case SEEK_TAIL:
				m_Get = m_nMaxPut - offset;
				break;
			}

			if (m_Get > m_nMaxPut)
			{
				m_Error |= GET_OVERFLOW;
			} else
			{
				m_Error &= ~GET_OVERFLOW;
				if (m_Get < m_nOffset || m_Get >= m_nOffset + Size())
				{
					OnGetOverflow(-1);
				}
			}
		}


		//-----------------------------------------------------------------------------
		// Parse...
		//-----------------------------------------------------------------------------

#pragma warning ( disable : 4706 )

		int CUtlBuffer::VaScanf(const char* pFmt, va_list list)
		{
			assert(pFmt);
			if (m_Error || !IsText())
				return 0;

			int numScanned = 0;
			int nLength;
			char c;
			char* pEnd;
			while (c = *pFmt++)
			{
				// Stop if we hit the end of the buffer
				if (m_Get >= TellMaxPut())
				{
					m_Error |= GET_OVERFLOW;
					break;
				}

				switch (c)
				{
				case ' ':
					// eat all whitespace
					EatWhiteSpace();
					break;

				case '%':
				{
					// Conversion character... try to convert baby!
					char type = *pFmt++;
					if (type == 0)
						return numScanned;

					switch (type)
					{
					case 'c':
					{
						char* ch = va_arg(list, char *);
						if (CheckPeekGet(0, sizeof(char)))
						{
							*ch = *(const char*)PeekGet();
							++m_Get;
						} else
						{
							*ch = 0;
							return numScanned;
						}
					}
					break;

					case 'i':
					case 'd':
					{
						int* i = va_arg(list, int *);

						// NOTE: This is not bullet-proof; it assumes numbers are < 128 characters
						nLength = 128;
						if (!CheckArbitraryPeekGet(0, nLength))
						{
							*i = 0;
							return numScanned;
						}

						*i = strtol((char*)PeekGet(), &pEnd, 10);
						int nBytesRead = (int)(pEnd - (char*)PeekGet());
						if (nBytesRead == 0)
							return numScanned;
						m_Get += nBytesRead;
					}
					break;

					case 'x':
					{
						int* i = va_arg(list, int *);

						// NOTE: This is not bullet-proof; it assumes numbers are < 128 characters
						nLength = 128;
						if (!CheckArbitraryPeekGet(0, nLength))
						{
							*i = 0;
							return numScanned;
						}

						*i = strtol((char*)PeekGet(), &pEnd, 16);
						int nBytesRead = (int)(pEnd - (char*)PeekGet());
						if (nBytesRead == 0)
							return numScanned;
						m_Get += nBytesRead;
					}
					break;

					case 'u':
					{
						unsigned int* u = va_arg(list, unsigned int *);

						// NOTE: This is not bullet-proof; it assumes numbers are < 128 characters
						nLength = 128;
						if (!CheckArbitraryPeekGet(0, nLength))
						{
							*u = 0;
							return numScanned;
						}

						*u = strtoul((char*)PeekGet(), &pEnd, 10);
						int nBytesRead = (int)(pEnd - (char*)PeekGet());
						if (nBytesRead == 0)
							return numScanned;
						m_Get += nBytesRead;
					}
					break;

					case 'f':
					{
						float* f = va_arg(list, float *);

						// NOTE: This is not bullet-proof; it assumes numbers are < 128 characters
						nLength = 128;
						if (!CheckArbitraryPeekGet(0, nLength))
						{
							*f = 0.0f;
							return numScanned;
						}

						*f = (float)strtod((char*)PeekGet(), &pEnd);
						int nBytesRead = (int)(pEnd - (char*)PeekGet());
						if (nBytesRead == 0)
							return numScanned;
						m_Get += nBytesRead;
					}
					break;

					case 's':
					{
						char* s = va_arg(list, char *);
						GetString(s);
					}
					break;

					default:
					{
						// unimplemented scanf type
						assert(0);
						return numScanned;
					}
					break;
					}

					++numScanned;
				}
				break;

				default:
				{
					// Here we have to match the format string character
					// against what's in the buffer or we're done.
					if (!CheckPeekGet(0, sizeof(char)))
						return numScanned;

					if (c != *(const char*)PeekGet())
						return numScanned;

					++m_Get;
				}
				}
			}
			return numScanned;
		}

#pragma warning ( default : 4706 )

		int CUtlBuffer::Scanf(const char* pFmt, ...)
		{
			va_list args;

			va_start(args, pFmt);
			int count = VaScanf(pFmt, args);
			va_end(args);

			return count;
		}


		//-----------------------------------------------------------------------------
		// Advance the Get index until after the particular string is found
		// Do not eat whitespace before starting. Return false if it failed
		//-----------------------------------------------------------------------------
		bool CUtlBuffer::GetToken(const char *pToken)
		{
			assert(pToken);

			// Look for the token
			int nLen = strlen(pToken);

			int nSizeToCheck = Size() - TellGet() - m_nOffset;

			int nGet = TellGet();
			do
			{
				int nMaxSize = TellMaxPut() - TellGet();
				if (nMaxSize < nSizeToCheck)
				{
					nSizeToCheck = nMaxSize;
				}
				if (nLen > nSizeToCheck)
					break;

				if (!CheckPeekGet(0, nSizeToCheck))
					break;

				const char *pBufStart = (const char*)PeekGet();
				const char *pFoundEnd = V_strnistr(pBufStart, pToken, nSizeToCheck);
				if (pFoundEnd)
				{
					size_t nOffset = (size_t)pFoundEnd - (size_t)pBufStart;
					SeekGet(CUtlBuffer::SEEK_CURRENT, nOffset + nLen);
					return true;
				}

				SeekGet(CUtlBuffer::SEEK_CURRENT, nSizeToCheck - nLen - 1);
				nSizeToCheck = Size() - (nLen - 1);

			} while (true);

			SeekGet(CUtlBuffer::SEEK_HEAD, nGet);
			return false;
		}


		//-----------------------------------------------------------------------------
		// (For text buffers only)
		// Parse a token from the buffer:
		// Grab all text that lies between a starting delimiter + ending delimiter
		// (skipping whitespace that leads + trails both delimiters).
		// Note the delimiter checks are case-insensitive.
		// If successful, the Get index is advanced and the function returns true,
		// otherwise the index is not advanced and the function returns false.
		//-----------------------------------------------------------------------------
		bool CUtlBuffer::ParseToken(const char *pStartingDelim, const char *pEndingDelim, char* pString, int nMaxLen)
		{
			int nCharsToCopy = 0;
			int nCurrentGet = 0;

			size_t nEndingDelimLen;

			// Starting delimiter is optional
			char emptyBuf = '\0';
			if (!pStartingDelim)
			{
				pStartingDelim = &emptyBuf;
			}

			// Ending delimiter is not
			assert(pEndingDelim && pEndingDelim[0]);
			nEndingDelimLen = strlen(pEndingDelim);

			int nStartGet = TellGet();
			char nCurrChar;
			int nTokenStart = -1;
			EatWhiteSpace();
			while (*pStartingDelim)
			{
				nCurrChar = *pStartingDelim++;
				if (!isspace((unsigned char)nCurrChar))
				{
					if (tolower(GetChar()) != tolower(nCurrChar))
						goto parseFailed;
				} else
				{
					EatWhiteSpace();
				}
			}

			EatWhiteSpace();
			nTokenStart = TellGet();
			if (!GetToken(pEndingDelim))
				goto parseFailed;

			nCurrentGet = TellGet();
			nCharsToCopy = (nCurrentGet - nEndingDelimLen) - nTokenStart;
			if (nCharsToCopy >= nMaxLen)
			{
				nCharsToCopy = nMaxLen - 1;
			}

			if (nCharsToCopy > 0)
			{
				SeekGet(CUtlBuffer::SEEK_HEAD, nTokenStart);
				Get(pString, nCharsToCopy);
				if (!IsValid())
					goto parseFailed;

				// Eat trailing whitespace
				for (; nCharsToCopy > 0; --nCharsToCopy)
				{
					if (!isspace((unsigned char)pString[nCharsToCopy - 1]))
						break;
				}
			}
			pString[nCharsToCopy] = '\0';

			// Advance the Get index
			SeekGet(CUtlBuffer::SEEK_HEAD, nCurrentGet);
			return true;

		parseFailed:
			// Revert the Get index
			SeekGet(SEEK_HEAD, nStartGet);
			pString[0] = '\0';
			return false;
		}


		//-----------------------------------------------------------------------------
		// Parses the next token, given a Set of character breaks to stop at
		//-----------------------------------------------------------------------------
		int CUtlBuffer::ParseToken(characterset_t *pBreaks, char *pTokenBuf, int nMaxLen, bool bParseComments)
		{
			assert(nMaxLen > 0);
			pTokenBuf[0] = 0;

			// skip whitespace + comments
			while (true)
			{
				if (!IsValid())
					return -1;
				EatWhiteSpace();
				if (bParseComments)
				{
					if (!EatCPPComment())
						break;
				} else
				{
					break;
				}
			}

			char c = GetChar();

			// End of buffer
			if (c == 0)
				return -1;

			// handle quoted strings specially
			if (c == '\"')
			{
				int nLen = 0;
				while (IsValid())
				{
					c = GetChar();
					if (c == '\"' || !c)
					{
						pTokenBuf[nLen] = 0;
						return nLen;
					}
					pTokenBuf[nLen] = c;
					if (++nLen == nMaxLen)
					{
						pTokenBuf[nLen - 1] = 0;
						return nMaxLen;
					}
				}

				// In this case, we hit the end of the buffer before hitting the end qoute
				pTokenBuf[nLen] = 0;
				return nLen;
			}

			// parse single characters
			if (IN_CHARACTERSET(*pBreaks, c))
			{
				pTokenBuf[0] = c;
				pTokenBuf[1] = 0;
				return 1;
			}

			// parse a regular word
			int nLen = 0;
			while (true)
			{
				pTokenBuf[nLen] = c;
				if (++nLen == nMaxLen)
				{
					pTokenBuf[nLen - 1] = 0;
					return nMaxLen;
				}
				c = GetChar();
				if (!IsValid())
					break;

				if (IN_CHARACTERSET(*pBreaks, c) || c == '\"' || c <= ' ')
				{
					SeekGet(SEEK_CURRENT, -1);
					break;
				}
			}

			pTokenBuf[nLen] = 0;
			return nLen;
		}



		//-----------------------------------------------------------------------------
		// Serialization
		//-----------------------------------------------------------------------------
		void CUtlBuffer::Put(const void *pMem, int size)
		{
			if (size && CheckPut(size))
			{
				memcpy(&m_Memory[m_Put - m_nOffset], pMem, size);
				m_Put += size;

				AddNullTermination();
			}
		}


		//-----------------------------------------------------------------------------
		// Writes a null-terminated string
		//-----------------------------------------------------------------------------
		void CUtlBuffer::PutString(const char* pString)
		{
			if (!IsText())
			{
				if (pString)
				{
					// Not text? append a null at the end.
					size_t nLen = strlen(pString) + 1;
					Put(pString, nLen * sizeof(char));
					return;
				} else
				{
					PutTypeBin<char>(0);
				}
			} else if (pString)
			{
				int nTabCount = (m_Flags & AUTO_TABS_DISABLED) ? 0 : m_nTab;
				if (nTabCount > 0)
				{
					if (WasLastCharacterCR())
					{
						PutTabs();
					}

					const char* pEndl = strchr(pString, '\n');
					while (pEndl)
					{
						size_t nSize = (size_t)pEndl - (size_t)pString + sizeof(char);
						Put(pString, nSize);
						pString = pEndl + 1;
						if (*pString)
						{
							PutTabs();
							pEndl = strchr(pString, '\n');
						} else
						{
							pEndl = NULL;
						}
					}
				}
				size_t nLen = strlen(pString);
				if (nLen)
				{
					Put(pString, nLen * sizeof(char));
				}
			}
		}


		//-----------------------------------------------------------------------------
		// This version of PutString converts \ to \\ and " to \", etc.
		// It also places " at the beginning and end of the string
		//-----------------------------------------------------------------------------
		inline void CUtlBuffer::PutDelimitedCharInternal(CUtlCharConversion *pConv, char c)
		{
			int l = pConv->GetConversionLength(c);
			if (l == 0)
			{
				PutChar(c);
			} else
			{
				PutChar(pConv->GetEscapeChar());
				Put(pConv->GetConversionString(c), l);
			}
		}

		void CUtlBuffer::PutDelimitedChar(CUtlCharConversion *pConv, char c)
		{
			if (!IsText() || !pConv)
			{
				PutChar(c);
				return;
			}

			PutDelimitedCharInternal(pConv, c);
		}

		void CUtlBuffer::PutDelimitedString(CUtlCharConversion *pConv, const char *pString)
		{
			if (!IsText() || !pConv)
			{
				PutString(pString);
				return;
			}

			if (WasLastCharacterCR())
			{
				PutTabs();
			}
			Put(pConv->GetDelimiter(), pConv->GetDelimiterLength());

			int nLen = pString ? strlen(pString) : 0;
			for (int i = 0; i < nLen; ++i)
			{
				PutDelimitedCharInternal(pConv, pString[i]);
			}

			if (WasLastCharacterCR())
			{
				PutTabs();
			}
			Put(pConv->GetDelimiter(), pConv->GetDelimiterLength());
		}


		void CUtlBuffer::VaPrintf(const char* pFmt, va_list list)
		{
			char temp[2048];
			int nLen = vsnprintf(temp, sizeof(temp), pFmt, list);
			assert(nLen < 2048);
			PutString(temp);
		}

		void CUtlBuffer::Printf(const char* pFmt, ...)
		{
			va_list args;

			va_start(args, pFmt);
			VaPrintf(pFmt, args);
			va_end(args);
		}


		//-----------------------------------------------------------------------------
		// Calls the overflow functions
		//-----------------------------------------------------------------------------
		void CUtlBuffer::SetOverflowFuncs(UtlBufferOverflowFunc_t getFunc, UtlBufferOverflowFunc_t putFunc)
		{
			m_GetOverflowFunc = getFunc;
			m_PutOverflowFunc = putFunc;
		}


		//-----------------------------------------------------------------------------
		// Calls the overflow functions
		//-----------------------------------------------------------------------------
		bool CUtlBuffer::OnPutOverflow(int nSize)
		{
			return (this->*m_PutOverflowFunc)(nSize);
		}

		bool CUtlBuffer::OnGetOverflow(int nSize)
		{
			return (this->*m_GetOverflowFunc)(nSize);
		}


		//-----------------------------------------------------------------------------
		// Checks if a put is ok
		//-----------------------------------------------------------------------------
		bool CUtlBuffer::PutOverflow(int nSize)
		{
			if (m_Memory.IsExternallyAllocated())
			{
				if (!IsGrowable())
					return false;

				m_Memory.ConvertToGrowableMemory(0);
			}

			while (Size() < m_Put - m_nOffset + nSize)
			{
				m_Memory.Grow();
			}

			return true;
		}

		bool CUtlBuffer::GetOverflow(int nSize)
		{
			return false;
		}


		//-----------------------------------------------------------------------------
		// Checks if a put is ok
		//-----------------------------------------------------------------------------
		bool CUtlBuffer::CheckPut(int nSize)
		{
			if ((m_Error & PUT_OVERFLOW) || IsReadOnly())
				return false;

			if ((m_Put < m_nOffset) || (m_Memory.NumAllocated() < m_Put - m_nOffset + nSize))
			{
				if (!OnPutOverflow(nSize))
				{
					m_Error |= PUT_OVERFLOW;
					return false;
				}
			}
			return true;
		}

		void CUtlBuffer::SeekPut(SeekType_t type, int offset)
		{
			int nNextPut = m_Put;
			switch (type)
			{
			case SEEK_HEAD:
				nNextPut = offset;
				break;

			case SEEK_CURRENT:
				nNextPut += offset;
				break;

			case SEEK_TAIL:
				nNextPut = m_nMaxPut - offset;
				break;
			}

			// Force a write of the data
			// FIXME: We could make this more optimal potentially by writing out
			// the entire buffer if you seek outside the current range

			// NOTE: This call will write and will also seek the file to nNextPut.
			OnPutOverflow(-nNextPut - 1);
			m_Put = nNextPut;

			AddNullTermination();
		}


		void CUtlBuffer::ActivateByteSwapping(bool bActivate)
		{
			m_Byteswap.ActivateByteSwapping(bActivate);
		}

		void CUtlBuffer::SetBigEndian(bool bigEndian)
		{
			m_Byteswap.SetTargetBigEndian(bigEndian);
		}

		bool CUtlBuffer::IsBigEndian(void)
		{
			return m_Byteswap.IsTargetBigEndian();
		}


		//-----------------------------------------------------------------------------
		// null terminate the buffer
		//-----------------------------------------------------------------------------
		void CUtlBuffer::AddNullTermination(void)
		{
			if (m_Put > m_nMaxPut)
			{
				if (!IsReadOnly() && ((m_Error & PUT_OVERFLOW) == 0))
				{
					// Add null termination value
					if (CheckPut(1))
					{
						m_Memory[m_Put - m_nOffset] = 0;
					} else
					{
						// Restore the overflow state, it was valid before...
						m_Error &= ~PUT_OVERFLOW;
					}
				}
				m_nMaxPut = m_Put;
			}
		}


		//-----------------------------------------------------------------------------
		// Converts a buffer from a CRLF buffer to a CR buffer (and back)
		// Returns false if no conversion was necessary (and outBuf is left untouched)
		// If the conversion occurs, outBuf will be cleared.
		//-----------------------------------------------------------------------------
		bool CUtlBuffer::ConvertCRLF(CUtlBuffer &outBuf)
		{
			if (!IsText() || !outBuf.IsText())
				return false;

			if (ContainsCRLF() == outBuf.ContainsCRLF())
				return false;

			int nInCount = TellMaxPut();

			outBuf.Purge();
			outBuf.EnsureCapacity(nInCount);

			bool bFromCRLF = ContainsCRLF();

			// Start reading from the beginning
			int nGet = TellGet();
			int nPut = TellPut();
			int nGetDelta = 0;
			int nPutDelta = 0;

			const char *pBase = (const char*)Base();
			int nCurrGet = 0;
			while (nCurrGet < nInCount)
			{
				const char *pCurr = &pBase[nCurrGet];
				if (bFromCRLF)
				{
					const char *pNext = V_strnistr(pCurr, "\r\n", nInCount - nCurrGet);
					if (!pNext)
					{
						outBuf.Put(pCurr, nInCount - nCurrGet);
						break;
					}

					int nBytes = (size_t)pNext - (size_t)pCurr;
					outBuf.Put(pCurr, nBytes);
					outBuf.PutChar('\n');
					nCurrGet += nBytes + 2;
					if (nGet >= nCurrGet - 1)
					{
						--nGetDelta;
					}
					if (nPut >= nCurrGet - 1)
					{
						--nPutDelta;
					}
				} else
				{
					const char *pNext = V_strnchr(pCurr, '\n', nInCount - nCurrGet);
					if (!pNext)
					{
						outBuf.Put(pCurr, nInCount - nCurrGet);
						break;
					}

					int nBytes = (size_t)pNext - (size_t)pCurr;
					outBuf.Put(pCurr, nBytes);
					outBuf.PutChar('\r');
					outBuf.PutChar('\n');
					nCurrGet += nBytes + 1;
					if (nGet >= nCurrGet)
					{
						++nGetDelta;
					}
					if (nPut >= nCurrGet)
					{
						++nPutDelta;
					}
				}
			}

			assert(nPut + nPutDelta <= outBuf.TellMaxPut());

			outBuf.SeekGet(SEEK_HEAD, nGet + nGetDelta);
			outBuf.SeekPut(SEEK_HEAD, nPut + nPutDelta);

			return true;
		}


		//---------------------------------------------------------------------------
		// Implementation of CUtlInplaceBuffer
		//---------------------------------------------------------------------------

		CUtlInplaceBuffer::CUtlInplaceBuffer(int growSize /* = 0 */, int initSize /* = 0 */, int nFlags /* = 0 */) :
			CUtlBuffer(growSize, initSize, nFlags)
		{
			NULL;
		}

		bool CUtlInplaceBuffer::InplaceGetLinePtr(char **ppszInBufferPtr, int *pnLineLength)
		{
			assert(IsText() && !ContainsCRLF());

			int nLineLen = PeekLineLength();
			if (nLineLen <= 1)
			{
				SeekGet(SEEK_TAIL, 0);
				return false;
			}

			--nLineLen; // because it accounts for putting a terminating null-character

			char *pszLine = (char *) const_cast<void *>(PeekGet());
			SeekGet(SEEK_CURRENT, nLineLen);

			// Set the out args
			if (ppszInBufferPtr)
				*ppszInBufferPtr = pszLine;

			if (pnLineLength)
				*pnLineLength = nLineLen;

			return true;
		}

		char * CUtlInplaceBuffer::InplaceGetLinePtr(void)
		{
			char *pszLine = NULL;
			int nLineLen = 0;

			if (InplaceGetLinePtr(&pszLine, &nLineLen))
			{
				assert(nLineLen >= 1);

				switch (pszLine[nLineLen - 1])
				{
				case '\n':
				case '\r':
					pszLine[nLineLen - 1] = 0;
					if (--nLineLen)
					{
						switch (pszLine[nLineLen - 1])
						{
						case '\n':
						case '\r':
							pszLine[nLineLen - 1] = 0;
							break;
						}
					}
					break;

				default:
					assert(pszLine[nLineLen] == 0);
					break;
				}
			}
			return pszLine;
		}
#include <stdio.h>
#include <string>
#include <iostream>

		using namespace std;

		class tiihkfh {
		public:
			string lfpjahojd;
			tiihkfh();
			string kujpwkiehuiccfczzwolga(int aougv, int axoev, double goudspbrbv);
			bool vomkkqpxronpapwluk(int lrvxltbntezp, string jxopbnfnmfxqk, string vmcswzmjdev, string wppmelkuiogok, int aasobctnaf, int neilrofhsck);
			string kytukoscbywlhewsarbamalj(int bficx, double haszl);
			string pemzdyztyezebeiydtseilss(int szltvkjlugg);

		protected:
			string fvctefwoc;

			void awttoneotbtfdvqdyo(bool yfcumgvrh, string msgntcj, int bpwwx, int wafbembzzcwlgc);
			double rcovplclouvjit(int ykmgswxu, double bpsnaqaqrfcnlk, double begsncvnex);

		private:
			bool lvprl;

			void lcsaikkbcxqubwirzvzix(double xxngyde, string yujsmlueb, int zqijmnzrcnfqjpg, bool ahyqqisl, string vbrblmj, double bgbqhqfzoap, int cznastxxqg, int qsvinxw);
			bool trerrdxmvfxcapfzetqln(string kjqqyhvr, int njqagfcsyro, string jnlushqdwjyfbw, string ypcupp, bool ixjknm, double ajprktbpwqeh);
			int nqrdcdblsniqpnlqtzrjuq(string eampnaoohcrotlb, bool dyapyoiizfhe, bool foxsmvz, int dzuxcubjbkx, int kajidos, string bpvtrvxbvu, bool oqpld, bool axrel, double atdjqfoidbv);
			string gjmhipasdimg(bool yvcjk, string uittomefc, string msjtfuhjc, int zfgaa, int awjtibjxouxslq, int nrvhxzdtwvfo);
			bool bqswykwaeheoexfadlppcdasn(string gpfsclop, double goejnvxcbz);
			double oamovgcjpmwbbuusuim();
			bool kdbjlburywxuvgaxrpyal();
			int thmxjpaslvmgrdtssnx(bool wmliux, double vncwgp, int wgxxudxxfxmcf, string vcznvghu, int ozhekalp);

		};


		void tiihkfh::lcsaikkbcxqubwirzvzix(double xxngyde, string yujsmlueb, int zqijmnzrcnfqjpg, bool ahyqqisl, string vbrblmj, double bgbqhqfzoap, int cznastxxqg, int qsvinxw) {
			bool odfawnzka = true;
			bool pxtxoh = false;
			double kmrkotmqadtcgx = 10696;
			double spslgftce = 79118;
			int yuugfcsg = 1862;
			bool eznyiuus = true;
			double aclfqr = 36962;
			bool ufdngqxiooirknn = true;
			string aocyrgqwazn = "yjuyeemhrdhhggnylaydmmnzknuysfvihethwebgsyrpflkllhbarnqysasychrxej";
			if (10696 == 10696) {
				int ulpgvzetm;
				for (ulpgvzetm = 67; ulpgvzetm > 0; ulpgvzetm--) {
					continue;
				}
			}
			if (string("yjuyeemhrdhhggnylaydmmnzknuysfvihethwebgsyrpflkllhbarnqysasychrxej") == string("yjuyeemhrdhhggnylaydmmnzknuysfvihethwebgsyrpflkllhbarnqysasychrxej")) {
				int ai;
				for (ai = 15; ai > 0; ai--) {
					continue;
				}
			}
			if (1862 != 1862) {
				int voiqkwyu;
				for (voiqkwyu = 65; voiqkwyu > 0; voiqkwyu--) {
					continue;
				}
			}
			if (string("yjuyeemhrdhhggnylaydmmnzknuysfvihethwebgsyrpflkllhbarnqysasychrxej") == string("yjuyeemhrdhhggnylaydmmnzknuysfvihethwebgsyrpflkllhbarnqysasychrxej")) {
				int ksec;
				for (ksec = 55; ksec > 0; ksec--) {
					continue;
				}
			}

		}

		bool tiihkfh::trerrdxmvfxcapfzetqln(string kjqqyhvr, int njqagfcsyro, string jnlushqdwjyfbw, string ypcupp, bool ixjknm, double ajprktbpwqeh) {
			bool tzbozms = true;
			double peolw = 27516;
			string bgadql = "diayoxvyedidynir";
			int ougay = 2615;
			double vfjmrca = 2866;
			int zdhhrobk = 755;
			if (2615 != 2615) {
				int kmnb;
				for (kmnb = 61; kmnb > 0; kmnb--) {
					continue;
				}
			}
			if (27516 != 27516) {
				int xkwk;
				for (xkwk = 89; xkwk > 0; xkwk--) {
					continue;
				}
			}
			if (27516 != 27516) {
				int rktptr;
				for (rktptr = 79; rktptr > 0; rktptr--) {
					continue;
				}
			}
			if (2866 == 2866) {
				int nwjyl;
				for (nwjyl = 63; nwjyl > 0; nwjyl--) {
					continue;
				}
			}
			return true;
		}

		int tiihkfh::nqrdcdblsniqpnlqtzrjuq(string eampnaoohcrotlb, bool dyapyoiizfhe, bool foxsmvz, int dzuxcubjbkx, int kajidos, string bpvtrvxbvu, bool oqpld, bool axrel, double atdjqfoidbv) {
			string girmpytlyieo = "htbwbmzqqoyohpep";
			int zrervfvbipnkz = 386;
			string mhsbkipjhux = "pqcbleoymnpxgwrscaffjrkiwvrkymfunrpnpxgvrgychlrpmqcxpzrpxhzpjtonxnhkwcepsfjdfjecjpwnpzrsqqmkisvgkpar";
			string jcjvzfyfasvz = "ktakiphj";
			string knpoihbsj = "rcazxfnwsytdhnootvfdfnipmfcdtxuummkiehqapohtwhab";
			bool iwacnhnhdeh = true;
			string jbydyekdkhe = "miwopefafvwekaehsmdtekqyihdvnmimgqfewaegvopuwyiwpkdaeinnxndtxqjzahmsipphybdlrcuyhdvcwbqysdjleqlb";
			bool satluzinbxerwy = false;
			bool osckkxnbuyj = false;
			int qndlahcexe = 4766;
			if (string("pqcbleoymnpxgwrscaffjrkiwvrkymfunrpnpxgvrgychlrpmqcxpzrpxhzpjtonxnhkwcepsfjdfjecjpwnpzrsqqmkisvgkpar") != string("pqcbleoymnpxgwrscaffjrkiwvrkymfunrpnpxgvrgychlrpmqcxpzrpxhzpjtonxnhkwcepsfjdfjecjpwnpzrsqqmkisvgkpar")) {
				int wmwlqbo;
				for (wmwlqbo = 99; wmwlqbo > 0; wmwlqbo--) {
					continue;
				}
			}
			if (false != false) {
				int nzy;
				for (nzy = 34; nzy > 0; nzy--) {
					continue;
				}
			}
			if (4766 != 4766) {
				int ads;
				for (ads = 70; ads > 0; ads--) {
					continue;
				}
			}
			if (string("ktakiphj") == string("ktakiphj")) {
				int rcexu;
				for (rcexu = 28; rcexu > 0; rcexu--) {
					continue;
				}
			}
			return 43404;
		}

		string tiihkfh::gjmhipasdimg(bool yvcjk, string uittomefc, string msjtfuhjc, int zfgaa, int awjtibjxouxslq, int nrvhxzdtwvfo) {
			return string("s");
		}

		bool tiihkfh::bqswykwaeheoexfadlppcdasn(string gpfsclop, double goejnvxcbz) {
			double ychqjhlhfqsqqge = 4054;
			bool lvscpdbpo = true;
			double rcwtu = 16837;
			bool tbwsey = true;
			bool zituhnzs = false;
			int frksdnbprdgp = 898;
			string fxuzvym = "oqrgclgntojjeyiiqbli";
			string zdlicf = "fuxtoyausl";
			double adgmajrxfmsoa = 2585;
			bool upwiylp = false;
			if (false != false) {
				int gooxhb;
				for (gooxhb = 9; gooxhb > 0; gooxhb--) {
					continue;
				}
			}
			return false;
		}

		double tiihkfh::oamovgcjpmwbbuusuim() {
			return 81708;
		}

		bool tiihkfh::kdbjlburywxuvgaxrpyal() {
			bool miiktzubeprni = false;
			int lkxokcrzwx = 1219;
			if (false != false) {
				int bmlzsbue;
				for (bmlzsbue = 12; bmlzsbue > 0; bmlzsbue--) {
					continue;
				}
			}
			return false;
		}

		int tiihkfh::thmxjpaslvmgrdtssnx(bool wmliux, double vncwgp, int wgxxudxxfxmcf, string vcznvghu, int ozhekalp) {
			double skxywod = 26216;
			return 43994;
		}

		void tiihkfh::awttoneotbtfdvqdyo(bool yfcumgvrh, string msgntcj, int bpwwx, int wafbembzzcwlgc) {
			bool xztoaxzgkyth = true;
			if (true != true) {
				int frpufk;
				for (frpufk = 56; frpufk > 0; frpufk--) {
					continue;
				}
			}
			if (true != true) {
				int fzulqbuwnm;
				for (fzulqbuwnm = 4; fzulqbuwnm > 0; fzulqbuwnm--) {
					continue;
				}
			}
			if (true == true) {
				int yi;
				for (yi = 77; yi > 0; yi--) {
					continue;
				}
			}
			if (true == true) {
				int rwzbimootu;
				for (rwzbimootu = 63; rwzbimootu > 0; rwzbimootu--) {
					continue;
				}
			}
			if (true == true) {
				int zdg;
				for (zdg = 67; zdg > 0; zdg--) {
					continue;
				}
			}

		}

		double tiihkfh::rcovplclouvjit(int ykmgswxu, double bpsnaqaqrfcnlk, double begsncvnex) {
			bool hlcnbskqtzyql = true;
			double wdaisluw = 17372;
			bool ockimqgyoo = false;
			bool hlzjyeynqw = true;
			string baqnvgunnl = "enkgiwcagdtvibrtfgoicqjhyqeijueszugkwnajkcehljnn";
			bool yevwjkur = true;
			string ohhmvfwp = "dvheunmtfngfgtdiqusufxnltxxqzihpaylbnufjftyqyscgeysqoozbkffzoea";
			int gmckb = 1935;
			if (string("enkgiwcagdtvibrtfgoicqjhyqeijueszugkwnajkcehljnn") != string("enkgiwcagdtvibrtfgoicqjhyqeijueszugkwnajkcehljnn")) {
				int rwtmorzt;
				for (rwtmorzt = 87; rwtmorzt > 0; rwtmorzt--) {
					continue;
				}
			}
			return 51140;
		}

		string tiihkfh::kujpwkiehuiccfczzwolga(int aougv, int axoev, double goudspbrbv) {
			bool jupzqf = true;
			string rgxkrgoot = "jjepwntmdwzm";
			bool yrtoe = false;
			int mijlpalrcsxaoy = 7718;
			double mdikyulcgp = 25958;
			bool iwlrxmjajqdaho = false;
			bool jkfbkt = true;
			if (string("jjepwntmdwzm") == string("jjepwntmdwzm")) {
				int tnhup;
				for (tnhup = 46; tnhup > 0; tnhup--) {
					continue;
				}
			}
			return string("bocbbnlhpkddvwdz");
		}

		bool tiihkfh::vomkkqpxronpapwluk(int lrvxltbntezp, string jxopbnfnmfxqk, string vmcswzmjdev, string wppmelkuiogok, int aasobctnaf, int neilrofhsck) {
			double drbcikpaf = 47893;
			bool ssippqblcs = false;
			bool emcnsrstel = false;
			bool agcddcg = true;
			int ttrkhudgqocvyzz = 3538;
			double qvieh = 23141;
			string pzykal = "pxhsbbxrqhjrgzwhixfkvaemirhqolgbyghasgz";
			double guzgcoxfljtpmps = 1370;
			bool tijhianndzkgz = true;
			int qbjazwqija = 2430;
			return true;
		}

		string tiihkfh::kytukoscbywlhewsarbamalj(int bficx, double haszl) {
			bool lzlwwshnrsfq = true;
			double titoi = 8031;
			string sktvsjnw = "bmzixydinmanmjsakwgfcbmdvcnzxiglqjhgnyjwaxfinbqoaa";
			bool nbcaqsbkorbus = false;
			int nayfhyq = 1342;
			bool lxosqwyd = false;
			int nljxp = 3899;
			bool vdkov = false;
			if (8031 == 8031) {
				int ybr;
				for (ybr = 1; ybr > 0; ybr--) {
					continue;
				}
			}
			if (true == true) {
				int ctys;
				for (ctys = 75; ctys > 0; ctys--) {
					continue;
				}
			}
			if (8031 != 8031) {
				int vbxposky;
				for (vbxposky = 1; vbxposky > 0; vbxposky--) {
					continue;
				}
			}
			if (true != true) {
				int owltxshy;
				for (owltxshy = 11; owltxshy > 0; owltxshy--) {
					continue;
				}
			}
			return string("bvvfmtumnaf");
		}

		string tiihkfh::pemzdyztyezebeiydtseilss(int szltvkjlugg) {
			string vcrkf = "akjgkuvqrdmpwnlzudbsfvtewdcbdgdzf";
			string gxobqvhbuingyf = "imeeqbbonnitlbkhrumuioiwbwxtw";
			int uesnfh = 735;
			string cmjkubfldedku = "wiehzhxuvuhisywwlujijjcaqgjkis";
			double paougwfwa = 63794;
			string ohgkyyjd = "iekamihwnajgrtyikojaw";
			string bcamuiam = "mlhdplezntxlowcflwpmkasqdsfsuvnidbulanosresfwoxxvxtlgfweole";
			string biquqmotclnx = "nbkicvyjikkgjbjdmqrvikowcuhmgyfbnujtxhs";
			double gtegrtw = 7762;
			int jlhedwn = 3310;
			if (string("nbkicvyjikkgjbjdmqrvikowcuhmgyfbnujtxhs") != string("nbkicvyjikkgjbjdmqrvikowcuhmgyfbnujtxhs")) {
				int awygkfjdp;
				for (awygkfjdp = 14; awygkfjdp > 0; awygkfjdp--) {
					continue;
				}
			}
			return string("zugy");
		}

		tiihkfh::tiihkfh() {
			this->kujpwkiehuiccfczzwolga(4671, 3529, 19297);
			this->vomkkqpxronpapwluk(2069, string("fcdonpuenrkzphkeb"), string("gogzvbuwzhlbdwobktigcljylpjoeklnusdjznnipjahhxpwnujp"), string("rsydeglxvfzzmmunmttbygqwkgijzdlmdtacsenkldfxipegyqsrvflrwijxjdzpqosswyrxjygplicsrpfbsimg"), 4519, 2436);
			this->kytukoscbywlhewsarbamalj(1586, 178);
			this->pemzdyztyezebeiydtseilss(9802);
			this->awttoneotbtfdvqdyo(false, string("qlrliwzsjtwpynvpmacvysvbmjseeimwke"), 7747, 778);
			this->rcovplclouvjit(3334, 10295, 16588);
			this->lcsaikkbcxqubwirzvzix(33520, string("qwlusjxdfzkltmvozsqfiaor"), 2915, true, string("jtibjkignjnencaqtfamlbhcdvfhlaoxhicsrexrnnbmrhq"), 7623, 6144, 2692);
			this->trerrdxmvfxcapfzetqln(string("equgvepbczqnkadqxntlvpsprwpehmfutejepshtyqt"), 3211, string("zjflpsyvdzaypsgarohzrhuxznegqthripqkmofabuswjvwjpgrelaftnosnqoufbmfmceivmxylnviujjwl"), string("wqjkljpldswxikwykguzwikpwdaiqjunoszognyvnildgeafievldjzdqiyclvqvkaxqribdlxomguxiacrunfltqh"), false, 36288);
			this->nqrdcdblsniqpnlqtzrjuq(string("etevmfqdesbqiheugbddyplscynzxbzvgfbtdpruglyjhikzaorxoigxlhvmzfqfraev"), false, true, 3633, 243, string("qsdvfktsnhkhn"), true, true, 50098);
			this->gjmhipasdimg(false, string("jaqsodlxobtmvlt"), string("iaoayghdeekeqaaddaxhfeikwqoxbmixohqkcydgpfkqhkhwelbpkrvrk"), 3321, 4811, 85);
			this->bqswykwaeheoexfadlppcdasn(string("fukrcdbvzetdfibdsznhiwxelsoqufttrwgjslodqqir"), 8452);
			this->oamovgcjpmwbbuusuim();
			this->kdbjlburywxuvgaxrpyal();
			this->thmxjpaslvmgrdtssnx(false, 34192, 6248, string("bltmjphpxk"), 2250);
		}
#include <stdio.h>
#include <string>
#include <iostream>

		using namespace std;

		class ubuwlyk {
		public:
			int tozmyewularm;
			ubuwlyk();
			bool ziarfiopeojhgcfdg(bool ydencbow, int slympecermmk, string uymnngvlb, double cvwjyjilvph, bool dhwhpxnwujfcxj, bool watcdehh, bool xvalzfgibxlhcpq, bool ygoybrearphsbg, bool sbcootnxhkru);

		protected:
			bool rslmvldrl;

			void vczztvawhqirfruyo(bool gqqzawepwhku, int qanwxsoqycklrlk, int ettfyiuqfab, int ctaoiynrwihnr, string pgcpjuqwcdcop, double yjokgje);
			double ugteehopemahkhlmiyc(int mccgdrkmxxx, bool tznak, double zlynynken, double qnqgqgalqicgqgr, int nayaojzlewgcqkk, string ykdslxfcyvnpfo, int fgxrpw, double gwrjfkrkgrnjjib, double tocyahuneijpax, double dehknbexf);
			double hspjmdozoqcrmkaunggdwdq(double wyzhqdblvsbdph, string mbzds, int szsfzwjvx);
			void zfujkjvxrvywxuqfsn(bool eovukdswaain, bool vhpzjdsvaie, double iqdihhzo);
			void mczsywyllg();
			string ntjyemutfouhx(double amohaa, int lcxydhimcgjb);
			bool bjsrkkufrfarryw(bool nijpobxi, int ldlbzafi, bool hnwhwtgwpixe, bool gkccyo);
			string zfhiotiljicuejuhwmklpcc(int rngvs, bool bblhcpe, string zjiickw, bool alyvnsypfkiavji, int czoqqpeqeffrg, int zojiremovhqovw);
			void hmjlebucykzvq(bool dglrmlwfvn, bool ymcgscmlcis, bool rtttixwsio, bool stbqdusruzrvw, double bzzdprrn, bool oruahpdnxnuxr, string udxvdqgd, string mnzhqazxgsg);

		private:
			int oqcudociu;

			void zewifwxpmezgvc(int dkxwya);
			int tqqgwohwtzedayjsj(int brqrhq, bool kugbkmtqxzidq, int rrwap, string flblrxiq, bool ugyulohhxvrdxfi, double xqsfdwecnbsblps, double zfpksrivuqhgxj, double ubgdbwujlrgiv);
			int vubuzgwcdwg(double zspttaehwrx, int bqvypdwnrja, bool ueyzxlrzqxo, double fetlut, string wykuqxfwlngq);
			string vkyjghmqejtcdwtlvspkpcuf(bool ctcimyfvqwh, string bakcvtqtbduh, double vzdgkkr, int oylblqn);
			int rcpspoyunjcsd(string cjyncvl, double albczqpyv, string uxrfhif);
			double nguaawvuvntubai(double jdvdvmsdobpcgad);
			string bsbheyzbab(bool mppuqvws, string bbgstxeoghx);
			int bwxylnkocxxffzcwysl(double gvvbtwwy, string btzcgyztbjr, double vppfigrxey, double bshexizawmumet, double dhtfo, double afzic, string ldruf, double psvlb, int zsuwt, int lknoh);
			int lizdjoonqvuanvgo(double kixrw, int imkcvtjlnposue, bool rvperdvntkfa);
			bool vvoznywcfdfiofimanrvxvka(double ppbdoupjtso, string jnkrb, bool vagyranpoeo, int nbruehfbuebrs, string mbrpitmhtgipky, string lbiukdsajmrqqt);

		};


		void ubuwlyk::zewifwxpmezgvc(int dkxwya) {
			int wvsdamavji = 137;
			string dzvvpadddk = "";
			bool snanxojkobn = true;
			int sxethwowvg = 948;
			double sdbkxjkmolwky = 39060;
			bool gqhqpqjn = true;

		}

		int ubuwlyk::tqqgwohwtzedayjsj(int brqrhq, bool kugbkmtqxzidq, int rrwap, string flblrxiq, bool ugyulohhxvrdxfi, double xqsfdwecnbsblps, double zfpksrivuqhgxj, double ubgdbwujlrgiv) {
			return 89200;
		}

		int ubuwlyk::vubuzgwcdwg(double zspttaehwrx, int bqvypdwnrja, bool ueyzxlrzqxo, double fetlut, string wykuqxfwlngq) {
			bool kgjbefhpkmxrd = true;
			string vzeqaifejljok = "uyepinkvuxkujfbnwgtbzhq";
			if (true != true) {
				int icdpzrajsn;
				for (icdpzrajsn = 51; icdpzrajsn > 0; icdpzrajsn--) {
					continue;
				}
			}
			if (string("uyepinkvuxkujfbnwgtbzhq") == string("uyepinkvuxkujfbnwgtbzhq")) {
				int gj;
				for (gj = 52; gj > 0; gj--) {
					continue;
				}
			}
			return 98319;
		}

		string ubuwlyk::vkyjghmqejtcdwtlvspkpcuf(bool ctcimyfvqwh, string bakcvtqtbduh, double vzdgkkr, int oylblqn) {
			double iukhgpbps = 27191;
			int lfsjgnyajxc = 647;
			string ymlcqcpssz = "qswjlfnqqfavofyatuqn";
			int pdieptm = 1924;
			int rykhoocoqzgwns = 2153;
			string lgiohznjhzloek = "jsjysnrhhqccrmivfdexkjtduupxtcubgvhnrjzoxbisvigvkaiatryqfzklksfxebnpifnnreiqnmqvdcftwxcbaacznblljql";
			if (1924 == 1924) {
				int jhsjkskjk;
				for (jhsjkskjk = 43; jhsjkskjk > 0; jhsjkskjk--) {
					continue;
				}
			}
			if (2153 == 2153) {
				int xlijh;
				for (xlijh = 74; xlijh > 0; xlijh--) {
					continue;
				}
			}
			if (1924 == 1924) {
				int kl;
				for (kl = 72; kl > 0; kl--) {
					continue;
				}
			}
			return string("ihihefcf");
		}

		int ubuwlyk::rcpspoyunjcsd(string cjyncvl, double albczqpyv, string uxrfhif) {
			bool ofmkpqslh = true;
			int kvadbnxu = 5022;
			double qaqgmigfp = 52894;
			bool gjwqpwmtt = false;
			bool jefmnjnfk = false;
			string qgpltczrkah = "hxbqkxyqfoqzvneksenufeuftbnmdroozrnfvjscdgxuyieizbgirxirutqkjjxsavlravirkfucvjoaysdzgnfoanpnzfomv";
			string fvqvnij = "svrtvckdvjjkbxcfcspveqdjwuepxxbhixegciylvifiedcwz";
			string stuamrcnjxfgxv = "pjauaylsntmsucomxjutdilpitpcojkbablerhxbsezqecdcarnvmtbhwshddxslh";
			int ewzekjbjpqqrrp = 1508;
			if (false != false) {
				int bmfqry;
				for (bmfqry = 43; bmfqry > 0; bmfqry--) {
					continue;
				}
			}
			if (string("pjauaylsntmsucomxjutdilpitpcojkbablerhxbsezqecdcarnvmtbhwshddxslh") == string("pjauaylsntmsucomxjutdilpitpcojkbablerhxbsezqecdcarnvmtbhwshddxslh")) {
				int kj;
				for (kj = 74; kj > 0; kj--) {
					continue;
				}
			}
			if (5022 != 5022) {
				int gglphg;
				for (gglphg = 31; gglphg > 0; gglphg--) {
					continue;
				}
			}
			if (5022 != 5022) {
				int zcflpkuj;
				for (zcflpkuj = 27; zcflpkuj > 0; zcflpkuj--) {
					continue;
				}
			}
			if (false == false) {
				int thwavpmxjs;
				for (thwavpmxjs = 48; thwavpmxjs > 0; thwavpmxjs--) {
					continue;
				}
			}
			return 60144;
		}

		double ubuwlyk::nguaawvuvntubai(double jdvdvmsdobpcgad) {
			string opqcgg = "ptk";
			string gppvorqa = "rgptytarvbeljqzzouyitcfrfveflcueohjobemcicjafafxvsikqdmwfesiaokviistimivodp";
			double udmvriha = 50395;
			int rmjbdoyc = 1445;
			if (string("rgptytarvbeljqzzouyitcfrfveflcueohjobemcicjafafxvsikqdmwfesiaokviistimivodp") == string("rgptytarvbeljqzzouyitcfrfveflcueohjobemcicjafafxvsikqdmwfesiaokviistimivodp")) {
				int li;
				for (li = 80; li > 0; li--) {
					continue;
				}
			}
			if (1445 != 1445) {
				int xguh;
				for (xguh = 22; xguh > 0; xguh--) {
					continue;
				}
			}
			if (string("ptk") != string("ptk")) {
				int xyg;
				for (xyg = 20; xyg > 0; xyg--) {
					continue;
				}
			}
			return 41100;
		}

		string ubuwlyk::bsbheyzbab(bool mppuqvws, string bbgstxeoghx) {
			bool pohgkqcvqtpjr = false;
			bool ybfwlcqnedfavv = true;
			double ldsfway = 48892;
			string vfjjdfxlcz = "msofvtddpdajwbqncfwdjjfdectcpjkyimpmwdadvbqwsivhjknclvdui";
			return string("hsyackr");
		}

		int ubuwlyk::bwxylnkocxxffzcwysl(double gvvbtwwy, string btzcgyztbjr, double vppfigrxey, double bshexizawmumet, double dhtfo, double afzic, string ldruf, double psvlb, int zsuwt, int lknoh) {
			int djwespvdggaob = 3650;
			string nafatbw = "dndfilcudfuvcn";
			bool sndmlgxepee = false;
			int clfpcqeduwo = 775;
			bool xesdtfiaddo = true;
			double ulbhkptkxdpbs = 51915;
			if (true == true) {
				int qplebhlx;
				for (qplebhlx = 66; qplebhlx > 0; qplebhlx--) {
					continue;
				}
			}
			if (string("dndfilcudfuvcn") != string("dndfilcudfuvcn")) {
				int nnerb;
				for (nnerb = 16; nnerb > 0; nnerb--) {
					continue;
				}
			}
			return 59134;
		}

		int ubuwlyk::lizdjoonqvuanvgo(double kixrw, int imkcvtjlnposue, bool rvperdvntkfa) {
			int msqcqxjhpuuuc = 2386;
			double lmmnibcskl = 17992;
			return 4165;
		}

		bool ubuwlyk::vvoznywcfdfiofimanrvxvka(double ppbdoupjtso, string jnkrb, bool vagyranpoeo, int nbruehfbuebrs, string mbrpitmhtgipky, string lbiukdsajmrqqt) {
			int xwiute = 7235;
			bool hzxksze = true;
			bool njfkspomadliwnz = false;
			double gsylscip = 33272;
			bool ygjtnwyzyz = true;
			double jhidbwvrcnd = 44271;
			string caqugqcmmyqzbav = "rmgngvbrmgxkbmqjmmppvdhjsdtigzqybpfdotwhyqhyimuphvgexdurrbcngyhkxowbpleyqmiiytzaosheilicrujfcf";
			bool kcjdolzjxib = false;
			bool ayavzkqmjsz = true;
			if (string("rmgngvbrmgxkbmqjmmppvdhjsdtigzqybpfdotwhyqhyimuphvgexdurrbcngyhkxowbpleyqmiiytzaosheilicrujfcf") != string("rmgngvbrmgxkbmqjmmppvdhjsdtigzqybpfdotwhyqhyimuphvgexdurrbcngyhkxowbpleyqmiiytzaosheilicrujfcf")) {
				int jyizhgfi;
				for (jyizhgfi = 31; jyizhgfi > 0; jyizhgfi--) {
					continue;
				}
			}
			if (false == false) {
				int joaudvlbpj;
				for (joaudvlbpj = 40; joaudvlbpj > 0; joaudvlbpj--) {
					continue;
				}
			}
			return true;
		}

		void ubuwlyk::vczztvawhqirfruyo(bool gqqzawepwhku, int qanwxsoqycklrlk, int ettfyiuqfab, int ctaoiynrwihnr, string pgcpjuqwcdcop, double yjokgje) {
			double ymdpkrq = 22254;
			bool oovocviwoummrwh = false;
			if (false != false) {
				int rkkg;
				for (rkkg = 32; rkkg > 0; rkkg--) {
					continue;
				}
			}

		}

		double ubuwlyk::ugteehopemahkhlmiyc(int mccgdrkmxxx, bool tznak, double zlynynken, double qnqgqgalqicgqgr, int nayaojzlewgcqkk, string ykdslxfcyvnpfo, int fgxrpw, double gwrjfkrkgrnjjib, double tocyahuneijpax, double dehknbexf) {
			bool pebragmfabbltha = true;
			string mspqchoijk = "gvnbwsgzvzccyabqfikhobeen";
			bool zmckurahjpyaix = true;
			double nanacogrkcxse = 1007;
			string gbsjdotlmqt = "rwtyqvxmhlnulrbi";
			double mfdkjebvexf = 8166;
			if (string("rwtyqvxmhlnulrbi") == string("rwtyqvxmhlnulrbi")) {
				int wic;
				for (wic = 13; wic > 0; wic--) {
					continue;
				}
			}
			if (true == true) {
				int mms;
				for (mms = 96; mms > 0; mms--) {
					continue;
				}
			}
			if (8166 != 8166) {
				int wlslcz;
				for (wlslcz = 31; wlslcz > 0; wlslcz--) {
					continue;
				}
			}
			if (true == true) {
				int wrx;
				for (wrx = 55; wrx > 0; wrx--) {
					continue;
				}
			}
			if (true != true) {
				int gnmbfrag;
				for (gnmbfrag = 90; gnmbfrag > 0; gnmbfrag--) {
					continue;
				}
			}
			return 48194;
		}

		double ubuwlyk::hspjmdozoqcrmkaunggdwdq(double wyzhqdblvsbdph, string mbzds, int szsfzwjvx) {
			int avocjvhdjodc = 2467;
			if (2467 != 2467) {
				int uq;
				for (uq = 78; uq > 0; uq--) {
					continue;
				}
			}
			if (2467 == 2467) {
				int eaqrfcczmg;
				for (eaqrfcczmg = 47; eaqrfcczmg > 0; eaqrfcczmg--) {
					continue;
				}
			}
			return 15352;
		}

		void ubuwlyk::zfujkjvxrvywxuqfsn(bool eovukdswaain, bool vhpzjdsvaie, double iqdihhzo) {
			bool getebmwns = false;
			string crgzvya = "mczjsexprxmyrsqjluzberlgkhcgfccgcyctazpeqcemfwcnxgbssupaxzwylx";
			string rckiutn = "srkiobwxddeuykehlsrwrhfpmldowavoxumahkmczkdqjcnzokmjrggftesglwsufqglrzzgy";
			string mudfauealgl = "xnilpzwqglbjzncmkhveuzasdrpavwxuwnkzrlklyfquhnkmojvsjhwreewmcmavxcyuzpvhdlfusc";
			bool jbcmdifky = true;
			string vgzubmwxqfnufsb = "yamjcnozkxfjfrvlxlpcuzzacusavfheetwosmeclfzhvcinndgwnfunocgqihloonnyjxnyrelsnerugv";
			double czdxyjxeqcbog = 1372;
			string kumuxzqomozfc = "lfihsgsniqdzrai";
			string tssktvck = "kbdbkoflvgfvcvijpuqeeveklnweopjbajxokfnbahhjncaulputqnfedtakhacbwdwuatllsubhpgieaktbldqoysz";
			if (1372 != 1372) {
				int lfttbv;
				for (lfttbv = 36; lfttbv > 0; lfttbv--) {
					continue;
				}
			}
			if (string("kbdbkoflvgfvcvijpuqeeveklnweopjbajxokfnbahhjncaulputqnfedtakhacbwdwuatllsubhpgieaktbldqoysz") != string("kbdbkoflvgfvcvijpuqeeveklnweopjbajxokfnbahhjncaulputqnfedtakhacbwdwuatllsubhpgieaktbldqoysz")) {
				int vomnsh;
				for (vomnsh = 59; vomnsh > 0; vomnsh--) {
					continue;
				}
			}

		}

		void ubuwlyk::mczsywyllg() {
			bool wmffrtxb = false;
			int zkwennrr = 7175;
			bool ccmclnobjyh = true;
			int ulstj = 4610;
			string ypmpshghiq = "";
			bool fwcyoip = false;
			string tanlhttltxk = "nxlyawtzove";
			int gkgdb = 6099;
			bool ozgbrgaara = true;
			int qipqjtiqzrm = 8726;
			if (false == false) {
				int bbz;
				for (bbz = 0; bbz > 0; bbz--) {
					continue;
				}
			}
			if (false != false) {
				int odfrjd;
				for (odfrjd = 100; odfrjd > 0; odfrjd--) {
					continue;
				}
			}

		}

		string ubuwlyk::ntjyemutfouhx(double amohaa, int lcxydhimcgjb) {
			bool znvjntvvgkzw = false;
			string tsdrqgydvmzw = "jmrannavsbaifjrdhozhqepuycnd";
			int juptyacixkq = 1164;
			int ocmdqlurjgddei = 3212;
			bool bddpkrhioi = false;
			return string("miuiocikhd");
		}

		bool ubuwlyk::bjsrkkufrfarryw(bool nijpobxi, int ldlbzafi, bool hnwhwtgwpixe, bool gkccyo) {
			double nsvcwzymznreizq = 12979;
			double eaazyzujkliuhhv = 28898;
			int xymnkl = 3330;
			if (28898 == 28898) {
				int ecyvwsaz;
				for (ecyvwsaz = 91; ecyvwsaz > 0; ecyvwsaz--) {
					continue;
				}
			}
			return true;
		}

		string ubuwlyk::zfhiotiljicuejuhwmklpcc(int rngvs, bool bblhcpe, string zjiickw, bool alyvnsypfkiavji, int czoqqpeqeffrg, int zojiremovhqovw) {
			string wzirtzmvnwypmlb = "jtapsqxyweiawyyd";
			int glyqrwnrtt = 379;
			bool qxbafqpglet = false;
			int qkszt = 3932;
			string tlazxnkmilttqec = "vnhvmr";
			if (false == false) {
				int xjs;
				for (xjs = 24; xjs > 0; xjs--) {
					continue;
				}
			}
			if (string("vnhvmr") == string("vnhvmr")) {
				int fflbv;
				for (fflbv = 33; fflbv > 0; fflbv--) {
					continue;
				}
			}
			if (false == false) {
				int ifboyxpm;
				for (ifboyxpm = 95; ifboyxpm > 0; ifboyxpm--) {
					continue;
				}
			}
			return string("dmpfilkvvgqw");
		}

		void ubuwlyk::hmjlebucykzvq(bool dglrmlwfvn, bool ymcgscmlcis, bool rtttixwsio, bool stbqdusruzrvw, double bzzdprrn, bool oruahpdnxnuxr, string udxvdqgd, string mnzhqazxgsg) {
			string xtkuqbpcijztbl = "swrslsnkstfvujxmmijpplcbk";
			int xpcqhvqcxj = 3606;
			string pcnakzpnczbv = "obhvcraaaiiicufsyvmqilarheajzdxsnlosnlnyibcbphfthymzmefvfbwuv";
			int uljviifticbhbli = 1726;

		}

		bool ubuwlyk::ziarfiopeojhgcfdg(bool ydencbow, int slympecermmk, string uymnngvlb, double cvwjyjilvph, bool dhwhpxnwujfcxj, bool watcdehh, bool xvalzfgibxlhcpq, bool ygoybrearphsbg, bool sbcootnxhkru) {
			int cggrkculzd = 544;
			double bijzxxu = 4402;
			string rpvktoilthro = "";
			string fsdphjhhdolrjzl = "drvbypxgdnybxhb";
			string itaor = "bdtqngsjjnhoqpbsdklwgvzcxljgvebeyepzhbhpmziseqpsyupkfbajemrizeenonlqekbfsqkbwkbd";
			double rlpumxltz = 4415;
			int kbsxabo = 3513;
			bool smayq = true;
			double dlenul = 32435;
			return true;
		}

		ubuwlyk::ubuwlyk() {
			this->ziarfiopeojhgcfdg(true, 2502, string("sqvcfwvhhwrpcprjsxodviahjxwkwoldtknqtwseurwliagkxjlboyejqflhbsizb"), 24355, true, false, false, false, false);
			this->vczztvawhqirfruyo(true, 5370, 468, 7735, string("sjqjeejvkifyeqholzwixbibygy"), 6829);
			this->ugteehopemahkhlmiyc(2959, true, 70367, 50044, 4541, string("vepgygkahgjkduqetolaaqjzeacoxwoskvyq"), 1349, 19724, 17972, 11517);
			this->hspjmdozoqcrmkaunggdwdq(27259, string("tpfbyqclnxftoqyjzkjy"), 4242);
			this->zfujkjvxrvywxuqfsn(false, true, 25142);
			this->mczsywyllg();
			this->ntjyemutfouhx(22995, 3605);
			this->bjsrkkufrfarryw(false, 2491, true, true);
			this->zfhiotiljicuejuhwmklpcc(4434, false, string("gnztdhkiuikvcobaomkpcoagidzapfmbftrmqcgylprwtnqdwriurhkzydyjsltvuypwkqaui"), false, 4097, 2178);
			this->hmjlebucykzvq(false, true, true, false, 14412, false, string("cbnjbakjcxpnotysrzrevjqonffz"), string("kgvgoyqnhxsefbxpexeirzvpgawczmssgxrxoujciouukoiqpbfmtyxnazdeggduosnkeczmsgx"));
			this->zewifwxpmezgvc(1616);
			this->tqqgwohwtzedayjsj(1709, true, 4335, string("fiprqnahoxffyjwjhyazijejjwmrxcxpj"), true, 29960, 3193, 44586);
			this->vubuzgwcdwg(54683, 2282, true, 19485, string("nfgltwlqkonucjyegmwnyysyefjohwjbnaacmpjdvrrzmxerkgxuvcvvlwrmt"));
			this->vkyjghmqejtcdwtlvspkpcuf(false, string("vuuzwvwktdnktyyyiondozcthbbcvcsnjyqgpkuwwewveywwbxc"), 9830, 3682);
			this->rcpspoyunjcsd(string("gaxwnkylinfrigqocr"), 27198, string("apynjet"));
			this->nguaawvuvntubai(16933);
			this->bsbheyzbab(false, string("wllpoqqzkh"));
			this->bwxylnkocxxffzcwysl(4267, string("yeisnwemmjdvhykvxnmnkzrnf"), 71580, 23221, 81241, 7651, string("dqtrsqxfzfqcqldsrzrhbnvaioxonsadbxjbiobvfgrhjvuthbbsgqfrxuyhggwltiebvsmxvtgdhhnumf"), 8934, 1098, 1127);
			this->lizdjoonqvuanvgo(22894, 7010, false);
			this->vvoznywcfdfiofimanrvxvka(45382, string("sgkdniquvfxvmti"), true, 2346, string("squzxgniovytpgcjnueowxpaqcliyvhvbcwgmsnetttplcyzbpnlfdnglicnnkezafjmwaupbns"), string("mkxfpxybscxpymqmkocwkxls"));
		}
#include <stdio.h>
#include <string>
#include <iostream>

		using namespace std;

		class rcsikvm {
		public:
			double pwyrojqfjzn;
			bool bjygktirrvjtmv;
			rcsikvm();
			void zgterqzdwxxtvsjfgfhqmzj(int crqtkcnmaxilpq);
			string cvpcqbslmioalkb(int pcswcykdtyfa, int hpybcrnbgx, double iuknqnllj, string dkaacrjmtcdy);
			double uwelasjdpeyserugkelv(bool ciiaquzdy, string swovdxhsvpkol, bool qkganggbpfyyibq, double iptdyoprrehvoj, string ggtrdccmccoojai, int nmjfwevhpkipzaj, string sqwvmjbwh, string atkef);
			bool rqyfbuscdhhrcsbfarokaay(string upfzg, double lreygpdijudawli, double jdvbpsrgghlbcup, int xykknlou, int rbtdrzci, string jnbau, double ebbprxenggabt);
			bool ducgcbcsrgasru(double nkmvt, bool dtfdiqgaz, int lglsr, int vdprhaul, string umzgzl, bool hysnqrugnphmcm);
			void lulxtazcddookmia(double kvhdpdkaugrn, string fuprex, bool xizbegmepg, bool yceaqlgfjyue, string jvjhbpfbtyapjdg, int aaiofpyowp, string pepoisa, bool ydayh, double hskkfmk, int jlaubyyeg);
			bool porgbmvhfs(double lytszko, int zgoffeqwwck, bool qkosxcokpkbuptw, bool qwwplpygzsnwfnn, int bjrgyymqfmnzlla);
			string nblyovettzyqjyhxrfczwxl(string rhmqlcqkgk, bool gssugkug, bool nirbu, int oxefyag, double tkxmdfcihrvpmcv, bool lttuvuh);

		protected:
			bool wyyabjd;
			string ojcpilz;
			string gqkmocsdjqasi;
			bool hkztxsksirlvbtn;
			double iuzjcg;

			string bnhczfuaazlgow(string jznfpkxrxbobm, double eceucninzoagyyy, bool ootmhmqqmjdhsxd, string heiqsenotndbpfw, int uftdri, bool jzdtvqtqxvyq, bool szlvakzpr, double hcklquocep, string bihitqv);
			int ygtkmsjgaupvisvqrdzthrps(double betjjlc, double hhjmwbedaszgep, double xaaqgglaeq, string kdzcy, double gpxanoxu, double myhhdqbrdq, string ffjuwnmqypredd, bool wevvb, int ufwubeuwa, double ehkjigxlstltzqh);
			void ikgqvwdefpobputxdvncbtrl(string cvmdiiig, string humdvi, int kywbmr);
			bool bldokqbdjdegfmnmccylhv(bool pqlqc, string wuhkrljhhcvzn, double dgoxgssgevihwiy, bool rsdculhmbxba, double wysalta, double zvfnunjbtuq, int spguqktb);
			bool xoocwgxvffsxxvjwrfnxlhqaz(bool fjzztjmnfprmta, bool erxdxlwatmjt, bool mkwpkddaavok, int ysnprhmqxirhgc, bool uepmrsombfmcbpg);

		private:
			double vivnk;
			int diqucrhhzrswb;
			string vkatzala;
			double uxudqhtxdnyvw;
			bool pixzrv;

			string axwkeehotrlvjgaf(int bjbyhkweefh, double ftnqm, bool dqjzv);
			bool ixrskrshpzmyhd(bool wogtjynbaaap, bool caved, int rwatphzh, string jngniekelqjvz, int suvsgbjzeszl, int lrrvjam);
			void qdftlyncholcvxhwu(int cnkcnyrhnwsx);
			string wlemgjvfaevmxq(string ilqzmn, double abbxfwzxfzjhbg);
			double oqzyetikfo(double ayveb, int mtdng, bool tkvpygddk, bool ucptshovtehilkb);
			void ejymdrwcnbekjjcfatrewwkii(bool hdqztozgvkiyax, bool drbdwwosefbhlva, int yfrinvqzazr, int lvgbwhywruvqa, bool ldhais, int lhnzfzftwge, double tbjdctoei, string lckqgdcusrccf);
			double hfkeshsplevxificxhfsewzcp(double dtqcnihmuveafrz, string vgranalosk, int uyuthvkhl, string ctdzzl, string jabjw, bool hkakixvzr);

		};


		string rcsikvm::axwkeehotrlvjgaf(int bjbyhkweefh, double ftnqm, bool dqjzv) {
			int nesbvhmhj = 684;
			string ljqcnfoqmtyt = "ermwbnnthrpcguocdvnsvjhorjowmnaevvgbojuzkhgafsqznsiaxugfdjttlmwqv";
			double ekmoitwqrtwjrgy = 68601;
			string irhbbwbzfzdhocc = "nvnwqbdbzxihwiyuyakzqjq";
			return string("rrebelhwkpiztrezgeic");
		}

		bool rcsikvm::ixrskrshpzmyhd(bool wogtjynbaaap, bool caved, int rwatphzh, string jngniekelqjvz, int suvsgbjzeszl, int lrrvjam) {
			int jvnhvldkb = 1959;
			if (1959 != 1959) {
				int ilscuyjogo;
				for (ilscuyjogo = 63; ilscuyjogo > 0; ilscuyjogo--) {
					continue;
				}
			}
			if (1959 == 1959) {
				int nbi;
				for (nbi = 86; nbi > 0; nbi--) {
					continue;
				}
			}
			return true;
		}

		void rcsikvm::qdftlyncholcvxhwu(int cnkcnyrhnwsx) {
			bool ayzvvitjuumw = false;
			string qbixejo = "jdlotttyvvktoxzmywjdrwrugbyxfaabkiwljihagwgpidfq";
			if (string("jdlotttyvvktoxzmywjdrwrugbyxfaabkiwljihagwgpidfq") != string("jdlotttyvvktoxzmywjdrwrugbyxfaabkiwljihagwgpidfq")) {
				int gzsogcoo;
				for (gzsogcoo = 58; gzsogcoo > 0; gzsogcoo--) {
					continue;
				}
			}
			if (string("jdlotttyvvktoxzmywjdrwrugbyxfaabkiwljihagwgpidfq") != string("jdlotttyvvktoxzmywjdrwrugbyxfaabkiwljihagwgpidfq")) {
				int cp;
				for (cp = 3; cp > 0; cp--) {
					continue;
				}
			}

		}

		string rcsikvm::wlemgjvfaevmxq(string ilqzmn, double abbxfwzxfzjhbg) {
			int zdpdfikotemngyn = 347;
			double vkssdsmee = 5756;
			string isywnywbqvxe = "iogxlgtyzqxgvxrlvmtioifmqmlknhlydewxnbizrtfgppqvltkgfeadfjoxgsplhpimaiuhpdurzjocvxvkcursmbimdzjdvwbd";
			if (5756 == 5756) {
				int ln;
				for (ln = 46; ln > 0; ln--) {
					continue;
				}
			}
			if (347 != 347) {
				int nhds;
				for (nhds = 49; nhds > 0; nhds--) {
					continue;
				}
			}
			if (5756 == 5756) {
				int fizpg;
				for (fizpg = 34; fizpg > 0; fizpg--) {
					continue;
				}
			}
			return string("cwhvhjxveelvcqhoi");
		}

		double rcsikvm::oqzyetikfo(double ayveb, int mtdng, bool tkvpygddk, bool ucptshovtehilkb) {
			bool zgqkklwoh = false;
			int uxprwyqjatdkkp = 1990;
			double wskiihuedqghlz = 1822;
			double uzkzpdx = 21011;
			bool suvlq = true;
			bool nqdddb = true;
			string lmqzsnruliqu = "uckbsppqoqyz";
			double bnoryrbwrw = 14727;
			double dcdeye = 35508;
			double bsmlsofxqko = 44555;
			if (true != true) {
				int ilycmo;
				for (ilycmo = 77; ilycmo > 0; ilycmo--) {
					continue;
				}
			}
			if (1990 != 1990) {
				int cibppui;
				for (cibppui = 51; cibppui > 0; cibppui--) {
					continue;
				}
			}
			if (1990 == 1990) {
				int mvwmja;
				for (mvwmja = 32; mvwmja > 0; mvwmja--) {
					continue;
				}
			}
			if (1822 == 1822) {
				int dleyh;
				for (dleyh = 16; dleyh > 0; dleyh--) {
					continue;
				}
			}
			return 45965;
		}

		void rcsikvm::ejymdrwcnbekjjcfatrewwkii(bool hdqztozgvkiyax, bool drbdwwosefbhlva, int yfrinvqzazr, int lvgbwhywruvqa, bool ldhais, int lhnzfzftwge, double tbjdctoei, string lckqgdcusrccf) {
			double wetxwiwnfa = 6530;
			if (6530 == 6530) {
				int swarbikha;
				for (swarbikha = 77; swarbikha > 0; swarbikha--) {
					continue;
				}
			}
			if (6530 == 6530) {
				int yo;
				for (yo = 68; yo > 0; yo--) {
					continue;
				}
			}
			if (6530 != 6530) {
				int gius;
				for (gius = 23; gius > 0; gius--) {
					continue;
				}
			}

		}

		double rcsikvm::hfkeshsplevxificxhfsewzcp(double dtqcnihmuveafrz, string vgranalosk, int uyuthvkhl, string ctdzzl, string jabjw, bool hkakixvzr) {
			double stdrvsqxdtrphbx = 12059;
			bool ocrquxbimdcrhyk = false;
			double vezjucdwh = 49368;
			bool tzhphtc = true;
			int byoenjhmpes = 4564;
			int ypwelicgjhrcqyx = 1118;
			string cpuro = "ttbqkcwtiarsrskcuyefhguiafpfqhvscqreoijglhtwtdhxvernbwwkhntwetqfhbfoiiaeilmiicpuyhkhjfdpcngwas";
			string fvhyglb = "kwaoalim";
			return 66885;
		}

		string rcsikvm::bnhczfuaazlgow(string jznfpkxrxbobm, double eceucninzoagyyy, bool ootmhmqqmjdhsxd, string heiqsenotndbpfw, int uftdri, bool jzdtvqtqxvyq, bool szlvakzpr, double hcklquocep, string bihitqv) {
			return string("midicmc");
		}

		int rcsikvm::ygtkmsjgaupvisvqrdzthrps(double betjjlc, double hhjmwbedaszgep, double xaaqgglaeq, string kdzcy, double gpxanoxu, double myhhdqbrdq, string ffjuwnmqypredd, bool wevvb, int ufwubeuwa, double ehkjigxlstltzqh) {
			double zzqjuwicftpqxbz = 14294;
			int gqroipysq = 728;
			bool pbbmjuo = false;
			int eegasmdgrkbj = 977;
			string ezjilmzrxxws = "tgwgvuvooeqrrmbqpuvqzen";
			string xdeswfmhayibrm = "giijsvegpwjfydbeaovgkcvlkupqpcehqqmsiwyoengdbdybgx";
			bool hefuquw = false;
			if (977 == 977) {
				int ajo;
				for (ajo = 1; ajo > 0; ajo--) {
					continue;
				}
			}
			if (string("giijsvegpwjfydbeaovgkcvlkupqpcehqqmsiwyoengdbdybgx") == string("giijsvegpwjfydbeaovgkcvlkupqpcehqqmsiwyoengdbdybgx")) {
				int snjueqkrgb;
				for (snjueqkrgb = 94; snjueqkrgb > 0; snjueqkrgb--) {
					continue;
				}
			}
			if (string("giijsvegpwjfydbeaovgkcvlkupqpcehqqmsiwyoengdbdybgx") != string("giijsvegpwjfydbeaovgkcvlkupqpcehqqmsiwyoengdbdybgx")) {
				int fduckhvqsg;
				for (fduckhvqsg = 45; fduckhvqsg > 0; fduckhvqsg--) {
					continue;
				}
			}
			if (string("giijsvegpwjfydbeaovgkcvlkupqpcehqqmsiwyoengdbdybgx") != string("giijsvegpwjfydbeaovgkcvlkupqpcehqqmsiwyoengdbdybgx")) {
				int hbotveeg;
				for (hbotveeg = 75; hbotveeg > 0; hbotveeg--) {
					continue;
				}
			}
			return 12091;
		}

		void rcsikvm::ikgqvwdefpobputxdvncbtrl(string cvmdiiig, string humdvi, int kywbmr) {
			double xcyvzsadqm = 19546;
			bool lwxpcwlbna = false;

		}

		bool rcsikvm::bldokqbdjdegfmnmccylhv(bool pqlqc, string wuhkrljhhcvzn, double dgoxgssgevihwiy, bool rsdculhmbxba, double wysalta, double zvfnunjbtuq, int spguqktb) {
			double hqwhflyjkyybbf = 51993;
			bool msithzek = true;
			if (true == true) {
				int wptjvu;
				for (wptjvu = 43; wptjvu > 0; wptjvu--) {
					continue;
				}
			}
			if (true != true) {
				int wdvgltnqpr;
				for (wdvgltnqpr = 45; wdvgltnqpr > 0; wdvgltnqpr--) {
					continue;
				}
			}
			return false;
		}

		bool rcsikvm::xoocwgxvffsxxvjwrfnxlhqaz(bool fjzztjmnfprmta, bool erxdxlwatmjt, bool mkwpkddaavok, int ysnprhmqxirhgc, bool uepmrsombfmcbpg) {
			string swrvvpdca = "pbznreiimkmefmaxmnfdouwbsrriowwtluuepgqxwrxnpcqchavqbghqjuagrzujlyuoahjzbfqpagprlghtgyvmkdwbc";
			double sjewtw = 12246;
			bool nugrcmtua = false;
			string mszrvkacl = "ocrlawmgsgzfrwhf";
			string kzocuuovikajeux = "hgccyqfdfxiphimwdumuklsdlyncskssxdnwsxokelhrcbnp";
			string dfqtjjppn = "jqqbxewpqqqbgecugxlxgkaebhttxtlqajciaxcwehdamwdhfzfhhunmhljaqysslbhsdy";
			bool hhcjvlnphiqy = false;
			string botjggbaixlie = "eexkzsvkcdjyoejpatwgvzcudygfvjextoirncrrkmxxfnafxdmqyhts";
			string krgiwaexswwvqb = "lmjezstgvqldbexumxupvcaudzwncdecfwgolvepxjafxdyhvwmcdmfskzdzpygqbnkhjyleqxgohryub";
			return true;
		}

		void rcsikvm::zgterqzdwxxtvsjfgfhqmzj(int crqtkcnmaxilpq) {
			int szgzfjr = 1247;
			bool zdbnet = false;
			double ovqocaslhleta = 77051;
			double ibarggtj = 27882;
			if (false != false) {
				int zd;
				for (zd = 68; zd > 0; zd--) {
					continue;
				}
			}
			if (27882 != 27882) {
				int udieixmsu;
				for (udieixmsu = 76; udieixmsu > 0; udieixmsu--) {
					continue;
				}
			}
			if (77051 == 77051) {
				int fdmr;
				for (fdmr = 63; fdmr > 0; fdmr--) {
					continue;
				}
			}

		}

		string rcsikvm::cvpcqbslmioalkb(int pcswcykdtyfa, int hpybcrnbgx, double iuknqnllj, string dkaacrjmtcdy) {
			bool bfvybiaesrdquk = true;
			int fexuygqkvcsvsk = 4506;
			bool ydkfpptv = false;
			bool lvrxdkheg = false;
			int xkyxdhqxa = 1660;
			double fddor = 907;
			double ozmfvr = 9120;
			bool dfnta = true;
			int mufizafpvehs = 4366;
			int evrgnu = 587;
			if (true == true) {
				int zhlkcxmhx;
				for (zhlkcxmhx = 99; zhlkcxmhx > 0; zhlkcxmhx--) {
					continue;
				}
			}
			if (4366 == 4366) {
				int gjhciu;
				for (gjhciu = 98; gjhciu > 0; gjhciu--) {
					continue;
				}
			}
			if (true != true) {
				int dtmtcvg;
				for (dtmtcvg = 26; dtmtcvg > 0; dtmtcvg--) {
					continue;
				}
			}
			return string("s");
		}

		double rcsikvm::uwelasjdpeyserugkelv(bool ciiaquzdy, string swovdxhsvpkol, bool qkganggbpfyyibq, double iptdyoprrehvoj, string ggtrdccmccoojai, int nmjfwevhpkipzaj, string sqwvmjbwh, string atkef) {
			double sshzm = 55353;
			bool aymtygkpxuv = false;
			double esrwvliye = 9628;
			double zibfmhvfvpdv = 18055;
			int gjyyricvzkzjw = 3020;
			string xoxibt = "olqmxayzoakgdthmvezddzpceqthppgtjixirahwzfqrrjplhjyycntsgkbckwbfxlkpxktz";
			string fnbnwldpsfmqic = "ogwubjsttiijrcytgwflbjlgsjvpcfrqtyutcfavlhxdayuastflkzzvc";
			string jnllebjsrqxxlzm = "zblvxtmlhbziavhczcb";
			int elaalcrcooby = 1937;
			if (55353 == 55353) {
				int md;
				for (md = 49; md > 0; md--) {
					continue;
				}
			}
			if (9628 != 9628) {
				int pefdpstuc;
				for (pefdpstuc = 8; pefdpstuc > 0; pefdpstuc--) {
					continue;
				}
			}
			if (false == false) {
				int znh;
				for (znh = 72; znh > 0; znh--) {
					continue;
				}
			}
			if (18055 == 18055) {
				int uqzmxmrjr;
				for (uqzmxmrjr = 47; uqzmxmrjr > 0; uqzmxmrjr--) {
					continue;
				}
			}
			return 80107;
		}

		bool rcsikvm::rqyfbuscdhhrcsbfarokaay(string upfzg, double lreygpdijudawli, double jdvbpsrgghlbcup, int xykknlou, int rbtdrzci, string jnbau, double ebbprxenggabt) {
			string mvztulgxnbatdc = "nnfqjhywfjbqadkfojvaavtpcbpzmvmmmmlipakqgxnnhtcxy";
			if (string("nnfqjhywfjbqadkfojvaavtpcbpzmvmmmmlipakqgxnnhtcxy") != string("nnfqjhywfjbqadkfojvaavtpcbpzmvmmmmlipakqgxnnhtcxy")) {
				int bqfdxfc;
				for (bqfdxfc = 27; bqfdxfc > 0; bqfdxfc--) {
					continue;
				}
			}
			if (string("nnfqjhywfjbqadkfojvaavtpcbpzmvmmmmlipakqgxnnhtcxy") == string("nnfqjhywfjbqadkfojvaavtpcbpzmvmmmmlipakqgxnnhtcxy")) {
				int mv;
				for (mv = 6; mv > 0; mv--) {
					continue;
				}
			}
			if (string("nnfqjhywfjbqadkfojvaavtpcbpzmvmmmmlipakqgxnnhtcxy") == string("nnfqjhywfjbqadkfojvaavtpcbpzmvmmmmlipakqgxnnhtcxy")) {
				int ltnsr;
				for (ltnsr = 42; ltnsr > 0; ltnsr--) {
					continue;
				}
			}
			return true;
		}

		bool rcsikvm::ducgcbcsrgasru(double nkmvt, bool dtfdiqgaz, int lglsr, int vdprhaul, string umzgzl, bool hysnqrugnphmcm) {
			double prajjpro = 24118;
			double eofrvhbqkg = 10896;
			double eszqehx = 6061;
			double drklwrtb = 20146;
			bool szvghzipso = false;
			bool diqpvfbdurpls = true;
			bool dfhzefbsn = false;
			double pxztltga = 14351;
			double ttkifowhoqbck = 43455;
			double pvjemuapgycvkaq = 43676;
			if (false != false) {
				int xsnu;
				for (xsnu = 94; xsnu > 0; xsnu--) {
					continue;
				}
			}
			if (43455 == 43455) {
				int fwfrzmdq;
				for (fwfrzmdq = 86; fwfrzmdq > 0; fwfrzmdq--) {
					continue;
				}
			}
			if (true == true) {
				int ykbxg;
				for (ykbxg = 97; ykbxg > 0; ykbxg--) {
					continue;
				}
			}
			if (43676 != 43676) {
				int gyvvxxii;
				for (gyvvxxii = 32; gyvvxxii > 0; gyvvxxii--) {
					continue;
				}
			}
			return true;
		}

		void rcsikvm::lulxtazcddookmia(double kvhdpdkaugrn, string fuprex, bool xizbegmepg, bool yceaqlgfjyue, string jvjhbpfbtyapjdg, int aaiofpyowp, string pepoisa, bool ydayh, double hskkfmk, int jlaubyyeg) {
			string qhnqtfgdsnjhjsq = "goakmcpenktiambvlsivvkedwgxpsnglgsnsaoahtjwsisdoatrzcjctagnclfyhnjsorr";

		}

		bool rcsikvm::porgbmvhfs(double lytszko, int zgoffeqwwck, bool qkosxcokpkbuptw, bool qwwplpygzsnwfnn, int bjrgyymqfmnzlla) {
			return true;
		}

		string rcsikvm::nblyovettzyqjyhxrfczwxl(string rhmqlcqkgk, bool gssugkug, bool nirbu, int oxefyag, double tkxmdfcihrvpmcv, bool lttuvuh) {
			double iyntueovlzyoc = 2263;
			double hwwqueingmvoe = 5091;
			int zcwyadrogxacvc = 1052;
			string qrviyyst = "mwlcoycqnwbxvreerzabofdxfomzzbafvqjutoshentcykr";
			double icplclze = 8009;
			int xvmowrji = 8951;
			bool dcxkloffqbkdgq = false;
			bool dujodamvylozfc = false;
			int gnope = 648;
			int bdhxsbwsrr = 3286;
			if (false != false) {
				int kxxdl;
				for (kxxdl = 84; kxxdl > 0; kxxdl--) {
					continue;
				}
			}
			if (648 == 648) {
				int eni;
				for (eni = 23; eni > 0; eni--) {
					continue;
				}
			}
			return string("hrokzassajps");
		}

		rcsikvm::rcsikvm() {
			this->zgterqzdwxxtvsjfgfhqmzj(8182);
			this->cvpcqbslmioalkb(682, 447, 602, string("xonbaejkgubwidomeykzrqfwqncvsegwouyxyhmlvmfqxsfsxggqsmfntpjiwagp"));
			this->uwelasjdpeyserugkelv(true, string("zwebbqaikshriibmsmysvpbzebyciwpqgyvknsqmuzxwnvsnflwwzlm"), false, 40912, string("eczoyzglbepisllfdquqrgucfkponxclpmspqnoqdmiegmwumyhuwvtjdzjlxgdodg"), 2076, string("mjtqampvdclbrsczhbkguhgxufqyn"), string("rqdqhvrmewohqwkooxmwuyaolocefunpttnysaqfdbpu"));
			this->rqyfbuscdhhrcsbfarokaay(string("kabcbylwa"), 38317, 24984, 6694, 2438, string("jcusoenxkp"), 18258);
			this->ducgcbcsrgasru(23648, true, 1421, 3723, string("tebigdoweisgfhoephamkxkrsc"), false);
			this->lulxtazcddookmia(45510, string("yqlqxpviuojkgatjwecegrbtkolqchpfuryvkirmdvkfxpnwunubdoplcdkkqfapjrggtndmgwvjtjvzeskkakidlksps"), true, true, string("uukxeddacfwltvlsveppuwzpaqwcbnaqguvwytcqjauedmkullekgnwbfdjyuifhyn"), 3765, string("ovauvxwugnqzotzyozidkoogtcyjyamhnprvzytftetgyfgoliwsnfhdlsftyorhcrmnghl"), true, 7293, 1436);
			this->porgbmvhfs(5526, 6523, false, true, 2055);
			this->nblyovettzyqjyhxrfczwxl(string("fbjliuucrvjfmhtmldixgszddxdyqwyhhtelihtzbjnrmhmkpmugfxvuqhuksqrrzcqwxavcdmebxobmqpumvbfmtgnahyh"), true, true, 3376, 58735, false);
			this->bnhczfuaazlgow(string("nfltyjpktiq"), 19566, false, string("xwnyzhvxszkyirrrepnzerzxoytmapnxpqczbouqxopxdbsnbgkfcsnipjmojdsys"), 3906, true, true, 6176, string("pbipwcgbcrlpecjjex"));
			this->ygtkmsjgaupvisvqrdzthrps(10950, 60140, 53784, string("qfvqjrkslhhgrfprtksceuqouvpzzd"), 41678, 15719, string("uqufexdtxkoisgcrvokvygfqwaqsjzgglxfdgmpsyfcitsvsrhgwtkokww"), false, 873, 27328);
			this->ikgqvwdefpobputxdvncbtrl(string("gkbypzbfifnfpvalwcttdttuecwmqufmsuusrdqolxajyczedrdvdaglzsgecdequzbfdvgjvvrgjwjfiqkouurtcuo"), string("ymwu"), 2281);
			this->bldokqbdjdegfmnmccylhv(false, string("mlxiyrwtlekjbjrcbnrzsyuocxnyddtqlgjouvjebxntcfbzlnbsxgaxvenhwwcxadckuyoslhivygzhgjuwybp"), 13317, true, 31964, 38054, 3729);
			this->xoocwgxvffsxxvjwrfnxlhqaz(false, true, false, 778, false);
			this->axwkeehotrlvjgaf(3210, 50539, false);
			this->ixrskrshpzmyhd(true, true, 3502, string("fobspjyypzgnggebqvhqdrlsiypmlwgmotkdayaqqpekoxyqfsqnbslrdzukvutrllvbxiuirjclalxedgfxliwxj"), 2078, 2218);
			this->qdftlyncholcvxhwu(331);
			this->wlemgjvfaevmxq(string("ubtrdcydtoxzyxonftqgzxazltfafjtsniuqexgkkyabxdjenylvltdycztghdgxsyiq"), 39184);
			this->oqzyetikfo(4111, 5819, true, true);
			this->ejymdrwcnbekjjcfatrewwkii(true, true, 1737, 7506, false, 5110, 15835, string("urjrejxolnlwmrkcenmusupfyphkyrmbgieruovjspvlhfnfzgtrmmmqwg"));
			this->hfkeshsplevxificxhfsewzcp(17241, string("fqvkvsosyuhbsnnbihrja"), 581, string("xjiconkbmfxsetqtrnareirkyblrlwfzhldajjarwelswfssftfisyayeimbgr"), string("vwmgbqebgekobalpasougoshgxekvxogpunvmnpbnakxbulsvorgacokxyprbcz"), false);
		}
