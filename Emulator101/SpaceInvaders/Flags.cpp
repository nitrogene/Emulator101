#include "Flags.h"

std::string Flags::toString()
{
	std::string r = "";
	auto fun = [](bool b, std::string c) -> std::string
	{
		return b ? c : ".";
	};

	r += fun(Value & ZERO_CHECK, "Z");
	r += fun(Value & SIGN_CHECK, "S");
	r += fun(Value & PARITY_CHECK, "P");
	r += fun(Value & CARRY_CHECK, "CY");
	r += fun(Value & AUXILIARY_CARRY_CHECK, "AC");

	return r;
}

uint8_t Flags::DCR(const uint8_t value)
{
	// Perform operation in higher precision
	uint16_t a = ((uint16_t)value);
	uint16_t b = ((uint16_t)-1);
	uint16_t chp = a + b;
	uint8_t c = chp & 0xFF;

	// ZERO CHECK
	// If the result of an instruction has the value 0, this flag is set; otherwise it is reset
	if (c == 0)
	{
		Value |= ZERO_CHECK;
	}
	else
	{
		Value &= ~ZERO_CHECK;
	}

	// SIGN CHECK
	// If the most significant bit of the result of	the operation has the value 1, this flag is	set; otherwise it is reset.
	if (chp & 0x80)
	{
		Value |= SIGN_CHECK;
	}
	else
	{
		Value &= ~SIGN_CHECK;
	}

	// PARITY CHECK
	// If the modulo 2 sum of the bits of the result of the operation is 0, (Le., if the result has even parity), this flag is set;
	// otherwise it is reset(Le., if the result has	odd parity)
	if (Flags::isOddParity(c))
	{
		Value |= PARITY_CHECK;
	}
	else
	{
		Value &= ~PARITY_CHECK;
	}

	// AUXILIARY CARRY
	// 1 if a borrow happens between bits 3 & 4
	uint8_t a_= a & 0x0F;
	uint8_t b_ = b & 0x0F;
	uint8_t c_ = a_ + b_;
	if (c_ & 0x10)
	{
		Value |= AUXILIARY_CARRY_CHECK;
	}
	else
	{
		Value &= ~AUXILIARY_CARRY_CHECK;
	}
	
	return c;
}

bool Flags::isOddParity(const uint8_t value)
{
	// https://www.geeksforgeeks.org/finding-the-parity-of-a-number-efficiently/
	uint8_t y = value ^ (value >> 1);
	y = y ^ (y >> 2);
	y = y ^ (y >> 4);

	return (y & 1) == 1;
}
