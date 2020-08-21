#pragma once
#include "SpellSlot.h"
#include "SpellEntry.h"
class SpellSlot;

enum class SpellSlotID
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
enum class SpellState
{
    //Possible flags

    Ready = 0,
    NotAvailable = 4,
    Surpressed = 8,
    NotLearned = 12,
    Cooldown = 32,
    NoMana = 64,
    Unknown
};

class SpellBook
{
public:
    SpellEntry* GetActiveSpell();
    SpellSlot* GetSpellSlotByID(SpellSlotID ID);

    SpellState GetSpellState(SpellSlotID ID);

};