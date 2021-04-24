#include "InstructionSetLine.h"

bool InstructionSetLine::TryConvertBits(unsigned char& r)
{
	unsigned char base = 1;
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