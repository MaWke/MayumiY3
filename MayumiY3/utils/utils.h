#pragma once

#include <memory>

namespace utils 
{
	bool checkConstantlyFor(int duration, const std::unique_ptr<bool>& valueToCheck);
}
