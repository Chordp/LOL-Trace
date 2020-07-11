#pragma once
#include "GameObject.h"
#include "SpellBook.h"
#include "AIManager.h"
class GameObject;
class SpellBook;

class Hero :public GameObject
{
public:

	string GetTitle();
	/// <summary>
	/// ��ȡӢ����
	/// </summary>
	/// <returns></returns>
	char* GetChampionName();
	/// <summary>
	/// ����Ƥ������
	/// </summary>
	/// <param name="dataName">Ӣ����</param>
	/// <param name="skinID">Ƥ����</param>
	/// <param name="one">�Ƿ�ı�??</param>
	void SetBaseCharacterData(char* dataName, int skinID, bool one);
	/// <summary>
	/// ����Ƥ��,�����øı�
	/// </summary>
	/// <param name="SkinId">Ƥ����</param>
	void SetCharacter(int SkinId);
	/// <summary>
	/// ����
	/// </summary>
	/// <returns></returns>
	SpellBook* GetSpellBook();
	
	AIManager* GetAIManager();

	bool GetHpBarPosition(Vector& out);
	/// <summary>
	/// ��ȡ�����ӳ�
	/// </summary>
	/// <returns></returns>
	float GetAttackDelay();

	float GetAttackCastDelay();

};

//class LocalPlayer : public Hero
//{
//public:
//	int IssueOrder(int Order, Vector* Loc, GameObject* Target, bool IsAttackMove, bool IsMinion, DWORD Unknown);
//	void MoveTo(Vector pos);
//	void Attack(GameObject* target, bool isMinion);
//};
