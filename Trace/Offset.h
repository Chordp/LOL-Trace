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











//class LocalPlayer
//{
//public:
//	char pad_0000[32]; //0x0000
//	int16_t oObjIndex; //0x0020
//	char pad_0022[42]; //0x0022
//	int32_t oObjTeam; //0x004C
//	char pad_0050[28]; //0x0050
//	char* oObjName; //0x006C
//	char pad_0070[92]; //0x0070
//	int32_t oObjNetworkID; //0x00CC
//	char pad_00D0[228]; //0x00D0
//	int32_t GameTime; //0x01B4
//	char pad_01B8[32]; //0x01B8
//	Vector3 oObjPos; //0x01D8
//	char pad_01E4[440]; //0x01E4
//	bool oObjVisibility; //0x039C
//	char pad_039D[3051]; //0x039D
//	float oObjHealth; //0x0F88
//	char pad_0F8C[12]; //0x0F8C
//	float oObjMaxHealth; //0x0F98
//	char pad_0F9C[728]; //0x0F9C
//	float CDR; //0x1274
//	char pad_1278[12]; //0x1278
//	float MaxCDR; //0x1284
//	char pad_1288[64]; //0x1288
//	float BaseManaRegen; //0x12C8
//	char pad_12CC[12]; //0x12CC
//	float TotalManaRegen; //0x12D8
//	char pad_12DC[60]; //0x12DC
//	float Tenacity; //0x1318
//	char pad_131C[72]; //0x131C
//	float MagicPen; //0x1364
//	float IgnoreArmorPct; //0x1368
//	float IgnoreMRPct; //0x136C
//	char pad_1370[16]; //0x1370
//	float ArmorPen; //0x1380
//	char pad_1384[56]; //0x1384
//	float oObjBonusAtk; //0x13BC
//	char pad_13C0[12]; //0x13C0
//	float TotalAP; //0x13CC
//	char pad_13D0[68]; //0x13D0
//	float BonusAttackSpeed; //0x1414
//	char pad_1418[16]; //0x1418
//	float LifeSteal; //0x1428
//	char pad_142C[12]; //0x142C
//	float AttackSpeed; //0x1438
//	float oObjBaseAtk; //0x143C
//	float oObjBaseAtk2; //0x1440
//	char pad_1444[28]; //0x1444
//	float CritChance; //0x1460
//	float TotalArmor; //0x1464
//	float BonusArmor; //0x1468
//	float TotalMR; //0x146C
//	float BonusMR; //0x1470
//	float BaseHealthRegen; //0x1474
//	float TotalHealthRegen; //0x1478
//	float MovementSpeed; //0x147C
//	char pad_1480[4]; //0x1480
//	float oObjAtkRange; //0x1484
//	char pad_1488[1120]; //0x1488
//	float TotalAP; //0x18E8
//	char pad_18EC[364]; //0x18EC
//	float MovementSpeed2; //0x1A58
//	char pad_1A5C[620]; //0x1A5C
//	float CurrentGold; //0x1CC8
//	char pad_1CCC[12]; //0x1CCC
//	float TotalGold; //0x1CD8
//	char pad_1CDC[28]; //0x1CDC
//	float GoldMax; //0x1CF8
//	char pad_1CFC[3548]; //0x1CFC
//	class N000024FF* N00000B9F; //0x2AD8
//	char pad_2ADC[2736]; //0x2ADC
//	char oObjChampionName[16]; //0x358C
//	char pad_359C[6408]; //0x359C
//	int32_t oObjLevel; //0x4EA4
//}; //Size: 0x4EA8
namespace GameClass
{
	const auto GameTime = 0x034dcc14;
	const auto BuildVersion = 0x034ef230;

	const auto ObjManager = 0x01c41db0;
	const auto LocalPlayer = 0x034e4a70;
	const auto ChatClient = 0x1C3F608;

};
namespace ObjectStruct
{
	const auto Index = 0x0020;
	const auto Team = 0x004C;
	const auto Name = 0x006C;
	const auto NetworkID = 0x00CC;
	const auto ObjPos = 0x1D8;
	const auto Health = 0x0F88;
	const auto MaxHealth = 0x0F98;
	const auto ObjAtkRange = 0x1484;
	const auto SpellBook = 0x2AD8;
	const auto ChampionName = 0x358C;

	
}
namespace Function
{
	const auto WorldToScreen = 0x00947380;
	const auto SetBaseCharacterData = 0x001ac9f0;
	const auto GetFirstObj = 0x2b9a10;
	const auto GetNextObj = 0x2ba950;
	const auto PrintChat = 0x569B30;
	const auto IsAlive = 0x1b2120;
};
namespace AIMGR
{
	const auto  TARGETPOS = 0x10; //NOT VERIFIED
	const auto  ISMOVING = 0x198; //NOT VERIFIED
	const auto  ISDASHING = 0x1E8;//NOT VERIFIED
	const auto  NAVBEGIN = 0x1BC; //NOT VERIFIED
	const auto  NAVEND = 0x1C0; //NOT VERIFIED
	const auto  HASNAVPATH = 0x19C; //NOT VERIFIED
	const auto  CURRENTPOS = 0x2BC; //NOT VERIFIED
	const auto  VELOCITY = 0x2C0; //NOT VERIFIED
}