#pragma once

#include "../Window.h"
#include "../Graph.h"
#include "../GUI.h"
#include "../../Options.h"

class MainWindow : public Graph_lib::Window
{
public:
	MainWindow(Point tl, const std::string& title, char* iconSrc, int width = 350, int height = 150);
																//default width and height should always be used
	static Graph_lib::Out_box errorBox;

	static Graph_lib::In_box smoothInBox;
	static Graph_lib::In_box fovInBox;
	static Graph_lib::In_box aimKeyInBox;
private:
	Graph_lib::Checkbox bhopCheckbox;
	Graph_lib::Checkbox glowCheckbox;

	Graph_lib::Checkbox aimbotCheckbox;
	Graph_lib::Text aimKeyText;
	Graph_lib::Text smoothText;
	Graph_lib::Text fovText;

	void runBhop();
	void runGlow();
	void runAimbot();

	int width;
	int height;
};