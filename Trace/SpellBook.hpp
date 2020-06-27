#pragma once
#include "SpellSlot.h"
class SpellSlot;
class SpellBook
{
public:
	SpellSlot* GetSpellSlotByID(int ID) {
		return *(SpellSlot**)((DWORD)this + 0x500 + (0x4 * ID));
	}
};