#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <array>
#include <memory>
#include "InstructionSetLine.h"
#include "State.h"
#include "MemoryMap.h"

class Processor
{
private:
	// Instruction set
	std::array<std::shared_ptr<InstructionSetLine>, 256> InstructionSet{};

	// Registers
	State m_State{};

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