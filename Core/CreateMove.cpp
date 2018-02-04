#include "Hooks.h"
#include "../Cheat/aim.h"
#include "globals.h"

bool isastronaut = false;
int(__cdecl *ReadInt) (uintptr_t);
DWORD pClientState;
int nasaindex = 0;

#define NETVAR_OFFSET(funcname, class_name, var_name, offset, ...) \
auto funcname() -> std::add_lvalue_reference_t<__VA_ARGS__> \
{ \
	const auto addr = std::uintptr_t(this) + offset + NetvarSys::Get().GetOffset(class_name, var_name); \
	return *reinterpret_cast<std::add_pointer_t<__VA_ARGS__>>(addr); \
}

#define NETVAR_N(funcname, class_name, var_name, ...) \
	NETVAR_OFFSET(funcname, class_name, var_name, 0, __VA_ARGS__)

class C_PlayerResource
{
public:
	NETPROP(GetTeamProp, "CPlayerResource", "m_iTeam");
};

class C_CS_PlayerResource : public C_PlayerResource
{
public:
	NETVAR_N(GetRanks, "CCSPlayerResource", "m_iCompetitiveRanking", int[65]);
	NETVAR_N(GetWins, "CCSPlayerResource", "m_iCompetitiveWins", int[65]);
	NETVAR_N(GetClanTags, "CCSPlayerResource", "m_szClan", char[65][32]);
	NETVAR_N(GetCoins, "CCSPlayerResource", "m_nActiveCoinRank", unsigned[65]);
	NETVAR_N(GetMusicKits, "CCSPlayerResource", "m_nMusicID", unsigned[65]);
};

__declspec(naked) void __stdcall Hooks::hkCreateMove_Proxy(int Sequence, float Frametime, bool Active)
{
	__asm
	{
		push ebp
		mov  ebp, esp
		push ebx
		lea  ecx, [esp]
		push ecx
		push dword ptr[Active]
		push dword ptr[Frametime]
		push dword ptr[Sequence]
		call Hooks::hkCreateMove
		pop  ebx
		pop  ebp
		retn 0Ch
	}
}

//--------------------------------------------------------------------------------

void Spoof(C_BaseEntity* pLocal, int Coin, int Music, int Level, int CommendsLeader, int CommendsTeacher, int CommendsFriendly)
{
	static DWORD DT_CSPlayerResource = NULL;

	if (DT_CSPlayerResource == NULL)
		DT_CSPlayerResource = Utilities::FindPattern("client.dll", (PBYTE)"x8B\x3D\x00\x00\x00\x00\x85\xFF\x0F\x84\x00\x00\x00\x00\x81\xC7", "xx????xxxx????xx") + 2;

	if (!DT_CSPlayerResource)
		return;

	DWORD ptrResource = **(DWORD**)DT_CSPlayerResource;
	DWORD m_nActiveCoinRank = (DWORD)ptrResource + 0x4884 + (pLocal->EntIndex() * 4);
	DWORD m_nMusicID = (DWORD)ptrResource + 0x493C + (pLocal->EntIndex() * 4);
	DWORD m_nPersonaDataPublicLevel = (DWORD)ptrResource + 0x4A40 + (pLocal->EntIndex() * 4);
	DWORD m_nPersonaDataPublicCommendsLeader = (DWORD)ptrResource + 0x4B44 + (pLocal->EntIndex() * 4);
	DWORD m_nPersonaDataPublicCommendsTeacher = (DWORD)ptrResource + 0x4C48 + (pLocal->EntIndex() * 4);
	DWORD m_nPersonaDataPublicCommendsFriendly = (DWORD)ptrResource + 0x4D4C + (pLocal->EntIndex() * 4);

	if (Coin > 0) *(PINT)((DWORD)m_nActiveCoinRank) = Coin;
	if (Music > 0) *(PINT)((DWORD)m_nMusicID) = Music;
	if (Level > 0) *(PINT)((DWORD)m_nPersonaDataPublicLevel) = Level;
	if (CommendsLeader > 0) *(PINT)((DWORD)m_nPersonaDataPublicCommendsLeader) = CommendsLeader;
	if (CommendsTeacher > 0) *(PINT)((DWORD)m_nPersonaDataPublicCommendsTeacher) = CommendsTeacher;
	if (CommendsFriendly > 0) *(PINT)((DWORD)m_nPersonaDataPublicCommendsFriendly) = CommendsFriendly;
}


void __stdcall Hooks::hkCreateMove(int Sequence, float Frametime, bool Active, bool& bSendPacket)
{
	auto ofunc = HLClientHook.get_original<CreateMove>(Index::CreateMove);
	ofunc(g_CHLClient, Sequence, Frametime, Active);

	auto cmd = g_Input->GetUserCmd(Sequence);
	auto verified = g_Input->GetVerifiedCmd(Sequence);
	auto weapon = g_LocalPlayer->m_hActiveWeapon().Get();

	if (!cmd || !cmd->command_number)
		return;


	if (g_Options.MISC_Bunnyhop && g_LocalPlayer && g_LocalPlayer->GetMoveType() != MOVETYPE_LADDER)
	{
		static bool bLastJumped = false;
		static bool bShouldFake = false;
		if (!bLastJumped && bShouldFake)
		{
			bShouldFake = false;
			cmd->buttons |= IN_JUMP;
		} else if (cmd->buttons & IN_JUMP)
		{
			if (g_LocalPlayer->m_fFlags() & FL_ONGROUND)
			{
				bLastJumped = true;
				bShouldFake = true;
			} else
			{
				cmd->buttons &= ~IN_JUMP;
				bLastJumped = false;
			}
		} else
		{
			bLastJumped = false;
			bShouldFake = false;
		}
	}

	
	grenade_prediction::Get().Tick(cmd->buttons);
	TimeWarp::Get().CreateMove(cmd);
	legitbot::Get().on_create_move(cmd, g_LocalPlayer, weapon);
	

	verified->m_cmd = *cmd;
	verified->m_crc = cmd->GetChecksum();
}
