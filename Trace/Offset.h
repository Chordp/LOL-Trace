#pragma once

//#define oGameVersion  0x034ff0b0
//#define oNetClient  0x034f5bb4
//#define oChatClientPtr  0x0289e8e8
//#define oObjManager  0x01c51bf4
//#define oZoomClass  0x034eca70
//#define oGameTime  0x034ecaec
//#define oGameInfo   0x034edba0
//#define oLocalPlayer  0x034f489c
//#define oHudInstance  0x01c51c30
//#define oRenderer  0x0351507c
//#define oUnderMouseObject  0x0289e898
//#define oD3DRenderer  0x03517bf4
//FUNCTIONS
//#define oCastSpell  0x-0400001
//#define oDrawCircle  0x005192c0
//#define oGetBasicAttack	 0x00174970
//#define oGetAttackCastDelay  0x002b2500
//#define oGetAttackDelay  0x002b2600
//#define oGetPing  0x-0400001
//#define oGetSpellState  0x004fd740
//#define oIsTargetable  0x001ec150
//#define oIsAlive  0x001b1a60
//#define oIsBaron   0x-0400001
//#define oIsTurret  0x001c4080
//#define oIsInhib  0x001c3d10
//#define oIsTroy 0x001c4170
//#define oIsHero  0x-0400001
//#define oIsMinion  0x-0400001
//#define oIsDragon  0x-0400001
//#define oIsMissile  0x001c3ef0
//#define oIsNexus  0x001c3e10
//#define oIsNotWall  0x008dd580
//#define oIssueOrder  0x00186310
//#define oPrintChat  0x-0400001
//#define oWorldToScreen  0x0095a650
//#define oSetBaseCharacterData  0x001ac2f

namespace GameInfo
{
	const auto GameTime = 0x01c51bf4;
	const auto BuildVersion = 0x034ff0b0;
};
namespace ObjectManager
{
	const auto ObjManager = 0x01c51bf4;
	const auto LocalPlayer = 0x034f489c;
	
};
namespace ObjectStruct
{
	const auto ChampionName = 0x3594;
}
namespace Function
{
	const auto WorldToScreen = 0x0095a650;
	const auto SetBaseCharacterData = 0x001ac2f0;
};