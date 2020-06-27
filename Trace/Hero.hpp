#pragma once
#include "CObject.hpp"
#include "SpellBook.hpp"
class SpellBook;
class CObject;
class Hero :public CObject
{
public:
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

	SpellBook* GetSpellBook();
};