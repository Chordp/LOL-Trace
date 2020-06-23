#pragma once
#include "CObject.hpp"
class Hero :public CObject
{
public:

	char* GetChampionName();
	void SetBaseCharacterData(char* dataName, int skinID, bool one);
	void SetCharacter(int SkinId);
};