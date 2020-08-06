#include "SpellEntry.h"

bool SpellEntry::isAutoAttack() {
	return *(bool*)((DWORD)(this) + 0x4E1);
	
}
