#pragma once
#include <string>

struct State
{
	// Flags
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
	bool S = false;
	bool Z = false;
	bool AC = false;
	bool P = false;
	bool CY = false;

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

	std::string toString();
};