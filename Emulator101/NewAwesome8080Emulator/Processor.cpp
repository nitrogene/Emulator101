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

	throw new std::exception("unable to open file");
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

	switch (opCode[0])
	{
	case 0x00:
	{
		// NOP
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x01:
	{
		// LXI B,D16 
		m_State.B = opCode[2];
		m_State.C = opCode[1];
		m_State.PC += 3;
		m_State.Cycles += 10;
		break;
	}
	case 0x02:
	{
		// STAX B 
		uint16_t adr = (m_State.B << 8) + m_State.C;
		p_MemoryMap->Poke(adr, m_State.A);
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0x03:
	{
		// INX B 
		uint16_t value = (m_State.B << 8) + m_State.C;
		value++;
		m_State.B = (value & 0b1111111100000000) >> 8;
		m_State.C = value & 0b0000000011111111;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x04:
	{
		// INR B 
		Utilities::INR(m_State, m_State.B);
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x05:
	{
		// DCR B 
		Utilities::DCR(m_State, m_State.B);
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x06:
	{
		// MVI B,D8 
		m_State.B = opCode[1];
		m_State.PC += 2;
		m_State.Cycles += 7;
		break;
	}
	case 0x07:
	{
		// RLC 
		Utilities::RLC(m_State);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x09:
	{
		// DAD B 
		Utilities::DAD(m_State, m_State.B, m_State.C);
		m_State.PC += 1;
		m_State.Cycles += 10;
		break;
	}
	case 0x0A:
	{
		// LDAX B  
		uint16_t adr = (m_State.B << 8) + m_State.C;
		m_State.A = p_MemoryMap->Peek(adr);
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0x0B:
	{
		// DCX B  
		uint16_t value = (m_State.B << 8) + m_State.C;
		value--;
		m_State.B = (value & 0b1111111100000000) >> 8;
		m_State.C = value & 0b0000000011111111;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x0C:
	{
		// INR C  
		Utilities::INR(m_State, m_State.C);
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x0D:
	{
		// DCR C  
		Utilities::DCR(m_State, m_State.C);
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x0E:
	{
		// MVI C,D8 
		m_State.C = opCode[1];
		m_State.PC += 2;
		m_State.Cycles += 7;
		break;
	}
	case 0x0F:
	{
		// RRC 
		Utilities::RRC(m_State);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x11:
	{
		// LXI D,D16 
		m_State.D = opCode[2];
		m_State.E = opCode[1];
		m_State.PC += 3;
		m_State.Cycles += 10;
		break;
	}
	case 0x12:
	{
		// STAX D 
		uint16_t adr = (m_State.D << 8) + m_State.E;
		p_MemoryMap->Poke(adr, m_State.A);
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0x13:
	{
		// INX D 
		uint16_t value = (m_State.D << 8) + m_State.E;
		value++;
		m_State.D = (value & 0b1111111100000000) >> 8;
		m_State.E = value & 0b0000000011111111;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x14:
	{
		// INR D 
		Utilities::INR(m_State, m_State.D);
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x15:
	{
		// DCR D 
		Utilities::DCR(m_State, m_State.D);
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x16:
	{
		// MVI D,D8
		m_State.D = opCode[1];
		m_State.PC += 2;
		m_State.Cycles += 7;
		break;
	}
	case 0x17:
	{
		// RAL
		Utilities::RAL(m_State);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x19:
	{
		// DAD D
		Utilities::DAD(m_State, m_State.D, m_State.E);
		m_State.PC += 1;
		m_State.Cycles += 10;
		break;
	}
	case 0x1A:
	{
		// LDAX D
		uint16_t adr = (m_State.D << 8) + m_State.E;
		m_State.A = p_MemoryMap->Peek(adr);
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0x1B:
	{
		// DCX D
		uint16_t value = (m_State.D << 8) + m_State.E;
		value--;
		m_State.D = (value & 0b1111111100000000) >> 8;
		m_State.E = value & 0b0000000011111111;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x1C:
	{
		// INR E
		Utilities::INR(m_State, m_State.E);
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x1D:
	{
		// DCR E
		Utilities::DCR(m_State, m_State.E);
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x1E:
	{
		// MVI E,D8
		m_State.E = opCode[1];
		m_State.PC += 2;
		m_State.Cycles += 7;
		break;
	}
	case 0x1F:
	{
		// RAR
		Utilities::RAR(m_State);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x21:
	{
		// LXI H,D16
		m_State.H = opCode[2];
		m_State.L = opCode[1];
		m_State.PC += 3;
		m_State.Cycles += 10;
		break;
	}
	case 0x22:
	{
		// SHLD adr
		uint16_t adr = (opCode[2] << 8) + opCode[1];
		p_MemoryMap->Poke(adr, m_State.L);
		p_MemoryMap->Poke(adr + 1, m_State.H);
		m_State.PC += 3;
		m_State.Cycles += 16;
		break;
	}
	case 0x23:
	{
		// INX H
		uint16_t value = (m_State.H << 8) + m_State.L;
		value++;
		m_State.H = (value & 0b1111111100000000) >> 8;
		m_State.L = value & 0b0000000011111111;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x24:
	{
		// INR H
		Utilities::INR(m_State, m_State.H);
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x25:
	{
		// DCR H
		Utilities::DCR(m_State, m_State.H);
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x26:
	{
		// MVI H,D8
		m_State.H = opCode[1];
		m_State.PC += 2;
		m_State.Cycles += 7;
		break;
	}
	case 0x27:
	{
		// DAA
		Utilities::DAA(m_State);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x29:
	{
		// DAD H
		Utilities::DAD(m_State, m_State.H, m_State.L);
		m_State.PC += 1;
		m_State.Cycles += 10;
		break;
	}
	case 0x2A:
	{
		// LHLD adr
		auto adr = (opCode[2] << 8) + opCode[1];
		m_State.L = p_MemoryMap->Peek(adr);
		m_State.H = p_MemoryMap->Peek(adr + 1);
		m_State.PC += 3;
		m_State.Cycles += 16;
		break;
	}
	case 0x2B:
	{
		// DCX H
		uint16_t value = (m_State.H << 8) + m_State.L;
		value--;
		m_State.H = (value & 0b1111111100000000) >> 8;
		m_State.L = value & 0b0000000011111111;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x2C:
	{
		// INR L
		Utilities::INR(m_State, m_State.L);
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x2D:
	{
		// DCR L
		Utilities::DCR(m_State, m_State.L);
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x2E:
	{
		// MVI L,D8
		m_State.L = opCode[1];
		m_State.PC += 2;
		m_State.Cycles += 7;
		break;
	}
	case 0x2F:
	{
		// CMA
		m_State.A = ~m_State.A;
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x31:
	{
		// LXI SP,D16
		m_State.SP = (opCode[2] << 8) + opCode[1];
		m_State.PC += 3;
		m_State.Cycles += 10;
		break;
	}
	case 0x32:
	{
		// STA adr
		auto adr = (opCode[2] << 8) + opCode[1];
		p_MemoryMap->Poke(adr, m_State.A);
		m_State.PC += 3;
		m_State.Cycles += 13;
		break;
	}
	case 0x33:
	{
		// INX SP
		m_State.SP += 1;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x34:
	{
		// INR M
		uint16_t adr = (m_State.H << 8) + m_State.L;
		auto value = p_MemoryMap->Peek(adr);
		Utilities::INR(m_State, value);
		p_MemoryMap->Poke(adr, value);
		m_State.PC += 1;
		m_State.Cycles += 10;
		break;
	}
	case 0x35:
	{
		// DCR M
		uint16_t adr = (m_State.H << 8) + m_State.L;
		auto value = p_MemoryMap->Peek(adr);
		Utilities::DCR(m_State, value);
		p_MemoryMap->Poke(adr, value);
		m_State.PC += 1;
		m_State.Cycles += 10;
		break;
	}
	case 0x36:
	{
		// MVI M,D8
		uint16_t adr = (m_State.H << 8) + m_State.L;
		p_MemoryMap->Poke(adr, opCode[1]);
		m_State.PC += 2;
		m_State.Cycles += 10;
		break;
	}
	case 0x37:
	{
		// STC
		m_State.Flags.Carry = true;
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x39:
	{
		// DAD SP
		uint8_t b = (m_State.SP & 0b1111111100000000) >> 8;
		uint8_t c = m_State.SP & 0b0000000011111111;
		Utilities::DAD(m_State, b, c);
		m_State.PC += 1;
		m_State.Cycles += 10;
		break;
	}
	case 0x3A:
	{
		// LDA adr
		uint16_t adr = (opCode[2] << 8) + opCode[1];
		m_State.A = p_MemoryMap->Peek(adr);
		m_State.PC += 3;
		m_State.Cycles += 13;
		break;
	}
	case 0x3B:
	{
		// DCX SP
		m_State.SP--;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x3C:
	{
		// INR A
		Utilities::INR(m_State, m_State.A);
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x3D:
	{
		// DCR A
		Utilities::DCR(m_State, m_State.A);
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x3E:
	{
		// MVI A,D8
		m_State.A = opCode[1];
		m_State.PC += 2;
		m_State.Cycles += 7;
		break;
	}
	case 0x3F:
	{
		// CMC
		m_State.Flags.Carry = !m_State.Flags.Carry;
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x40:
	{
		// MOV B,B
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x41:
	{
		// MOV B,C
		m_State.B = m_State.C;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x42:
	{
		// MOV B,D
		m_State.B = m_State.D;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x43:
	{
		// MOV B,E
		m_State.B = m_State.E;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x44:
	{
		// MOV B,H
		m_State.B = m_State.H;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x45:
	{
		// MOV B,L
		m_State.B = m_State.L;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x46:
	{
		// MOV B,M
		uint16_t adr = (m_State.H << 8) + m_State.L;
		m_State.B = p_MemoryMap->Peek(adr);
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0x47:
	{
		// MOV B,A
		m_State.B = m_State.A;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x48:
	{
		// MOV C,B
		m_State.C = m_State.B;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x49:
	{
		// MOV C,C
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x4A:
	{
		// MOV C,D
		m_State.C = m_State.D;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x4B:
	{
		// MOV C,E
		m_State.C = m_State.E;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x4C:
	{
		// MOV C,H
		m_State.C = m_State.H;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x4D:
	{
		// MOV C,L
		m_State.C = m_State.L;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x4E:
	{
		// MOV C,M
		uint16_t adr = (m_State.H << 8) + m_State.L;
		m_State.C = p_MemoryMap->Peek(adr);
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0x4F:
	{
		// MOV C,A
		m_State.C = m_State.A;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x50:
	{
		// MOV D,B
		m_State.D = m_State.B;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x51:
	{
		// MOV D,C
		m_State.D = m_State.C;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x52:
	{
		// MOV D,D
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x53:
	{
		// MOV D,E
		m_State.D = m_State.E;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x54:
	{
		// MOV D,H
		m_State.D = m_State.H;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x55:
	{
		// MOV D,L
		m_State.D = m_State.L;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x56:
	{
		// MOV D,M
		uint16_t adr = (m_State.H << 8) + m_State.L;
		m_State.D = p_MemoryMap->Peek(adr);
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0x57:
	{
		// MOV D,A
		m_State.D = m_State.A;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x58:
	{
		// MOV E,B
		m_State.E = m_State.B;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x59:
	{
		// MOV E,C
		m_State.E = m_State.C;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x5A:
	{
		// MOV E,D
		m_State.E = m_State.D;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x5B:
	{
		// MOV E,E
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x5C:
	{
		// MOV E,H
		m_State.E = m_State.H;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x5D:
	{
		// MOV E,L
		m_State.E = m_State.L;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x5E:
	{
		// MOV E,M
		uint16_t adr = (m_State.H << 8) + m_State.L;
		m_State.E = p_MemoryMap->Peek(adr);
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0x5F:
	{
		// MOV E,A
		m_State.E = m_State.A;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x60:
	{
		// MOV H,B
		m_State.H = m_State.B;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x61:
	{
		// MOV H,C
		m_State.H = m_State.C;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x62:
	{
		// MOV H,D
		m_State.H = m_State.D;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x63:
	{
		// MOV H,E
		m_State.H = m_State.E;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x64:
	{
		// MOV H,H
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x65:
	{
		// MOV H,L
		m_State.H = m_State.L;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x66:
	{
		// MOV H,M
		uint16_t adr = (m_State.H << 8) + m_State.L;
		m_State.H = p_MemoryMap->Peek(adr);
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0x67:
	{
		// MOV H,A
		m_State.H = m_State.A;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x68:
	{
		// MOV L,B
		m_State.L = m_State.B;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x69:
	{
		// MOV L,C
		m_State.L = m_State.C;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x6A:
	{
		// MOV L,D
		m_State.L = m_State.D;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x6B:
	{
		// MOV L,E
		m_State.L = m_State.E;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x6C:
	{
		// MOV L,H
		m_State.L = m_State.H;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x6D:
	{
		// MOV L,L
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x6E:
	{
		// MOV L,M
		uint16_t adr = (m_State.H << 8) + m_State.L;
		m_State.L = p_MemoryMap->Peek(adr);
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0x6F:
	{
		// MOV L,A
		m_State.L = m_State.A;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x70:
	{
		// MOV M,B
		uint16_t adr = (m_State.H << 8) + m_State.L;
		p_MemoryMap->Poke(adr, m_State.B);
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0x71:
	{
		// MOV M,C
		uint16_t adr = (m_State.H << 8) + m_State.L;
		p_MemoryMap->Poke(adr, m_State.C);
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0x72:
	{
		// MOV M,D
		uint16_t adr = (m_State.H << 8) + m_State.L;
		p_MemoryMap->Poke(adr, m_State.D);
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0x73:
	{
		// MOV M,E
		uint16_t adr = (m_State.H << 8) + m_State.L;
		p_MemoryMap->Poke(adr, m_State.E);
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0x74:
	{
		// MOV M,H
		uint16_t adr = (m_State.H << 8) + m_State.L;
		p_MemoryMap->Poke(adr, m_State.H);
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0x75:
	{
		// MOV M,L
		uint16_t adr = (m_State.H << 8) + m_State.L;
		p_MemoryMap->Poke(adr, m_State.L);
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0x76:
	{
		// HLT
		m_State.HLT = true;
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0x77:
	{
		// MOV M,A
		uint16_t adr = (m_State.H << 8) + m_State.L;
		p_MemoryMap->Poke(adr, m_State.A);
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0x78:
	{
		// MOV A,B
		m_State.A = m_State.B;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x79:
	{
		// MOV A,C
		m_State.A = m_State.C;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x7A:
	{
		// MOV A,D
		m_State.A = m_State.D;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x7B:
	{
		// MOV A,E
		m_State.A = m_State.E;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x7C:
	{
		// MOV A,H
		m_State.A = m_State.H;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x7D:
	{
		// MOV A,L
		m_State.A = m_State.L;
		m_State.PC += 1;
		m_State.Cycles += 5;
		break;
	}
	case 0x7E:
	{
		// MOV A,M
		uint16_t adr = (m_State.H << 8) + m_State.L;
		m_State.A = p_MemoryMap->Peek(adr);
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0x80:
	{
		// ADD B
		Utilities::ADD(m_State, m_State.B);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x81:
	{
		// ADD C
		Utilities::ADD(m_State, m_State.C);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x82:
	{
		// ADD D
		Utilities::ADD(m_State, m_State.D);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x83:
	{
		// ADD E
		Utilities::ADD(m_State, m_State.E);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x84:
	{
		// ADD H
		Utilities::ADD(m_State, m_State.H);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x85:
	{
		// ADD L
		Utilities::ADD(m_State, m_State.L);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x86:
	{
		// ADD M
		uint16_t adr = (m_State.H << 8) + m_State.L;
		Utilities::ADD(m_State, p_MemoryMap->Peek(adr));
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0x87:
	{
		// ADD A
		Utilities::ADD(m_State, m_State.A);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x88:
	{
		// ADC B
		Utilities::ADC(m_State, m_State.B);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x89:
	{
		// ADC C
		Utilities::ADC(m_State, m_State.C);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x8A:
	{
		// ADC D
		Utilities::ADC(m_State, m_State.D);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x8B:
	{
		// ADC E
		Utilities::ADC(m_State, m_State.E);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x8C:
	{
		// ADC H
		Utilities::ADC(m_State, m_State.H);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x8D:
	{
		// ADC L
		Utilities::ADC(m_State, m_State.L);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x8E:
	{
		// ADC M
		uint16_t adr = (m_State.H << 8) + m_State.L;
		Utilities::ADC(m_State, p_MemoryMap->Peek(adr));
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0x8F:
	{
		// ADC A
		Utilities::ADC(m_State, m_State.A);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x90:
	{
		// SUB B
		Utilities::SUB(m_State, m_State.B);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x91:
	{
		// SUB C
		Utilities::SUB(m_State, m_State.C);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x92:
	{
		// SUB D
		Utilities::SUB(m_State, m_State.D);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x93:
	{
		// SUB E
		Utilities::SUB(m_State, m_State.E);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x94:
	{
		// SUB H
		Utilities::SUB(m_State, m_State.H);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x95:
	{
		// SUB L
		Utilities::SUB(m_State, m_State.L);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x96:
	{
		// SUB M
		uint16_t adr = (m_State.H << 8) + m_State.L;
		Utilities::SUB(m_State, p_MemoryMap->Peek(adr));
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0x97:
	{
		// SUB A
		Utilities::SUB(m_State, m_State.A);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x98:
	{
		// SBB B
		Utilities::SBB(m_State, m_State.B);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x99:
	{
		// SBB C
		Utilities::SBB(m_State, m_State.C);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x9A:
	{
		// SBB D
		Utilities::SBB(m_State, m_State.D);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x9B:
	{
		// SBB E
		Utilities::SBB(m_State, m_State.E);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x9C:
	{
		// SBB H
		Utilities::SBB(m_State, m_State.H);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x9D:
	{
		// SBB L
		Utilities::SBB(m_State, m_State.L);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0x9E:
	{
		// SBB M
		uint16_t adr = (m_State.H << 8) + m_State.L;
		Utilities::SBB(m_State, p_MemoryMap->Peek(adr));
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0x9F:
	{
		// SBB A
		Utilities::SBB(m_State, m_State.A);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xA0:
	{
		// ANA B
		Utilities::ANA(m_State, m_State.B);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xA1:
	{
		// ANA C
		Utilities::ANA(m_State, m_State.C);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xA2:
	{
		// ANA D
		Utilities::ANA(m_State, m_State.D);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xA3:
	{
		// ANA E
		Utilities::ANA(m_State, m_State.E);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xA4:
	{
		// ANA H
		Utilities::ANA(m_State, m_State.H);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xA5:
	{
		// ANA L
		Utilities::ANA(m_State, m_State.L);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xA6:
	{
		// ANA M
		uint16_t adr = (m_State.H << 8) + m_State.L;
		Utilities::ANA(m_State, p_MemoryMap->Peek(adr));
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0xA7:
	{
		// ANA A
		Utilities::ANA(m_State, m_State.A);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xA8:
	{
		// XRA B
		Utilities::XRA(m_State, m_State.B);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xA9:
	{
		// XRA C
		Utilities::XRA(m_State, m_State.C);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xAA:
	{
		// XRA D
		Utilities::XRA(m_State, m_State.D);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xAB:
	{
		// XRA E
		Utilities::XRA(m_State, m_State.E);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xAC:
	{
		// XRA H
		Utilities::XRA(m_State, m_State.H);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xAD:
	{
		// XRA L
		Utilities::XRA(m_State, m_State.L);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xAE:
	{
		// XRA M
		uint16_t adr = (m_State.H << 8) + m_State.L;
		Utilities::XRA(m_State, p_MemoryMap->Peek(adr));
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0xAF:
	{
		// XRA A
		Utilities::XRA(m_State, m_State.A);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xB0:
	{
		// ORA B
		Utilities::ORA(m_State, m_State.B);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xB1:
	{
		// ORA C
		Utilities::ORA(m_State, m_State.C);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xB2:
	{
		// ORA D
		Utilities::ORA(m_State, m_State.D);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xB3:
	{
		// ORA E
		Utilities::ORA(m_State, m_State.E);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xB4:
	{
		// ORA H
		Utilities::ORA(m_State, m_State.H);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xB5:
	{
		// ORA L
		Utilities::ORA(m_State, m_State.L);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xB6:
	{
		// ORA M
		uint16_t adr = (m_State.H << 8) + m_State.L;
		Utilities::ORA(m_State, p_MemoryMap->Peek(adr));
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0xB7:
	{
		// ORA A
		Utilities::ORA(m_State, m_State.A);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xB8:
	{
		// CMP B
		Utilities::CMP(m_State, m_State.B);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xB9:
	{
		// CMP C
		Utilities::CMP(m_State, m_State.C);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xBA:
	{
		// CMP D
		Utilities::CMP(m_State, m_State.D);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xBB:
	{
		// CMP E
		Utilities::CMP(m_State, m_State.E);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xBC:
	{
		// CMP H
		Utilities::CMP(m_State, m_State.H);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xBD:
	{
		// CMP L
		Utilities::CMP(m_State, m_State.L);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xBE:
	{
		// CMP M
		uint16_t adr = (m_State.H << 8) + m_State.L;
		Utilities::CMP(m_State, p_MemoryMap->Peek(adr));
		m_State.PC += 1;
		m_State.Cycles += 7;
		break;
	}
	case 0xBF:
	{
		// CMP A
		Utilities::CMP(m_State, m_State.A);
		m_State.PC += 1;
		m_State.Cycles += 4;
		break;
	}
	case 0xC0:
	{
		// RNZ
		if (m_State.Flags.Zero)
		{
			m_State.PC += 1;
			m_State.Cycles += 4;
		}
		else
		{
			auto spc = p_MemoryMap->Peek(m_State.SP);
			auto spc1 = p_MemoryMap->Peek(m_State.SP + 1);
			m_State.PC = (spc1 << 8) + spc;
			m_State.SP += 2;
			m_State.Cycles += 11;
		}
		break;
	}
	case 0xC1:
	{
		// POP B
		m_State.B = p_MemoryMap->Peek(m_State.SP + 1);
		m_State.C = p_MemoryMap->Peek(m_State.SP);
		m_State.SP += 2;
		m_State.PC += 1;
		m_State.Cycles += 10;
		break;
	}
	case 0xC2:
	{
		// JNZ adr
		if (m_State.Flags.Zero)
		{
			m_State.PC += 3;
		}
		else
		{
			m_State.PC = (opCode[2] << 8) + opCode[1];
		}
		m_State.Cycles += 10;
		break;
	}
	case 0xC3:
	{
		// JMP adr
		m_State.PC = (opCode[2] << 8) + opCode[1];
		m_State.Cycles += 10;
		break;
	}
	case 0xC4:
	{
		// CNZ adr
		if (!m_State.Flags.Zero)
		{
			m_State.PC += 3;
			p_MemoryMap->Poke(m_State.SP - 1, (m_State.PC & 0xFF00) >> 8);
			p_MemoryMap->Poke(m_State.SP - 2, m_State.PC & 0x00FF);
			m_State.SP -= 2;
			m_State.PC = (opCode[2] << 8) + opCode[1];
			m_State.Cycles += 17;
		}
		else
		{
			m_State.PC += 3;
			m_State.Cycles += 11;
		}
		break;
	}
	case 0xC5:
	{
		// PUSH B
		p_MemoryMap->Poke(m_State.SP - 1, m_State.B);
		p_MemoryMap->Poke(m_State.SP - 2, m_State.C);
		m_State.PC += 1;
		m_State.SP -= 2;
		m_State.Cycles += 11;
		break;
	}
	case 0xC6:
	{
		// ADI D8
		Utilities::ADI(m_State, opCode[1]);
		m_State.PC += 2;
		m_State.Cycles += 7;
		break;
	}
	case 0xC8:
	{
		// RZ
		if (!m_State.Flags.Zero)
		{
			m_State.PC += 1;
			m_State.Cycles += 5;
		}
		else
		{
			auto spc = p_MemoryMap->Peek(m_State.SP);
			auto spc1 = p_MemoryMap->Peek(m_State.SP + 1);
			m_State.PC = (spc1 << 8) + spc;
			m_State.SP += 2;
			m_State.Cycles += 11;
		}
		break;
	}
	case 0xC9:
	{
		// RET
		auto spc = p_MemoryMap->Peek(m_State.SP);
		auto spc1 = p_MemoryMap->Peek(m_State.SP + 1);
		m_State.PC = (spc1 << 8) + spc;
		m_State.SP += 2;
		m_State.Cycles += 10;
		break;
	}
	case 0xCA:
	{
		// JZ adr
		if (m_State.Flags.Zero)
		{
			m_State.PC = (opCode[2] << 8) + opCode[1];
		}
		else
		{
			m_State.PC += 3;
		}
		m_State.Cycles += 10;
		break;
	}
	case 0xCC:
	{
		// CZ adr
		if (m_State.Flags.Zero)
		{
			m_State.PC += 3;
			p_MemoryMap->Poke(m_State.SP - 1, (m_State.PC & 0xFF00) >> 8);
			p_MemoryMap->Poke(m_State.SP - 2, m_State.PC & 0x00FF);
			m_State.SP -= 2;
			m_State.PC = (opCode[2] << 8) + opCode[1];
			m_State.Cycles += 17;
		}
		else
		{
			m_State.Cycles += 11;
			m_State.PC += 3;
		}

		break;
	}
	case 0xCD:
	{
		// CALL adr
		m_State.PC += 3;
		p_MemoryMap->Poke(m_State.SP - 1, (m_State.PC & 0xFF00) >> 8);
		p_MemoryMap->Poke(m_State.SP - 2, m_State.PC & 0x00FF);
		m_State.SP -= 2;
		m_State.PC = (opCode[2] << 8) + opCode[1];
		m_State.Cycles += 17;

		break;
	}
	case 0xCE:
	{
		// ACI D8
		Utilities::ACI(m_State, opCode[1]);
		m_State.PC += 2;
		m_State.Cycles += 7;

		break;
	}
	case 0xD0:
	{
		// RNC
		if (m_State.Flags.Carry)
		{
			m_State.PC += 1;
			m_State.Cycles += 5;
		}
		else
		{
			auto spc = p_MemoryMap->Peek(m_State.SP);
			auto spc1 = p_MemoryMap->Peek(m_State.SP + 1);
			m_State.PC = (spc1 << 8) + spc;
			m_State.SP += 2;
			m_State.Cycles += 11;
		}

		break;
	}
	case 0xD1:
	{
		// POP D
		m_State.D = p_MemoryMap->Peek(m_State.SP + 1);
		m_State.E = p_MemoryMap->Peek(m_State.SP);
		m_State.SP += 2;
		m_State.PC += 1;
		m_State.Cycles += 10;

		break;
	}
	case 0xD2:
	{
		// JNC adr
		if (m_State.Flags.Carry)
		{
			m_State.PC += 3;
		}
		else
		{
			m_State.PC = (opCode[2] << 8) + opCode[1];
		}
		m_State.Cycles += 10;

		break;
	}
	case 0xD3:
	{
		// OUT adr
		if (m_State.EI)
		{
			Utilities::machineOUT(m_State, opCode[1]);
		}
		m_State.PC += 2;
		m_State.Cycles += 10;

		break;
	}
	case 0xD4:
	{
		// CNC adr
		if (!m_State.Flags.Carry)
		{
			m_State.PC += 3;
			p_MemoryMap->Poke(m_State.SP - 1, (m_State.PC & 0xFF00) >> 8);
			p_MemoryMap->Poke(m_State.SP - 2, m_State.PC & 0x00FF);
			m_State.SP -= 2;
			m_State.PC = (opCode[2] << 8) + opCode[1];
			m_State.Cycles += 17;
		}
		else
		{
			m_State.PC += 3;
			m_State.Cycles += 11;
		}
		m_State.Steps++;

		break;
	}
	case 0xD5:
	{
		// PUSH D
		p_MemoryMap->Poke(m_State.SP - 1, m_State.D);
		p_MemoryMap->Poke(m_State.SP - 2, m_State.E);
		m_State.PC += 1;
		m_State.SP -= 2;
		m_State.Cycles += 11;
		break;
	}
	case 0xD6:
	{
		// SUI D8
		Utilities::SUI(m_State, opCode[1]);
		m_State.PC += 2;
		m_State.Cycles += 7;
		break;
	}
	case 0xD8:
	{
		// RC
		if (!m_State.Flags.Carry)
		{
			m_State.PC += 1;
			m_State.Cycles += 5;
		}
		else
		{
			auto spc = p_MemoryMap->Peek(m_State.SP);
			auto spc1 = p_MemoryMap->Peek(m_State.SP + 1);
			m_State.PC = (spc1 << 8) + spc;
			m_State.SP += 2;
			m_State.Cycles += 11;
		}

		break;
	}
	case 0xDA:
	{
		// JC adr
		if (m_State.Flags.Carry)
		{
			m_State.PC = (opCode[2] << 8) + opCode[1];
		}
		else
		{
			m_State.PC += 3;
		}
		m_State.Cycles += 10;

		break;
	}
	case 0xDB:
	{
		// IN D8
		if(m_State.EI)
		{ 
			Utilities::machineIN(m_State, opCode[1]);
		}
		m_State.PC += 2;
		m_State.Cycles += 10;

		break;
	}
	case 0xDC:
	{
		// CC adr
		if (m_State.Flags.Carry)
		{
			m_State.PC += 3;
			p_MemoryMap->Poke(m_State.SP - 1, (m_State.PC & 0xFF00) >> 8);
			p_MemoryMap->Poke(m_State.SP - 2, m_State.PC & 0x00FF);
			m_State.SP -= 2;
			m_State.PC = (opCode[2] << 8) + opCode[1];
			m_State.Cycles += 17;
		}
		else
		{
			m_State.PC += 3;
			m_State.Cycles += 11;
		}

		break;
	}
	case 0xDE:
	{
		// SBI D8
		Utilities::SBI(m_State, opCode[1]);
		m_State.PC += 2;
		m_State.Cycles += 7;

		break;
	case 0xEB:
	{
		// XCHG
		std::swap(m_State.H, m_State.D);
		std::swap(m_State.L, m_State.E);
		m_State.PC += 1;
		m_State.Cycles += 4;

		break;
	}
	case 0xE0:
	{
		// RPO
		if (m_State.Flags.Parity)
		{
			m_State.PC += 1;
			m_State.Cycles += 5;
		}
		else
		{
			auto spc = p_MemoryMap->Peek(m_State.SP);
			auto spc1 = p_MemoryMap->Peek(m_State.SP + 1);
			m_State.PC = (spc1 << 8) + spc;
			m_State.SP += 2;
			m_State.Cycles += 11;
		}

		break;
	}
	case 0xE1:
	{
		// POP H
		m_State.H = p_MemoryMap->Peek(m_State.SP + 1);
		m_State.L = p_MemoryMap->Peek(m_State.SP);
		m_State.SP += 2;
		m_State.PC += 1;
		m_State.Cycles += 10;

		break;
	}
	case 0xE2:
	{
		// JPO adr
		if (m_State.Flags.Parity)
		{
			m_State.PC += 3;
		}
		else
		{
			m_State.PC = (opCode[2] << 8) + opCode[1];
		}
		m_State.Cycles += 10;

		break;
	}
	case 0xE3:
	{
		// XTHL
		auto value = m_State.L;
		m_State.L = p_MemoryMap->Peek(m_State.SP);
		p_MemoryMap->Poke(m_State.SP, value);

		value = m_State.H;
		m_State.H = p_MemoryMap->Peek(m_State.SP + 1);
		p_MemoryMap->Poke(m_State.SP + 1, value);

		m_State.PC += 1;
		m_State.Cycles += 8;

		break;
	}
	case 0xE4:
	{
		// CPO adr
		if (m_State.Flags.Parity)
		{
			m_State.PC += 3;
			p_MemoryMap->Poke(m_State.SP - 1, (m_State.PC & 0xFF00) >> 8);
			p_MemoryMap->Poke(m_State.SP - 2, m_State.PC & 0x00FF);
			m_State.SP -= 2;
			m_State.PC = (opCode[2] << 8) + opCode[1];
			m_State.Cycles += 17;
		}
		else
		{
			m_State.PC += 3;
			m_State.Cycles += 11;
		}

		break;
	}
	case 0xE5:
	{
		// PUSH H
		p_MemoryMap->Poke(m_State.SP - 1, m_State.H);
		p_MemoryMap->Poke(m_State.SP - 2, m_State.L);
		m_State.PC += 1;
		m_State.SP -= 2;
		m_State.Cycles += 11;

		break;
	}
	case 0xE6:
	{
		// ANI D8
		Utilities::ANI(m_State, opCode[1]);
		m_State.PC += 2;
		m_State.Cycles += 7;

		break;
	}
	case 0xE8:
	{
		// RPE
		if (!m_State.Flags.Parity)
		{
			m_State.PC += 1;
			m_State.Cycles += 5;
		}
		else
		{
			auto spc = p_MemoryMap->Peek(m_State.SP);
			auto spc1 = p_MemoryMap->Peek(m_State.SP + 1);
			m_State.PC = (spc1 << 8) + spc;
			m_State.SP += 2;
			m_State.Cycles += 11;
		}	

		break;
	}
	case 0xE9:
	{
		// PCHL
		m_State.PC = (m_State.H << 8) + m_State.L;
		m_State.Cycles += 5;

		break;
	}
	case 0xEA:
	{
		// JPE adr
		if (m_State.Flags.Parity)
		{
			m_State.PC = (opCode[2] << 8) + opCode[1];
		}
		else
		{
			m_State.PC += 3;
		}
		m_State.Cycles += 10;

		break;
	}
	case 0xEC:
	{
		// CPE adr
		if (!m_State.Flags.Parity)
		{
			m_State.PC += 3;
			p_MemoryMap->Poke(m_State.SP - 1, (m_State.PC & 0xFF00) >> 8);
			p_MemoryMap->Poke(m_State.SP - 2, m_State.PC & 0x00FF);
			m_State.SP -= 2;
			m_State.PC = (opCode[2] << 8) + opCode[1];
			m_State.Cycles += 17;
		}
		else
		{
			m_State.PC += 3;
			m_State.Cycles += 11;
		}

		break;
	}
	case 0xEE:
	{
		// XRI D8
		Utilities::XRI(m_State, opCode[1]);
		m_State.PC += 2;
		m_State.Cycles += 7;

		break;
	}
	case 0xF0:
	{
		// RP
		if (m_State.Flags.Sign)
		{
			m_State.PC += 1;
			m_State.Cycles += 5;
		}
		else
		{
			auto spc = p_MemoryMap->Peek(m_State.SP);
			auto spc1 = p_MemoryMap->Peek(m_State.SP + 1);
			m_State.PC = (spc1 << 8) + spc;
			m_State.SP += 2;
			m_State.Cycles += 11;
		}

		break;
	}
	case 0xF1:
	{
		// POP PSW
		m_State.A = p_MemoryMap->Peek(m_State.SP + 1);
		m_State.F = p_MemoryMap->Peek(m_State.SP);
		m_State.Flags.setF(m_State.F);
		m_State.SP += 2;
		m_State.PC += 1;
		m_State.Cycles += 10;

		break;
	}
	case 0xF2:
	{
		// JP adr
		if (m_State.Flags.Sign)
		{
			m_State.PC += 3;
		}
		else
		{
			m_State.PC = (opCode[2] << 8) + opCode[1];
		}
		m_State.Cycles += 10;

		break;
	}
	case 0xF3:
	{
		// DI
		m_State.EI = false;
		m_State.PC += 1;
		m_State.Cycles += 4;

		break;
	}
	case 0xF4:
	{
		// CP adr
		if (!m_State.Flags.Sign)
		{
			m_State.PC += 3;
			p_MemoryMap->Poke(m_State.SP - 1, (m_State.PC & 0xFF00) >> 8);
			p_MemoryMap->Poke(m_State.SP - 2, m_State.PC & 0x00FF);
			m_State.SP -= 2;
			m_State.PC = (opCode[2] << 8) + opCode[1];
			m_State.Cycles += 17;
		}
		else
		{
			m_State.PC += 3;
			m_State.Cycles += 11;
		}

		break;
	}
	case 0xF5:
	{
		// PUSH PSW
		p_MemoryMap->Poke(m_State.SP - 1, m_State.A);
		p_MemoryMap->Poke(m_State.SP - 2, m_State.F);
		m_State.PC += 1;
		m_State.SP -= 2;
		m_State.Steps++;
		m_State.Cycles += 11;

		break;
	}
	case 0xF6:
	{
		// ORI D8
		Utilities::ORI(m_State, opCode[1]);
		m_State.PC += 2;
		m_State.Cycles += 7;

		break;
	}
	case 0xF8:
	{
		// RM
		if (!m_State.Flags.Sign)
		{
			m_State.PC += 1;
			m_State.Cycles += 5;
		}
		else
		{
			auto spc = p_MemoryMap->Peek(m_State.SP);
			auto spc1 = p_MemoryMap->Peek(m_State.SP + 1);
			m_State.PC = (spc1 << 8) + spc;
			m_State.SP += 2;
			m_State.Cycles += 11;
		}

		break;
	}
	case 0xFA:
	{
		// JM adr
		if (m_State.Flags.Sign)
		{
			m_State.PC = (opCode[2] << 8) + opCode[1];
		}
		else
		{
			m_State.PC += 3;
		}
		m_State.Cycles += 10;

		break;
	}
	case 0xFB:
	{
		// EI
		m_State.EI = true;
		m_State.PC += 1;
		m_State.Steps++;
		m_State.Cycles += 4;

		break;
	}
	case 0xFC:
	{
		// CM adr
		if (m_State.Flags.Sign)
		{
			m_State.PC += 3;
			p_MemoryMap->Poke(m_State.SP - 1, (m_State.PC & 0xFF00) >> 8);
			p_MemoryMap->Poke(m_State.SP - 2, m_State.PC & 0x00FF);
			m_State.SP -= 2;
			m_State.PC = (opCode[2] << 8) + opCode[1];
			m_State.Cycles += 17;
		}
		else
		{
			m_State.PC += 3;
			m_State.Cycles += 11;
		}

		break;
	}
	case 0xFE:
	{
		// CPI D8
		Utilities::CPI(m_State, opCode[1]);
		m_State.PC += 2;
		m_State.Cycles += 7;

		break;
	}

	default:
		throw new std::exception("not implemented");
	}
	
	m_State.Steps++;
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

void Processor::ShowState()
{
	fmt::print("{0}", m_State.toString());
	this->DisassembleRomStacksize(m_State.PC, 1);
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

 MemoryMap& Processor::getMemoryMap()
{
	return *p_MemoryMap;
}