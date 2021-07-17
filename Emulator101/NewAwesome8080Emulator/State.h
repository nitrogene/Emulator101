#pragma once
#include <string>
#include "Flags.h"

struct State
{
	Flags Flags;

	// Registers
	// 8 bits registers
	uint8_t B = 0;
	uint8_t C = 0;
	uint8_t D = 0;
	uint8_t E = 0;
	uint8_t H = 0;
	uint8_t L = 0;
	uint8_t A = 0;

	// 16 bits Program counter
	uint16_t PC = 0;

	// 16 bits stack pointer
	uint16_t SP = 0;

	// Number of cpu cycles
	uint64_t Cycles = 0;

	// Number of cpu steps
	uint64_t Steps = 0;

	// Interrupt enable ?
	bool EI=false;

	// ToString
	std::string toString();

	bool HLT = false;
};