#pragma once

#include <memory>
#include <utility>
#include <string>
#include <map>

#define OPTIONDEC(type, name) extern std::unique_ptr<type> name;

namespace Options
//
//Options declarations.
//
{
	OPTIONDEC(bool, g_bunnyhop);
	OPTIONDEC(bool, g_glow);
	OPTIONDEC(bool, g_aimbot);

}