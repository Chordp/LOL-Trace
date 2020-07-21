#pragma once
#include "GameObject.hpp"
#include "SpellBook.hpp"
#include "AIManager.h"
class SpellBook;
class GameObject;
enum class IssueOrderType {
	HoldPosition = 1,
	MoveTo,
	AttackUnit,
	AutoAttackPet,
	AutoAttack,
	MovePet,
	AttackTo,
	Stop = 10
};
class Hero :public GameObject
{
public:

	string GetTitle();
	/// <summary>
	/// 获取英雄名
	/// </summary>
	/// <returns></returns>
	char* GetChampionName();
	/// <summary>
	/// 设置皮肤数据
	/// </summary>
	/// <param name="dataName">英雄名</param>
	/// <param name="skinID">皮肤号</param>
	/// <param name="one">是否改变??</param>
	void SetBaseCharacterData(char* dataName, int skinID, bool one);
	/// <summary>
	/// 设置皮肤,并永久改变
	/// </summary>
	/// <param name="SkinId">皮肤号</param>
	void SetCharacter(int SkinId);
	/// <summary>
	/// 技能
	/// </summary>
	/// <returns></returns>
	SpellBook* GetSpellBook();
	
	AIManager* GetAIManager();

	//bool GetHpBarPosition(Vector& out);

};


class LocalPlayer :public Hero
{
public :
	int IssueOrder(IssueOrderType Order, Vector* Loc, GameObject* Target, bool IsAttackMove, bool IsMinion, DWORD Unknown);
	void MoveTo(Vector pos);
	void Attack(GameObject* target);
	float GetAttackDelay();
	float GetAttackCastDelay();

};