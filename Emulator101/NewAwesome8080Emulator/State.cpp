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
	return fmt::format("af      bc      de      hl      pc      sp      flags   cycles  steps\n{0:02x}{1:02x}\t{2:02x}{3:02x}\t{4:02x}{5:02x}\t{6:02x}{7:02x}\t{8:04x}\t{9:04x}\t{10}\t{11:06}\t{12:06}\n", 
		this->A, this->F,this->B,this->C,this->D,this->E, this->H, this->L,this->PC,this->SP, flags,this->Cycles,this->Steps);
}

void State::setF()
{
	/*
	| | | |A| | | | |
	|S|Z|0|C|0|P|1|C|
	*/
	//this->F = 0b00000010;
	this->F = 0b00000000;
	if (this->CY)
	{
		this->F |= 0b00000001;
	}
	if (this->P)
	{
		this->F |= 0b00000100;
	}
	if (this->AC)
	{
		this->F |= 0b00010000;
	}
	if (this->Z)
	{
		this->F |= 0b01000000;
	}
	if (this->S)
	{
		this->F |= 0b10000000;
	}
}