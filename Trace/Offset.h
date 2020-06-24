#pragma once

//[*] League of Legends Client Update Tool
//[*] By Chord
//[*] Started at: 2020-06-24 14:31:49.655000
//[*] 
//----------------------------
//#define oGameVersion  0x034ef230
//#define oNetClient  0x034e5d2c
//#define oChatClientPtr  0x0288ea10
//#define oObjManager  0x01c41db0
//#define oZoomClass  0x034dcb9c
//#define oGameTime  0x034dcc14
//#define oGameInfo   0x034ddcc0
//#define oLocalPlayer  0x034e4a70
//#define oHudInstance  0x01c41dec
//#define oRenderer  0x035058d4
//#define oUnderMouseObject  0x0288e9c0
//#define oD3DRenderer  0x035085a4
//FUNCTIONS
//#define oCastSpell  0x-0400001
//#define oDrawCircle  0x004ff9f0
//#define oGetBasicAttack	 0x00175ce0
//#define oGetAttackCastDelay  0x002b4150
//#define oGetAttackDelay  0x002b4250
//#define oGetPing  0x-0400001
//#define oGetSpellState  0x004fcc90
//#define oIsTargetable  0x001ecab0
//#define oIsAlive  0x001b2120
//#define oIsBaron   0x-0400001
//#define oIsTurret  0x001c46f0
//#define oIsInhib  0x001c4380
//#define oIsTroy 0x-0400001
//#define oIsHero  0x-0400001
//#define oIsMinion  0x-0400001
//#define oIsDragon  0x002e9470
//#define oIsMissile  0x-0400001
//#define oIsNexus  0x001c4480
//#define oIsNotWall  0x008ca120
//#define oIssueOrder  0x00187480
//#define oPrintChat  0x-0400001
//#define oWorldToScreen  0x00947380
//#define oSetBaseCharacterData  0x001ac9f0
namespace GameInfo
{
	const auto GameTime = 0x034dcc14;
	const auto BuildVersion = 0x034ef230;
};
namespace ObjectManager
{
	const auto ObjManager = 0x01c41db0;
	const auto LocalPlayer = 0x034e4a70;
	
};
namespace ObjectStruct
{
	const auto ChampionName = 0x358C;
}
namespace Function
{
	const auto WorldToScreen = 0x00947380;
	const auto SetBaseCharacterData = 0x001ac9f0;
};