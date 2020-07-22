#include "SpellData.h"

char* SpellData::GetMissileName()
{
	return (char*) ((DWORD)this + (int)SpellDataClient::missileName);
}

char* SpellData::GetSpellName()
{
	return (char*)((DWORD)this + (int)SpellDataClient::spellName);
}

float SpellData::GetMissileSpeed()
{
	return *(float*)((DWORD)this + (int)SpellDataClient::missileSpeed);
}
