#include <fmt/core.h>
#include "Registers.h"

std::string Registers::toString()
{
	return fmt::format("B={0:02x}, C={1:02x}, D={2:02x}, E={3:02x}, H={4:02x}, L={5:02x}, A={6:02x}", B, C, D, E, H, L, A);
}