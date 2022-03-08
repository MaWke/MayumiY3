#pragma once
namespace glow
{
	void run();
	void startThread();	//Deatches a new thread that runs until g_glow is disabled
						//If used correctly there shouldn't be multiple glow threads.
}

