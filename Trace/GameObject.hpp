#pragma once
#include "Trace.h"
enum class ObjectType {
	CObject = (1 << 0),  //0x1
	NeutralCamp = (1 << 1),  //0x2
	DeadObject = (1 << 4),  //0x10
	InvalidObject = (1 << 5),  //0x20
	AIBaseCommon = (1 << 7),  //0x80
	AttackableUnit = (1 << 9),  //0x200
	AI = (1 << 10), //0x400
	Minion = (1 << 11), //0x800
	Troy = 0x801,
	Dragon = 0x802,
	Baron = 0x803,
	Hero = (1 << 12), //0x1000
	Turret = (1 << 13), //0x2000
	//Unknown0 = (1 << 14), //0x4000
	Missile = (1 << 15), //0x8000
	//Unknown1 = (1 << 16), //0x10000
	Building = (1 << 17), //0x20000/
	Nexus = 0x20001,
	//Unknown2 = (1 << 18), //0x40000
};
class GameObject
{
public:
	
	/// <summary>
	/// ��ȡid
	/// </summary>
	/// <returns></returns>
	int GetIndex();
	/// <summary>
	/// ��ȡ������
	/// </summary>
	/// <returns></returns>
	char* GetName();
	/// <summary>
	/// ��ȡ�ŶӺ�
	/// </summary>
	/// <returns></returns>
	int GetTeam();
	/// <summary>
	/// ��ȡ����id
	/// </summary>
	/// <returns></returns>
	int GetNetworkId();
	/// <summary>
	/// ����Ӣ����?
	/// </summary>
	/// <returns></returns>
	bool IsHero();
	bool IsMissile();
	/// <summary>
	/// ��ȡ������Χ
	/// </summary>
	/// <returns></returns>
	float GetAttackRange();
	/// <summary>
	/// ��ȡ�����������λ��
	/// </summary>
	/// <returns></returns>
	Vector GetPos();
	/// <summary>
	/// ��ȡ��Χ
	/// </summary>
	/// <returns></returns>
	float GetBoundingRadius();
	/// <summary>
	/// ��ȡѪ��
	/// </summary>
	/// <returns></returns>
	float GetHealth();
	/// <summary>
	/// �Ƿ���
	/// </summary>
	/// <returns></returns>
	bool IsAlive();

	int GetType();
};