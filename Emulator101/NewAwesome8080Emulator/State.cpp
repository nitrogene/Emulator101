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

void State::DCR(uint8_t& value)
{
	/*
	DCR R (Decrement register)
	0 0 r r r 1 0 1						R <- R - 1 Data in R is decremented by 1
										flags affected: Z, S, P, AC
	*/

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)value);
	uint16_t b = ((uint16_t)-1);
	uint16_t chp = a + b;

	// get result in normal precision
	value = chp & 0xFF;

	// ZERO CHECK
	this->Z = value == 0;

	// SIGN CHECK
	this->S = chp & 0x80;

	// PARITY CHECK
	this->P = Utilities::isOddParity(value);

	// AUXILIARY CARRY
	uint8_t a_ = a & 0x0F;
	uint8_t b_ = b & 0x0F;
	uint8_t c_ = a_ + b_;
	this->AC = c_ & 0x10;

	// change F 
	this->setF();
}

void State::ANI(uint8_t& value, const uint8_t& opCode1)
{
	/*
	ANI data8 (AND immediate)
	1	1	1	0	0	1	1	0		A <- A ^ data8 Bits in A logically multiplied with bits from
	d7	d6	d5	d4	d3	d2	d1	d0		2nd byte of instruction, flags affected: Z, S, P, CY=0, AC=0

	*/

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)value);
	uint16_t b = ((uint16_t)opCode1);
	uint16_t chp = a & b;

	// get result in normal precision
	value = chp & 0xFF;

	// ZERO CHECK
	this->Z = value == 0;

	// SIGN CHECK
	this->S = chp & 0x80;

	// PARITY CHECK
	this->P = Utilities::isOddParity(value);

	this->CY = 0;
	this->AC = 0;

	// change F 
	this->setF();
}

void State::ADI(const uint8_t& opCode1)
{
	/*
	ADI data8 (Add immediate)
	1	1	0	0	0	1	1	0		A<-A + data8 one byte (next to instruction) added to A
	d7	d6	d5	d4	d3	d2	d1	d0		flags affected: Z, S, P, CY, AC
	*/

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)this->A);
	uint16_t b = ((uint16_t)opCode1);
	uint16_t chp = a + b;

	// get result in normal precision
	this->A = chp & 0xFF;

	// ZERO CHECK
	this->Z = this->A == 0;

	// SIGN CHECK
	this->S = chp & 0x80;

	// PARITY CHECK
	this->P = Utilities::isOddParity(this->A);

	this->CY = 0;
	this->AC = 0;

	// change F 
	this->setF();
}

void State::CPI(const uint8_t& opCode1)
{
	/*
	CPI data8 (Compare immediate)
	1	1	1	1	1	1	1	0		A - data8 Data in 2nd byte of instruction is subtracted
	d7	d6	d5	d4	d3	d2	d1	d0		from data in A, only flags are affected: Z, S, P, CY, AC
	*/

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)this->A);
	uint16_t b = ((uint16_t)opCode1);
	uint16_t chp = a - b;

	// get result in normal precision
	this->A = chp & 0xFF;

	// ZERO CHECK
	this->Z = this->A == 0;

	// SIGN CHECK
	this->S = chp & 0x80;

	// PARITY CHECK
	this->P = Utilities::isOddParity(this->A);

	// CARRY BIT
	this->CY = chp & 0x100;

	// AUXILIARY CARRY
	uint8_t a_ = a & 0x0F;
	uint8_t b_ = b & 0x0F;
	uint8_t c_ = a_ - b_;
	this->AC = c_ & 0x10;

	// change F 
	this->setF();
}

void State::ACI(const uint8_t& opCode1)
{
	/*
	ACI data8 (Add immediate with carry)
	1	1	0	0	1	1	1	0		A <- A + data8 + CY one byte and CY added to A
	d7	d6	d5	d4	d3	d2	d1 d0		flags affected: Z, S, P, CY, AC
	*/

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)this->A);
	uint16_t b = ((uint16_t)opCode1);
	uint16_t chp = a + b;

	if (this->CY)
	{
		chp += 1;
	}

	// get result in normal precision
	this->A = chp & 0xFF;

	// ZERO CHECK
	this->Z = this->A == 0;

	// SIGN CHECK
	this->S = chp & 0x80;

	// PARITY CHECK
	this->P = Utilities::isOddParity(this->A);

	// CARRY BIT
	this->CY = chp & 0x100;

	// AUXILIARY CARRY
	uint8_t a_ = a & 0x0F;
	uint8_t b_ = b & 0x0F;
	uint8_t c_ = a_ - b_;
	this->AC = c_ & 0x10;

	// change F 
	this->setF();
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