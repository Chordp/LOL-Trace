#pragma once
#include "Engine.hpp"
#include "SpellInfo.h"
class SpellSlot
{
public:

	int GetLevel();
	float GetTime();
	bool IsReady();
	float GetCooldown();
	float GetCD();
	SpellInfo* GetSpellInfo();
};

