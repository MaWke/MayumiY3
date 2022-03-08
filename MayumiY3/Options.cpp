#include "Options.h"
#include "GUI/Windows/MainWindow.h"

#define OPTIONDEF(type, name, val) std::unique_ptr<type> Options::name = std::make_unique<type>(val);

//
//Remember to declare your variables in "Options.h"
//
OPTIONDEF(bool, g_bunnyhop, false);
OPTIONDEF(bool, g_glow, false);
OPTIONDEF(bool, g_aimbot, false);
