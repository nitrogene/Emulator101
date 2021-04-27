#include <fmt/core.h>
#include "State.h"
#include "Utilities.h"

std::string State::toString()
{
	Utilities::ClearScreen();
	std::string flags = "";
	auto fun = [](bool b, std::string c) -> std::string
	{
		return b ? c : ".";
	};

	flags += fun(Z, "Z");
	flags += fun(S, "S");
	flags += fun(P, "P");
	flags += fun(CY, "CY");
	flags += fun(AC, "AC");
	return fmt::format("af      bc      de      hl      pc      sp      flags   cycles  steps\n{0:02x}{1:02x}\t{2:02x}{3:02x}\t{4:02x}{5:02x}\t{6:02x}{7:02x}\t{8:06}\t{9:06}\t{10}\t{11:06}\t{12:06}\n", 
		this->A,0,this->B,this->C,this->D,this->E, this->H, this->L,this->PC,this->SP, flags,this->Cycles,this->Steps);
}