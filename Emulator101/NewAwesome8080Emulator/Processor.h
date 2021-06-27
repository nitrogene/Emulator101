#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <array>
#include <memory>
#include "InstructionSet.h"
#include "State.h"
#include "MemoryMap.h"

class Processor
{
private:
	// Instruction set
	InstructionSet m_InstructionSet;

	// Registers
	State m_State{};

	// MemoryMap
	std::shared_ptr<MemoryMap> p_MemoryMap;

	// dissassemble instruction pointed by PC
	void Disassemble(uint16_t& pc);

	// Load into buffer hopefully valid 8080 assembly code
	static void LoadIntoBuffer(const std::filesystem::path& pathToRomFile, std::vector<uint8_t>& buffer);

	std::function<void(State&, const uint8_t)> m_machineIN;
	std::function<void(State&, const uint8_t)> m_machineOUT;

	std::vector<uint8_t> m_OpCodeInterrupt{};

public:
	Processor() = delete;

	Processor(const std::filesystem::path& pathToInstructiosSet);

	void DisplayInstructionSet();

	// Load rom files into memory map
	void Initialize(const std::vector<std::filesystem::path>& pathToRomFiles, const uint16_t totalRam, const uint16_t workRamAddress, const uint16_t videoRamAddress, const uint16_t mirrorRamAddress, const std::vector<uint8_t>& bytes = {}, const bool allowWritingToRom=false);

	// Hexadeximal dump of buffer
	void Hexdump(MemoryMapPart mmPart);

	// dissassemble stackSize instructions starting at PC=offset
	void DisassembleRomStacksize(const uint16_t offset, const uint16_t stackSize);

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