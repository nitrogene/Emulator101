#include "Flags.h"

std::string Flags::toString() const
{
	std::string flags = "";
	auto fun = [](bool b, std::string c) -> std::string
	{
		return b ? c : ".";
	};

	flags += fun(Zero, "Z");
	flags += fun(Sign, "S");
	flags += fun(Parity, "P");
	flags += fun(Carry, "CY");
	flags += fun(AuxiliaryCarry, "AC");

	return flags;
}

uint8_t Flags::getF() const
{
	uint8_t f = 0;
	f |= Sign << 7;
	f |= Zero << 6;
	f |= AuxiliaryCarry << 4;
	f |= Parity << 2;
	f |= 1 << 1; 
	f |= Carry << 0;

	return f;
}

void Flags::setF(uint8_t f)
{
	Sign = (f >> 7) & 1;
	Zero = (f >> 6) & 1;
	AuxiliaryCarry = (f >> 4) & 1;
	Parity = (f >> 2) & 1;
	Carry = (f >> 0) & 1;
}