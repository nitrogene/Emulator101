#include <fmt/core.h>
#include "State.h"
#include "Utilities.h"

/*
	ZERO CHECK
	If the result of an instruction has the value 0, this flag is set; otherwise it is reset

	SIGN CHECK
	If the most significant bit of the result of	the operation has the value 1, this flag is	set; otherwise it is reset

	PARITY CHECK
	If the modulo 2 sum of the bits of the result of the operation is 0, (Le., if the result has even parity), this flag is set;
	otherwise it is reset(Le., if the result has odd parity)

	AUXILIARY CARRY
	1 if a borrow happens between bits 3 & 4

	CARRY BIT
	1 if an instruction produce a carry out of the high-order bit
*/

std::string State::toString()
{
	std::string flags = "";
	auto fun = [](bool b, std::string c) -> std::string
	{
		return b ? c : ".";
	};

	flags += fun(this->Flags.Zero,				"Z");
	flags += fun(this->Flags.Sign,				"S");
	flags += fun(this->Flags.Parity,			"P");
	flags += fun(this->Flags.Carry,				"CY");
	flags += fun(this->Flags.AuxiliaryCarry,	"AC");

	return fmt::format("af      bc      de      hl      pc      sp      flags   cycles  steps\n{0:02x}{1:02x}\t{2:02x}{3:02x}\t{4:02x}{5:02x}\t{6:02x}{7:02x}\t{8:04x}\t{9:04x}\t{10}\t{11:06}\t{12:06}\n", 
		this->A, this->F,this->B,this->C,this->D,this->E, this->H, this->L,this->PC,this->SP, flags,this->Cycles,this->Steps);
}

