#define NOMINMAX
#include "stdafx.h"
#include "Utilities.h"
#include "SDK.h"
#include "options.h"
#include "Hooks.h"
#include "InputSys.h"
#include "../Cheat/Cheats.h"
#include "../Menu/Menu.h"

#include <fstream>

HMODULE Library;

void Cheat(LPVOID lpReserved)
{
	if (Utilities::WaitForModules(10000, { L"client.dll", L"engine.dll", L"shaderapidx9.dll" }) == WAIT_TIMEOUT)
		return;

	Utilities::SetupConsole();
	Utilities::ConsolePrint("Initializing Aimdose...");
	Interfaces::Initialize();
	NetvarSys::Get().Initialize();
	InputSys::Get().Initialize();
	Hooks::Initialize();
	Utilities::ConsolePrint("Initializing complete.");

	InputSys::Get().RegisterHotkey(VK_INSERT, [lpReserved]()
	{
		g_Options.menu_open = !g_Options.menu_open;
	});

/*	InputSys::Get().RegisterHotkey(VK_LBUTTON, [lpReserved]()
	{
		Menu::Get().Click();
	});*/

	Utilities::ConsolePrint("Aimdose is ready to use.");

	Utilities::ConsolePrint("Welcome to Aimdose. Enjoy your cheating experience.");

	while (!Die)
		Sleep(750);

	Utilities::ConsolePrint("Unloading Aimdose");
	Utilities::CloseConsole();
	Hooks::Kill();
	Sleep(300);


	FreeLibraryAndExitThread(Library, true);
}

DWORD WINAPI DllMain(HMODULE hinstDLL, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			Library = hinstDLL;
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE) Cheat, lpReserved, 0, 0);
			return TRUE;
		}
		case DLL_PROCESS_DETACH:
		{
			return TRUE;
		}
		default:
			break;
	}

	return FALSE;
}
