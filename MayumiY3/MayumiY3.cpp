#include "GUI/Windows/MainWindow.h"
#include "process/Process.h"

int main() 
try
{
	#ifdef _DEBUG
	ShowWindow(GetConsoleWindow(), SW_RESTORE);
	#endif

	int windowX = 350;
	int windowY = 150;
	MainWindow mw(
		Point(
		Graph_lib::x_max() / 2 - windowY /2,				//Window topleft point x
		Graph_lib::y_max()/2- windowY /2),					//Window topleft point y
		"MayumiY3",											//Label
		(char*)LoadIcon(fl_display, MAKEINTRESOURCE(101)));	//Icon from MayumiY3.rc IDI_ICON1

	Graph_lib::gui_main();
	//The active threads try to read values from MainWindow's input boxes, but the window is already closed.
	//I think that it's "not a big deal" as it's not trying to read from csgo's memory.
	//On MainWindow's exit we would have to turn all options to false thus stopping the threads before actually closing the window.
	//This program is not perfect in many ways, it was made purely for the purpose of learning. Check README.md
	Process::closeProcessHandle(Process::getProcessID());
	return 1;
}
catch (std::exception e)
{
	std::cerr << e.what() << '\n';
	return 0;
}
catch (...)
{
	std::cerr << "Unknown exception\n";
	return 0;
}