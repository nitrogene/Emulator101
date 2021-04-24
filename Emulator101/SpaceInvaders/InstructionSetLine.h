#pragma once
#include <string>

class InstructionSetLine
{
public:
	std::string Mnemonic{};
	unsigned char Bits[8]{};
	std::string Periods{};
	unsigned short Size{};
	std::string Description;

	bool TryConvertBits(unsigned char& r);
};