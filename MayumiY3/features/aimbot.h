#pragma once
namespace aimbot
{
	void run();
	void startThread();	//Deatches a new thread that runs until g_aimbot is disabled
						//If used correctly there shouldn't be multiple aimbot threads.
}
