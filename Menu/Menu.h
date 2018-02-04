#pragma once

#include "../Core/SDK.h"
#include "../Core/singleton.h"
#include "GUI.h"

class CCStrike15ItemSchema;

class CCStrike15ItemSystem;

template <typename Key, typename Value>

struct Node_t
{
	int previous_id;        //0x0000
	int next_id;            //0x0004
	void* _unknown_ptr;        //0x0008
	int _unknown;            //0x000C
	Key key;                //0x0010
	Value value;            //0x0014
};

template <typename Key, typename Value>

struct Head_t
{
	Node_t<Key, Value>* memory;        //0x0000
	int allocation_count;            //0x0004
	int grow_size;                    //0x0008
	int start_element;                //0x000C
	int next_available;                //0x0010
	int _unknown;                    //0x0014
	int last_element;                //0x0018
}; //Size=0x001C

struct String_t
{
	char* buffer;    //0x0000
	int capacity;    //0x0004
	int grow_size;    //0x0008
	int length;        //0x000C
}; //Size=0x0010

struct CPaintKit
{
	int id;                        //0x0000

	String_t name;                //0x0004
	String_t description;        //0x0014
	String_t item_name;            //0x0024
	String_t material_name;        //0x0034
	String_t image_inventory;    //0x0044

	char pad_0x0054[0x8C];        //0x0054
}; //Size=0x00E0

struct paint_kit {
	int id;
	std::string name;

	auto operator < (const paint_kit& other) const -> bool {
		return name < other.name;
	}
};

extern std::vector<paint_kit> k_skins;

void InitializePaintKits();

void initialize();

void Init();

class Menu : public Singleton<Menu>
{
	bool m_isVisible;
	bool m_DOisVisible;
	CForm MenuForm;
public:
	void Kill();

	void PaintTraverse();
	void Click();
	void HandleInput(WPARAM vk);

	void Toggle();

	bool IsVisible() const { return m_isVisible; }
};