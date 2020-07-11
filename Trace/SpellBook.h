#pragma once
#include "SpellSlot.h"
class SpellSlot;

enum SpellSlotID
{
    Q = 0,
    W = 1,
    E = 2,
    R = 3,
    Summoner1 = 4,
    Summoner2 = 5,
    Item1 = 6,
    Item2 = 7,
    Item3 = 8,
    Item4 = 9,
    Item5 = 10,
    Item6 = 11,
    Trinket = 12,
    Recall = 13
};

class SpellBook
{
public:
	SpellSlot* GetSpellSlotByID(int ID) {
		return *(SpellSlot**)((DWORD)this + 0x508 + (0x4 * ID));
	}

    std::vector<SpellSlot*> GetAllSpellSlot()
    {
        std::vector<SpellSlot*> hRet;
        for (size_t i = 0; i < 6; i++)
        {
            hRet.push_back(GetSpellSlotByID(i));
        }
        return hRet;
    }
};