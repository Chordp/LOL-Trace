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
	ObjPos = 0x220,
	Visibility = 0x270,
	Health = 0xDC4,
	MaxHealth = Health+0x10,
	Armor = 0x12B0,
	BonusAtk = 0x1208,
	BaseAtk = 0x1288,
	ObjAtkRange = 0x12D0,
	SpellBook = 0x2720,
	MoveSpeed = 0x12C8,
	ChampionName = 0x314C
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
//[*] Started at: 2020-11-12 15:30:27.052000
//----------------------------
namespace GameClass
{
	const auto GameTime = 0x350B318;	//0x390B318
	const auto ObjManager = 0x1C766B0;	//0x20766B0
	const auto LocalPlayer = 0x3513170;	//0x3913170
	const auto ChatClient = 0x1C74090;	//0x2074090
	const auto MenuGuiIns = 0x3512C40;	//0x3912C40
	const auto HudInstance = 0x350C4AC;	//0x390C4AC
	const auto PingInstance = 0x351A66C;	//0x391A66C
};

namespace Function
{
	const auto GetPing = 0x2A1B60;	//0x6A1B60
	const auto IsAlive = 0x19F810;	//0x59F810
	const auto IsMissile = 0x1AE120;	//0x5AE120
	const auto IsTargetable = 0x4FD900;	//0x8FD900
	const auto IssueOrder = 0x175950;	//0x575950
	const auto GetFirstObj = 0x28B2F0;	//0x68B2F0
	const auto GetNextObj = 0x28C510;	//0x68C510
	const auto GetSpellState = 0x4FD900;	//0x8FD900
	const auto SendChat = 0x5F1F60;	//0x9F1F60
	const auto PrintChat = 0x5A7CB0;	//0x9A7CB0
	const auto GetHpBarPos = 0x591C40;	//0x991C40
	const auto GetAiManager = 0x16CAB0;	//0x56CAB0
	const auto WorldToScreen = 0x982490;	//0xD82490
	const auto BaseDrawPosition = 0x16A4A0;	//0x56A4A0
	const auto GetAttackDelay = 0x2A1B60;	//0x6A1B60
	const auto GetAttackCastDelay = 0x2A1A60;	//0x6A1A60
	const auto SetBaseCharacterData = 0x19A040;	//0x59A040
	const auto OnCreateObject = 0x292B10;	//0x692B10
	const auto OnDeleteObject = 0x2897B0;	//0x6897B0
	const auto OnProcessSpell = 0x50E6E0;	//0x90E6E0
};
