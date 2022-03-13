#include "offsets.h"

#define OFFSETF(name, mod, pattern, offsetsVec, extra) \
uintptr_t offsets::signatures::name()\
{\
	static uintptr_t name = Process::findPattern(mod, pattern, offsetsVec, extra);\
	return name;\
}

OFFSETF(LocalPlayer, Process::Modules::CLIENTDLL, "8D 34 85 ? ? ? ? 89 15 ? ? ? ? 8B 41 08 8B 48 04 83 F9 FF", { 3 }, 4);
OFFSETF(GlowObjectManager, Process::Modules::CLIENTDLL, "A1 ? ? ? ? A8 01 75 4B", { 1 }, 4);
OFFSETF(ClientState, Process::Modules::ENGINEDLL, "A1 ? ? ? ? 33 D2 6A 00 6A 00 33 C9 89 B0", { 1 }, 0);
OFFSETF(ClientState_ViewAngles, Process::Modules::ENGINEDLL, "F3 0F 11 86 ? ? ? ? F3 0F 10 44 24 ? F3 0F 11 86", { 4 }, 0);
OFFSETF(ForceJump, Process::Modules::CLIENTDLL, "8B 0D ? ? ? ? 8B D6 8B C1 83 CA 02", { 2 }, 0);
OFFSETF(EntityList, Process::Modules::CLIENTDLL, "BB ? ? ? ? 83 FF 01 0F 8C ? ? ? ? 3B F8", { 1 }, 0);
OFFSETF(ClientState_MaxPlayer, Process::Modules::ENGINEDLL, "A1 ? ? ? ? 8B 80 ? ? ? ? C3 CC CC CC CC 55 8B EC 8A 45 08", { 7 }, 0);

