#pragma once
#define NOMINMAX
#include <Windows.h>
#include "SDK.h"

#include "draw.h"
#include "Utilities.h"
#include "Font.h"
#include "SDK.h"
#include "math.h"
#include "../SDK/csgostructs.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <wchar.h>

#define M_PI 3.14159265358979323846f
namespace Draw
{
	inline void Line(int x0, int y0, int x1, int y1, Color col)
	{
		g_VGuiSurface->DrawSetColor(col);
		g_VGuiSurface->DrawLine(x0, y0, x1, y1);
	}

	inline void Line(Vector2D start_pos, Vector2D end_pos, Color col)
	{
		Line(start_pos.x, start_pos.y, end_pos.x, end_pos.y, col);
	}

	inline void DrawOutlinedRect(int x, int y, int w, int h, Color col)
	{
		g_VGuiSurface->DrawSetColor(col);
		g_VGuiSurface->DrawOutlinedRect(x, y, x + w, y + h);
	}

	inline void Outline(int x, int y, int w, int h, Color color)
	{
		g_VGuiSurface->DrawSetColor(color);
		g_VGuiSurface->DrawOutlinedRect(x, y, x + w, y + h);
	}

	inline void TexturedPolygon(int n, Vertex_t* vertice, Color col)
	{
		static int texture_id = g_VGuiSurface->CreateNewTextureID(true);
		static unsigned char buf[4] = { 255, 255, 255, 255 };
		g_VGuiSurface->DrawSetTextureRGBA(texture_id, buf, 1, 1);
		g_VGuiSurface->DrawSetColor(col);
		g_VGuiSurface->DrawSetTexture(texture_id);
		g_VGuiSurface->DrawTexturedPolygon(n, vertice);
	}

	inline void Circle(Vector2D position, float points, float radius, Color color)
	{
		float step = (float) M_PI * 2.0f / points;

		for (float a = 0; a < (M_PI * 2.0f); a += step)
		{
			Vector2D start(radius * cosf(a) + position.x, radius * sinf(a) + position.y);
			Vector2D end(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y);
			Line(start, end, color);
		}
	}

	inline void Circle(int x, int y, float points, float radius, Color color)
	{
		Circle(Vector2D(x, y), points, radius, color);
	}

	inline void FilledCircle(Vector2D position, float points, float radius, Color color)
	{
		std::vector<Vertex_t> vertices;
		float step = (float) M_PI * 2.0f / points;

		for (float a = 0; a < (M_PI * 2.0f); a += step)
			vertices.push_back(Vertex_t(Vector2D(radius * cosf(a) + position.x, radius * sinf(a) + position.y)));

		TexturedPolygon((int) points, vertices.data(), color);
	}

	inline void FilledCircle(int x, int y, float points, float radius, Color color)
	{
		FilledCircle(Vector2D(x, y), points, radius, color);
	}

	inline void Circle3D(Vector position, float points, float radius, Color color)
	{
		float step = (float) M_PI * 2.0f / points;

		std::vector<Vector> points3d;
		for (float a = 0; a < (M_PI * 2.0f); a += step)
		{
			Vector start(radius * cosf(a) + position.x, radius * sinf(a) + position.y, position.z);
			Vector end(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y, position.z);

			Vector start2d, end2d;
			if (g_DebugOverlay->ScreenPosition(start, start2d) || g_DebugOverlay->ScreenPosition(end, end2d))
				return;

			Line(Vector2D(start2d.x, start2d.y), Vector2D(end2d.x, end2d.y), color);
		}
	}

	inline void FilledRectangle(int x0, int y0, int x1, int y1, Color col)
	{
		g_VGuiSurface->DrawSetColor(col);
		g_VGuiSurface->DrawFilledRect(x0, y0, x1, y1);
	}

	inline void FilledRectangle(Vector2D start_pos, Vector2D end_pos, Color col)
	{
		FilledRectangle(start_pos.x, start_pos.y, end_pos.x, end_pos.y, col);
	}

	inline void Rectangle(int x0, int y0, int x1, int y1, Color col)
	{
		g_VGuiSurface->DrawSetColor(col);
		g_VGuiSurface->DrawOutlinedRect(x0, y0, x1, y1);
	}

	inline void Rectangle(Vector2D start_pos, Vector2D end_pos, Color col)
	{
		Rectangle(start_pos.x, start_pos.y, end_pos.x, end_pos.y, col);
	}

	inline void FilledGradientRectangle(int x0, int y0, int x1, int y1, Color top, Color bottom)
	{
		g_VGuiSurface->DrawSetColor(top);
		g_VGuiSurface->DrawFilledRectFade(x0, y0, x1, y1, 255, 255, false);

		g_VGuiSurface->DrawSetColor(bottom);
		g_VGuiSurface->DrawFilledRectFade(x0, y0, x1, y1, 0, 255, false);
	}

	inline void FilledGradientRectangle(Vector2D pos, Vector2D size, Color top, Color bottom)
	{
		FilledGradientRectangle(pos.x, pos.y, pos.x + size.x, pos.y + size.y, top, bottom);
	}

	inline void Bone(int start, int end, C_BasePlayer* target, Color col)
	{
		Vector startBonePos, endBonePos;
		Vector2D startDrawPos, endDrawPos;

		if (!target)
			return;

		startBonePos = target->GetBonePos(start);
		endBonePos = target->GetBonePos(end);

		if (!Math::WorldToScreen(startBonePos, startDrawPos))
			return;

		if (!Math::WorldToScreen(endBonePos, endDrawPos))
			return;

		g_VGuiSurface->DrawSetColor(col);
		g_VGuiSurface->DrawLine(startDrawPos.x, startDrawPos.y, endDrawPos.x, endDrawPos.y);

	}

	inline void PolyLine(int* px, int* py, int num_points, Color col)
	{
		g_VGuiSurface->DrawSetColor(col);
		g_VGuiSurface->DrawPolyLine(px, py, num_points);
	}

	inline void PolyLine(Vertex_t* vertice, int num_points, Color col)
	{
		static int* points_x = new int[num_points];
		static int* points_y = new int[num_points];
		for (int i = 0; i < num_points; i++)
		{
			points_x[i] = static_cast<int>(vertice[i].m_Position.x);
			points_y[i] = static_cast<int>(vertice[i].m_Position.y);
		}
		PolyLine(points_x, points_y, num_points, col);
	}

	inline void Text(int x, int y, const wchar_t* text, vgui::HFont font, Color color, bool centeredx, bool centeredy)
	{
		if (centeredx || centeredy)
		{
			int wide, tall;
			g_VGuiSurface->GetTextSize(font, text, wide, tall);
			if (centeredx)
				x -= wide / 2;
			if (centeredy)
				y -= tall / 2;
		}

		g_VGuiSurface->DrawSetTextPos(x, y);
		g_VGuiSurface->DrawSetTextFont(font);
		g_VGuiSurface->DrawSetTextColor(color);
		g_VGuiSurface->DrawPrintText(text, wcslen(text));
	}

	inline void Text(int x, int y, const wchar_t* text, vgui::HFont font, Color color, bool centered = false)
	{
		Text(x, y, text, font, color, centered, centered);
	}

	inline Vector2D GetTextSize(const wchar_t* text, vgui::HFont font)
	{
		int x_res, y_res;
		g_VGuiSurface->GetTextSize(font, text, x_res, y_res);
		return Vector2D(x_res, y_res);
	}
}