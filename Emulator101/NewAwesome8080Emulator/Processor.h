#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <filesystem>
#include <array>
#include <memory>
#include <functional>
#include "State.h"
#include "MemoryMap.h"
#include "InstructionSetLine.h"

class Processor
{
private:
	// Instruction set
	std::array<InstructionSetLine, std::numeric_limits<uint8_t>::max() + 1> m_InstructionSet{};

	// Registers
	State m_State{};

	// MemoryMap
	MemoryMap m_MemoryMap;

	void LoadInstructionSet(const std::filesystem::path& pathToInstructionSet);

	// dissassemble instruction pointed by PC
	void Disassemble(uint16_t& pc, std::ostream& outs = std::cout);

	// Load into buffer hopefully valid 8080 assembly code
	static void LoadIntoBuffer(const std::filesystem::path& pathToRomFile, std::vector<uint8_t>& buffer);

	void incrementeProgramCounter(const uint8_t* opCodes);
	void handleOpcode(const uint8_t* opCodes);

	std::function<uint8_t(const uint8_t, void* data)> m_IN;
	std::function<void(const uint8_t, void* data)> m_OUT;

	std::array<uint8_t,3> m_OpCodeInterrupt;
	bool m_Interrupt = false;
	void* p_Data = nullptr;

public:
	Processor() = delete;

	Processor(const std::filesystem::path& pathToInstructiosSet);

	// Load rom files into memory map
	void Initialize(const std::vector<std::filesystem::path>& pathToRomFiles, const uint16_t totalMemorySize =0xFFFF, const std::vector<uint8_t>& bytes = {}, const bool allowWritingToRom=false);
	void Initialize(const std::vector<uint8_t>& rom, const uint16_t totalMemorySize = 0xFFFF, const bool allowWritingToRom = false);

	// Hexadeximal dump of buffer
	void Hexdump();

	// dissassemble stackSize instructions starting at PC=offset
	void DisassembleRomStacksize(const uint16_t offset, const uint16_t stackSize, std::ostream& outs = std::cout);

	// run one step and return
	void RunStep();

	// show processor state (registers, next stackSize instructions)
	void ShowState(const uint16_t stackSize);
	void ShowState();

	// Return current step
	State& getState();

	void setPC(const uint16_t pc);

	const InstructionSetLine& getIsl(const uint8_t idx) const;

	MemoryMap& getMemoryMap();

	// IN / OUT
	void setIN(std::function<uint8_t(const uint8_t, void* data)> a_IN) { m_IN = a_IN; }
	void setOUT(std::function<void(const uint8_t, void* data)> a_OUT) { m_OUT= a_OUT; }
	void setData(void* data) { p_Data = data; }

	void setInterrupt(const std::array<uint8_t, 3>& opCode);

};
