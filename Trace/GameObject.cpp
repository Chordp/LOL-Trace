#include "Trace.h"
#include "GameObject.hpp"


int GameObject::DecType()
{
	unsigned __int8* v2; // edi
	unsigned int v3; // edx
	unsigned int v4; // esi
	int* v5; // ecx
	int v6; // eax
	unsigned __int8 v7; // al
	unsigned int v8; // eax
	unsigned __int8* v9; // edx
	char v10; // cl
	int objectId; // [esp+8h] [ebp-4h]

	v2 = (unsigned __int8*)this;
	v3 = 0;
	v4 = *(BYTE*)(v2 + 81);

	objectId = *(DWORD*)&v2[4 * v2[88] + 92];
	if (v4)
	{
		v5 = (int*)(this + 84);
		do
		{
			v6 = *v5;
			++v5;
			*(&objectId + v3) ^= ~v6;
			++v3;
		} while (v3 < v4);
	}
	v7 = v2[82];
	if (v7)
	{
		v8 = 4 - v7;
		if (v8 < 4)
		{
			v9 = &v2[v8 + 84];
			do
			{
				v10 = *v9++;
				*((BYTE*)&objectId + v8++) ^= ~v10;
			} while (v8 < 4);
		}
	}

	return objectId;
}
ObjectType GameObject::GetType()
{
	if (this->IsHero())
		return ObjectType::Hero;
	if (this->IsMissile())
		return ObjectType::Missile;
	return (ObjectType)0;
}
//bool GameObject::GetHpBarPosition(Vector& out)
//{
//	static auto fnHpBarPos = reinterpret_cast<void(*)(PVOID, Vector*)>(Engine::GetBaseModule()+ Function::GetHpBarPos);
//	fnHpBarPos(this->GetUnitInfoComponent(), &out);
//	//Engine::PrintChats(Color::Red, "%p", this->GetUnitInfoComponent());
//	return false ;
//}
GameObject* GameObject::GetFirst() {
	static auto fnGetFirst = reinterpret_cast<GameObject * (__thiscall*)(ObjManager*)>(Engine::GetBaseModule() + Function::GetFirstObj);
	static auto ObjMana = Engine::GetObjManager();
	return fnGetFirst(ObjMana);
}
GameObject* GameObject::GetNext() {
	static auto fnGetNext = reinterpret_cast<GameObject * (__thiscall*)(ObjManager*, GameObject*)>(Engine::GetBaseModule() + Function::GetNextObj);
	static auto ObjMana = Engine::GetObjManager();
	return fnGetNext(ObjMana, this);
}
PVOID GameObject::GetUnitInfoComponent()
{
	auto a3 = *(DWORD*)((DWORD)(this) + 4 * *(unsigned __int8*)((DWORD)(this) + 0x4AB8) + 0x4ABC);
	auto v32 = *(DWORD*)((DWORD)(this) + 0x4AB4);
	a3 ^= ~v32;
	return PVOID(a3);
}
int GameObject::GetIndex()
{
	return *(int*)((DWORD)this + (int)Entity::Index);
}
char* GameObject::GetName()
{
	return GetStr((DWORD)this + (int)Entity::Name);
}
int GameObject::GetTeam()
{
	return *(int*)((DWORD)this + (int)Entity::Team);
}
int GameObject::GetNetworkId()
{
	return *(int*)((DWORD)this + (int)Entity::NetworkID);
}
bool GameObject::IsHero()
{
	//Engine::PrintChats(Color::White, "type %d", DecType());
	return (DecType() & (int)ObjectType::Hero) != 0;
}
bool GameObject::IsMissile()
{
	//static auto  fnIsMissile = reinterpret_cast<bool(__thiscall*)(GameObject*)>(Engine::GetBaseModule() + Function::IsMissile);
	return (DecType() & (int)ObjectType::Missile) != 0; //fnIsMissile(this);
}
float GameObject::GetAttackRange()
{
	return *reinterpret_cast<float*>(reinterpret_cast<DWORD_PTR>(this)+ (int)Entity::ObjAtkRange);
}

Vector GameObject::GetPos()
{
	return *(Vector*)((DWORD)this + (int)Entity::ObjPos);
}

float GameObject::GetBoundingRadius() {
	typedef float(__thiscall* OriginalFn)(PVOID);

	return CallVirtual<OriginalFn>(this, 36)(this);

}

float GameObject::GetHealth()
{
	return *(float*)((DWORD)this + (int)Entity::Health);
}

bool GameObject::IsAlive() {
	static auto  fnIsAlive = reinterpret_cast<bool(__thiscall*)(GameObject*)>(Engine::GetBaseModule() + Function::IsAlive);
	return fnIsAlive(this) && this->GetHealth() > 0.0f;
}
