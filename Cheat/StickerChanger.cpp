#include "Skinchanger.h"

/**
 * Creds to namazso for blatant paste :))))
 */


enum class EStickerAttributeType
{
	Index,
	Wear,
	Scale,
	Rotation
};

static uint16_t s_econ_item_interface_wrapper_offset = 0;

namespace StickerHooks
{
	static void* oGetStickerAttributeBySlotIndexFloat;

	static float __fastcall GetStickerAttributeBySlotIndexFloat(void* thisptr, void*, int slot, EStickerAttributeType attribute, float unknown)
	{
		auto pItem = reinterpret_cast<C_BaseAttributableItem*>(uintptr_t(thisptr) - s_econ_item_interface_wrapper_offset);

		auto nWeaponIndex = pItem->m_iItemDefinitionIndex();

		if (Skinchanger::Get().WeaponCfg.find(nWeaponIndex) != Skinchanger::Get().WeaponCfg.end())
		{
			switch (attribute)
			{
				case EStickerAttributeType::Wear:
					return Skinchanger::Get().WeaponCfg[nWeaponIndex].stickers[slot].flWear;
				case EStickerAttributeType::Scale:
					return Skinchanger::Get().WeaponCfg[nWeaponIndex].stickers[slot].flScale;
				case EStickerAttributeType::Rotation:
					return Skinchanger::Get().WeaponCfg[nWeaponIndex].stickers[slot].flRotation;
				default:
					break;
			}
		}

		return reinterpret_cast<decltype(GetStickerAttributeBySlotIndexFloat)*>(oGetStickerAttributeBySlotIndexFloat)(thisptr, nullptr, slot, attribute, unknown);
	}

	static void* oGetStickerAttributeBySlotIndexInt;

	static unsigned int __fastcall GetStickerAttributeBySlotIndexInt(void* thisptr, void*, int slot, EStickerAttributeType attribute, unsigned unknown)
	{
		auto pItem = reinterpret_cast<C_BaseAttributableItem*>(uintptr_t(thisptr) - s_econ_item_interface_wrapper_offset);

		if (attribute == EStickerAttributeType::Index)
		{
			auto nWeaponIndex = pItem->m_iItemDefinitionIndex();

			if(Skinchanger::Get().WeaponCfg.find(nWeaponIndex) != Skinchanger::Get().WeaponCfg.end())
				return Skinchanger::Get().WeaponCfg[nWeaponIndex].stickers[slot].nIndex;
		}

		return reinterpret_cast<decltype(GetStickerAttributeBySlotIndexInt)*>(oGetStickerAttributeBySlotIndexInt)(thisptr, nullptr, slot, attribute, unknown);
	}
}

void Skinchanger::ApplyStickerHooks(C_BaseAttributableItem* item)
{
	if (!s_econ_item_interface_wrapper_offset)
		s_econ_item_interface_wrapper_offset = NetvarSys::Get().GetOffset("DT_EconEntity", "m_Item") + 0xC;

	void**& vmt = *reinterpret_cast<void***>(uintptr_t(item) + s_econ_item_interface_wrapper_offset);

	static void** hooked_vmt = nullptr;

	if (!hooked_vmt)
	{
		size_t size = 0;

		while (Utilities::IsCodePtr(vmt[size]))
			++size;

		hooked_vmt = new void*[size];
		memcpy(hooked_vmt, vmt, size * sizeof(void*));

		StickerHooks::oGetStickerAttributeBySlotIndexFloat = hooked_vmt[4];
		hooked_vmt[4] = reinterpret_cast<void*>(&StickerHooks::GetStickerAttributeBySlotIndexFloat);

		StickerHooks::oGetStickerAttributeBySlotIndexInt = hooked_vmt[5];
		hooked_vmt[5] = reinterpret_cast<void*>(&StickerHooks::GetStickerAttributeBySlotIndexInt);
	}

	vmt = hooked_vmt;
}
