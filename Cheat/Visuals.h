#pragma once
#include "../Core/stdafx.h"
#include "../SDK/csgostructs.h"
#include "../Core/singleton.h"

RECT getPlayerBox(C_BaseEntity* pEntity);

class Visuals : public Singleton<Visuals>
{
	struct ESPBox
	{
		int x, y, w, h, gay;
	};
	void Overlay();
	void SniperCrosshair();
	void ESP_Box(int x, int y, int w, int h, C_BasePlayer * pEntity, Color clr, Color clr1);
	void ESP_Name(int x, int y, int w, int h, C_BasePlayer* pEntity);
	void DrawHealth(Vector2D bot, Vector2D top, float health);
	void ESP_Bone(C_BasePlayer * pEntity, Color clr);
	void NightMode();
	void ESP_Snapline(C_BasePlayer * pEntity, Color clr);
	void ESP_Weapon(int x, int y, int w, int h, C_BasePlayer* pEntity);
	void DrawHealth(C_BasePlayer * entity, Visuals::ESPBox size);
	void Radar();
public:
	void PaintTraverse();
	void DrawModelExecute(IMatRenderContext * ctx, const DrawModelState_t &state, const ModelRenderInfo_t &info, matrix3x4_t *matrix);
};
