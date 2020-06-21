#pragma once
#include "CObject.hpp"
class Hero :public CObject
{
public:
	char* GetChampionName() {
		return GetStr(reinterpret_cast<DWORD>(this) + 100);
	}
	void SetBaseCharacterData(char* dataName, int skinID, bool one)
	{
		static auto fnSetBaseCharacterData = reinterpret_cast<void(__thiscall*)(CObject*, char*, int, bool)>(Engine::GetBaseModule());
		fnSetBaseCharacterData(this, dataName, skinID, one);
	}
	void SetCharacter(int SkinId)
	{
		SetBaseCharacterData(GetChampionName(), SkinId, true);
	}
};