#include <fstream>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <fmt/core.h>
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
	// TODO: Create ROM "read only" throw exception whenever we write in rom part or out of ram
		// Simplify
	auto opCode = &p_MemoryMap->Peek(m_State.PC);
	auto isl = m_InstructionSet[opCode[0]];
	this->m_State.Cycles += isl->ClockCycle.A;
	this->m_State.Steps++;


	if (isl != nullptr)
	{
		// TODO: replace by script stored in nstruction set ???
		switch (opCode[0])
		{
		case 0x00:
		{
			// NOP
			m_State.PC += isl->Size;
			break;
		}

		case 0x01:
		{
			// LXI B,D16 
			this->m_State.B = opCode[2];
			this->m_State.C = opCode[1];
			m_State.PC += isl->Size;
			break;
		}

		case 0x02:
		{
			// STAX B 
			uint16_t adr = (this->m_State.B << 8) + this->m_State.C;
			p_MemoryMap->Poke(adr, this->m_State.A);
			m_State.PC += isl->Size;
			break;
		}

		case 0x03:
		{
			// INX B
			this->m_State.C += 1;
			if (this->m_State.C == 0)
			{
				this->m_State.B += 1;
			}

			m_State.PC += isl->Size;
			break;
		}

		case 0x05:
		{
			// DCR B
			this->m_State.DCR(this->m_State.B);
			m_State.PC += isl->Size;
			break;
		}

		case 0x06:
			// MVI B, D8
			this->m_State.B = opCode[1];
			m_State.PC += isl->Size;
			break;

		case 0x0d:
		{
			// DCR C
			this->m_State.DCR(this->m_State.C);
			m_State.PC += isl->Size;
			break;
		}

		case 0x11:
			// LXI D,16
			this->m_State.D = opCode[2];
			this->m_State.E = opCode[1];
			m_State.PC += isl->Size;
			break;

		case 0x13:
		{
			// INX D
			this->m_State.E += 1;
			if (this->m_State.E == 0)
			{
				this->m_State.D += 1;
			}
			m_State.PC += isl->Size;
			break;
		}

		case 0x15:
		{
			// DCR D
			this->m_State.DCR(this->m_State.D);
			m_State.PC += isl->Size;
			break;
		}

		case 0x1A:
		{
			// LDAX D
			uint16_t adr = (this->m_State.D << 8) + this->m_State.E;
			this->m_State.A = p_MemoryMap->Peek(adr);
			m_State.PC += isl->Size;
		}
		break;

		case 0x1D:
		{
			// DCR E
			this->m_State.DCR(this->m_State.E);
			m_State.PC += isl->Size;
			break;
		}

		case 0x21:
			// LXI H,16
			this->m_State.H = opCode[2];
			this->m_State.L = opCode[1];
			m_State.PC += isl->Size;
			break;

		case 0x23:
		{
			// INX H
			this->m_State.L += 1;
			if (this->m_State.L == 0)
			{
				this->m_State.H += 1;
			}
			m_State.PC += isl->Size;
			break;
		}

		case 0x25:
		{
			// DCR H
			this->m_State.DCR(this->m_State.H);
			m_State.PC += isl->Size;
			break;
		}

		case 0x2D:
		{
			// DCR L
			this->m_State.DCR(this->m_State.L);
			m_State.PC += isl->Size;
			break;
		}

		case 0x31:
			// LXI SP,D16
			m_State.SP = (opCode[2] << 8) + opCode[1];
			m_State.PC += isl->Size;
			break;

		case 0x33:
		{
			// INX SP
			m_State.SP += 1;
			m_State.PC += isl->Size;
			break;
		}

		case 0x3D:
		{
			// DCR A
			this->m_State.DCR(this->m_State.A);
			m_State.PC += isl->Size;
			break;
		}

		case 0x77:
		{
			// MOV M,A
			uint16_t adr = (m_State.H << 8) + m_State.L;
			p_MemoryMap->Poke(adr, m_State.A);
			m_State.PC += isl->Size;
			break;
		}

		case 0xC2:
		{
			// JNZ adr
			if (m_State.Z)
			{
				m_State.PC += isl->Size;
			}
			else
			{
				m_State.PC = (opCode[2] << 8) + opCode[1];
			}
			break;
		}

		case 0xC3:
		{
			// JMP adr
			m_State.PC = (opCode[2] << 8) + opCode[1];
			break;
		}

		case 0xC5:
		{
			// PUSH B
			p_MemoryMap->Poke(this->m_State.SP - 1, this->m_State.B);
			p_MemoryMap->Poke(this->m_State.SP - 2, this->m_State.C);
			m_State.PC += isl->Size;
			m_State.SP -= 2;
			break;
		}

		case 0xC9:
		{
			// RET
			auto spc = p_MemoryMap->Peek(m_State.SP);
			auto spc1 = p_MemoryMap->Peek(m_State.SP + 1);
			m_State.PC = (spc1 << 8) + spc;
			m_State.SP += 2;
			break;
		}

		case 0xC6:
		{
			// ADI D8
			m_State.ADI(opCode[1]);
			m_State.PC += isl->Size;
			break;
		}

		case 0xCA:
		{
			// JZ adr
			if (m_State.Z)
			{
				m_State.PC = (opCode[2] << 8) + opCode[1];
			}
			else
			{
				m_State.PC += isl->Size;
			}
			break;
		}

		case 0xCD:
		{
			// CALL adr
			p_MemoryMap->Poke(m_State.SP - 1, (m_State.PC & 0xFF00) >> 8);
			p_MemoryMap->Poke(m_State.SP - 2, m_State.PC & 0x00FF);
			m_State.SP -= 2;
			m_State.PC = (opCode[2] << 8) + opCode[1];
			break;
		}

		case 0xCE:
		{
			// ACI D8
			m_State.ACI(opCode[1]);
			m_State.PC += isl->Size;
			break;
		}

		case 0xD2:
		{
			// JNC adr
			if (m_State.C)
			{
				m_State.PC += isl->Size;
			}
			else
			{
				m_State.PC = (opCode[2] << 8) + opCode[1];
			}
			break;
		}

		case 0xD5:
		{
			// PUSH D 
			p_MemoryMap->Poke(this->m_State.SP-1, this->m_State.D);
			p_MemoryMap->Poke(this->m_State.SP-2, this->m_State.E);
			m_State.PC += isl->Size;
			m_State.SP -= 2;
			break;
		}

		case 0xDA:
		{
			// JC adr
			if (m_State.C)
			{
				m_State.PC = (opCode[2] << 8) + opCode[1];
			}
			else
			{
				m_State.PC += isl->Size;
				
			}
			break;
		}

		case 0xE5:
		{
			// PUSH H
			p_MemoryMap->Poke(this->m_State.SP - 1, this->m_State.H);
			p_MemoryMap->Poke(this->m_State.SP - 2, this->m_State.L);
			m_State.PC += isl->Size;
			m_State.SP -= 2;
			break;
		}

		case 0xE6:
		{
			// ANI D8
			this->m_State.ANI(this->m_State.H, opCode[1]);
			m_State.PC += isl->Size;
			break;
		}

		case 0xE9:
		{
			// PCHL
			m_State.PC = (this->m_State.H << 8) + this->m_State.L;
			break;
		}

		case 0xEA:
		{
			// JPE adr
			if (m_State.P)
			{
				m_State.PC = (opCode[2] << 8) + opCode[1];
			}
			else
			{
				m_State.PC += isl->Size;

			}
			break;
		}

		case 0xE2:
		{
			// JPO adr
			if (m_State.P)
			{
				m_State.PC += isl->Size;
			}
			else
			{
				m_State.PC = (opCode[2] << 8) + opCode[1];
			}
			break;
		}

		case 0xF2:
		{
			// JP adr
			if (m_State.S)
			{
				m_State.PC += isl->Size;
			}
			else
			{
				m_State.PC = (opCode[2] << 8) + opCode[1];
			}
			break;
		}

		case 0xF5:
		{
			// PUSH PSW
			p_MemoryMap->Poke(this->m_State.SP - 1, this->m_State.A);
			p_MemoryMap->Poke(this->m_State.SP - 2, this->m_State.F);
			m_State.PC += isl->Size;
			m_State.SP -= 2;
			break;
		}

		case 0xFA:
		{
			// JM adr
			if (m_State.S)
			{
				m_State.PC = (opCode[2] << 8) + opCode[1];
			}
			else
			{
				m_State.PC += isl->Size;

			}
			break;
		}

		case 0xFE:
		{
			// CPI D8
			m_State.CPI(opCode[1]);
			m_State.PC += isl->Size;
			break;
		}


		default:
			throw new std::exception("not implemented");
			break;
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

	while (true)
	{
		if (m_State.Steps % n==0)
		{
			this->ShowState(stackSize);
		}
		
		this->RunStep();
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
	return *m_InstructionSet[idx];
}