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

void Utilities::DCR(uint8_t& value, State& state)
{
	// Perform operation in higher precision
	uint16_t a = ((uint16_t)value);
	uint16_t b = ((uint16_t)-1);
	uint16_t chp = a + b;
	// get result in normal precision
	value = chp & 0xFF;

	// ZERO CHECK
	// If the result of an instruction has the value 0, this flag is set; otherwise it is reset
	state.Z = value == 0;

	// SIGN CHECK
	// If the most significant bit of the result of	the operation has the value 1, this flag is	set; otherwise it is reset
	state.S = chp & 0x80;

	// PARITY CHECK
	// If the modulo 2 sum of the bits of the result of the operation is 0, (Le., if the result has even parity), this flag is set;
	// otherwise it is reset(Le., if the result has	odd parity)
	state.P = isOddParity(value);

	// AUXILIARY CARRY
	// 1 if a borrow happens between bits 3 & 4
	uint8_t a_ = a & 0x0F;
	uint8_t b_ = b & 0x0F;
	uint8_t c_ = a_ + b_;
	state.AC = c_ & 0x10;
}

bool Utilities::isOddParity(const uint8_t value)
{
	// https://www.geeksforgeeks.org/finding-the-parity-of-a-number-efficiently/
	uint8_t y = value ^ (value >> 1);
	y = y ^ (y >> 2);
	y = y ^ (y >> 4);

	return (y & 1) == 1;
}