#pragma once
#include <stdint.h>
#include <string>

struct ClockCycle
{
	uint8_t A;
	uint8_t B;

	std::string toString() const;
};