#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <array>
#include <memory>
#include "InstructionSetLine.h"
#include "Registers.h"
#include "MemoryMap.h"
#include "Flags.h"


class Processor
{
private:
	// Instruction set
	std::array<std::shared_ptr<InstructionSetLine>,256> InstructionSet{};

	// Registers
	Registers Registers{};

	// Flags
	Flags Flags{};

	// 16 bits Program counter
	uint16_t PC = 0;

	// 16 bits stack pointer
	uint16_t SP = 0;

	uint64_t Cycles = 0;

	uint64_t Steps = 0;

	// MemoryMap
	std::shared_ptr<MemoryMap> p_MemoryMap;

	// dissassemble instruction pointed by PC
	void Disassemble(uint16_t& pc);

	// Load into buffer hopefully valid 8080 assembly code
	static void LoadIntoBuffer(const std::filesystem::path& pathToRomFile, std::vector<uint8_t>& buffer);

public:
	Processor() = delete;

	Processor(const std::filesystem::path& pathToInstructiosSet);

	void DisplayInstructionSet();

	void Initialize(const std::vector<std::filesystem::path>& pathToRomFiles, const uint16_t totalRam, const uint16_t workRamAddress, const uint16_t videoRamAddress, const uint16_t mirrorRamAddress);

	// Hexadeximal dump of buffer
	void Hexdump(MemoryMapPart mmPart);

	// dissassemble stackSize instructions starting at PC=offset
	void DisassembleRomStacksize(const uint16_t offset, const uint16_t stackSize);

	// run and show stackSize instruction at each step
	void Run(const uint16_t stackSize, const uint64_t n=1);

	// show processor state (registers, next stackSize instructions)
	void ShowState(const uint16_t stackSize);
};