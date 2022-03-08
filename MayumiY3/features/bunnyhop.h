#pragma once
namespace bunnyhop
{
	void run();
	void startThread();	//Deatches a new thread that runs until g_bunnyhop is disabled
						//If used correctly there shouldn't be multiple bunnyhop threads.
}

