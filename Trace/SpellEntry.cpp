#include "SpellEntry.h"

bool SpellEntry::isAutoAttack() {
	return *(bool*)((DWORD)this +0x230+ 0x4d9);
	
}
