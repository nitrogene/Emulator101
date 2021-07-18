#include <fstream>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <fmt/core.h>
#include <thread>
#include <chrono>
#include "Processor.h"
#include "Utilities.h"

using namespace std::chrono_literals;

Processor::Processor(const std::filesystem::path& pathToInstructionSet)
{
	LoadInstructionSet(pathToInstructionSet);
}

void Processor::LoadInstructionSet(const std::filesystem::path& pathToInstructionSet)
{
	std::ifstream fs(pathToInstructionSet);

	if (fs.is_open())
	{
		std::string line;

		// skip header row
		std::getline(fs, line);

		// read line by line
		for (line; std::getline(fs, line); )
		{
			// OpCode;Mnemonic;Description;Cycles;Size
			auto t = Utilities::Split(line, ';');
			InstructionSetLine isl;
			auto opCode = (uint8_t)std::stoul(t[0],nullptr,16);
			isl.Mnemonic = t[1];
			if (isl.Mnemonic.compare("-") != 0)
			{
				isl.Description = t[2];
				auto cycles = Utilities::Split(t[3], '/');
				isl.ClockCycle.A = (uint8_t)std::atoi(cycles[0].c_str());
				if (cycles.size() > 1)
				{
					isl.ClockCycle.B = (uint8_t)std::atoi(cycles[1].c_str());
				}
				isl.Size = (uint8_t)atoi(t[4].c_str());
			}
			m_InstructionSet[opCode] = isl;
		}
	}
}

void Processor::Hexdump()
{
	m_MemoryMap.Hexdump();
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

	throw new std::exception("unable to open file");
}

void Processor::Initialize(const std::vector<std::filesystem::path>& pathToRomFiles, const uint16_t totalMemorySize, const std::vector<uint8_t>& bytes, const bool allowWritingToRom)
{
	std::vector<uint8_t> buffer(bytes);

	for (auto& pathToRomFile : pathToRomFiles)
	{
		LoadIntoBuffer(pathToRomFile, buffer);
	}

	m_MemoryMap = MemoryMap(buffer, totalMemorySize, allowWritingToRom);
}

void Processor::Initialize(const std::vector<uint8_t>& rom, const uint16_t totalMemorySize, const bool allowWritingToRom)
{
	m_MemoryMap = MemoryMap(rom, totalMemorySize, allowWritingToRom);
}

void Processor::DisassembleRomStacksize(const uint16_t offset, const uint16_t stackSize, std::ostream& outs)
{
	auto pc = offset;
	auto count = 0;
	while (count < stackSize)
	{
		Disassemble(pc, outs);
		count++;
	}
}

void Processor::Disassemble(uint16_t& pc, std::ostream& outs)
{
	auto opCode = m_MemoryMap.getOpCode(pc);
	uint16_t opbytes = 1;

	outs << fmt::format("{0:04x}\t", pc);

	if (m_InstructionSet[opCode[0]].Mnemonic.compare("-")!=0)
	{
		auto isl = m_InstructionSet[opCode[0]];
		auto instruction = isl.Mnemonic;
		if (isl.Mnemonic.ends_with("adr"))
		{
			unsigned  short d = Utilities::getWordFromBytes(opCode[2], opCode[1]);
			instruction = fmt::format("{0}${1:04x}", instruction.substr(0, instruction.size() - 3), d);
		}
		else if (isl.Mnemonic.ends_with("D8"))
		{
			unsigned  short d = opCode[1];
			instruction = fmt::format("{0}#${1:02x}", instruction.substr(0, instruction.size() - 2), d);
		}
		else if (isl.Mnemonic.ends_with("D16"))
		{
			unsigned  short d = Utilities::getWordFromBytes(opCode[2], opCode[1]);
			instruction = fmt::format("{0}#${1:02x}", instruction.substr(0, instruction.size() - 3), d);
		}
		outs << fmt::format("{0:10}\n", instruction);
		opbytes = isl.Size;
	}
	else
	{
		outs << fmt::format("-\n");
	}

	pc += opbytes;
}

void Processor::RunStep()
{
	auto opCodes = m_MemoryMap.getOpCode(m_State.PC);
	auto isl = getIsl(opCodes[0]);
	m_AllStates.push_back(std::make_pair(m_State, isl));
	if (m_AllStates.size() > 1000)
	{
		m_AllStates.pop_front();
	}

	if (m_OpCodeInterrupt.size()>0)
	{
		opCodes = m_OpCodeInterrupt.data();
	}

	m_State.Steps++;

	auto adr = Utilities::getWordFromBytes(opCodes[2], opCodes[1]);
	auto d8 = opCodes[1];
	
	switch (opCodes[0])
	{
	case 0x00:
	{
		// NOP
		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x01:
	{
		// LXI B,D16 
		Utilities::setBytesFromWord(adr, m_State.B, m_State.C);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x02:
	{
		// STAX B 
		m_MemoryMap.Poke(Utilities::getWordFromBytes(m_State.B, m_State.C), m_State.A);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x03:
	{
		// INX B 
		uint16_t value = Utilities::getWordFromBytes(m_State.B, m_State.C);
		value++;
		Utilities::setBytesFromWord(value, m_State.B, m_State.C);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x04:
	{
		// INR B 
		Utilities::INR(m_State, m_State.B);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x05:
	{
		// DCR B 
		Utilities::DCR(m_State, m_State.B);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x06:
	{
		// MVI B,D8 
		m_State.B = d8;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x07:
	{
		// RLC 
		Utilities::RLC(m_State);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x09:
	{
		// DAD B 
		Utilities::DAD(m_State, m_State.B, m_State.C);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x0A:
	{
		// LDAX B  
		m_State.A = m_MemoryMap.Peek(Utilities::getWordFromBytes(m_State.B, m_State.C));

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x0B:
	{
		// DCX B  
		auto value = Utilities::getWordFromBytes(m_State.B, m_State.C);
		value--;
		Utilities::setBytesFromWord(value, m_State.B, m_State.C);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x0C:
	{
		// INR C  
		Utilities::INR(m_State, m_State.C);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x0D:
	{
		// DCR C  
		Utilities::DCR(m_State, m_State.C);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x0E:
	{
		// MVI C,D8 
		m_State.C = d8;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x0F:
	{
		// RRC 
		Utilities::RRC(m_State);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x11:
	{
		// LXI D,D16 
		Utilities::setBytesFromWord(adr, m_State.D, m_State.E);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x12:
	{
		// STAX D 
		m_MemoryMap.Poke(Utilities::getWordFromBytes(m_State.D, m_State.E), m_State.A);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x13:
	{
		// INX D 
		uint16_t value = Utilities::getWordFromBytes(m_State.D, m_State.E);
		value++;
		Utilities::setBytesFromWord(value, m_State.D, m_State.E);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x14:
	{
		// INR D 
		Utilities::INR(m_State, m_State.D);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x15:
	{
		// DCR D 
		Utilities::DCR(m_State, m_State.D);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x16:
	{
		// MVI D,D8
		m_State.D = d8;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x17:
	{
		// RAL
		Utilities::RAL(m_State);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x19:
	{
		// DAD D
		Utilities::DAD(m_State, m_State.D, m_State.E);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x1A:
	{
		// LDAX D
		m_State.A = m_MemoryMap.Peek(Utilities::getWordFromBytes(m_State.D, m_State.E));

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x1B:
	{
		// DCX D
		auto value = Utilities::getWordFromBytes(m_State.D, m_State.E);
		value--;
		Utilities::setBytesFromWord(value, m_State.D, m_State.E);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x1C:
	{
		// INR E
		Utilities::INR(m_State, m_State.E);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x1D:
	{
		// DCR E
		Utilities::DCR(m_State, m_State.E);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x1E:
	{
		// MVI E,D8
		m_State.E = d8;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x1F:
	{
		// RAR
		Utilities::RAR(m_State);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x21:
	{
		// LXI H,D16
		Utilities::setBytesFromWord(adr, m_State.H, m_State.L);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x22:
	{
		// SHLD adr
		auto hl= Utilities::getWordFromBytes(m_State.H, m_State.L);
		m_MemoryMap.Poke16(adr, hl);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x23:
	{
		// INX H
		uint16_t value = Utilities::getWordFromBytes(m_State.H, m_State.L);
		value++;
		Utilities::setBytesFromWord(value, m_State.H, m_State.L);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x24:
	{
		// INR H
		Utilities::INR(m_State, m_State.H);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x25:
	{
		// DCR H
		Utilities::DCR(m_State, m_State.H);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x26:
	{
		// MVI H,D8
		m_State.H = d8;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x27:
	{
		// DAA
		Utilities::DAA(m_State);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x29:
	{
		// DAD H
		Utilities::DAD(m_State, m_State.H, m_State.L);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x2A:
	{
		// LHLD adr
		auto hl=m_MemoryMap.Peek16(adr);
		Utilities::setBytesFromWord(hl, m_State.H, m_State.L);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x2B:
	{
		// DCX H
		Utilities::setBytesFromWord(Utilities::getWordFromBytes(m_State.H, m_State.L)-1, m_State.H, m_State.L);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x2C:
	{
		// INR L
		Utilities::INR(m_State, m_State.L);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x2D:
	{
		// DCR L
		Utilities::DCR(m_State, m_State.L);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x2E:
	{
		// MVI L,D8
		m_State.L = d8;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x2F:
	{
		// CMA
		m_State.A = ~m_State.A;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x31:
	{
		// LXI SP,D16
		m_State.SP = adr;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x32:
	{
		// STA adr
		m_MemoryMap.Poke(adr, m_State.A);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x33:
	{
		// INX SP
		m_State.SP += 1;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x34:
	{
		// INR M
		auto madr = Utilities::getWordFromBytes(m_State.H, m_State.L);
		auto value = m_MemoryMap.Peek(madr);
		Utilities::INR(m_State, value);
		m_MemoryMap.Poke(madr, value);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x35:
	{
		// DCR M
		auto madr = Utilities::getWordFromBytes(m_State.H, m_State.L);
		auto value = m_MemoryMap.Peek(madr);
		Utilities::DCR(m_State, value);
		m_MemoryMap.Poke(madr, value);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x36:
	{
		// MVI M,D8
		m_MemoryMap.Poke(Utilities::getWordFromBytes(m_State.H, m_State.L), d8);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x37:
	{
		// STC
		m_State.Flags.Carry = true;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x39:
	{
		// DAD SP
		uint8_t b=0, c=0;
		Utilities::setBytesFromWord(m_State.SP, b, c);
		Utilities::DAD(m_State, b, c);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x3A:
	{
		// LDA adr
		m_State.A = m_MemoryMap.Peek(adr);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x3B:
	{
		// DCX SP
		m_State.SP--;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x3C:
	{
		// INR A
		Utilities::INR(m_State, m_State.A);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x3D:
	{
		// DCR A
		Utilities::DCR(m_State, m_State.A);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x3E:
	{
		// MVI A,D8
		m_State.A = d8;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x3F:
	{
		// CMC
		m_State.Flags.Carry = !m_State.Flags.Carry;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x40:
	{
		// MOV B,B
		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x41:
	{
		// MOV B,C
		m_State.B = m_State.C;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x42:
	{
		// MOV B,D
		m_State.B = m_State.D;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x43:
	{
		// MOV B,E
		m_State.B = m_State.E;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x44:
	{
		// MOV B,H
		m_State.B = m_State.H;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x45:
	{
		// MOV B,L
		m_State.B = m_State.L;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x46:
	{
		// MOV B,M
		m_State.B = m_MemoryMap.Peek(Utilities::getWordFromBytes(m_State.H, m_State.L));

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x47:
	{
		// MOV B,A
		m_State.B = m_State.A;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x48:
	{
		// MOV C,B
		m_State.C = m_State.B;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x49:
	{
		// MOV C,C

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x4A:
	{
		// MOV C,D
		m_State.C = m_State.D;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x4B:
	{
		// MOV C,E
		m_State.C = m_State.E;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x4C:
	{
		// MOV C,H
		m_State.C = m_State.H;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x4D:
	{
		// MOV C,L
		m_State.C = m_State.L;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x4E:
	{
		// MOV C,M
		m_State.C = m_MemoryMap.Peek(Utilities::getWordFromBytes(m_State.H, m_State.L));

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x4F:
	{
		// MOV C,A
		m_State.C = m_State.A;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x50:
	{
		// MOV D,B
		m_State.D = m_State.B;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x51:
	{
		// MOV D,C
		m_State.D = m_State.C;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x52:
	{
		// MOV D,D
		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x53:
	{
		// MOV D,E
		m_State.D = m_State.E;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x54:
	{
		// MOV D,H
		m_State.D = m_State.H;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x55:
	{
		// MOV D,L
		m_State.D = m_State.L;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x56:
	{
		// MOV D,M
		m_State.D = m_MemoryMap.Peek(Utilities::getWordFromBytes(m_State.H, m_State.L));

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x57:
	{
		// MOV D,A
		m_State.D = m_State.A;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x58:
	{
		// MOV E,B
		m_State.E = m_State.B;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x59:
	{
		// MOV E,C
		m_State.E = m_State.C;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x5A:
	{
		// MOV E,D
		m_State.E = m_State.D;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x5B:
	{
		// MOV E,E
		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x5C:
	{
		// MOV E,H
		m_State.E = m_State.H;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x5D:
	{
		// MOV E,L
		m_State.E = m_State.L;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x5E:
	{
		// MOV E,M
		m_State.E = m_MemoryMap.Peek(Utilities::getWordFromBytes(m_State.H, m_State.L));

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x5F:
	{
		// MOV E,A
		m_State.E = m_State.A;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x60:
	{
		// MOV H,B
		m_State.H = m_State.B;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x61:
	{
		// MOV H,C
		m_State.H = m_State.C;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x62:
	{
		// MOV H,D
		m_State.H = m_State.D;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x63:
	{
		// MOV H,E
		m_State.H = m_State.E;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x64:
	{
		// MOV H,H
		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x65:
	{
		// MOV H,L
		m_State.H = m_State.L;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x66:
	{
		// MOV H,M
		m_State.H = m_MemoryMap.Peek(Utilities::getWordFromBytes(m_State.H, m_State.L));

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x67:
	{
		// MOV H,A
		m_State.H = m_State.A;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x68:
	{
		// MOV L,B
		m_State.L = m_State.B;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x69:
	{
		// MOV L,C
		m_State.L = m_State.C;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x6A:
	{
		// MOV L,D
		m_State.L = m_State.D;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x6B:
	{
		// MOV L,E
		m_State.L = m_State.E;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x6C:
	{
		// MOV L,H
		m_State.L = m_State.H;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x6D:
	{
		// MOV L,L
		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x6E:
	{
		// MOV L,M
		m_State.L = m_MemoryMap.Peek(Utilities::getWordFromBytes(m_State.H, m_State.L));

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x6F:
	{
		// MOV L,A
		m_State.L = m_State.A;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x70:
	{
		// MOV M,B
		m_MemoryMap.Poke(Utilities::getWordFromBytes(m_State.H, m_State.L), m_State.B);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x71:
	{
		// MOV M,C
		m_MemoryMap.Poke(Utilities::getWordFromBytes(m_State.H, m_State.L), m_State.C);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x72:
	{
		// MOV M,D
		m_MemoryMap.Poke(Utilities::getWordFromBytes(m_State.H, m_State.L), m_State.D);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x73:
	{
		// MOV M,E
		m_MemoryMap.Poke(Utilities::getWordFromBytes(m_State.H, m_State.L), m_State.E);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x74:
	{
		// MOV M,H
		m_MemoryMap.Poke(Utilities::getWordFromBytes(m_State.H, m_State.L), m_State.H);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x75:
	{
		// MOV M,L
		m_MemoryMap.Poke(Utilities::getWordFromBytes(m_State.H, m_State.L), m_State.L);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x76:
	{
		// HLT
		m_State.HLT = true;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x77:
	{
		// MOV M,A
		m_MemoryMap.Poke(Utilities::getWordFromBytes(m_State.H, m_State.L), m_State.A);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x78:
	{
		// MOV A,B
		m_State.A = m_State.B;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x79:
	{
		// MOV A,C
		m_State.A = m_State.C;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x7A:
	{
		// MOV A,D
		m_State.A = m_State.D;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x7B:
	{
		// MOV A,E
		m_State.A = m_State.E;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x7C:
	{
		// MOV A,H
		m_State.A = m_State.H;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x7D:
	{
		// MOV A,L
		m_State.A = m_State.L;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x7E:
	{
		// MOV A,M
		m_State.A = m_MemoryMap.Peek(Utilities::getWordFromBytes(m_State.H, m_State.L));

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x7F:
	{
		// MOV A,A
		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x80:
	{
		// ADD B
		Utilities::ADD(m_State, m_State.B,0);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x81:
	{
		// ADD C
		Utilities::ADD(m_State, m_State.C,0);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x82:
	{
		// ADD D
		Utilities::ADD(m_State, m_State.D,0);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x83:
	{
		// ADD E
		Utilities::ADD(m_State, m_State.E,0);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x84:
	{
		// ADD H
		Utilities::ADD(m_State, m_State.H,0);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x85:
	{
		// ADD L
		Utilities::ADD(m_State, m_State.L,0);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x86:
	{
		// ADD M
		Utilities::ADD(m_State, m_MemoryMap.Peek(Utilities::getWordFromBytes(m_State.H, m_State.L)),0);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x87:
	{
		// ADD A
		Utilities::ADD(m_State, m_State.A,0);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x88:
	{
		// ADC B
		Utilities::ADD(m_State, m_State.B, m_State.Flags.Carry);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x89:
	{
		// ADC C
		Utilities::ADD(m_State, m_State.C, m_State.Flags.Carry);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x8A:
	{
		// ADC D
		Utilities::ADD(m_State, m_State.D, m_State.Flags.Carry);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x8B:
	{
		// ADC E
		Utilities::ADD(m_State, m_State.E, m_State.Flags.Carry);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x8C:
	{
		// ADC H
		Utilities::ADD(m_State, m_State.H, m_State.Flags.Carry);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x8D:
	{
		// ADC L
		Utilities::ADD(m_State, m_State.L, m_State.Flags.Carry);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x8E:
	{
		// ADC M
		Utilities::ADD(m_State, m_MemoryMap.Peek(Utilities::getWordFromBytes(m_State.H, m_State.L)), m_State.Flags.Carry);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x8F:
	{
		// ADC A
		Utilities::ADD(m_State, m_State.A, m_State.Flags.Carry);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x90:
	{
		// SUB B
		Utilities::SUB(m_State, m_State.B,0);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x91:
	{
		// SUB C
		Utilities::SUB(m_State, m_State.C, 0);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x92:
	{
		// SUB D
		Utilities::SUB(m_State, m_State.D, 0);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x93:
	{
		// SUB E
		Utilities::SUB(m_State, m_State.E, 0);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x94:
	{
		// SUB H
		Utilities::SUB(m_State, m_State.H, 0);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x95:
	{
		// SUB L
		Utilities::SUB(m_State, m_State.L, 0);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x96:
	{
		// SUB M
		Utilities::SUB(m_State, m_MemoryMap.Peek(Utilities::getWordFromBytes(m_State.H, m_State.L)), 0);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x97:
	{
		// SUB A
		Utilities::SUB(m_State, m_State.A, 0);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x98:
	{
		// SBB B
		Utilities::SUB(m_State, m_State.B,m_State.Flags.Carry);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x99:
	{
		// SBB C
		Utilities::SUB(m_State, m_State.C, m_State.Flags.Carry);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x9A:
	{
		// SBB D
		Utilities::SUB(m_State, m_State.D, m_State.Flags.Carry);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x9B:
	{
		// SBB E
		Utilities::SUB(m_State, m_State.E, m_State.Flags.Carry);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x9C:
	{
		// SBB H
		Utilities::SUB(m_State, m_State.H, m_State.Flags.Carry);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x9D:
	{
		// SBB L
		Utilities::SUB(m_State, m_State.L, m_State.Flags.Carry);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x9E:
	{
		// SBB M
		Utilities::SUB(m_State, m_MemoryMap.Peek(Utilities::getWordFromBytes(m_State.H, m_State.L)), m_State.Flags.Carry);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0x9F:
	{
		// SBB A
		Utilities::SUB(m_State, m_State.A, m_State.Flags.Carry);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xA0:
	{
		// ANA B
		Utilities::ANA(m_State, m_State.B);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xA1:
	{
		// ANA C
		Utilities::ANA(m_State, m_State.C);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xA2:
	{
		// ANA D
		Utilities::ANA(m_State, m_State.D);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xA3:
	{
		// ANA E
		Utilities::ANA(m_State, m_State.E);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xA4:
	{
		// ANA H
		Utilities::ANA(m_State, m_State.H);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xA5:
	{
		// ANA L
		Utilities::ANA(m_State, m_State.L);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xA6:
	{
		// ANA M
		Utilities::ANA(m_State, m_MemoryMap.Peek(Utilities::getWordFromBytes(m_State.H, m_State.L)));

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xA7:
	{
		// ANA A
		Utilities::ANA(m_State, m_State.A);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xA8:
	{
		// XRA B
		Utilities::XRA(m_State, m_State.B);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xA9:
	{
		// XRA C
		Utilities::XRA(m_State, m_State.C);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xAA:
	{
		// XRA D
		Utilities::XRA(m_State, m_State.D);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xAB:
	{
		// XRA E
		Utilities::XRA(m_State, m_State.E);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xAC:
	{
		// XRA H
		Utilities::XRA(m_State, m_State.H);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xAD:
	{
		// XRA L
		Utilities::XRA(m_State, m_State.L);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xAE:
	{
		// XRA M
		Utilities::XRA(m_State, m_MemoryMap.Peek(Utilities::getWordFromBytes(m_State.H, m_State.L)));

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xAF:
	{
		// XRA A
		Utilities::XRA(m_State, m_State.A);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xB0:
	{
		// ORA B
		Utilities::ORA(m_State, m_State.B);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xB1:
	{
		// ORA C
		Utilities::ORA(m_State, m_State.C);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xB2:
	{
		// ORA D
		Utilities::ORA(m_State, m_State.D);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xB3:
	{
		// ORA E
		Utilities::ORA(m_State, m_State.E);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xB4:
	{
		// ORA H
		Utilities::ORA(m_State, m_State.H);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xB5:
	{
		// ORA L
		Utilities::ORA(m_State, m_State.L);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xB6:
	{
		// ORA M
		Utilities::ORA(m_State, m_MemoryMap.Peek(Utilities::getWordFromBytes(m_State.H, m_State.L)));

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xB7:
	{
		// ORA A
		Utilities::ORA(m_State, m_State.A);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xB8:
	{
		// CMP B
		Utilities::CMP(m_State, m_State.B);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xB9:
	{
		// CMP C
		Utilities::CMP(m_State, m_State.C);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xBA:
	{
		// CMP D
		Utilities::CMP(m_State, m_State.D);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xBB:
	{
		// CMP E
		Utilities::CMP(m_State, m_State.E);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xBC:
	{
		// CMP H
		Utilities::CMP(m_State, m_State.H);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xBD:
	{
		// CMP L
		Utilities::CMP(m_State, m_State.L);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xBE:
	{
		// CMP M
		Utilities::CMP(m_State, m_MemoryMap.Peek(Utilities::getWordFromBytes(m_State.H, m_State.L)));

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xBF:
	{
		// CMP A
		Utilities::CMP(m_State, m_State.A);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xC0:
	{
		// RNZ
		if (!m_State.Flags.Zero)
		{
			Utilities::RET(m_State, m_MemoryMap);
			m_State.Cycles += isl.ClockCycle.B;
		}
		else
		{
			m_State.PC += isl.Size;
			m_State.Cycles += isl.ClockCycle.A;
		}
		
		break;
	}
	case 0xC1:
	{
		// POP B
		auto bc = Utilities::PopStack(m_State, m_MemoryMap);
		Utilities::setBytesFromWord(bc, m_State.B, m_State.C);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xC2:
	{
		// JNZ adr
		if (!m_State.Flags.Zero)
		{
			Utilities::JMP(m_State, adr);
		}
		else 
		{
			m_State.PC += isl.Size;
		}

		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xC3:
	{
		// JMP adr
		Utilities::JMP(m_State, adr);

		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xC4:
	{
		// CNZ adr
		if (!m_State.Flags.Zero)
		{
			Utilities::CALL(m_State, adr, m_MemoryMap);
			m_State.Cycles += isl.ClockCycle.B;
		}
		else
		{
			m_State.PC += isl.Size;
			m_State.Cycles += isl.ClockCycle.A;
		}
		
		break;
	}
	case 0xC5:
	{
		// PUSH B
		auto bc = Utilities::getWordFromBytes(m_State.B, m_State.C);
		Utilities::PushStack(m_State, bc, m_MemoryMap);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xC6:
	{
		// ADI D8
		Utilities::ADD(m_State, d8,0);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xC7:
	{
		// RST 0
		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;

		Utilities::CALL(m_State, 0x00, m_MemoryMap);
		break;
	}
	case 0xC8:
	{
		// RZ
		if (m_State.Flags.Zero)
		{
			Utilities::RET(m_State, m_MemoryMap);
			m_State.Cycles += isl.ClockCycle.B;
		}
		else
		{
			m_State.PC += isl.Size;
			m_State.Cycles += isl.ClockCycle.A;
		}
		break;
	}
	case 0xC9:
	{
		// RET
		Utilities::RET(m_State, m_MemoryMap);

		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xCA:
	{
		// JZ adr
		if (m_State.Flags.Zero)
		{
			Utilities::JMP(m_State, adr);
		}
		else
		{
			m_State.PC += isl.Size;
		}
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xCC:
	{
		// CZ adr
		if (m_State.Flags.Zero)
		{
			Utilities::CALL(m_State, adr, m_MemoryMap);
			m_State.Cycles += isl.ClockCycle.B;
		}
		else 
		{
			m_State.PC += isl.Size;
			m_State.Cycles += isl.ClockCycle.A;
		}

		break;
	}
	case 0xCD:
	{
		// CALL adr
		Utilities::CALL(m_State, adr, m_MemoryMap);

		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xCE:
	{
		// ACI D8
		Utilities::ADD(m_State, d8, m_State.Flags.Carry);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xCF:
	{
		// RST 1
		Utilities::CALL(m_State, 0x08, m_MemoryMap);

		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xD0:
	{
		// RNC
		if (!m_State.Flags.Carry)
		{
			Utilities::RET(m_State, m_MemoryMap);
			m_State.Cycles += isl.ClockCycle.B;
		}
		else
		{
			m_State.PC += isl.Size;
			m_State.Cycles += isl.ClockCycle.A;
		}

		break;
	}
	case 0xD1:
	{
		// POP D
		auto de = Utilities::PopStack(m_State, m_MemoryMap);
		Utilities::setBytesFromWord(de, m_State.D, m_State.E);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xD2:
	{
		// JNC adr
		if (!m_State.Flags.Carry)
		{
			Utilities::JMP(m_State, adr);
		}
		else
		{
			m_State.PC += isl.Size;
		}
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xD3:
	{
		// OUT adr

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;

		if (m_machineOUT!=nullptr)
		{
			m_machineOUT(m_State, d8);
		}
		break;
	}
	case 0xD4:
	{
		// CNC adr
		if (!m_State.Flags.Carry)
		{
			Utilities::CALL(m_State, adr, m_MemoryMap);
			m_State.Cycles += isl.ClockCycle.B;
		}
		else
		{
			m_State.PC += isl.Size;
			m_State.Cycles += isl.ClockCycle.A;
		}

		break;
	}
	case 0xD5:
	{
		// PUSH D
		auto de = Utilities::getWordFromBytes(m_State.D, m_State.E);
		Utilities::PushStack(m_State, de, m_MemoryMap);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xD6:
	{
		// SUI D8
		Utilities::SUB(m_State, d8,0);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xD7:
	{
		// RST 2
		Utilities::CALL(m_State, 0x10, m_MemoryMap);

		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xD8:
	{
		// RC
		if (m_State.Flags.Carry)
		{
			Utilities::RET(m_State, m_MemoryMap);
			m_State.Cycles += isl.ClockCycle.B;
		}
		else
		{
			m_State.PC += isl.Size;
			m_State.Cycles += isl.ClockCycle.A;
		}

		break;
	}
	case 0xDA:
	{
		// JC adr
		if (m_State.Flags.Carry)
		{
			Utilities::JMP(m_State, adr);
		}
		else
		{
			m_State.PC += isl.Size;
		}
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xDB:
	{
		// IN D8
		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		if(m_machineIN!=nullptr)
		{ 
			m_machineIN(m_State, d8);
		}
		break;
	}
	case 0xDC:
	{
		// CC adr
		if (m_State.Flags.Carry)
		{
			Utilities::CALL(m_State, adr, m_MemoryMap);
			m_State.Cycles += isl.ClockCycle.B;
		}
		else
		{
			m_State.PC += isl.Size;
			m_State.Cycles += isl.ClockCycle.A;
		}
		break;
	}
	case 0xDE:
	{
		// SBI D8
		Utilities::SUB(m_State, d8,m_State.Flags.Carry);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xDF:
	{
		// RST 3
		Utilities::CALL(m_State, 0x18, m_MemoryMap);

		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xEB:
	{
		// XCHG
		std::swap(m_State.H, m_State.D);
		std::swap(m_State.L, m_State.E);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xE0:
	{
		// RPO
		if (!m_State.Flags.Parity)
		{
			Utilities::RET(m_State, m_MemoryMap);
			m_State.Cycles += isl.ClockCycle.B;
		}
		else
		{
			m_State.PC += isl.Size;
			m_State.Cycles += isl.ClockCycle.A;
		}
		
		break;
	}
	case 0xE1:
	{
		// POP H
		auto hl = Utilities::PopStack(m_State, m_MemoryMap);
		Utilities::setBytesFromWord(hl, m_State.H, m_State.L);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xE2:
	{
		// JPO adr
		if (!m_State.Flags.Parity)
		{
			Utilities::JMP(m_State, adr);
		}
		else
		{
			m_State.PC += isl.Size;
		}

		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xE3:
	{
		// XTHL
		auto value = m_State.L;
		m_State.L = m_MemoryMap.Peek(m_State.SP);
		m_MemoryMap.Poke(m_State.SP, value);

		value = m_State.H;
		m_State.H = m_MemoryMap.Peek(m_State.SP + 1);
		m_MemoryMap.Poke(m_State.SP + 1, value);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xE4:
	{
		// CPO adr
		if (!m_State.Flags.Parity)
		{
			Utilities::CALL(m_State, adr, m_MemoryMap);
			m_State.Cycles += isl.ClockCycle.B;
		}
		else
		{
			m_State.PC += isl.Size;
			m_State.Cycles += isl.ClockCycle.A;
		}
		
		break;
	}
	case 0xE5:
	{
		// PUSH H
		auto hl = Utilities::getWordFromBytes(m_State.H, m_State.L);
		Utilities::PushStack(m_State, hl, m_MemoryMap);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xE6:
	{
		// ANI D8
		Utilities::ANA(m_State,d8);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xE7:
	{
		// RST 4
		Utilities::CALL(m_State, 0x20, m_MemoryMap);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xE8:
	{
		// RPE
		if (m_State.Flags.Parity)
		{
			Utilities::RET(m_State, m_MemoryMap);
			m_State.Cycles += isl.ClockCycle.B;
		}	
		else
		{
			m_State.PC += isl.Size;
			m_State.Cycles += isl.ClockCycle.A;
		}
		break;
	}
	case 0xE9:
	{
		// PCHL
		m_State.PC = Utilities::getWordFromBytes(m_State.H, m_State.L);

		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xEA:
	{
		// JPE adr
		if (m_State.Flags.Parity)
		{
			Utilities::JMP(m_State, adr);
		}
		else
		{
			m_State.PC += isl.Size;
		}
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xEC:
	{
		// CPE adr
		if (m_State.Flags.Parity)
		{
			Utilities::CALL(m_State, adr, m_MemoryMap);
			m_State.Cycles += isl.ClockCycle.B;
		}
		else
		{
			m_State.PC += isl.Size;
			m_State.Cycles += isl.ClockCycle.A;
		}
		
		break;
	}
	case 0xEE:
	{
		// XRI D8
		Utilities::XRA(m_State, d8);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xEF:
	{
		// RST 5
		Utilities::CALL(m_State, 0x28, m_MemoryMap);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xF0:
	{
		// RP
		if (!m_State.Flags.Sign)
		{
			Utilities::RET(m_State, m_MemoryMap);
			m_State.Cycles += isl.ClockCycle.B;
		}
		else
		{
			m_State.PC += isl.Size;
			m_State.Cycles += isl.ClockCycle.A;
		}
		
		break;
	}
	case 0xF1:
	{
		// POP PSW
		auto af = Utilities::PopStack(m_State, m_MemoryMap);
		uint8_t f;
		Utilities::setBytesFromWord(af, m_State.A, f);
		m_State.Flags.setF(f);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xF2:
	{
		// JP adr
		if (!m_State.Flags.Sign)
		{
			m_State.PC = adr;
		}
		else
		{
			m_State.PC += isl.Size;
		}
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xF3:
	{
		// DI
		m_State.EI = false;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xF4:
	{
		// CP adr
		if (!m_State.Flags.Sign)
		{
			Utilities::CALL(m_State, adr, m_MemoryMap);
			m_State.Cycles += isl.ClockCycle.B;
		}
		else
		{
			m_State.PC += isl.Size;
			m_State.Cycles += isl.ClockCycle.A;
		}
		
		break;
	}
	case 0xF5:
	{
		// PUSH PSW
		auto f = m_State.Flags.getF();
		Utilities::PushStack(m_State, m_State.A << 8 | f,m_MemoryMap);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xF6:
	{
		// ORI D8
		Utilities::ORA(m_State, d8);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xF7:
	{
		// RST 6
		Utilities::CALL(m_State, 0x30, m_MemoryMap);

		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xF8:
	{
		// RM
		if (m_State.Flags.Sign)
		{
			Utilities::RET(m_State, m_MemoryMap);
			m_State.Cycles += isl.ClockCycle.B;
		}
		else
		{
			m_State.PC += isl.Size;
			m_State.Cycles += isl.ClockCycle.A;
		}
		
		break;
	}
	case 0xFA:
	{
		// JM adr
		if (m_State.Flags.Sign)
		{
			Utilities::JMP(m_State, adr);
		}
		else
		{
			m_State.PC += isl.Size;
		}
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xFB:
	{
		// EI
		m_State.EI = true;

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xFC:
	{
		// CM adr
		if (m_State.Flags.Sign)
		{
			Utilities::CALL(m_State, adr, m_MemoryMap);
			m_State.Cycles += isl.ClockCycle.B;
		}
		else
		{
			m_State.PC += isl.Size;
			m_State.Cycles += isl.ClockCycle.A;
		}
		
		break;
	}
	case 0xFE:
	{
		// CPI D8 (same as CMP)
		Utilities::CMP(m_State, d8);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xF9:
	{
		// SPHL
		m_State.SP= Utilities::getWordFromBytes(m_State.H, m_State.L);

		m_State.PC += isl.Size;
		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	case 0xFF:
	{
		// RST 7
		Utilities::CALL(m_State, 0x38, m_MemoryMap);

		m_State.Cycles += isl.ClockCycle.A;
		break;
	}
	default:
		throw new std::exception("not implemented");
	}

	m_OpCodeInterrupt.clear();
}

void Processor::Run(std::function<void(void)> preProcessFunc, std::function<void(void)> postProcessFunc)
{
	m_State.PC = 0;

	// Nombre de cycles par "image" @60fps
	uint64_t n = 33333;

	auto c0 = m_State.Cycles;
	auto t0 = std::chrono::high_resolution_clock::now();


	while (!m_State.HLT)
	{
		if (preProcessFunc)
		{
			preProcessFunc();
		}

		this->RunStep();

		auto c1 = m_State.Cycles;

		if (postProcessFunc)
		{
			postProcessFunc();
		}

		if ((c1 - c0) >= n)
		{
			auto t1 = std::chrono::high_resolution_clock::now();

			auto tsim = (c1 - c0) * 500ns;
			auto trea = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0);

			std::this_thread::sleep_for(tsim - trea);

			c0 = c1;
			t0 = t1;
		}
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

void Processor::ShowState()
{
	fmt::print("{0}", m_State.toString());
}

void Processor::setPC(const uint16_t pc)
{
	this->m_State.PC = pc;
}

const InstructionSetLine& Processor::getIsl(const uint8_t idx) const
{
	return m_InstructionSet[idx];
}

 MemoryMap& Processor::getMemoryMap()
{
	return m_MemoryMap;
}

 void Processor::setInterrupt(const std::vector<uint8_t> opCodeInterrupt)
 {
	 if (!m_State.EI)
	 {
		 return;
	 }

	 if (opCodeInterrupt.size() == 0 || opCodeInterrupt.size() > 3)
	 {
		 throw std::runtime_error("Invalid interrupt opCode");
	 }

	 m_State.EI = false;
	 m_OpCodeInterrupt = opCodeInterrupt;
 }