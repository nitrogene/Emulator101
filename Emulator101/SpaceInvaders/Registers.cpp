#include <fmt/core.h>
#include "Registers.h"

std::string Registers::toString(const unsigned char F)
{
	return fmt::format("{0:02x}{1:02x} {2:02x}{3:02x} {4:02x}{5:02x} {6:02x}{7:02x}",A,F,B,C,D,E,H,L);
}