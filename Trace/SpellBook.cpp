
#include "SpellBook.hpp"
#include "Engine.hpp"
SpellSlot* SpellBook::GetSpellSlotByID(SpellSlotID ID) {
	return *(SpellSlot**)((DWORD)this + 0x508 + (0x4 * (UINT)ID));
}

SpellEntry* SpellBook::GetActiveSpell() {
    return *(SpellEntry**)((DWORD)this + 0x10);
}

SpellState SpellBook::GetSpellState(SpellSlotID ID)
{
    static auto fnGetSpellState = reinterpret_cast<SpellState(__thiscall*)(void*, const SpellSlotID, const DWORD&)>
        (Engine::GetBaseModule() + Function::GetSpellState);
    return fnGetSpellState(this, ID, NULL);
       
        
}
