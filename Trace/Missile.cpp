#include "Missile.h"

Vector Missile::GetSpellStartPos()
{
	return *(Vector*)((DWORD)this + (int)MissileClient::StartPos);
}

Vector Missile::GetSpellEndPos()
{
	return *(Vector*)((DWORD)this + (int)MissileClient::EndPos);
}

float Missile::GetSpellWidth()
{
	return *(float*)((DWORD)this + (int)MissileClient::SpellWidth);
}

bool Missile::IsBasicAttack()
{
	return *(bool*)((DWORD)this + (int)MissileClient::IsBasicAttack);
}

bool Missile::IsAutoAttack()
{
	return *(bool*)((DWORD)this + (int)MissileClient::IsAutoAttack);
}
