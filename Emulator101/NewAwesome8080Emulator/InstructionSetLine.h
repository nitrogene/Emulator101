#pragma once
#include <string>

struct ClockCycle
{
	uint8_t A;
	uint8_t B;

	std::string toString();
};

class InstructionSetLine
{
public:
	std::string Mnemonic{};
	uint8_t Bits[8]{};
	ClockCycle ClockCycle{};
	uint16_t Size{};
	std::string Description;

	bool TryConvertBits(uint8_t& r);
};