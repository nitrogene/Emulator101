#pragma once
#include <string>

class Registers
{
public:
	// 8 bits registers
	uint8_t B = 0, C = 0, D = 0, E = 0, H = 0, L = 0, A = 0;

	std::string toString(const unsigned char F);
};