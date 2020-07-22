#include "SpellSlot.h"

 int SpellSlot::GetLevel() {
	return *(int*)((DWORD)this + 0x20);
}

 float SpellSlot::GetTime() {
	return *(float*)((DWORD)this + 0x28);
}

 bool SpellSlot::IsReady()
{
	return this->GetLevel() > 0 && this->GetCooldown() <= 0;
}

 float SpellSlot::GetCooldown()
{
	auto cd = this->GetTime() - Engine::GetGameTime();
	if (cd <= 0.f)
		cd = 0.f;
	return cd;
}

float SpellSlot::GetCD() {
	return *(float*)((DWORD)this + 0x78);
}

SpellInfo* SpellSlot::GetSpellInfo()
{
	return *(SpellInfo**)((DWORD)this + 0x134);
}
