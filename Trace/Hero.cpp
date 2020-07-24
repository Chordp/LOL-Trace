#include "Hero.hpp"

string Hero::GetTitle()
{
	string name = GetChampionName();
	if (Config::GetIns()->Contrast.Champion.empty() || Config::GetIns()->Contrast.Champion.count(name) == 0)
		return name;
	return Config::GetIns()->Contrast.Champion[name].name;
}

char* Hero::GetChampionName() {
	return GetStr(reinterpret_cast<DWORD>(this) + (int)Entity::ChampionName);
}
void Hero::SetBaseCharacterData(char* dataName, int skinID, bool one)
{
	static auto fnSetBaseCharacterData = reinterpret_cast<void(__thiscall*)(GameObject*, char*, int, bool)>(Engine::GetBaseModule() + Function::SetBaseCharacterData);
	fnSetBaseCharacterData(this, dataName, skinID, one);
}
void Hero::SetCharacter(int SkinId)
{
	auto _this = reinterpret_cast<DWORD>(this);
	auto key = ~*reinterpret_cast<DWORD*>(_this + 0x10B0);
	*reinterpret_cast<DWORD*>(_this + *reinterpret_cast<BYTE*>(_this + 0x10B4) * 4 + 0x10B8) = key ^ SkinId;
	SetBaseCharacterData(GetChampionName(), SkinId, true);
}

SpellBook* Hero::GetSpellBook() {
	return (SpellBook*)((DWORD)this + (int)Entity::SpellBook);
}

AIManager* Hero::GetAIManager() {
	//typedef AIManager* (__thiscall* OriginalFn)(PVOID);
	//return CallVirtual<OriginalFn>(this, 150)(this);
	static auto fnGetAiManager = reinterpret_cast<AIManager * (__thiscall*)(PVOID)>(Engine::GetBaseModule() + Function::GetAiManager);
	return fnGetAiManager(this);
}

//bool Hero::GetHpBarPosition(Vector& out)
//{
//	static auto  BaseDrawPosition = reinterpret_cast<int(__thiscall*)(GameObject*, Vector*)> (Engine::GetBaseModule() + Function::BaseDrawPosition);
//
//	Vector baseDrawPosition;
//	BaseDrawPosition(this, &baseDrawPosition);
//
//	float delta = abs(baseDrawPosition.Y - this->GetPos().Y);
//	delta *= 5 / 6.0f;
//	if (!Engine::WorldToScreen(baseDrawPosition, out))
//		return false;
//
//	out.X += 20;
//	out.Y -= delta;
//	out.Z = 0;
//	return true;
//
//}
