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

	return (y & 1) == 1;
}