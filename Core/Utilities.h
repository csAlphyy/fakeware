#pragma once

#define NOMINMAX
#include <Windows.h>
#include <string>
#include <initializer_list>
#include "stdafx.h"
#include "SDK.h"
#include "../SDK/Math/Vector.h"
#ifdef NDEBUG
#define strenc( s ) std::string( cx_make_encrypted_string( s ) )
#define charenc( s ) strenc( s ).c_str()
#define wstrenc( s ) std::wstring( strenc( s ).begin(), strenc( s ).end() )
#define wcharenc( s ) wstrenc( s ).c_str()
#else
#define strenc( s ) ( s )
#define charenc( s ) ( s )
#define wstrenc( s ) ( s )
#define wcharenc( s ) ( s )
#endif


namespace Utilities
{
	std::string GetTimeString();
	std::string GetFPS();
	std::string Truncate(std::string str, size_t width, bool show_ellipsis = true);

	template <typename T>
	std::wstring to_wstring(const T a_value, const int precision = 6)
	{
		std::wostringstream out;
		out << std::setprecision(precision) << a_value;
		return out.str();
	}

	void SetupConsole();
	void CloseConsole();
	bool ConsolePrint(const char* Message, bool NewLine = true);
	bool ConsolePrint(std::string Message, bool NewLine = true);
	DWORD GetModuleSize(char * ModuleName);
	std::uint8_t* PatternScan(void* Module, const char* Signature);
	DWORD FindPattern(std::string moduleName, BYTE* Mask, char* szMask);
	int WaitForModules(std::int32_t Timeout, const std::initializer_list<std::wstring>& Modules);
	Vector ExtrapolateTick(Vector P0, Vector V0);
	void SetClantag(const char* tag);
	void SetClantag(std::wstring tag);
	void SetName(const char* name);
	void RevealRanks();
	std::string WstringToString(std::wstring wstr);
	std::wstring StringToWstring(std::string str);
	bool IsCodePtr(void* ptr);
	float random_number_range(float min, float max);
}
