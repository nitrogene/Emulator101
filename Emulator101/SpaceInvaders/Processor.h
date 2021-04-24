#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <array>
#include <memory>
#include "InstructionSetLine.h"
#include "Registers.h"
#include "MemoryMap.h"


class Processor
{
private:
	// Instruction set
	std::array<std::shared_ptr<InstructionSetLine>,256> InstructionSet{};

	// Registers
	Registers Registers{};

	// 16 bits Program counter
	unsigned short PC = 0;

	// 16 bits stack pointer
	unsigned short SP = 0;

	// MemoryMap
	std::shared_ptr<MemoryMap> p_MemoryMap;

	// dissassemble instruction pointed by PC
	void Disassemble(unsigned short& pc);

	// Load into buffer hopefully valid 8080 assembly code
	static void LoadIntoBuffer(const std::filesystem::path& pathToRomFile, std::vector<unsigned char>& buffer);

public:
	Processor() = delete;

	Processor(const std::filesystem::path& pathToInstructiosSet);

	void DisplayInstructionSet();

	void Initialize(const std::vector<std::filesystem::path>& pathToRomFiles, const unsigned short totalRam, const unsigned short workRamAddress, const unsigned short videoRamAddress, const unsigned short mirrorRamAddress);

	// Hexadeximal dump of buffer
	void Hexdump(MemoryMapPart mmPart);

	// dissassemble stackSize instructions starting at PC=offset
	void DisassembleRomStacksize(const unsigned short offset, const unsigned short stackSize);

	// run and show stackSize instruction at each step
	void Run(const unsigned short stackSize);

	// show processor state (registers, next stackSize instructions)
	void ShowState(const unsigned short stackSize);
};