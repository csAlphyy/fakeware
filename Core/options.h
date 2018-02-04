#pragma once

#include <string>
#include "../skins.h"

class Config
{
public:
#pragma region Visuals
	bool VIS_Chams;
	bool VIS_ChamsToggle;
	bool VIS_ESP;
	bool VIS_ESP_Box;
	bool VIS_ESP_DeadCheck;
	bool VIS_ESP_Bone;
	bool VIS_ESP_Name;
	bool VIS_ESP_Health;
	bool VIS_ESP_Weapon;
	bool VIS_ESP_Fill;
	bool VIS_ChamsXQZ;
	bool VIS_ESP_Snaplines;
	bool VIS_ESP_Radar;
	bool VIS_SniperCH;
	int VIS_Red = 35;
	int VIS_Green = 35;
	int VIS_Blue = 35;
	int InVIS_Red = 35;
	float CHAMS[3] = { 0.5f, 0.66f, 0.15f };
	float BOX[3] = { 1.f, 1.f, 1.f };
	float fillalpha[1] = { 1.f };
	float BOXFill[4] = { 0.f, 0.f, 0.f, fillalpha[0] };
	float skelclr[3] = { 1.f, 1.f, 1.f };
	float snapclr[3] = { 1.f, 1.f, 1.f };
	int InVIS_Green = 35;
	int InVIS_Blue = 35;
#pragma endregion 
#pragma region Misc
	bool MISC_Bunnyhop;
	bool MISC_ForceInventoryOpen;
	int knifemodel;
	int nasaindex;
	bool nasa;
	bool night;
	int hitbox = 1;
	bool nasa2;
	bool nasawhilewalk;
	bool glow;
	bool MISC_AutoAccept;
	bool MISC_GrenadeTrajectory;
	bool MISC_RankReveal;
	SkinInfo_t skins[5035];
	bool MISC_Backtrack;
	int MISC_ViewmodelFOV;
	int MISC_ChamsType;
	bool menu_open;
#pragma endregion
#pragma region aimbot
	bool enablemain;
	float MainSmooth = 1;
	float Mainfov;
	float main_random_Smooth = 1;
	float main_recoil_min;
	float main_recoil_max;
	float main_randomized_angle;
	int MainKey = 1;

	bool enablepistol = false;
	float Pistolfov;
	float PistolSmooth = 1;
	float pistol_random_Smooth = 1;
	float pistol_recoil_min;
	float pistol_recoil_max;
	float pistol_randomized_angle;
	int PistolKey = 1;
#pragma endregion
};

extern Config g_Options;
