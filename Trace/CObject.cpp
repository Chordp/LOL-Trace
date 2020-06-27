#include "Trace.h"
#include "CObject.hpp"


bool CObject::CompareObjectTypeFlags(int objectTypeFlag)
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

	return (objectId & objectTypeFlag) != 0;
}
bool CObject::IsHero()
{
	return CompareObjectTypeFlags((int)ObjectTypeFlags::Hero);
}

float CObject::GetAttackRange()
{
	return *reinterpret_cast<float*>(reinterpret_cast<DWORD_PTR>(this)+ObjectStruct::ObjAtkRange);
}

Vector CObject::GetPos()
{
	return *(Vector*)((DWORD)this + ObjectStruct::ObjPos);
}

float CObject::GetBoundingRadius() {
	typedef float(__thiscall* OriginalFn)(PVOID);

	return CallVirtual<OriginalFn>(this, 36)(this);

}