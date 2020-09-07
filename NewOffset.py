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
        of = 0
        if offset[1] != 0:
            of = offset[1] - idaapi.get_imagebase()
        print("\tconst auto %-20s = 0x%X;\t//0x%X" % (offset[0],  of,offset[1]))
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
    if pattern == '':
        return 0
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

def main():
    print("//[*] League of Legends Client Update Tool")
    print("//[*] By Chord")
    print("//[*] Started at: %s" % datetime.datetime.now())
    print("//----------------------------")

    MakeNamespace('GameClass', [
        ["GameTime", FindOffsetValue('F3 0F 11 05 ? ? ? ? 8B 49')],
        #["BuildVersion", FindOffsetValue('8B 44 24 04 BA ? ? ? ? 2B D0', 4)],
        ["ObjManager", FindOffsetValue('8B 0D ? ? ? ? E8 ? ? ? ? FF 77')],
        ["LocalPlayer", FindOffsetValue('A1 ?? ?? ?? ?? 85 C0 74 07 05 ?? ?? ?? ?? EB 02 33 C0 56')],
        ["ChatClient", FindOffsetValue('8B ?? ?? ?? ?? ?? 6A ?? 50 E8 ?? ?? ?? ?? 33 C0 5F C2')],
        ["MenuGuiIns", FindOffsetValue('8B 0D ? ? ? ? 6A 00 E8 ? ? ? ? C7')],
        ["HudInstance", FindOffsetValue('A1 ? ? ? ? 83 78 08 02 0F 94 C0')],
        ["PingInstance", FindOffsetValue('8B 0D ? ? ? ? 85 C9 74 07 8B 01 6A 01 FF 50 08 8B')],
    ])
    MakeNamespace('Function', [
        ["GetPing", FindOffsetValue('E8 ? ? ? ? 8B 44 24 1C 83 C4 0C 8B CE')],
        ["IsAlive", FindOffsetValue('56 8B F1 8B 06 8B 80 ? ? ? ? FF D0 84 C0 74 19')],
        ["IsMissile", FindOffsetValue('E8 ?? ?? ?? ?? 83 C4 04 84 C0 74 60')],
        ["IsTargetable", FindOffsetValue('E8 ? ? ? ? 8B F8 8B CB 89 ')],
        ["IssueOrder", FindOffsetValue('E8 ?? ?? ?? ?? 89 ?? ?? ?? ?? ?? 8B 84 ?? ?? ?? ?? ?? 8B CF F3 0F 7E 00')],
        ["GetFirstObj", FindOffsetValue('8B 41 14 8B 51 18')],
        ["GetNextObj", FindOffsetValue('E8 ? ? ? ? 8B F0 85 F6 75 E4')],
        ["SendChat", FindOffsetValue('E8 ? ? ? ? 5B 8B 8C 24 ? ? ? ? B8')],
        ["PrintChat", FindOffsetValue('E8 ? ? ? ? 33 C0 5F C2')],
        ["GetHpBarPos", FindOffsetValue('E8 ?? ?? ?? ?? 8B 4E ?? 8D 54 ?? ?? 52 8B 01 FF ?? ?? 5E 83 ?? ?? C3')],
        ["GetAiManager", FindOffsetValue('E8 ?? ?? ?? ?? 8B CB 8B B0 ?? ?? ?? ?? E8 ?? ?? ?? ?? 50 8B CE E8')],
        ["WorldToScreen", FindOffsetValue('83 EC 10 56 E8 ? ? ? ? 8B 08')],
        ["BaseDrawPosition", FindOffsetValue('E8 ?? ?? ?? ?? EB ?? 8B 01 8B 40')],
        ["GetAttackDelay", FindOffsetValue('E8 ? ? ? ? 8B 44 24 1C 83 C4 0C 8B CE ')],
        ["GetAttackCastDelay", FindOffsetValue('E8 ?? ?? ?? ?? D9 ?? ?? ?? ?? ?? 56 E8')],
        ["SetBaseCharacterData", FindOffsetValue('8B 44 ?? ?? 8D 8B ?? ?? ?? ?? 89 44 ?? ?? 8D 44 ?? ?? 50 E8 ?? ?? ?? ?? 8B 44 ?? ?? 8D 8B')],
        ["OnCreateObject", FindOffsetValue('E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? 84 C0 74 ?? 32 C9 EB')],
        ["OnDeleteObject", FindOffsetValue('E8 ?? ?? ?? ?? 57 E8 ?? ?? ?? ?? 83 ?? ?? 5F 5E C3')],
        ["OnProcessSpell", FindOffsetValue('E8 ?? ?? ?? ?? 85 C0 5D 0F 94 C0 83 ?? ?? C2')],
    ])


if __name__ == '__main__':
    main()
