#pragma once
#include <string>
#include "ClockCycle.h"

struct InstructionSetLine
{
	std::string Mnemonic{};
	ClockCycle ClockCycle{};
	uint16_t Size{};
	std::string Description;
};