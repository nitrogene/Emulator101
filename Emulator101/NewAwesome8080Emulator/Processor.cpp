#include <fstream>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <fmt/core.h>
#include <thread>
#include <chrono>
#include "Processor.h"
#include "Utilities.h"

Processor::Processor(const std::filesystem::path& pathToInstructiosSet):m_InstructionSet(InstructionSet(pathToInstructiosSet))
{
}

void Processor::DisplayInstructionSet()
{
	std::cout << "formatted instruction set\n";

	std::cout << this->m_InstructionSet.DisplayInstructionSet();
}

void Processor::Hexdump(MemoryMapPart mmPart)
{
	p_MemoryMap->Hexdump(mmPart);
}

void Processor::LoadIntoBuffer(const std::filesystem::path& pathToRomFile, std::vector<uint8_t>& buffer)
{
	std::ifstream fs(pathToRomFile, std::ios_base::binary);

	if (fs.is_open())
	{
		auto n = (size_t)std::filesystem::file_size(pathToRomFile);
		auto offset = buffer.size();
		constexpr auto limit = std::numeric_limits<uint16_t>().max();

		if (n + offset > limit)
		{
			throw new std::exception("overflow");
		}

		buffer.resize(n+offset);
		fs.read((char*)buffer.data()+offset, n);
		fs.close();
		return;
	}

	throw new std::exception("unabe to open file");
}

void Processor::Initialize(const std::vector<std::filesystem::path>& pathToRomFiles, const uint16_t totalRam, const uint16_t workRamAddress, const uint16_t videoRamAddress, const uint16_t mirrorRamAddress, const std::vector<uint8_t>& bytes)
{
	std::vector<uint8_t> buffer(bytes);

	for (auto& pathToRomFile : pathToRomFiles)
	{
		LoadIntoBuffer(pathToRomFile, buffer);
	}

	this->p_MemoryMap = std::make_shared<MemoryMap>(buffer, totalRam, workRamAddress, videoRamAddress, mirrorRamAddress);
}

void Processor::DisassembleRomStacksize(const uint16_t offset, const uint16_t stackSize)
{
	auto pc = offset;
	auto count = 0;
	while (count < stackSize)
	{
		Disassemble(pc);
		count++;
	}
}

void Processor::Disassemble(uint16_t& pc)
{
	auto opCode = &p_MemoryMap->Peek(pc);
	uint16_t opbytes = 1;

	fmt::print("{0:04x}\t", pc);

	if (m_InstructionSet[opCode[0]]!=nullptr)
	{
		auto isl = m_InstructionSet[opCode[0]];
		auto instruction = isl->Mnemonic;
		if (isl->Mnemonic.ends_with("adr"))
		{
			unsigned  short d = (opCode[2] << 8) + opCode[1];
			instruction = fmt::format("{0}${1:04x}", instruction.substr(0, instruction.size() - 3), d);
		}
		else if (isl->Mnemonic.ends_with("D8"))
		{
			unsigned  short d = opCode[1];
			instruction = fmt::format("{0}#${1:02x}", instruction.substr(0, instruction.size() - 2), d);
		}
		else if (isl->Mnemonic.ends_with("D16"))
		{
			unsigned  short d = (opCode[2] << 8) + opCode[1];
			instruction = fmt::format("{0}#${1:02x}", instruction.substr(0, instruction.size() - 3), d);
		}
		fmt::print("{0:10}\n", instruction);
		opbytes = isl->Size;
	}
	else
	{
		fmt::print("-\n");
	}

	pc += opbytes;
}

void Processor::RunStep()
{
	auto opCode = &p_MemoryMap->Peek(m_State.PC);
	auto isl = m_InstructionSet[opCode[0]];

	if (isl != nullptr)
	{
		if (isl->exec != nullptr)
		{
			isl->exec(this->m_State, *this->p_MemoryMap, opCode,isl->Size, isl->ClockCycle);
		}
		else
		{	
			throw new std::exception("not implemented");
		}
	}
	else
	{
		throw new std::exception("opCode not in instruction set");
	}
}

void Processor::Run(const uint16_t stackSize, const uint64_t n)
{ 
	m_State.PC = 0;

	while (!m_State.HLT)
	{
		if (m_State.Steps % n==0)
		{
			this->ShowState(stackSize);
		}
		
		this->RunStep();
	}
}

void Processor::Run()
{
	m_State.PC = 0;

	while (!m_State.HLT)
	{
		this->RunStep();

		//std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

State& Processor::getState()
{
	return m_State;
}

void Processor::ShowState(const uint16_t stackSize)
{
	Utilities::ClearScreen();
	fmt::print("{0}", m_State.toString());
	fmt::print("Next {0} instructions in rom:\n", stackSize);
	this->DisassembleRomStacksize(m_State.PC, stackSize);
}

void Processor::setPC(const uint16_t pc)
{
	this->m_State.PC = pc;
}

const uint8_t& Processor::Peek(const uint16_t idx) const
{
	return p_MemoryMap->Peek(idx);
}

const InstructionSetLine& Processor::getIsl(const uint8_t idx) const
{
	return *m_InstructionSet[idx].get();
}

const MemoryMap& Processor::getMemoryMap()
{
	return *p_MemoryMap;
}