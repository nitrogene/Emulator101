#include "Utilities.h"
#include <sstream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef max

void Utilities::ClearScreen()
{
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}

// To be replaced by using ranges
std::vector<std::string> Utilities::Split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

bool Utilities::isOddParity(const uint8_t value)
{
	// https://www.geeksforgeeks.org/finding-the-parity-of-a-number-efficiently/
	uint8_t y = value ^ (value >> 1);
	y = y ^ (y >> 2);
	y = y ^ (y >> 4);

	return (y & 1) == 0;
}


void Utilities::DCR(State& state, uint8_t& value)
{
	auto& flags = state.Flags;

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)value);
	uint16_t b = ((uint16_t)-1);
	uint16_t chp = a + b;

	// get result in normal precision
	value = chp & 0xFF;

	// ZERO CHECK
	flags.Zero = value == 0;

	// SIGN CHECK
	flags.Sign = chp & 0x80;

	// PARITY CHECK
	flags.Parity = Utilities::isOddParity(value);

	// AUXILIARY CARRY
	uint8_t a_ = a & 0x0F;
	uint8_t b_ = b & 0x0F;
	uint8_t c_ = a_ + b_;
	flags.AuxiliaryCarry = c_ & 0x10;

	// change F 
	state.F=flags.getF();
}

void Utilities::INR(State& state, uint8_t& value)
{
	auto& flags = state.Flags;

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)value);
	uint16_t b = ((uint16_t)1);
	uint16_t chp = a + b;

	// get result in normal precision
	value = chp & 0xFF;

	// ZERO CHECK
	flags.Zero = value == 0;

	// SIGN CHECK
	flags.Sign = chp & 0x80;

	// PARITY CHECK
	flags.Parity = Utilities::isOddParity(value);

	// AUXILIARY CARRY
	uint8_t a_ = a & 0x0F;
	uint8_t b_ = b & 0x0F;
	uint8_t c_ = a_ + b_;
	flags.AuxiliaryCarry = c_ & 0x10;

	// change F 
	state.F=flags.getF();
}

void Utilities::ANI(State& state, const uint8_t& opCode1)
{
	auto& flags = state.Flags;

	// get result in normal precision
	state.A &= opCode1;

	// ZERO CHECK
	flags.Zero = state.A == 0;

	// SIGN CHECK
	flags.Sign = state.A & 0x80;

	// PARITY CHECK
	flags.Parity = Utilities::isOddParity(state.A);

	flags.Carry = 0;
	flags.AuxiliaryCarry = 0;

	// change F 
	state.F=flags.getF();
}

void Utilities::ADI(State& state, const uint8_t& opCode1)
{
	auto& flags = state.Flags;

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)state.A);
	uint16_t b = ((uint16_t)opCode1);
	uint16_t chp = a + b;

	// get result in normal precision
	state.A = chp & 0xFF;

	// ZERO CHECK
	flags.Zero = state.A == 0;

	// SIGN CHECK
	flags.Sign = chp & 0x80;

	// PARITY CHECK
	flags.Parity = Utilities::isOddParity(state.A);

	// CARRY BIT
	flags.Carry = chp & 0x100;

	// AUXILIARY CARRY
	uint8_t a_ = a & 0x0F;
	uint8_t b_ = b & 0x0F;
	uint8_t c_ = a_ + b_;
	flags.AuxiliaryCarry = c_ & 0x10;

	// change F 
	state.F=flags.getF();
}

void Utilities::CPI(State& state, const uint8_t& opCode1)
{
	auto& flags = state.Flags;

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)state.A);
	uint16_t b = ((uint16_t)(uint8_t(-opCode1)));
	uint16_t chp = a + b;

	// get result in normal precision
	uint8_t value = chp & 0xFF;

	// ZERO CHECK
	flags.Zero = value == 0;

	// SIGN CHECK
	flags.Sign = chp & 0x80;

	// PARITY CHECK
	flags.Parity = Utilities::isOddParity(value);

	// CARRY BIT
	flags.Carry = !(chp & 0x100);
	// AUXILIARY CARRY
	uint8_t a_ = a & 0x0F;
	uint8_t b_ = b & 0x0F;
	uint8_t c_ = a_ + b_;
	flags.AuxiliaryCarry = !(c_ & 0x10);

	// user manual, page 29
	// NOTE: If the two quantities to be compared differ in sign,
	// the sense of the Carry bit is reversed.
	// But superzazu code does not take that intop account...
	//if ((state.A & 0x80) != (opCode1 & 0x80))
	//{
	//	flags.Carry = !flags.Carry;
	//	flags.AuxiliaryCarry = !flags.AuxiliaryCarry;
	//}

	// change F 
	state.F=flags.getF();
}

void Utilities::ACI(State& state, const uint8_t& opCode1)
{
	auto& flags = state.Flags;

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)state.A);
	uint16_t b = ((uint16_t)opCode1);
	uint16_t chp = a + b;

	if (flags.Carry)
	{
		chp += 1;
	}

	// get result in normal precision
	state.A = chp & 0xFF;

	// ZERO CHECK
	flags.Zero = state.A == 0;

	// SIGN CHECK
	flags.Sign = chp & 0x80;

	// PARITY CHECK
	flags.Parity = Utilities::isOddParity(state.A);

	// CARRY BIT
	flags.Carry = chp & 0x100;

	// AUXILIARY CARRY
	uint8_t a_ = a & 0x0F;
	uint8_t b_ = b & 0x0F;
	uint8_t c_ = a_ + b_;
	flags.AuxiliaryCarry = c_ & 0x10;

	// change F 
	state.F=flags.getF();
}

void Utilities::RLC(State& state)
{
	auto& flags = state.Flags;

	// CARRY BIT
	flags.Carry = state.A & 0x80;
	state.A = std::rotl(state.A, 1);

	// change F 
	state.F=flags.getF();
}

void Utilities::RRC(State& state)
{
	auto& flags = state.Flags;

	// CARRY BIT
	flags.Carry = state.A & 0x01;
	state.A = std::rotr(state.A, 1);

	// change F 
	state.F=flags.getF();
}

void Utilities::RAL(State& state)
{
	auto& flags = state.Flags;

	uint8_t oldCarry = flags.Carry ? 1 : 0;

	state.A = std::rotl(state.A, 1);

	// CARRY BIT
	flags.Carry = state.A & 0b00000001;
	state.A = state.A & 0b11111110 | oldCarry;

	// change F 
	state.F=flags.getF();
}

void Utilities::RAR(State& state)
{
	auto& flags = state.Flags;

	uint8_t oldCarry = flags.Carry ? 1 : 0;

	state.A = std::rotr(state.A, 1);

	// CARRY BIT
	flags.Carry = state.A & 0b10000000;
	state.A = state.A & 0b01111111 | oldCarry << 7;

	// change F 
	state.F=flags.getF();
}

void Utilities::DAD(State& state, uint8_t& rh, uint8_t& rl)
{
	auto& flags = state.Flags;

	uint16_t p = ((uint16_t)rh) << 8 | ((uint16_t)rl);
	uint16_t hl = ((uint16_t)state.H) << 8 | ((uint16_t)state.L);

	uint32_t value = ((uint32_t)p) + ((uint32_t)hl);

	// CARRY BIT
	flags.Carry = value & 0b10000000000000000;
	state.H = (value & 0b1111111100000000) >> 8;
	state.L = value & 0b0000000011111111;
}

void Utilities::DAA(State& state)
{
	auto& flags = state.Flags;

	uint8_t a3_0 = state.A & 0b00001111;
	uint8_t a7_4 = (state.A & 0b11110000) >> 4;

	if (a3_0 > 9 || flags.AuxiliaryCarry)
	{
		a3_0 += 6;
	}

	if (a3_0 & 0b00010000)
	{
		flags.AuxiliaryCarry = true;
		a3_0 &= 0b00001111;
		a7_4++;
	}

	if (a7_4 > 9 || flags.Carry)
	{
		a7_4 += 6;
	}

	if (a7_4 & 0b00010000)
	{
		flags.Carry = true;
		a7_4 &= 0b00001111;
	}

	state.A = (a7_4 << 4) | a3_0;

	flags.Zero = state.A == 0;

	flags.Sign = state.A & 0x80;

	flags.Parity = Utilities::isOddParity(state.A);
}

void Utilities::ADD(State& state, const uint8_t value)
{
	auto& flags = state.Flags;

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)state.A);
	uint16_t b = ((uint16_t)value);
	uint16_t chp = a + b;

	// get result in normal precision
	state.A = chp & 0xFF;

	// ZERO CHECK
	flags.Zero = state.A == 0;

	// SIGN CHECK
	flags.Sign = chp & 0x80;

	// PARITY CHECK
	flags.Parity = Utilities::isOddParity(state.A);

	// CARRY BIT
	flags.Carry = chp & 0x100;

	// AUXILIARY CARRY
	uint8_t a_ = a & 0x0F;
	uint8_t b_ = b & 0x0F;
	uint8_t c_ = a_ + b_;
	flags.AuxiliaryCarry = c_ & 0x10;

	// change F 
	state.F=flags.getF();
}

void Utilities::ADC(State& state, const uint8_t value)
{
	auto& flags = state.Flags;

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)state.A);
	uint16_t b = ((uint16_t)value) + flags.Carry;
	uint16_t chp = a + b;

	// get result in normal precision
	state.A = chp & 0xFF;

	// ZERO CHECK
	flags.Zero = state.A == 0;

	// SIGN CHECK
	flags.Sign = chp & 0x80;

	// PARITY CHECK
	flags.Parity = Utilities::isOddParity(state.A);

	// CARRY BIT
	flags.Carry = chp & 0x100;

	// AUXILIARY CARRY
	uint8_t a_ = a & 0x0F;
	uint8_t b_ = b & 0x0F;
	uint8_t c_ = a_ + b_;
	flags.AuxiliaryCarry = c_ & 0x10;

	// change F 
	state.F=flags.getF();
}

void Utilities::SUB(State& state, const uint8_t value)
{
	auto& flags = state.Flags;

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)state.A);
	uint16_t b = ((uint16_t)(uint8_t(-value)));
	uint16_t chp = a + b;

	// get result in normal precision
	state.A = chp & 0xFF;

	// ZERO CHECK
	flags.Zero = state.A == 0;

	// SIGN CHECK
	flags.Sign = chp & 0x80;

	// PARITY CHECK
	flags.Parity = Utilities::isOddParity(state.A);

	// CARRY BIT
	flags.Carry = !(chp & 0x100);

	// AUXILIARY CARRY
	uint8_t a_ = a & 0x0F;
	uint8_t b_ = b & 0x0F;
	uint8_t c_ = a_ + b_;
	flags.AuxiliaryCarry = c_ & 0x10;

	// change F 
	state.F=flags.getF();
}

void Utilities::SBB(State& state, const uint8_t value)
{
	auto& flags = state.Flags;

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)state.A);
	uint16_t b = ((uint16_t)(uint8_t(-(value + flags.Carry))));
	uint16_t chp = a + b;

	// get result in normal precision
	state.A = chp & 0xFF;

	// ZERO CHECK
	flags.Zero = state.A == 0;

	// SIGN CHECK
	flags.Sign = chp & 0x80;

	// PARITY CHECK
	flags.Parity = Utilities::isOddParity(state.A);

	// CARRY BIT
	flags.Carry = !(chp & 0x100);

	// AUXILIARY CARRY
	uint8_t a_ = a & 0x0F;
	uint8_t b_ = b & 0x0F;
	uint8_t c_ = a_ + b_;
	flags.AuxiliaryCarry = c_ & 0x10;

	// change F 
	state.F=flags.getF();
}

void Utilities::ANA(State& state, const uint8_t value)
{
	auto& flags = state.Flags;

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)state.A);
	uint16_t b = ((uint16_t)value);
	uint16_t chp = a & b;

	// get result in normal precision
	state.A = chp & 0xFF;

	// ZERO CHECK
	flags.Zero = state.A == 0;

	// SIGN CHECK
	flags.Sign = chp & 0x80;

	// PARITY CHECK
	flags.Parity = Utilities::isOddParity(state.A);

	// CARRY BIT
	flags.Carry = false;

	// change F 
	state.F=flags.getF();
}

void Utilities::XRA(State& state, const uint8_t value)
{
	auto& flags = state.Flags;

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)state.A);
	uint16_t b = ((uint16_t)value);
	uint16_t chp = a ^ b;

	// get result in normal precision
	state.A = chp & 0xFF;

	// ZERO CHECK
	flags.Zero = state.A == 0;

	// SIGN CHECK
	flags.Sign = chp & 0x80;

	// PARITY CHECK
	flags.Parity = Utilities::isOddParity(state.A);

	// CARRY BIT
	flags.Carry = false;

	// AUXILIARY CARRY
	uint8_t a_ = a & 0x0F;
	uint8_t b_ = b & 0x0F;
	uint8_t c_ = a_ ^ b_;
	flags.AuxiliaryCarry = c_ & 0x10;

	// change F 
	state.F=flags.getF();
}
void Utilities::ORA(State& state, const uint8_t value)
{
	auto& flags = state.Flags;

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)state.A);
	uint16_t b = ((uint16_t)value);
	uint16_t chp = a | b;

	// get result in normal precision
	state.A = chp & 0xFF;

	// ZERO CHECK
	flags.Zero = state.A == 0;

	// SIGN CHECK
	flags.Sign = chp & 0x80;

	// PARITY CHECK
	flags.Parity = Utilities::isOddParity(state.A);

	// CARRY BIT
	flags.Carry = false;

	// change F 
	state.F=flags.getF();
}

void Utilities::CMP(State& state, const uint8_t value)
{
	auto& flags = state.Flags;

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)state.A);
	uint16_t b = ((uint16_t)(uint8_t(-value)));
	uint16_t chp = a + b;

	// get result in normal precision 
	auto tmp = chp & 0xFF;

	// ZERO CHECK
	flags.Zero = tmp == 0;

	// SIGN CHECK
	flags.Sign = chp & 0x80;

	// PARITY CHECK
	flags.Parity = Utilities::isOddParity(tmp);

	// CARRY BIT
	flags.Carry = !(chp & 0x100);

	// AUXILIARY CARRY
	uint8_t a_ = a & 0x0F;
	uint8_t b_ = b & 0x0F;
	uint8_t c_ = a_ + b_;
	flags.AuxiliaryCarry = c_ & 0x10;

	// change F 
	state.F=flags.getF();
}

void Utilities::SUI(State& state, const uint8_t value)
{
	auto& flags = state.Flags;

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)state.A);
	uint16_t b = ((uint16_t)(uint8_t(-value)));
	uint16_t chp = a + b;

	// get result in normal precision 
	state.A = chp & 0xFF;

	// ZERO CHECK
	flags.Zero = state.A == 0;

	// SIGN CHECK
	flags.Sign = chp & 0x80;

	// PARITY CHECK
	flags.Parity = Utilities::isOddParity(state.A);

	// CARRY BIT
	flags.Carry = !(chp & 0x100);

	// AUXILIARY CARRY
	uint8_t a_ = a & 0x0F;
	uint8_t b_ = b & 0x0F;
	uint8_t c_ = a_ + b_;
	flags.AuxiliaryCarry = c_ & 0x10;

	// change F 
	state.F=flags.getF();
}

void Utilities::SBI(State& state, const uint8_t value)
{
	auto& flags = state.Flags;

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)state.A);
	uint16_t b = ((uint16_t)(uint8_t(-value - flags.Carry)));
	uint16_t chp = a + b;

	// get result in normal precision 
	state.A = chp & 0xFF;

	// ZERO CHECK
	flags.Zero = state.A == 0;

	// SIGN CHECK
	flags.Sign = chp & 0x80;

	// PARITY CHECK
	flags.Parity = Utilities::isOddParity(state.A);

	// CARRY BIT
	flags.Carry = !(chp & 0x100);

	// AUXILIARY CARRY
	uint8_t a_ = a & 0x0F;
	uint8_t b_ = b & 0x0F;
	uint8_t c_ = a_ + b_;
	flags.AuxiliaryCarry = c_ & 0x10;

	// change F 
	state.F=flags.getF();
}

void Utilities::ORI(State& state, const uint8_t value)
{
	auto& flags = state.Flags;

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)state.A);
	uint16_t b = ((uint16_t)value);
	uint16_t chp = a | b;

	// get result in normal precision 
	state.A = chp & 0xFF;

	// ZERO CHECK
	flags.Zero = state.A == 0;

	// SIGN CHECK
	flags.Sign = chp & 0x80;

	// PARITY CHECK
	flags.Parity = Utilities::isOddParity(state.A);

	// CARRY BIT
	flags.Carry = false;

	// AUXILIARY CARRY
	flags.AuxiliaryCarry = false;

	// change F 
	state.F=flags.getF();
}

void Utilities::XRI(State& state, const uint8_t value)
{
	auto& flags = state.Flags;

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)state.A);
	uint16_t b = ((uint16_t)value);
	uint16_t chp = a ^ b;

	// get result in normal precision 
	state.A = chp & 0xFF;

	// ZERO CHECK
	flags.Zero = state.A == 0;

	// SIGN CHECK
	flags.Sign = chp & 0x80;

	// PARITY CHECK
	flags.Parity = Utilities::isOddParity(state.A);

	// CARRY BIT
	flags.Carry = false;

	// AUXILIARY CARRY
	flags.AuxiliaryCarry = false;

	// change F 
	state.F=flags.getF();
}

void Utilities::RST(State& state, const uint8_t num, MemoryMap& pMap)
{
	pMap.Poke(state.SP - 1, (state.PC & 0xFF00) >> 8);
	pMap.Poke(state.SP - 2, state.PC & 0x00FF);
	state.SP -= 2;
	state.PC = 8*num;
	state.Cycles += 11;
}