#include "Hero.hpp"

string Hero::GetTitle()
{
	if (Config::GetIns()->Contrast.empty())
		return GetChampionName();
	return Config::GetIns()->Contrast[GetChampionName()].name;
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
	auto key = ~*reinterpret_cast<DWORD*>(_this + 0x1090);
	*reinterpret_cast<DWORD*>(_this + *reinterpret_cast<BYTE*>(_this + 0x1094) * 4 + 0x1098) = key ^ SkinId;
	SetBaseCharacterData(GetChampionName(), SkinId, true);
}

SpellBook* Hero::GetSpellBook() {
	return (SpellBook*)((DWORD)this + (int)Entity::SpellBook);
}

AIManager* Hero::GetAIManager() {
	typedef AIManager* (__thiscall* OriginalFn)(PVOID);
	return CallVirtual<OriginalFn>(this, 150)(this);
}

bool Hero::GetHpBarPosition(Vector& out)
{
	static auto  BaseDrawPosition = reinterpret_cast<int(__thiscall*)(GameObject*, Vector*)> (Engine::GetBaseModule() + Function::BaseDrawPosition);

	Vector baseDrawPosition;
	BaseDrawPosition(this, &baseDrawPosition);

	float delta = abs(baseDrawPosition.Y - this->GetPos().Y);
	delta *= 5 / 6.0f;
	if (!Engine::WorldToScreen(baseDrawPosition, out))
		return false;

	out.X += 20;
	out.Y -= delta;
	out.Z = 0;
	return true;

}
