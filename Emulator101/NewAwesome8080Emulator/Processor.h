#pragma once
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

	std::function<void(State&, const uint8_t)> m_machineIN;
	std::function<void(State&, const uint8_t)> m_machineOUT;

	std::vector<uint8_t> m_OpCodeInterrupt{};

	std::deque<std::pair<State,InstructionSetLine>> m_AllStates;

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

	// run until not HLT
	void Run(std::function<void(void)> preProcessFunc=nullptr, std::function<void(void)> postProcessFunc = nullptr);

	// run one step and return
	void RunStep();

	// show processor state (registers, next stackSize instructions)
	void ShowState(const uint16_t stackSize);
	void ShowState();

	// Return current step
	State& getState();

	void setPC(const uint16_t pc);

	// Return the memory content at address =idx
	const uint8_t& Peek(const uint16_t idx) const;

	const InstructionSetLine& getIsl(const uint8_t idx) const;

	MemoryMap& getMemoryMap();

	// IN / OUT
	void setMachineIN(std::function<void(State&, const uint8_t)> machineIN) { m_machineIN = machineIN; }
	void setMachineOUT(std::function<void(State&, const uint8_t)> machineOUT) { m_machineOUT = machineOUT; }

	void setInterrupt(const std::vector<uint8_t> opCodeInterrupt);

};