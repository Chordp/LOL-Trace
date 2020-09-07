#pragma once

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
enum class Entity
{
	Index = 0x0020,
	Team = 0x004C,
	Name = 0x006C,
	NetworkID = 0x00CC,
	ObjPos = 0x1D8,
	Visibility = 0x48C,
	Health = 0xDC4,
	MaxHealth = 0xDD4,
	Armor = 0x12AC,
	BonusAtk = 0x13EC,
	BaseAtk = 0x146C,
	ObjAtkRange = 0x12CC,
	SpellBook = 0x2708,
	ChampionName = 0x312C
};
enum class SpellDataClient
{
	missileName = 0x58,
	spellName = 0x7c,
	missileSpeed = 0x418
};
enum class SpellInfoClient
{
	StartPos = 0x78,//10.1
	EndPos =  0x84, //10.1
	SpellData = 0x44,
	SpellWidth = 0x528

};
enum class MissileClient
{
	SpellInfo = 0x230,
	StartPos = SpellInfo + 0x78,//10.1
	EndPos = SpellInfo + 0x84, //10.1
	IsAutoAttack = SpellInfo + 0x04D9,
	IsBasicAttack = SpellInfo + 0x4D8,
	SpellWidth = SpellInfo + 0x528

};

enum class AI_Manager
{
	TARGETPOS = 0x10, //NOT VERIFIED
	ISMOVING = 0x198, //NOT VERIFIED
	ISDASHING = 0x1E8,//NOT VERIFIED
	NAVBEGIN = 0x1BC, //NOT VERIFIED
	NAVEND = 0x1C0, //NOT VERIFIED
	HASNAVPATH = 0x19C, //NOT VERIFIED
	CURRENTPOS = 0x2BC, //NOT VERIFIED
	VELOCITY = 0x2C0 //NOT VERIFIED
};


//[*] League of Legends Client Update Tool
//[*] By Chord
//[*] Started at: 2020-09-06 20:58:51.014000
//----------------------------
namespace GameClass
{
	const auto GameTime = 0x34D9C1C;	//0x38D9C1C
	const auto ObjManager = 0x1C3EE3C;	//0x203EE3C
	const auto LocalPlayer = 0x34E1A34;	//0x38E1A34
	const auto ChatClient = 0x1C3C6E8;	//0x203C6E8
	const auto MenuGuiIns = 0x34E1A00;	//0x38E1A00
	const auto HudInstance = 0x34DAB64;	//0x38DAB64
	const auto PingInstance = 0x34E8BE4;	//0x38E8BE4
};


namespace Function
{
	const auto GetPing = 0x29C660;	//0x69C660
	const auto IsAlive = 0x1A51C0;	//0x5A51C0
	const auto IsMissile = 0x1B3AB0;	//0x5B3AB0
	const auto IsTargetable = 0x4F44F0;	//0x8F44F0
	const auto IssueOrder = 0x17CD30;	//0x57CD30
	const auto GetFirstObj = 0x27B620;	//0x67B620
	const auto GetNextObj = 0x27D190;	//0x67D190
	const auto SendChat = 0x5E8650;	//0x9E8650
	const auto PrintChat = 0x5722A0;	//0x9722A0
	const auto GetHpBarPos = 0x593A30;	//0x993A30
	const auto GetAiManager = 0x1322C0;	//0x5322C0
	const auto WorldToScreen = 0x950E10;	//0xD50E10
	const auto BaseDrawPosition = 0x1731A0;	//0x5731A0
	const auto GetAttackDelay = 0x29C660;	//0x69C660
	const auto GetAttackCastDelay = 0x29C560;	//0x69C560
	const auto SetBaseCharacterData = 0x1425E0;	//0x5425E0
	const auto OnCreateObject = 0x286070;	//0x686070
	const auto OnDeleteObject = 0x11A9AA3;	//0x15A9AA3
	const auto OnProcessSpell = 0x504CF0;	//0x904CF0
};

