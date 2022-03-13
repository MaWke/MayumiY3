#include "glow.h"
#include <windows.h>
#include "../Struct/CS.h"
#include "../Options.h"
#include "../process/Process.h"

#include "../GUI/Windows/MainWindow.h"
#include "../utils/utils.h"

#include <thread>
#include <iostream>


namespace glow
{
	void setGlowHP(Player* player) 
	{
		LocalPlayer* localPlayer = EntityList::getLocalPlayer();
		int health = player->getHealth();
		int glowIndex = player->getGlowIndex();
		GlowStruct* glowStruct = GlowObjectManager::get(glowIndex);

		GlowStruct newStruct = Process::readMem<GlowStruct>((uintptr_t)glowStruct);	//read current struct and modify it

		if (localPlayer->getTeam() == player->getTeam()) {
			newStruct.red = (float)health * 0.01f;			//Teammates start white, and lose red the more health they lose	
			newStruct.green = 1.0f;
			newStruct.blue = 1.0f;
		}
		else {
			newStruct.red = 1.0f;
			newStruct.green = 0.0f;
			newStruct.blue = (float)health * -0.01f + 1.0f;	//Enemies start red, and turn more blue as they lose health.
		}
		newStruct.alpha = 1.0f;
		newStruct.renderWhenOccluded = true;
		newStruct.renderWhenUnOccluded = false;

		Process::writeMem((uintptr_t)glowStruct, newStruct);
	}
}

void glow::run()
//
//Thread quits when g_glow is false
//
try
{
	std::this_thread::sleep_for(std::chrono::milliseconds(5));

	LocalPlayer* localPlayer = EntityList::getLocalPlayer();
	while (!*localPlayer) if(!*Options::g_glow) return;		//Wait for localPlayer, check that option remains unchanged

	while (*Options::g_glow)
	//Glow running
	{
		for (int i = 0; i < Server::getMaxPlayer(); ++i) {
			Player* player = EntityList::get(i);

			if (!player
				|| player == localPlayer
				|| player->getHealth() <= 0)
				continue;

			setGlowHP(player);	//Set players glow color based on HP
		}
	}
}
catch (std::exception e)
{
	MainWindow::errorBox.put(e.what());

	//Wait for a bit before checking processes again.
	if (!utils::checkConstantlyFor(6, Options::g_glow))
		return;	//if checking fails g_glow is false, return.

	MainWindow::errorBox.put("");	//empty error box
	run();
}

void glow::startThread()
//
//The process needs to be running for this to work 
//
{
	std::thread glowThread(run);
	glowThread.detach();
}