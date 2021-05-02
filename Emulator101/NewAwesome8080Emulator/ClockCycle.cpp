#include <fmt/core.h>
#include "ClockCycle.h"

std::string ClockCycle::toString() const
{
	if (B != 0)
	{
		return fmt::format("{0}/{1}", A, B);
	}

	return fmt::format("{0}", A);
}