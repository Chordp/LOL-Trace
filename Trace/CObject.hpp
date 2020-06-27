#pragma once
#include "Trace.h"
enum class ObjectTypeFlags {
	GameObject = (1 << 0),  //0x1
	NeutralCamp = (1 << 1),  //0x2
	DeadObject = (1 << 4),  //0x10
	InvalidObject = (1 << 5),  //0x20
	AIBaseCommon = (1 << 7),  //0x80
	AttackableUnit = (1 << 9),  //0x200
	AI = (1 << 10), //0x400
	Minion = (1 << 11), //0x800
	Hero = (1 << 12), //0x1000
	Turret = (1 << 13), //0x2000
	Unknown0 = (1 << 14), //0x4000
	Missile = (1 << 15), //0x8000
	Unknown1 = (1 << 16), //0x10000
	Building = (1 << 17), //0x20000
	Unknown2 = (1 << 18), //0x40000
};
class CObject
{
public:
	bool CompareObjectTypeFlags(int objectTypeFlag);
	/// <summary>
	/// 你是英雄吗?
	/// </summary>
	/// <returns></returns>
	bool IsHero();
	/// <summary>
	/// 获取攻击范围
	/// </summary>
	/// <returns></returns>
	float GetAttackRange();
	/// <summary>
	/// 获取对象在世界的位置
	/// </summary>
	/// <returns></returns>
	Vector GetPos();

	float GetBoundingRadius();
};