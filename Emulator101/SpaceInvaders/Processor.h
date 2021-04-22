#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <array>
#include <memory>

struct Registers
{
	// 8 bits registers
	unsigned char A=0, B=0, C=0, D=0, E=0;
};

struct InstructionSetLine
{
	std::string Mnemonic{};
	unsigned char Bits[8]{};
	std::string Periods{};
	unsigned short Size{};
	std::string Description;

	bool TryConvertBits(unsigned char& r);
};

class Processor
{
private:
	// Instruction set
	std::array<std::shared_ptr<InstructionSetLine>,256> InstructionSet{};

	// Registers
	Registers Registers{};

	// 16 bits Program counter
	unsigned short PC = 0;

	// 8 bits buffer
	std::vector<unsigned char> Rom{};

	// dissassemble instruction pointed by PC
	void disassemble(unsigned short& pc);

	// Load into buffer hopefully valid 8080 assembly code
	bool TryLoadIntoBuffer(const std::filesystem::path& pathToRomFile);

public:
	Processor() = delete;

	Processor(const std::filesystem::path& pathToInstructiosSet);

	void DisplayInstructionSet();

	bool TryLoadIntoBuffer(const std::vector<std::filesystem::path>& pathToRomFiles);

	// Hexadeximal dump of buffer
	void hexdump();

	// dissassemble instruction pointed by PC
	void disassembleRom(const unsigned short offset, const unsigned short size);

	// dissassemble instruction pointed by PC
	void run();

	unsigned short romSize();
};