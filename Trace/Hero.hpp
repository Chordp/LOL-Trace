#pragma once
#include "GameObject.hpp"
#include "SpellBook.hpp"
#include "AIManager.h"
class SpellBook;
class GameObject;

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

	//bool GetHpBarPosition(Vector& out);

};