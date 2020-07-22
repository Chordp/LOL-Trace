#include "SpellInfo.h"

char* SpellInfo::GetSpellName()
{
	return GetStr(((DWORD)(this + 0x18)));
}

SpellData* SpellInfo::GetSpellData()
{
	return *(SpellData**)((DWORD)this + (int)SpellInfoClient::SpellData);
}
