#pragma once
#include <string>

struct Flags
{
private:
	static bool isOddParity(const uint8_t value);

public:
	static const uint8_t CARRY_CHECK			= 0b00000001;
	static const uint8_t PARITY_CHECK			= 0b00000100;
	static const uint8_t AUXILIARY_CARRY_CHECK	= 0b00010000;
	static const uint8_t ZERO_CHECK				= 0b01000000;
	static const uint8_t SIGN_CHECK				= 0b10000000;

	uint8_t Value = 0b00000010;

	/*
	If register pair PSW is specified, the first byte of information
	saved holds the contents of the A register; the
	second byte holds the settings of the five condition bits,
	i.e., Carry, Zero, Sign, Parity, and Auxiliary Carry. The format
	of this byte is:

	| | | |A| | | | |
	|S|Z|0|C|0|P|1|C|

	S  State of Sign bit
	Z  State of Zero bit
	0  always 0
	AC State of auxiliary Carry bit
	0  always 0
	P  State of Parity bit
	1  always 1
	C  State of Carry bit
	*/

	std::string toString();

	uint8_t DCR(const uint8_t value);
};