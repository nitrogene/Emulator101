#pragma once
#include <string>
#include <vector>
#include "State.h"

class Utilities
{
public:
	static const uint8_t CARRY_CHECK = 0b00000001;
	static const uint8_t PARITY_CHECK = 0b00000100;
	static const uint8_t AUXILIARY_CARRY_CHECK = 0b00010000;
	static const uint8_t ZERO_CHECK = 0b01000000;
	static const uint8_t SIGN_CHECK = 0b10000000;

	static void ClearScreen();
	static std::vector<std::string> Split(const std::string& s, char delimiter);
	static bool isOddParity(const uint8_t value);
};

