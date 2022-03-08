#include "MainWindow.h"

#include "../../features/bunnyhop.h"
#include "../../features/glow.h"
#include "../../features/aimbot.h"

/***************************************************/
//
//CHECK README.MD.
//PURELY JUST PLAYING AROUND WITH THE GUI
//AND GETTING SOMETHING TO SHOW ON THE SCREEN.
//THIS IS NOT GOOD CODE.
//I KNOW THAT THERE'S IMGUI FOR THIS STUFF, 
//CHECK README.MD.
//
/***************************************************/

#define LAMBDACBF(function) [](Graph_lib::Address, Graph_lib::Address pw) {Graph_lib::reference_to<MainWindow>(pw).function();}

//Just assigning positions manually, I know very well that it shouldn't be done like this ...

Graph_lib::Out_box MainWindow::errorBox(Point(350 / 2 - 125, 150 / 2 - 68), 250, 30, "");
Graph_lib::In_box MainWindow::aimKeyInBox(Point(350 / 2 + 40, 150 / 2 + 31), 40, 25, "");
Graph_lib::In_box MainWindow::smoothInBox(Point(350 / 2 + 80, 150 / 2 + 31), 40, 25, "");
Graph_lib::In_box MainWindow::fovInBox(Point(350 / 2 + 120, 150 / 2 + 31), 40, 25, "");

MainWindow::MainWindow(Point tl, const std::string& title, char* iconSrc, int width, int height)
	:
	Window(tl, width, height, title, iconSrc),

	bhopCheckbox(Point(width / 2 - 40, height / 2 - 25), 80, 50, "Bhop",
		LAMBDACBF(runBhop), Options::g_bunnyhop),

	glowCheckbox(Point(width / 2 - 140, height / 2 - 25), 80, 50, "Glow",
		LAMBDACBF(runGlow), Options::g_glow),

	aimbotCheckbox(Point(width / 2 + 60, height / 2 - 25), 80, 50, "Aimbot",
		LAMBDACBF(runAimbot), Options::g_aimbot),

	aimKeyText(Point(width / 2 + 40, height / 2 + 70), "KEY"),
	smoothText(Point(width / 2 + 80, height / 2 + 70), "SMT"),
	fovText(Point(width / 2 + 120, height / 2 + 70), "FOV")
{
//
//Window settings
//
	color((char)Graph_lib::Color::Color_type::black);	//Background color
	resizable(0);										//Disable resizing

//Attaches 
	attach(errorBox);

	attach(bhopCheckbox);
	attach(glowCheckbox);

	attach(aimbotCheckbox);

	aimKeyText.set_color(Graph_lib::Color::Color_type::dark_red);
	attach(aimKeyText);
	attach(aimKeyInBox);

	smoothText.set_color(Graph_lib::Color::Color_type::dark_red);
	attach(smoothText);
	attach(smoothInBox);

	fovText.set_color(Graph_lib::Color::Color_type::dark_red);
	attach(fovText);
	attach(fovInBox);
}

//
//Functions
//
void MainWindow::runBhop()
{
	bhopCheckbox.changeEnabled();
	if (bhopCheckbox.isEnabled())
		bunnyhop::startThread();
	flush();
}

void MainWindow::runGlow()
{
	glowCheckbox.changeEnabled();
	if (glowCheckbox.isEnabled())
		glow::startThread();
	flush();
}

void MainWindow::runAimbot()
{
	aimbotCheckbox.changeEnabled();
	if (aimbotCheckbox.isEnabled())
		aimbot::startThread();
	flush();
}
