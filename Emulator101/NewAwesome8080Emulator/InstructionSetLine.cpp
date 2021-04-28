#include <fmt/core.h>
#include "InstructionSetLine.h"

std::string ClockCycle::toString()
{
	if (B != 0)
	{
		return fmt::format("{0}/{1}", A,B);
	}

	return fmt::format("{0}", A);
}

bool InstructionSetLine::TryConvertBits(uint8_t& r)
{
	uint8_t base = 1;
	r = 0;
	for (auto i = 7; i >= 0; --i)
	{
		if (Bits[i] != '1' && Bits[i] != '0')
		{
			r = 0;
			return false;
		}

		if (Bits[i] == '1')
		{
			r += base;
		}

		base <<= 1;
	}

	return true;
}