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

void Utilities::setZeroSignAndParity(Flags& flags, uint8_t value)
{
	// ZERO CHECK
	flags.Zero = value == 0;

	// SIGN CHECK
	flags.Sign = value & 0x80;

	// PARITY CHECK
	flags.Parity = Utilities::isOddParity(value);
}

void Utilities::PushStack(State& state, const uint16_t value, MemoryMap& pMap)
{
	state.SP -= 2;
	pMap.Poke16(state.SP, value);
}

uint16_t Utilities::PopStack(State& state, MemoryMap& pMap)
{
	auto value = pMap.Peek16(state.SP);
	state.SP += 2;
	return value;
}


void Utilities::DCR(State& state, uint8_t& value)
{
	auto& flags = state.Flags;

	--value;
	// if result is equal to 0b00001111, that
	// means there was a carry out of bit 3
	flags.AuxiliaryCarry = (value & 0xF) != 0xF;
	setZeroSignAndParity(flags, value);
}

void Utilities::INR(State& state, uint8_t& value)
{
	auto& flags = state.Flags;

	++value;
	// if lower 4 bits are 0, that means that there was a 
	// carry out of bit 3
	flags.AuxiliaryCarry = (value & 0xF) == 0;
	setZeroSignAndParity(flags, value);
}

void Utilities::RLC(State& state)
{
	auto& flags = state.Flags;

	// CARRY BIT
	flags.Carry = state.A & 0x80;
	state.A = std::rotl(state.A, 1);
}

void Utilities::RRC(State& state)
{
	auto& flags = state.Flags;

	// CARRY BIT
	flags.Carry = state.A & 0x01;
	state.A = std::rotr(state.A, 1);
}

void Utilities::RAL(State& state)
{
	auto& flags = state.Flags;

	uint8_t oldCarry = flags.Carry ? 1 : 0;

	state.A = std::rotl(state.A, 1);

	// CARRY BIT
	flags.Carry = state.A & 0b00000001;
	state.A = state.A & 0b11111110 | oldCarry;
}

void Utilities::RAR(State& state)
{
	auto& flags = state.Flags;

	uint8_t oldCarry = flags.Carry ? 1 : 0;

	state.A = std::rotr(state.A, 1);

	// CARRY BIT
	flags.Carry = state.A & 0b10000000;
	state.A = state.A & 0b01111111 | oldCarry << 7;
}

void Utilities::DAD(State& state, const uint8_t h, const uint8_t l)
{
	auto& flags = state.Flags;

	uint16_t p = ((uint16_t)h) << 8 | ((uint16_t)l);
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

	setZeroSignAndParity(flags, state.A);
}

void Utilities::ADD(State& state, const uint8_t value, const bool carry)
{
	auto& flags = state.Flags;

	uint16_t chp = state.A + value + carry;
	uint8_t tmp = chp & 0xFF;
	setZeroSignAndParity(flags, tmp);
	flags.Carry = chp >> 8;
	flags.AuxiliaryCarry = (state.A ^ value ^ chp) & 0x10;

	state.A = tmp;
}

void Utilities::SUB(State& state, const uint8_t value, const bool carry)
{
	auto& flags = state.Flags;

	ADD(state, ~value, !carry);
	state.Flags.Carry = !state.Flags.Carry;
}

void Utilities::ANA(State& state, const uint8_t value)
{
	auto& flags = state.Flags;

	flags.Carry = false;
	flags.AuxiliaryCarry = (state.A | value) & 0x08;
	state.A &= value;
	setZeroSignAndParity(flags, state.A);
}

void Utilities::XRA(State& state, const uint8_t value)
{
	auto& flags = state.Flags;

	state.A ^= value;
	setZeroSignAndParity(flags, state.A);
	flags.Carry = false;
	flags.AuxiliaryCarry = false;
}

void Utilities::ORA(State& state, const uint8_t value)
{
	auto& flags = state.Flags;

	state.A |= value;
	setZeroSignAndParity(flags, state.A);
	flags.Carry = false;
	flags.AuxiliaryCarry = false;
}

void Utilities::CMP(State& state, const uint8_t value)
{
	auto& flags = state.Flags;

	uint16_t chp = state.A - value;
	flags.Carry = chp >> 8;
	flags.AuxiliaryCarry = ~(state.A ^ chp ^ value) & 0x10;
	uint8_t tmp = chp & 0xFF;
	setZeroSignAndParity(flags, tmp);
}

void Utilities::CALL(State& state, const uint16_t adr, MemoryMap& pMap)
{
	// For me PC is the program counter before executing step
	PushStack(state, state.PC+3, pMap);
	JMP(state, adr);
}

void Utilities::JMP(State& state, const uint16_t adr)
{
	state.PC = adr;
}

void Utilities::RET(State& state, MemoryMap& pMap)
{
	state.PC = PopStack(state, pMap);
}