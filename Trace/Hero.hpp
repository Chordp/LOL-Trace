#pragma once
#include "CObject.hpp"
#include "SpellBook.hpp"
class SpellBook;
class CObject;
class Hero :public CObject
{
public:
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

	SpellBook* GetSpellBook();
};