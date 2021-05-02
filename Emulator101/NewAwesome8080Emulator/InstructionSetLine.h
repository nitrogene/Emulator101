#pragma once
#include <string>
#include <functional>
#include "MemoryMap.h"
#include "State.h"
#include "ClockCycle.h"

class InstructionSetLine
{
public:
	std::string Mnemonic{};
	uint8_t Bits[8]{};
	ClockCycle ClockCycle{};
	uint16_t Size{};
	std::string Description;

	bool TryConvertBits(uint8_t& r);

	std::function<void(State, MemoryMap)> exec;
};