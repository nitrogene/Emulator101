#include "Flags.h"

uint8_t Flags::getF()
{
	/*
	| | | |A| | | | |
	|S|Z|0|C|0|P|1|C|
	*/

	uint8_t f = 0b00000010;

	if (this->Carry)
	{
		f |= 0b00000001;
	}

	if (this->Parity)
	{
		f |= 0b00000100;
	}

	if (this->AuxiliaryCarry)
	{
		f |= 0b00010000;
	}

	if (this->Zero)
	{
		f |= 0b01000000;
	}

	if (this->Sign)
	{
		f |= 0b10000000;
	}

	return f;
}