#include "Hero.hpp"


char* Hero::GetChampionName() {
	return GetStr(reinterpret_cast<DWORD>(this) + ObjectStruct::ChampionName);
}
void Hero::SetBaseCharacterData(char* dataName, int skinID, bool one)
{
	static auto fnSetBaseCharacterData = reinterpret_cast<void(__thiscall*)(CObject*, char*, int, bool)>(Engine::GetBaseModule() + Function::SetBaseCharacterData);
	fnSetBaseCharacterData(this, dataName, skinID, one);
}
void Hero::SetCharacter(int SkinId)
{
	auto _this = reinterpret_cast<DWORD>(this);
	auto key = ~*reinterpret_cast<DWORD*>(_this + 0x1090);
	*reinterpret_cast<DWORD*>(_this + *reinterpret_cast<BYTE*>(_this + 0x1094) * 4 + 0x1098) = key ^ SkinId;
	SetBaseCharacterData(GetChampionName(), SkinId, true);
}