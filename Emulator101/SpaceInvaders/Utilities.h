#pragma once
#include <string>
#include <vector>

class Utilities
{
public:
	static void ClearScreen();
	static std::vector<std::string> Split(const std::string& s, char delimiter);
};

