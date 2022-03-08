#include "utils.h"

#include <chrono>

bool utils::checkConstantlyFor(int duration, const std::unique_ptr<bool>& valueToCheck)
{
	std::chrono::time_point<std::chrono::steady_clock> startTime = std::chrono::steady_clock::now();
	int timeExecuted = 0;
	while (timeExecuted < 6) {
		if (!*valueToCheck) return false;
		std::chrono::time_point<std::chrono::steady_clock> endTime = std::chrono::steady_clock::now();
		std::chrono::duration<__int64, std::nano> difference = endTime - startTime;
		timeExecuted = std::chrono::duration_cast<std::chrono::seconds>(difference).count();
	}
	return true;
}