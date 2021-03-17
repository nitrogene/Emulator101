#pragma once
#include <vector>

struct Registers
{
	char A=0, B=0, C=0, D=0, E=0;
};

class Processor
{
private:
	void addToBuffer(const char* filename);

public:
	// Registers
	Registers Registers{};

	// Program counter
	size_t PC=0;

	// Buffer
	std::vector<char> Buffer{};

	// Load into buffer hopefully valid 8080 assembly code
	static Processor loadIntoBuffer(const char* filename);

	static Processor loadIntoBuffer(const char* filenames[]);

	// hexDump file
	static void hexdump(const char* filename);

	int disassemble();
};