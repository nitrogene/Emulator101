#pragma once
#include <stdint.h>
#include <string>

struct ClockCycle
{
	uint8_t A=0;
	uint8_t B=0;

	std::string toString() const;
};