#include "Menu.h"
#include "../Core/Font.h"
#include "../Cheat/Skinchanger.h"
#include "../Core/Hooks.h"
#include "../skins.h"
#include "../Core/options.h"

const char* keyNames[] =
{
	"",
	"Mouse 1",
	"Mouse 2",
	"Cancel",
	"Middle Mouse",
	"Mouse 4",
	"Mouse 5",
	"",
	"Backspace",
	"Tab",
	"",
	"",
	"Clear",
	"Enter",
	"",
	"",
	"Shift",
	"Control",
	"Alt",
	"Pause",
	"Caps",
	"",
	"",
	"",
	"",
	"",
	"",
	"Escape",
	"",
	"",
	"",
	"",
	"Space",
	"Page Up",
	"Page Down",
	"End",
	"Home",
	"Left",
	"Up",
	"Right",
	"Down",
	"",
	"",
	"",
	"Print",
	"Insert",
	"Delete",
	"",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"",
	"",
	"",
	"",
	"",
	"Numpad 0",
	"Numpad 1",
	"Numpad 2",
	"Numpad 3",
	"Numpad 4",
	"Numpad 5",
	"Numpad 6",
	"Numpad 7",
	"Numpad 8",
	"Numpad 9",
	"Multiply",
	"Add",
	"",
	"Subtract",
	"Decimal",
	"Divide",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",

};

void unload()
{
	Die = true;
}

std::vector<paint_kit> k_skins;

DWORD dwflag = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove;

const char* bolimnieglowa[] =
{
	"textured", "flat", "health based"
};

const char* niggers[] =
{
	"hitbox",
	"head",
	"neck",
	"chest",
	"stomach",
	"closest"
};

int selected_Weapon = 0;

namespace ImGui {
	static auto vector_getter = [](void* vec, int idx, const char** out_text) {
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values) {
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}
}

void InitializePaintKits() {
	static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	const auto sig_address = Utilities::FindPattern("client.dll", (PBYTE)"\xE8\x00\x00\x00\x00\xFF\x76\x0C\x8D\x48\x04\xE8", "x????xxxxxxx");
	const auto item_system_offset = *reinterpret_cast<std::int32_t*>(sig_address + 1);
	const auto item_system_fn = reinterpret_cast<CCStrike15ItemSystem* (*)()>(sig_address + 5 + item_system_offset);
	const auto item_schema = reinterpret_cast<CCStrike15ItemSchema*>(std::uintptr_t(item_system_fn()) + sizeof(void*));

	const auto get_paint_kit_definition_offset = *reinterpret_cast<std::int32_t*>(sig_address + 11 + 1);
	const auto get_paint_kit_definition_fn = reinterpret_cast<CPaintKit*(__thiscall*)(CCStrike15ItemSchema*, int)>(sig_address + 11 + 5 + get_paint_kit_definition_offset);
	const auto start_element_offset = *reinterpret_cast<std::intptr_t*>(std::uintptr_t(get_paint_kit_definition_fn) + 8 + 2);
	const auto head_offset = start_element_offset - 12;
	const auto map_head = reinterpret_cast<Head_t<int, CPaintKit*>*>(std::uintptr_t(item_schema) + head_offset);

	for (auto i = 0; i <= map_head->last_element; ++i) {
		const auto paint_kit = map_head->memory[i].value;

		if (paint_kit->id == 9001)
			continue;

		const auto wide_name = g_Localize->Find(paint_kit->item_name.buffer + 1);
		const auto name = converter.to_bytes(wide_name);

		if (paint_kit->id < 10000)
			k_skins.push_back({ paint_kit->id, name });
	}

	std::sort(k_skins.begin(), k_skins.end());
	k_skins.insert(k_skins.begin(), { 0, "None" });
}

std::vector<std::pair<ItemDefinitionIndex, std::string>> weapons = {
{ WEAPON_DEAGLE, "Desert Eagle" },
{ WEAPON_ELITE, "Dual Berettas" },
{ WEAPON_FIVESEVEN, "Five-SeveN" },
{ WEAPON_GLOCK, "Glock-18" },
{ WEAPON_AK47, "AK-47" },
{ WEAPON_AUG, "AUG" },
{ WEAPON_AWP, "AWP" },
{ WEAPON_FAMAS, "FAMAS" },
{ WEAPON_G3SG1, "G3SG1" },
{ WEAPON_GALILAR, "Galil AR" },
{ WEAPON_M249, "M249" },
{ WEAPON_M4A1, "M4A4" },
{ WEAPON_MAC10, "MAC-10" },
{ WEAPON_P90, "P90" },
{ WEAPON_UMP45, "UMP-45" },
{ WEAPON_XM1014, "XM1014" },
{ WEAPON_BIZON, "PP-Bizon" },
{ WEAPON_MAG7, "MAG-7" },
{ WEAPON_NEGEV, "Negev" },
{ WEAPON_SAWEDOFF, "Sawed.Off" },
{ WEAPON_TEC9, "Tec-9" },
{ WEAPON_HKP2000, "P2000" },
{ WEAPON_MP7, "MP7" },
{ WEAPON_MP9, "MP9" },
{ WEAPON_NOVA, "Nova" },
{ WEAPON_P250, "P250" },
{ WEAPON_SCAR20, "SCAR-20" },
{ WEAPON_SG556, "SG 553" },
{ WEAPON_SSG08, "SSG 08" },
{ WEAPON_M4A1_SILENCER, "M4A1-S" },
{ WEAPON_USP_SILENCER, "USP-S" },
{ WEAPON_CZ75A, "CZ75-Auto" },
{ WEAPON_REVOLVER, "R8 Revolver" }
};

static const char* knifemodels[] = {
	"default", "bayonet", "flip", "gut", "karambit", "m9 bayonet", "huntsman", "bowie", "butterfly", "falchion", "shadow daggers"
};

std::vector<std::pair<int, const char*>> weaponcomboname = {
{ WEAPON_DEAGLE, "Desert Eagle" },
{ WEAPON_ELITE, "Dual Berettas" },
{ WEAPON_FIVESEVEN, "Five-SeveN" },
{ WEAPON_GLOCK, "Glock-18" },
{ WEAPON_AK47, "AK-47" },
{ WEAPON_AUG, "AUG" },
{ WEAPON_AWP, "AWP" },
{ WEAPON_FAMAS, "FAMAS" },
{ WEAPON_G3SG1, "G3SG1" },
{ WEAPON_GALILAR, "Galil AR" },
{ WEAPON_M249, "M249" },
{ WEAPON_M4A1, "M4A4" },
{ WEAPON_MAC10, "MAC-10" },
{ WEAPON_P90, "P90" },
{ WEAPON_UMP45, "UMP-45" },
{ WEAPON_XM1014, "XM1014" },
{ WEAPON_BIZON, "PP-Bizon" },
{ WEAPON_MAG7, "MAG-7" },
{ WEAPON_NEGEV, "Negev" },
{ WEAPON_SAWEDOFF, "Sawed.Off" },
{ WEAPON_TEC9, "Tec-9" },
{ WEAPON_HKP2000, "P2000" },
{ WEAPON_MP7, "MP7" },
{ WEAPON_MP9, "MP9" },
{ WEAPON_NOVA, "Nova" },
{ WEAPON_P250, "P250" },
{ WEAPON_SCAR20, "SCAR-20" },
{ WEAPON_SG556, "SG 553" },
{ WEAPON_SSG08, "SSG 08" },
{ WEAPON_M4A1_SILENCER, "M4A1-S" },
{ WEAPON_USP_SILENCER, "USP-S" },
{ WEAPON_CZ75A, "CZ75-Auto" },
{ WEAPON_REVOLVER, "R8 Revolver" },
{ 10000, "Knife"}
};

const char* weaponNamesForCombobox[] = {
	"Desert Eagle",
	"Dual Berettas",
	"Five-SeveN",
	"Glock-18",
	"AK-47",
	"AUG",
	"AWP",
	"FAMAS",
	"G3SG1",
	"Galil AR",
	"M249",
	"M4A4",
	"MAC-10",
	"P90",
	"UMP-45",
	"XM1014",
	"PP-Bizon",
	"MAG-7",
	"Negev",
	"Sawed.Off",
	"Tec-9",
	"P2000",
	"MP7",
	"MP9",
	"Nova",
	"P250",
	"SCAR-20",
	"SG 553",
	"SSG 08",
	"M4A1-S",
	"USP-S",
	"CZ75-Auto" ,
	"R8 Revolver",
	"Knife"
};
const char* chuj[];



std::vector<std::string> skins_cc;

void initialize()
{
	static bool once = false;
	
	if (!once)
	{
		InitializePaintKits();	

		for (int i = 0; i < k_skins.size(); i++) {
			skins_cc.push_back(k_skins[i].name);
		}
		once = true;
	}
}

void Init()
{

	ImGuiStyle& style = ImGui::GetStyle();

	/*m_isVisible = false;
	CWindow Main(735, 415, 350, 245);

#pragma region Visuals
	auto Visuals = new CChild(0, 0, 120, L"Visuals");

	//default, bayonet, flip, gut, karambit, m9, huntsman, bowie, butterfly, falchion, daggers
	Visuals->AddControl(new CCheckBox(L"chams", &g_Options.VIS_ChamsToggle));
	Visuals->AddControl(new CCombo(&g_Options.MISC_ChamsType, { L"xqz", L"normal", L"normal clr", L"flat clr", L"xqz clr", L"flat xqz clr", L"health based", L"health based xqz" }));
	Visuals->AddControl(new CCheckBox(L"esp", &g_Options.VIS_ESP));
	Visuals->AddControl(new CCheckBox(L"box", &g_Options.VIS_ESP_Box));
	Visuals->AddControl(new CCheckBox(L"skeleton", &g_Options.VIS_ESP_Bone));
	Visuals->AddControl(new CCheckBox(L"name", &g_Options.VIS_ESP_Name));
	Visuals->AddControl(new CCheckBox(L"health", &g_Options.VIS_ESP_Health));
	Visuals->AddControl(new CCheckBox(L"weapon", &g_Options.VIS_ESP_Weapon));
	Visuals->AddControl(new CCheckBox(L"snap lines", &g_Options.VIS_ESP_Snaplines));
	Visuals->AddControl(new CCheckBox(L"radar", &g_Options.VIS_ESP_Radar));
	Visuals->AddControl(new CCheckBox(L"grenade prediction", &g_Options.MISC_GrenadeTrajectory));
	Visuals->AddControl(new CCheckBox(L"dead check", &g_Options.VIS_ESP_DeadCheck));
	Visuals->AddControl(new CCheckBox(L"sniper crosshair", &g_Options.VIS_SniperCH));
	Main.AddControl(Visuals);

	auto skins = new CChild(0, 217, 120, L"Skins");
	skins->AddControl(new CCombo(&g_Options.skins_knifemodel, { L"default", L"bayonet", L"flip", L"gut", L"karambit", L"m9", L"huntsman", L"bowie", L"butterfly", L"falchion", L"daggers" }));
	skins->AddControl(new CCombo(&g_Options.skins_knifeskins, { L"vanilla", L"forest ddpat", L"doppler", L"crimson web", L"doppler", L"slaughter", L"bone mask", L"fade", L"night", L"blue steel", L"stained", L"case hardened", L"safari mesh", L"boreal forest", L"ultraviolet", L"urban masked", L"doppler", L"doppler", L"damascus steel", L"damascus steel", L"scorched", L"tiger tooth", L"rust coat", L"doppler", L"doppler", L"doppler", L"marble fade" }));
	skins->AddControl(new CTextBox(105, 16, L"seed", &g_Options.skins_knifeseed));
	skins->AddControl(new CCombo(current_weapon, { L"vanilla", L"forest ddpat", L"doppler", L"crimson web", L"doppler", L"slaughter", L"bone mask", L"fade", L"night", L"blue steel", L"stained", L"case hardened", L"safari mesh", L"boreal forest", L"ultraviolet", L"urban masked", L"doppler", L"doppler", L"damascus steel", L"damascus steel", L"scorched", L"tiger tooth", L"rust coat", L"doppler", L"doppler", L"doppler", L"marble fade" }));
	skins->AddControl(new CButton(105, 16, L"apply", []() { Skinchanger::Get().LoadSkins(); }));
	Main.AddControl(skins);
#pragma endregion
#pragma region Misc
	auto Misc = new CChild(130, 0, 190, L"Misc");

	Misc->AddControl(new CCheckBox(L"backtrack", &g_Options.MISC_Backtrack));
	Misc->AddControl(new CCheckBox(L"auto accept", &g_Options.MISC_AutoAccept));
	Misc->AddControl(new CCheckBox(L"force inventory on", &g_Options.MISC_ForceInventoryOpen));
	Misc->AddControl(new CCheckBox(L"bunnyhop", &g_Options.MISC_Bunnyhop));
	Misc->AddControl(new CCheckBox(L"reveal ranks", &g_Options.MISC_RankReveal));
	Misc->AddControl(new CSlider<int>(L"viewmodel fov", 0.0f, 40.0f, &g_Options.MISC_ViewmodelFOV));
	Misc->AddControl(new CTextBox(80, 16, L"clan tag", &g_Options.MISC_ClanTag));
//	Misc->AddControl(new CButton(80, 16, L"load skins", []() { Skinchanger::Get().LoadSkins(); }));
	Misc->AddControl(new CButton(80, 16, L"unhook", []() { unload(); }));

	Main.AddControl(Misc);
#pragma endregion

#pragma region Colors
	auto colors = new CChild(130, 220, 190, L"chams visible");

	colors->AddControl(new CSlider<int>(L"vis chams r", 0, 255, &g_Options.VIS_Red));
	colors->AddControl(new CSlider<int>(L"vis chams g", 0, 255, &g_Options.VIS_Green));
	colors->AddControl(new CSlider<int>(L"vis chams b", 0, 255, &g_Options.VIS_Blue));

	Main.AddControl(colors);

	auto colors2 = new CChild(130, 155, 190, L"colors invisible");
	colors2->AddControl(new CSlider<int>(L"invis chams r", 0, 255, &g_Options.InVIS_Red));
	colors2->AddControl(new CSlider<int>(L"invis chams g", 0, 255, &g_Options.InVIS_Green));
	colors2->AddControl(new CSlider<int>(L"invis chams b", 0, 255, &g_Options.InVIS_Blue));
	Main.AddControl(colors2);
#pragma endregion

	MenuForm.AddWindow(Main);

	/*CWindow skinchanger(1000, 415, 350, 300);
#pragma region sc
	//default, bayonet, flip, gut, karambit, m9, huntsman, bowie, butterfly, falchion, daggers
	auto nigger = new CChild(0, 0, 190, L"skinchanger");
	nigger->AddControl(new CButton(80, 16, L"load skins", []() { Skinchanger::Get().LoadSkins(); }));
	//skins_knifemodel
	nigger->AddControl(new CCombo(&g_Options.skins_knifemodel, { L"default", L"bayonet", L"flip", L"gut", L"karambit", L"m9", L"huntsman", L"bowie", L"butterfly", L"falchion", L"daggers" }));
	nigger->AddControl(new CCombo(&g_Options.skins_knifeskins, { L"vanilla", L"doppler" }));
	skinchanger.AddControl(nigger);
#pragma endregion
	MenuForm.AddWindow(skinchanger);*/


	static bool colors;
	static int menu_tab = 0;

	float width = (ImGui::GetContentRegionAvailWidth() / 2) - 4;

	ImGui::SetNextWindowPos(ImVec2(1170, 416));
	ImGui::Begin("esp preview", &g_Options.menu_open, ImVec2(180, 247), 1.f, dwflag);
	{
		ImDrawList* draw = ImGui::GetWindowDrawList();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
		ImGui::GroupBox("##preview", "esp preview", ImVec2(0, 0), true, 0);
		{	
			int alpha = 0;
			const char* name = "";
			ImGui::Dummy(ImVec2(32, 0));
			ImGui::SameLine();	
			if (g_Options.VIS_ESP_Name) {
				name = "name";
			}
			ImGui::Text(name);
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImGui::Spacing();
			if (g_Options.VIS_ESP_Health) {
				alpha = 255;
			}
			else
			{
				alpha = 0;
			}
			draw->AddRect(ImVec2(p.x, p.y), ImVec2(p.x + 2, p.y + 175), ImColor(0, 255, 0, alpha), 0.0f, 15, 1.f);
			ImGui::SameLine(12);
			if (g_Options.VIS_ESP_Box) {
				ImVec2 p1 = ImGui::GetCursorScreenPos();
				draw->AddRect(ImVec2(p1.x, p1.y), ImVec2(p1.x + 100, p1.y + 175), ImColor(g_Options.BOX[0], g_Options.BOX[1], g_Options.BOX[2]), 0.0f, 15, 1.f);
			}
				
			if (g_Options.VIS_ESP_Weapon) {
				ImGui::SameLine(116);
				ImGui::Text("weapon");
			}								
		}
		ImGui::EndChild();
		ImGui::PopStyleColor();
	}
	ImGui::End();



	ImGui::SetNextWindowPosCenter(ImGuiSetCond_Appearing);
	ImGui::Begin("aimdose", &g_Options.menu_open, ImVec2((80 * 5) + (8 * 4), 247), 1.f, dwflag);
	{
		static int sub_tab = 0;
		static int aim_tab = 0;
		static int soemrandomshit;
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(54 / 255.f, 54 / 255.f, 54 / 255.f, 1.f));
		if (menu_tab == 0)
		{
			
			
				if (aim_tab == 0){
					ImGui::GroupBox("aimbot_settings##rifles", "rifles", ImVec2(-1, 200), true, 0); {
						ImGui::Checkbox("enable", &g_Options.enablemain);
						ImGui::Checkbox("backtrack", &g_Options.MISC_Backtrack);						
						ImGui::Combo("##hitboxtypes", &g_Options.hitbox, niggers, ARRAYSIZE(niggers));
						ImGui::Combo("##0key", &g_Options.MainKey, keyNames, ARRAYSIZE(keyNames));
						ImGui::SliderFloat("##0fov", &g_Options.Mainfov, 0.00f, 180.00f, "fov: %.1f");
						ImGui::SliderFloat("##0smoothing", &g_Options.MainSmooth, 1.00f, 100.00f, "smoothing: %.1f");
						ImGui::Dummy(ImVec2(0, 7.5));
						if (ImGui::Button("pistols>>", ImVec2(-1, 25))) aim_tab = 1;
					}
					ImGui::EndGroupBox();
				}
				if (aim_tab == 1) {
					ImGui::GroupBox("aimbot_settings##pistols", "pistols", ImVec2(-1, 200), true, 0); {
						ImGui::Checkbox("enable", &g_Options.enablepistol);
						ImGui::Checkbox("backtrack", &g_Options.MISC_Backtrack);
						ImGui::Combo("##hitboxtypes", &g_Options.hitbox, niggers, ARRAYSIZE(niggers));
						ImGui::Combo("##1key", &g_Options.PistolKey, keyNames, ARRAYSIZE(keyNames));
						ImGui::SliderFloat("##1fov", &g_Options.Pistolfov, 0.00f, 180.00f, "fov: %.1f");
						ImGui::SliderFloat("##1smoothing", &g_Options.PistolSmooth, 1.00f, 100.00f, "smoothing: %.1f");
						ImGui::Dummy(ImVec2(0, 7.5));
						if (ImGui::Button("<<rifles", ImVec2(-1, 25))) aim_tab = 0;
					}
					ImGui::EndGroupBox();
				}
				

				
		}
		if (menu_tab == 1)
		{
			ImGui::GroupBox("##visualstab", "visuals", ImVec2(-1, 200), true, 0); {
				if (sub_tab == 0)
				{
					ImGui::Columns(2, NULL, true);
					{
						ImGui::Checkbox("enable", &g_Options.VIS_ESP);
						ImGui::Checkbox("box", &g_Options.VIS_ESP_Box);
						ImGui::Checkbox("name", &g_Options.VIS_ESP_Name);
						ImGui::Checkbox("health", &g_Options.VIS_ESP_Health);
						ImGui::Checkbox("weapon", &g_Options.VIS_ESP_Weapon);
						ImGui::Checkbox("skeleton", &g_Options.VIS_ESP_Bone);
						ImGui::Checkbox("glow", &g_Options.glow);
						ImGui::Checkbox("chams", &g_Options.VIS_ChamsToggle);
						ImGui::PushItemWidth(120);
						ImGui::Combo("##chamstype", &g_Options.MISC_ChamsType, bolimnieglowa, ARRAYSIZE(bolimnieglowa));
						ImGui::PopItemWidth();
						ImGui::Checkbox("snap lines", &g_Options.VIS_ESP_Snaplines);

					}
					ImGui::NextColumn();
					{
						ImGui::Checkbox("radar", &g_Options.VIS_ESP_Radar);
						ImGui::Checkbox("night", &g_Options.night);
						ImGui::Checkbox("grenade prediction", &g_Options.MISC_GrenadeTrajectory);
						ImGui::Checkbox("dead check", &g_Options.VIS_ESP_DeadCheck);
						ImGui::Checkbox("sniper crosshair", &g_Options.VIS_SniperCH);
						if (ImGui::Button("unhook", ImVec2(-1, 18))) unload();
						if (ImGui::Button("colors>>", ImVec2(-1, 18))) sub_tab = 1;
						ImGui::PushItemWidth(-1);
						ImGui::SliderInt("##viewmodelfov", &g_Options.MISC_ViewmodelFOV, 0.0f, 40.f, "viewmodel fov: %.0f");
						ImGui::PopItemWidth();
					}

					ImGui::Columns(1);
				}

				if (sub_tab == 1)
				{
					ImGui::Columns(2, NULL, true);
					{
						ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
						ImGui::MyColorEdit3("chams color##chamscolor", g_Options.CHAMS, ImGuiColorEditFlags_NoSliders);

						ImGui::MyColorEdit3("box color##chamscolor", g_Options.BOX, ImGuiColorEditFlags_NoSliders);

						ImGui::MyColorEdit3("box fill color##chamscolor", g_Options.BOXFill, ImGuiColorEditFlags_NoSliders);	
						
						ImGui::SliderFloat("##boxfillalpha", g_Options.fillalpha, 0.0f, 1.0f, "fill alpha: %0.1f");

						ImGui::MyColorEdit3("skeleton color##skelcolor", g_Options.skelclr, ImGuiColorEditFlags_NoSliders);

						ImGui::MyColorEdit3("snaplines color##snapcolor", g_Options.snapclr, ImGuiColorEditFlags_NoSliders);
						ImGui::PopStyleVar();
					}
					ImGui::NextColumn();
					{

						if (ImGui::Button("<<visuals", ImVec2(-1, 18))) sub_tab = 0;
					}
					ImGui::Columns(1);
				}
			

				//&g_Options.InVIS_Red

				ImGui::Spacing();
			}
			ImGui::EndChild();
		}
		if (menu_tab == 2)
		{
			ImGui::GroupBox("##misctab", "misc", ImVec2(-1, 200), true, 0); {
				ImGui::Checkbox("bunnyhop", &g_Options.MISC_Bunnyhop);
				ImGui::Checkbox("auto accept", &g_Options.MISC_AutoAccept);
				ImGui::Checkbox("reveal ranks", &g_Options.MISC_RankReveal);
				ImGui::Checkbox("force inventory on", &g_Options.MISC_ForceInventoryOpen);				
				ImGui::Spacing();
			}
			ImGui::EndChild();
		}
		if (menu_tab == 3)
		{
			ImGui::GroupBox("##skinstab", "skins", ImVec2(-1, 200), true, 0);
			{

				ImGui::Columns(2, NULL, true);
				{
					ImGui::PushItemWidth(-1);
					ImGui::ListBox("##paintkit1", &selected_Weapon, weaponNamesForCombobox, ARRAYSIZE(weaponNamesForCombobox), 8);
					ImGui::PopItemWidth();
				}
				ImGui::NextColumn();
				{
					
					ImGui::PushItemWidth(-1);
					ImGui::Combo("##paintkit12", &g_Options.skins[weaponcomboname.at(selected_Weapon).first].weapon_skin_id, ImGui::vector_getter, static_cast<void*>(&skins_cc), skins_cc.size());
					//ImGui::Combo("Skin", &g_Options.skins[weapons[selected_Weapon].first].weapon_skin_id, skins_cc);

					ImGui::Text("seed");
					ImGui::SameLine();
					ImGui::InputInt("##seed", &g_Options.skins[weaponcomboname.at(selected_Weapon).first].weapon_seed, 0, 1000000000);
					
					ImGui::Text("wear");
					ImGui::SameLine();
					ImGui::SliderFloat("##wear", &g_Options.skins[weaponcomboname.at(selected_Weapon).first].weapon_wear, 0.0f, 1.0f, "%0.6f");

							
					ImGui::PopItemWidth();
				}
				ImGui::Columns(1);

				ImGui::Dummy(ImVec2(0, 2.5));
				if (ImGui::Button("apply", ImVec2(-1, 25))) Skinchanger::Get().LoadSkins();
				

				ImGui::Spacing();
			}
			ImGui::EndChild();
		}
		ImGui::PopStyleColor(2);
		ImGui::Dummy(ImVec2(0, 4.5));

		style.Colors[ImGuiCol_Text] = menu_tab == 0 ? ImColor(255, 255, 255) : ImColor(175, 175, 175, 255);
		style.Colors[ImGuiCol_Button] = menu_tab == 0 ? ImColor(45, 45, 45, 255) : ImColor(54, 54, 54, 255);
		if (ImGui::Button("aimbot", ImVec2(101, 20))) menu_tab = 0;
		ImGui::SameLine();
		style.Colors[ImGuiCol_Text] = menu_tab == 1 ? ImColor(255, 255, 255) : ImColor(175, 175, 175, 255);
		style.Colors[ImGuiCol_Button] = menu_tab == 1 ? ImColor(45, 45, 45, 255) : ImColor(54, 54, 54, 255);
		if (ImGui::Button("visuals", ImVec2(101, 20))) menu_tab = 1;
		ImGui::SameLine();
		style.Colors[ImGuiCol_Text] = menu_tab == 2 ? ImColor(255, 255, 255) : ImColor(175, 175, 175, 255);
		style.Colors[ImGuiCol_Button] = menu_tab == 2 ? ImColor(45, 45, 45, 255) : ImColor(54, 54, 54, 255);
		if (ImGui::Button("misc", ImVec2(101, 20))) menu_tab = 2;
		ImGui::SameLine();
		style.Colors[ImGuiCol_Text] = menu_tab == 3 ? ImColor(255, 255, 255) : ImColor(175, 175, 175, 255);
		style.Colors[ImGuiCol_Button] = menu_tab == 3? ImColor(45, 45, 45, 255) : ImColor(54, 54, 54, 255);
		if (ImGui::Button("skins", ImVec2(101, 20))) menu_tab = 3;
		
		
	}
	ImGui::End();

	/*if (colors)
	{
		ImGui::Begin("##colors", &colors, ImVec2((80 * 5) + (8 * 4), 245), 1.f, dwflag); {

			ImGui::BeginChild("colors", ImVec2(-1, -1), true);
			{
				ImGui::Text("chams");
				ImGui::Separator();

				ImGui::Spacing();

				ImGui::PushItemWidth(-1);
				ImGui::SliderInt("##chamsred", &g_Options.VIS_Red, 0, 255, "red: %.0f");
				ImGui::SliderInt("##chamsgreen", &g_Options.VIS_Green, 0, 255, "green: %.0f");
				ImGui::SliderInt("##chamsblue", &g_Options.VIS_Blue, 0, 255, "blue: %.0f");

				

				ImGui::MyColorEdit3("##chamsred", g_Options.CHAMS, 1 >> 21);

				ImGui::PopItemWidth();

				ImGui::Spacing();
			}
			ImGui::EndChild();

		}
		ImGui::End();
	}*/
	
}

void Menu::Kill()
{}

void Menu::PaintTraverse()
{
	if (m_isVisible) 
		MenuForm.Paint();
}

void Menu::Click()
{
	if (m_isVisible)
		MenuForm.Click();
}

void Menu::HandleInput(WPARAM vk)
{
	if (m_isVisible)
		MenuForm.HandleInput(vk);
}

void Menu::Toggle()
{
	m_isVisible = !m_isVisible;
}
