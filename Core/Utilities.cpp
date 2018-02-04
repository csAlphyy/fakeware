#include "Utilities.h"
#include <Windows.h>
#include <stdio.h>
#include <random>
#include <string>
#include <vector>
#include <Psapi.h>
#include "stdafx.h"
#include "../SDK/csgostructs.h"
#include "Math.h"


namespace Utilities
{
	HANDLE _out = NULL, _old_out = NULL;
	HANDLE _err = NULL, _old_err = NULL;
	HANDLE _in = NULL, _old_in = NULL;

	std::string GetTimeString()
	{
		time_t current_time;
		struct tm *time_info;
		static char timeString[10];
		time(&current_time);
		time_info = localtime(&current_time);
		strftime(timeString, sizeof(timeString), "%X", time_info);
		return timeString;
	}

	std::string GetFPS()
	{
		using namespace std::chrono;
		static int count = 0;
		static auto last = high_resolution_clock::now();
		auto now = high_resolution_clock::now();
		static int fps = 0;

		count++;

		if (duration_cast<milliseconds>(now - last).count() > 1000)
		{
			fps = count;
			count = 0;
			last = now;
		}

		using namespace std::literals::string_literals;
		return std::to_string(fps);
	}

	std::string Truncate(std::string str, size_t width, bool show_ellipsis)
	{
		if (str.length() > width)
			if (show_ellipsis)
				return str.substr(0, width - 3) + "...";
			else
				return str.substr(0, width);
		return str;
	}

	void SetupConsole()
	{
		_old_out = GetStdHandle(STD_OUTPUT_HANDLE);
		_old_err = GetStdHandle(STD_ERROR_HANDLE);
		_old_in = GetStdHandle(STD_INPUT_HANDLE);

		::AllocConsole() && ::AttachConsole(GetCurrentProcessId());

		_out = GetStdHandle(STD_OUTPUT_HANDLE);
		_err = GetStdHandle(STD_ERROR_HANDLE);
		_in = GetStdHandle(STD_INPUT_HANDLE);

		SetConsoleMode(_out,
					   ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);

		SetConsoleMode(_in,
					   ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS |
					   ENABLE_PROCESSED_INPUT | ENABLE_QUICK_EDIT_MODE);
	}

	void CloseConsole()
	{
		if (_out && _err && _in)
		{
			FreeConsole();

			if (_old_out)
				SetStdHandle(STD_OUTPUT_HANDLE, _old_out);
			if (_old_err)
				SetStdHandle(STD_ERROR_HANDLE, _old_err);
			if (_old_in)
				SetStdHandle(STD_INPUT_HANDLE, _old_in);
		}
	}

	bool ConsolePrint(const char* Message, bool NewLine)
	{
		if (!_out)
			return false;

		char buf[1024];
		va_list va;

		va_start(va, Message);
		_vsnprintf_s(buf, 1024, Message, va);
		va_end(va);

		if (NewLine) strcat(buf, "\n");

		return !!WriteConsoleA(_out, buf, static_cast<DWORD>(strlen(buf)), nullptr, nullptr);
	}

	bool ConsolePrint(std::string Message, bool NewLine)
	{
		return ConsolePrint(Message.c_str(), NewLine);
	}

	DWORD GetModuleSize(char* ModuleName)
	{
		HANDLE hSnap;
		MODULEENTRY32 xModule;
		hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetProcessId(GetCurrentProcess()));
		xModule.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnap, &xModule))
		{
			while (Module32Next(hSnap, &xModule))
			{
				if (!strncmp((char*)xModule.szModule, ModuleName, 8))
				{
					CloseHandle(hSnap);
					return (DWORD)xModule.modBaseSize;
				}
			}
		}
		CloseHandle(hSnap);
		return 0;
	}

	Vector ExtrapolateTick(Vector P0, Vector V0)
	{
		return P0 + (V0 * g_GlobalVars->interval_per_tick);
	}

	int WaitForModules(std::int32_t timeout, const std::initializer_list<std::wstring>& modules)
	{
		bool signaled[32] = { 0 };
		bool success = false;

		std::uint32_t totalSlept = 0;

		if (timeout == 0)
		{
			for (auto& mod : modules)
			{
				if (GetModuleHandleW(std::data(mod)) == NULL)
					return WAIT_TIMEOUT;
			}
			return WAIT_OBJECT_0;
		}

		if (timeout < 0)
			timeout = INT32_MAX;

		while (true)
		{
			for (auto i = 0u; i < modules.size(); ++i)
			{
				auto& module = *(modules.begin() + i);
				if (!signaled[i] && GetModuleHandleW(std::data(module)) != NULL)
				{
					signaled[i] = true;

					//
					// Checks if all modules are signaled
					//
					bool done = true;
					for (auto j = 0u; j < modules.size(); ++j)
					{
						if (!signaled[j])
						{
							done = false;
							break;
						}
					}
					if (done)
					{
						success = true;
						goto exit;
					}
				}
			}
			if (totalSlept > std::uint32_t(timeout))
			{
				break;
			}
			Sleep(10);
			totalSlept += 10;
		}

	exit:
		return success ? WAIT_OBJECT_0 : WAIT_TIMEOUT;
	}
	DWORD WaitOnModuleHandle(std::string moduleName)
	{
		DWORD ModuleHandle = NULL;
		while (!ModuleHandle)
		{
			ModuleHandle = (DWORD)GetModuleHandle(moduleName.c_str());
			if (!ModuleHandle)
				Sleep(50);
		}
		return ModuleHandle;
	}


	bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask)
	{
		for (; *szMask; ++szMask, ++Mask, ++Data)
		{
			if (*szMask == 'x' && *Mask != *Data)
			{
				return false;
			}
		}
		return (*szMask) == 0;
	}

	DWORD FindPattern(std::string moduleName, BYTE* Mask, char* szMask)
	{
		DWORD Address = WaitOnModuleHandle(moduleName.c_str());
		MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
		DWORD Length = ModInfo.SizeOfImage;
		for (DWORD c = 0; c < Length; c += 1)
		{
			if (bCompare((BYTE*)(Address + c), Mask, szMask))
			{
				return DWORD(Address + c);
			}
		}
		return 0;
	}

	std::uint8_t* PatternScan(void* module, const char* signature)
	{
		static auto pattern_to_byte = [](const char* pattern) {
			auto bytes = std::vector<int>{};
			auto start = const_cast<char*>(pattern);
			auto end = const_cast<char*>(pattern) + strlen(pattern);

			for (auto current = start; current < end; ++current)
			{
				if (*current == '?')
				{
					++current;
					if (*current == '?')
						++current;
					bytes.push_back(-1);
				} else
				{
					bytes.push_back(strtoul(current, &current, 16));
				}
			}
			return bytes;
		};

		auto dosHeader = (PIMAGE_DOS_HEADER)module;
		auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

		auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
		auto patternBytes = pattern_to_byte(signature);
		auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

		auto s = patternBytes.size();
		auto d = patternBytes.data();

		for (auto i = 0ul; i < sizeOfImage - s; ++i)
		{
			bool found = true;
			for (auto j = 0ul; j < s; ++j)
			{
				if (scanBytes[i + j] != d[j] && d[j] != -1)
				{
					found = false;
					break;
				}
			}
			if (found)
			{
				return &scanBytes[i];
			}
		}
		return nullptr;
	}

	void SetClantag(const char* tag)
	{
		static auto fnClantagChanged = (int(__fastcall*)(const char*, const char*))PatternScan(GetModuleHandleW(L"engine.dll"), "53 56 57 8B DA 8B F9 FF 15");

		fnClantagChanged(tag, tag);
	}

	void SetClantag(std::wstring tag)
	{
		std::string str(tag.begin(), tag.end());
		SetClantag(str.data());
	}

	void SetName(const char* name)
	{
		static auto nameConvar = g_CVar->FindVar("name");
		nameConvar->m_fnChangeCallbacks.m_Size = 0;

		// Fix so we can change names how many times we want
		// This code will only run once because of `static`
		static auto do_once = (nameConvar->SetValue("\nญญญ"), true);

		nameConvar->SetValue(name);
	}

	void RevealRanks()
	{
		using ServerRankRevealAll = char(__cdecl*)(int*);

		static auto fnServerRankRevealAll = PatternScan(GetModuleHandleW(L"client.dll"), "55 8B EC 8B 0D ? ? ? ? 68");

		int v[3] = { 0,0,0 };

		reinterpret_cast<ServerRankRevealAll>(fnServerRankRevealAll)(v);
	}

	std::string WstringToString(std::wstring wstr)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

		try
		{
			return converter.to_bytes(wstr);
		}
		catch (std::range_error)
		{
			std::stringstream s;
			s << wstr.c_str();
			return s.str();
		}
	}

	std::wstring StringToWstring(std::string str)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

		try
		{
			return converter.from_bytes(str);
		}
		catch (std::range_error)
		{
			std::wostringstream s;
			s << str.c_str();
			return s.str();
		}
	}

	bool IsCodePtr(void* ptr)
	{
		constexpr const DWORD protect_flags = PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;

		MEMORY_BASIC_INFORMATION out;
		VirtualQuery(ptr, &out, sizeof out);

		return out.Type
			&& !(out.Protect & (PAGE_GUARD | PAGE_NOACCESS))
			&& out.Protect & protect_flags;
	}
	float random_number_range(float min, float max)
	{
		std::random_device device;
		std::mt19937 engine(device());
		std::uniform_real_distribution<> distribution(min, max);
		return static_cast< float >(distribution(engine));
	}
}
