#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "../Core/SDK.h"
#include "../Core/Draw.h"
#include "../Core/Font.h"
#include "../Core/InputSys.h"
#include "../Core/math.h"

// Menu styling
// Layout
#define MENU_PADDING 2
// Colors
#define MENU_COLOR_CONTROL Color(80, 80, 80)
#define MENU_COLOR_CONTROL_BORDER Color(0, 0, 0)
#define MENU_COLOR_CONTROL_HOVER Color(144, 144, 144)
#define MENU_COLOR_CONTROL_ACTIVE Color(195, 195, 195)
#define MENU_COLOR_CONTROL_ACTIVE_HOVER Color(230, 230, 230)

class CObject
{
protected:
	int m_x;
	int m_y;
	int m_width;
	int m_height;
	int m_layer;
	bool m_hover;
public:
	CObject(int x = 0, int y = 0, int width = 0, int height = 0, int m_layer = 0);
	virtual void Paint(int = 0, int = 0, float* = nullptr) = 0;
	virtual void Click() {};
	virtual void HandleInput(WPARAM vk) {};

	bool operator < (const CObject& obj) const
	{
		return (m_layer < obj.m_layer);
	}
};

class CObjectContainer
{
protected:
	std::vector<CObject*> Objects;
public:
	void AddControl(CObject* obj);
};

class CWindow : public CObject, public CObjectContainer
{
	bool* m_open;
	float m_controlheight;
	bool m_dragging;
	int m_dragoffsetx, m_dragoffsety;
public:
	CWindow(int x = 0, int y = 0, int width = 0, int height = 0, bool* open = nullptr);
	void Paint(int t_x = 0, int t_y = 0, float* controlheight = nullptr);
	void Click();
	void HandleInput(WPARAM vk);
};

class CForm
{
	std::vector<CWindow> Windows;
public:
	void Paint();
	void Click();
	void AddWindow(CWindow window) { Windows.push_back(window); };
	void HandleInput(WPARAM vk);
};

class CChild: public CObject, public CObjectContainer
{
	float m_controlheight;
	std::wstring m_title;
public:
	CChild(int x, int y, int width, std::wstring title);
	void Paint(int t_x = 0, int t_y = 0, float* controlheight = nullptr);
	void Click();
	void HandleInput(WPARAM vk);
};

class CSpacing : public CObject
{
	int m_height;
public:
	CSpacing(int height);
	void Paint(int t_x = 0, int t_y = 0, float* controlheight = nullptr);
};

class CLabel : public CObject
{
	std::wstring m_text;
	vgui::HFont m_font;
	Color m_color;
	bool m_centered;
public:
	CLabel(std::wstring text = L"", vgui::HFont font = 0, Color color = Color::Error, bool centered = false);
	void Paint(int t_x = 0, int t_y = 0, float* controlheight = nullptr);
};

class CCheckBox : public CObject
{
	std::wstring m_label;
	bool* m_val;
public:
	CCheckBox(std::wstring label = L"", bool* value = nullptr);
	void Paint(int t_x = 0, int t_y = 0, float* controlheight = nullptr);
	void Click();
};

template<typename T> 
class CSlider : public CObject
{
	std::wstring m_label;
	T m_min, m_max;
	T* m_val;
public:
	CSlider(std::wstring label, T min, T max, T* value);
	void Paint(int t_x, int t_y, float* controlheight);
};

class CButton : public CObject
{
	std::wstring m_label;
	std::function<void()> m_cb;
public:
	CButton(int width = 0, int height = 0, std::wstring label = L"", std::function<void()> cb = {});
	void Paint(int t_x = 0, int t_y = 0, float* controlheight = nullptr);
	void Click();
};

class CTextBox : public CObject
{
	std::wstring m_placeholder;
	std::wstring* m_val;
	bool m_selected;
	Vector2D m_tl, m_br;
public:
	CTextBox(int width = 0, int height = 0, std::wstring placeholder = L"", std::wstring* value = nullptr);
	void Paint(int t_x = 0, int t_y = 0, float* controlheight = nullptr);
	void Click();
	void HandleInput(WPARAM vk);
};

class CCombo : public CObject
{
	int* m_val;
	std::vector<std::wstring> m_labels;
	bool m_lhover;
	bool m_rhover;
public:
	CCombo(int* value = nullptr, std::vector<std::wstring> labels = std::vector<std::wstring>());
	void Paint(int t_x = 0, int t_y = 0, float* controlheight = nullptr);
	void Click();
};