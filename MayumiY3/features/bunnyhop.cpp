#include "bunnyhop.h"
#include <windows.h>
#include "../Struct/CS.h"
#include "../Options.h"

#include "../GUI/Windows/MainWindow.h"
#include "../utils/utils.h"

#include <thread>
#include <chrono>

void bunnyhop::run()
//
//Thread quits when g_bunnyhop is false
//
try
{
	std::this_thread::sleep_for(std::chrono::milliseconds(5));

	LocalPlayer* localPlayer = EntityList::getLocalPlayer();
	while (!*localPlayer) if(!*Options::g_bunnyhop) return;		//Wait for localPlayer, check that option remains unchanged

	while (*Options::g_bunnyhop)
	//Bunnyhop running
	{
		if (localPlayer->getfFlags() & (int)EntityFlags::FL_ONGROUND && GetAsyncKeyState(VK_SPACE) & 0x8000)
			localPlayer->forceJump();
	}
}
catch(std::exception e)
{
	MainWindow::errorBox.put(e.what());

	//Wait for a bit before checking processes again.
	if (!utils::checkConstantlyFor(6, Options::g_bunnyhop))
		return;	//if checking fails g_glow is false, return.

	MainWindow::errorBox.put("");	//empty error box
	run();
}

void bunnyhop::startThread()
//
//The process needs to be running for this to work 
//
{
	std::thread bhopThread(run);
	bhopThread.detach();
}