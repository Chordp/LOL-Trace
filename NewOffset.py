# coding=utf-8
from idautils import XrefsTo
from idc import BADADDR, INF_BASEADDR, SEARCH_DOWN, FUNCATTR_START, FUNCATTR_END
import idc
import idaapi
import datetime


def MakeEnum(enumName, offsetArray):
    print ("enum class %s\r\n{" % enumName)
    for offset in offsetArray:
        if len(offset[0]) == 0:
            print ("")
            continue
        if type(offset[1]) is str:
            print ("   %s = %s," % (offset[0], offset[1]))
            continue

        '''
		fncValue = offset[1] if offset[1] != -1 else 0x0
		'''
        # print "   %s = 0x%08x,%s" % (offset[0], fncValue, ' // Unknown' if fncValue == 0x0 else '')

        fncValue = offset[1] if offset[1] != -1 else 0x0

        locByName = idc.LocByName(offset[0])
        isMismatch = locByName != fncValue

        if locByName == BADADDR:
            locByName = fncValue

        print ("   %s = 0x%08x,%s" % (offset[0], locByName, '// Possible mismatch' if isMismatch else ''))

    print ("};\r\n")


def MakeNamespace(name, offsetArray):
    # type: (object, object) -> object
    print ("namespace %s \n{" % name)
    for offset in offsetArray:
        print("\tconst auto %-20s = 0x%X;\t//0x%X" % (offset[0], offset[1] - idaapi.get_imagebase(),offset[1]))
    print("};\r\n")


def FindFunctionByPatternStartEA(pattern):
    address = idc.FindBinary(0, SEARCH_DOWN, pattern)
    if address == BADADDR:
        return BADADDR

    try:
        return idaapi.get_func(address).startEA
    except Exception:
        return -1


def FindOffsetValue(pattern, offset=0):
    # type: (object, object) -> object
    try:
        address = idc.FindBinary(0, SEARCH_DOWN, pattern) + offset
        op1 = idc.GetOpType(address, 0)
        op2 = idc.GetOpType(address, 1)

        if idc.Byte(address) == 0xE8:
            if op1 == idc.o_near and op2 == idc.o_void:
                return idc.GetOperandValue(address, 0)
        elif op1 == idc.o_reg and (op2 == idc.o_mem or op2 == idc.o_imm) and idc.Byte(address) != 0x83:
            return idc.GetOperandValue(address, 1)
        elif (op1 == idc.o_mem or op1 == idc.o_imm) and op2 == idc.o_reg and idc.Byte(address) != 0x83:
            return idc.GetOperandValue(address, 0)
        else:
            return idaapi.get_func(address).startEA

    except Exception:
        return 0


def GetStr(ea):
    Version = ""
    i = 0
    while True:
        bt = idc.Byte(ea + i)
        i = i + 1
        if bt != 0:
            Version = Version + chr(bt)
        else:
            return Version


# def main():
#     print("//[*] League of Legends Client Update Tool")
#     print("//[*] By Chord")
#     print("//[*] Started at: %s" % datetime.datetime.now())
#     GameVersionAddress = FindOffsetValue("8B 44 24 04 BA ? ? ? ? 2B D0", 4)
#     print ("//[*] " + GetStr(GameVersionAddress))
#     print("//----------------------------")
#
#     MakeDefine("oGameVersion ", GameVersionAddress)
#     MakeDefine("oNetClient ", FindOffsetValue("A1 ? ? ? ? 8A 1F"))
#
#     MakeDefine("oChatClientPtr ", FindOffsetValue("8B 35 ? ? ? ? 52 "))
#     MakeDefine("oObjManager ", FindOffsetValue("8B 0D ? ? ? ? E8 ? ? ? ? FF 77"))
#     MakeDefine("oZoomClass ", FindOffsetValue("A3 ? ? ? ? 83 FA 10 72 32"))
#     MakeDefine("oGameTime ", FindOffsetValue("F3 0F 11 05 ? ? ? ? 8B 49"))
#     MakeDefine("oGameInfo  ", FindOffsetValue("A1 ? ? ? ? 83 78 08 02 0F 94 C0"))
#     MakeDefine("oLocalPlayer ", FindOffsetValue("A1 ?? ?? ?? ?? 85 C0 74 07 05 ?? ?? ?? ?? EB 02 33 C0 56"))
#     MakeDefine("oHudInstance ", FindOffsetValue("8B 0D ? ? ? ? 6A 00 8B 49 34 E8 ? ? ? ? B0"))
#     MakeDefine("oRenderer ", FindOffsetValue("8B 15 ? ? ? ? 83 EC 08 F3"))
#     MakeDefine("oUnderMouseObject ", FindOffsetValue(" 8B 0D ? ? ? ? 89 0D "))
#     MakeDefine("oD3DRenderer ", FindOffsetValue("A1 ? ? ? ? 33 D2 56 57 8B 7C 24 14		"))
#
#     # # FUNCTIONS
#     print("FUNCTIONS")
#
#     # MakeDefine("oGetSpellSlotPtr ", FindFunctionByPatternStartEA(""))
#     MakeDefine("oCastSpell ", FindOffsetValue("E8 ? ? ? ? 8B 7C 24 10 5D"))
#     #
#     MakeDefine("oDrawCircle ", FindOffsetValue(" E8 ? ? ? ? 83 C4 1C 5F 5E 83 C4 20		"))
#     MakeDefine("oGetBasicAttack	", FindOffsetValue("53 8B D9 B8 ? ? ? ? 8B 93	"))
#     MakeDefine("oGetAttackCastDelay ", FindOffsetValue(
#         "83 ?? ?? 53 8B 5C ?? ?? 8B CB 56 57 8B 3"))  # 83 ?? ?? 53 8B 5C ?? ?? 8B CB 56 57 8B 3
#     MakeDefine("oGetAttackDelay ", FindOffsetValue("8B 44 24 04 51 F3"))
#     MakeDefine("oGetPing ", FindOffsetValue("E8 ? ? ? ? 8B E8 3B 6B		"))
#     MakeDefine("oGetSpellState ", FindOffsetValue(" E8 ? ? ? ? 8B F8 8B CB 89 	"))
#     MakeDefine("oIsTargetable ", FindOffsetValue("56 8B F1 E8 ? ? ? ? 84 C0 74 2E"))
#     MakeDefine("oIsAlive ", FindOffsetValue("56 8B F1 8B 06 8B 80 ? ? ? ? FF D0 84 C0 74 19	"))
#     MakeDefine("oIsBaron  ", FindOffsetValue("56 E8 ? ? ? ? 68 ? ? ? ? 8B 30"))
#     MakeDefine("oIsTurret ", FindOffsetValue("E8 ? ? ? ? 83 C4 04 84 C0 74 09 5F"))
#     MakeDefine("oIsInhib ", FindOffsetValue(" E8 ? ? ? ? 83 C4 04 84 C0 74 38"))
#     MakeDefine("oIsTroy", FindOffsetValue(" E8 ? ? ? ? 8B E8 83 C4 04 85 ED 0F 84 ? ? ? ? 6A 08"))
#     MakeDefine("oIsHero ", FindOffsetValue("E8 ? ? ? ? 83 C4 04 84 C0 75 22 8B 8D"))
#     MakeDefine("oIsMinion ", FindOffsetValue(" E8 ? ? ? ? 83 C4 04 84 C0 74 4A 8B 45"))
#     MakeDefine("oIsDragon ", FindOffsetValue("83 EC 10 A1 ? ? ? ? 33 C4 89 44 24 0C 56 E8"))
#     MakeDefine("oIsMissile ", FindOffsetValue("E8 ? ? ? ? 83 C4 04 84 C0 74 1C FF"))
#     MakeDefine("oIsNexus ", FindOffsetValue("E8 ? ? ? ? 83 C4 04 84 C0 75 05 5F"))
#     MakeDefine("oIsNotWall ", FindOffsetValue("E8 ? ? ? ? 33 C9 83 C4 0C 84"))
#     # MakeDefine("oIsTroy ", FindOffsetValue(""))
#     MakeDefine("oIssueOrder ", FindOffsetValue(
#         " 8B F9 C7 84 ?? ?? ?? ?? ?? ?? ?? ?? ?? 8B 87 ?? ?? ?? ?? 8D 8F"))
#     MakeDefine("oPrintChat ", FindOffsetValue(" 83 EC 44 A1 ? ? ? ? 33 C4 89 44 24 40 53 8B 5C 24 50 56 "))
#     MakeDefine("oWorldToScreen ", FindOffsetValue("83 EC 10 56 E8 ? ? ? ? 8B 08"))
#     MakeDefine("oSetBaseCharacterData ", FindOffsetValue(
#         "83 ?? ?? 80 7C ?? ?? ?? 56 8B 74 ?? ?? 57 8B F9 74 ?? 8D 8F ?? ?? ?? ?? 3B CE 74 ?? 83 7E"))
#     # MakeDefine("oSetBaseCharacterData ", FindOffsetValue("6A ?? FF ?? ?? ?? 8D 86 ?? ?? ?? ?? 8B CE 50 E8 ?? ?? ?? ??",15))

def main():
    print("//[*] League of Legends Client Update Tool")
    print("//[*] By Chord")
    print("//[*] Started at: %s" % datetime.datetime.now())
    print("//----------------------------")

    MakeNamespace('GameClass', [
        ["GameTime", FindOffsetValue('F3 0F 11 05 ? ? ? ? 8B 49')],
        ["BuildVersion", FindOffsetValue('8B 44 24 04 BA ? ? ? ? 2B D0', 4)],
        ["ObjManager", FindOffsetValue('8B 0D ? ? ? ? E8 ? ? ? ? FF 77')],
        ["LocalPlayer", FindOffsetValue('A1 ?? ?? ?? ?? 85 C0 74 07 05 ?? ?? ?? ?? EB 02 33 C0 56')],
        ["ChatClient", FindOffsetValue('8B ?? ?? ?? ?? ?? 6A ?? 50 E8 ?? ?? ?? ?? 33 C0 5F C2')],
        ["HudInstance",
         FindOffsetValue('8B ?? ?? ?? ?? ?? FF ?? ?? 8B 4E ?? E8 ?? ?? ?? ?? 8B 94 ?? ?? ?? ?? ?? 8D 8F')],
        ["PingInstance", FindOffsetValue('8B 0D ? ? ? ? 85 C9 74 07 8B 01 6A 01 FF 50 08 8B')],
    ])
    MakeNamespace('Function', [
        ["GetPing", FindOffsetValue("E8 ? ? ? ? 8B 4F 2C 85 C9 0F")],

        ["IsAlive", FindOffsetValue('56 8B F1 8B 06 8B 80 ? ? ? ? FF D0 84 C0 74 19')],
        ["IsMissile", FindOffsetValue('E8 ?? ?? ?? ?? 83 ?? ?? 84 C0 74 ?? 8B 17 8B CF FF')],
        ["IsTargetable", FindOffsetValue("56 8B F1 E8 ? ? ? ? 84 C0 74 2E")],
        ["IssueOrder", FindOffsetValue('E8 ?? ?? ?? ?? 85 FF 5F 0F 44 F3 89 ?? ?? ?? ?? ?? 5E 5B B0 ?? C2')],
        ["GetFirstObj", FindOffsetValue('E8 ?? ?? ?? ?? 8B F8 85 FF 0F ?? ?? ?? ?? ?? 53 8B 5C ?? ?? 56 0F')],
        ["GetNextObj", FindOffsetValue('E8 ?? ?? ?? ?? 8B F8 85 FF 75 ?? 5E 5B')],
        ["PrintChat", FindOffsetValue('E8 ?? ?? ?? ?? 33 C0 5F C2')],
        ["GetHpBarPos", FindOffsetValue('E8 ?? ?? ?? ?? 8B 4E ?? 8D 54 ?? ?? 52 8B 01 FF ?? ?? 5E 83 ?? ?? C3')],
        ["GetAiManager", FindOffsetValue('E8 ?? ?? ?? ?? 6A ?? 6A ?? FF ?? ?? ?? 8B C8 E8 ?? ?? ?? ?? 8D 8F ?? ?? ?? ?? E8')],
        ["WorldToScreen", FindOffsetValue('83 EC 10 56 E8 ? ? ? ? 8B 08')],
        ["BaseDrawPosition", FindOffsetValue('E8 ?? ?? ?? ?? EB ?? 8B 01 8B 40')],
        ["GetAttackDelay", FindOffsetValue('E8 ?? ?? ?? ?? 8B 44 ?? ?? 83 ?? ?? 8B CE 66 C7')],
        ["GetAttackCastDelay", FindOffsetValue('E8 ?? ?? ?? ?? D9 ?? ?? ?? ?? ?? 53 E8 ?? ?? ?? ?? 8B 44')],
        ["SetBaseCharacterData",
         FindOffsetValue('83 ?? ?? 80 7C ?? ?? ?? 56 8B 74 ?? ?? 57 8B F9 74 ?? 8D 8F ?? ?? ?? ?? 3B CE 74 ?? 83 7E')],


        ['OnCreateObject',
         FindOffsetValue("E8 ?? ?? ?? ?? 0F B6 ?? ?? 33 C9 0F B6 ?? ?? 8A 44 ?? ?? 88 44 ?? ?? 85 F6 74 ?? 8D 57")],
        ['OnDeleteObject',
         FindOffsetValue("E8 ?? ?? ?? ?? 83 ?? ?? 3B F7 75 ?? 8B ?? ?? ?? ?? ?? 8B CA 2B ?? ?? ?? ?? ?? C1")],
        ['OnProcessSpell', FindOffsetValue(" E8 ?? ?? ?? ?? 8B CE E8 ?? ?? ?? ?? 80 BE ?? ?? ?? ?? ?? D8")]
    ])


if __name__ == '__main__':
    main()
