#include "aimbot.h"
#include <windows.h>
#include "../Struct/CS.h"
#include "../Options.h"
#include "../utils/math.h"

#include "../GUI/Windows/MainWindow.h"
#include "../utils/input.h"
#include "../utils/utils.h"

#include <thread>

namespace aimbot

{
	void smoothen(ViewAngle lpViewAngles, ViewAngle& targetAngles, float smoothAmount)
	//
	//add smoothing to ViewAngle
	//
	{
		ViewAngle difference = ViewAngle(
			Math::getNormalized(targetAngles.x() - lpViewAngles.x()) / smoothAmount,
			Math::getNormalized(targetAngles.y() - lpViewAngles.y()) / smoothAmount
		);

		targetAngles = lpViewAngles + difference;
	}
}

void aimbot::run()
//
//Thread quits when g_aimbot is false
//
try
{
	LocalPlayer* localPlayer = EntityList::getLocalPlayer();
	while (!*localPlayer) if (!*Options::g_aimbot) return;		//Wait for localPlayer, check that option remains unchanged

	std::string curString(MainWindow::aimKeyInBox.get_string());
	int aimKey = Input::strToWinKeyCode(curString);
	
	while (*Options::g_aimbot)
	//Aimbot running
	{
		if (MainWindow::aimKeyInBox.get_string() != curString) 
			run();	//restart aimbot if aim key changes. 

		if (!(GetAsyncKeyState(aimKey) & 0x8000))
			continue;

		ViewAngle lpViewAngles = localPlayer->getViewAngles();
		Vec3 lpEyePos = localPlayer->getEyePos();

		ViewAngle newAngles =	//pass everything in as arguments to avoid unnecessary reads
			EntityList::calcViewAnglesToBestTarget(MainWindow::fovInBox.get_float(), lpViewAngles, lpEyePos);
		//Check for NaN, if the angles are constantly NaN you will be able to see the message.
		if (!newAngles) { MainWindow::errorBox.put("Error: NaN angle, check offsets"); MainWindow::errorBox.put(""); continue; };

		float smoothAmount =	//multiply smooth value by random [1.0f, 1.3f] to make it look more realistic (it doesn't)
			MainWindow::smoothInBox.get_float() * Math::getRandF(1.0f, 1.3f);
		if (smoothAmount) smoothen(lpViewAngles, newAngles, smoothAmount);

		localPlayer->setViewAngles(newAngles);
	}
}
catch (std::exception e)
{
	MainWindow::errorBox.put(e.what());
	
	//Wait for a bit before checking processes again.
	if (!utils::checkConstantlyFor(6, Options::g_aimbot))
		return;	//if checking fails g_glow is false, return.

	MainWindow::errorBox.put("");	//empty error box
	run();
}

void aimbot::startThread()
//
//The process needs to be running for this to work 
//
{
	std::thread aimbotThread(run);
	aimbotThread.detach();
}