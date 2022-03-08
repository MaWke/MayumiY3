#include "input.h"
#include "windows.h"

#include <stdexcept>
#include <algorithm>

static std::map<std::string, int> keyCodeStrPairs
{
	{"m1", VK_LBUTTON},
	{"m2", VK_RBUTTON},
	{"m3", VK_MBUTTON},
	{"m4", VK_XBUTTON1},
	{"m5", VK_XBUTTON2},
	{"alt", VK_MENU},
	{"z", 0x5A},	//Windows Virtual-Key codes
	{"x", 0x5A},	//
	{"c", 0x43},	//
	{"c", 0x56}		//
};

int Input::strToWinKeyCode(std::string keyString)
{
	std::transform(keyString.begin(), keyString.end(), keyString.begin(),
		[](unsigned char c) { return std::tolower(c); });

	for (const auto& pair : keyCodeStrPairs) {
		if (pair.first == keyString)
			return pair.second;
	}
	throw std::runtime_error("Key: key is not defined");
}