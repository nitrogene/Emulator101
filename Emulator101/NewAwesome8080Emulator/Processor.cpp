#include <fstream>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <fmt/core.h>
#include "Processor.h"
#include "Utilities.h"

Processor::Processor(const std::filesystem::path& pathToInstructiosSet)
{
	std::ifstream fs(pathToInstructiosSet);

	if (fs.is_open())
	{
		std::string line;
		std::getline(fs, line);
		// skip header row

		InstructionSet.fill(nullptr);

		// read line by line
		for (line; std::getline(fs, line); ) 
		{
			auto isl = std::make_shared<InstructionSetLine>();

			auto t= Utilities::Split(line, ';');

			if (t.size() != 12)
			{
				throw new std::exception("bad instruction set format");
			}

			isl->Mnemonic = t[0];
			isl->Description = t[1];

			for (auto i = 0; i < 8; ++i)
			{
				isl->Bits[i] = t[i + 2][0];
			}

			auto periods= Utilities::Split(t[10], '/');
			isl->ClockCycle.A = (uint8_t)std::atoi(periods[0].c_str());
			if (periods.size() > 1)
			{
				isl->ClockCycle.B = (uint8_t)std::atoi(periods[1].c_str());
			}
			isl->Size = (uint8_t)atoi(t[11].c_str());

			uint8_t r;
			if (isl->TryConvertBits(r))
			{
				if (InstructionSet[r]!=nullptr)
				{
					throw new std::exception("instruction already in map");
				}
				InstructionSet[r] = isl;
			}
		}

		fs.close();
		return;
	}

	throw new std::exception("Unable to load instruction set");
}

void Processor::DisplayInstructionSet()
{
	std::cout << "formatted instruction set\n";

	std::cout << "Opcode\tInstruction\tsize\tflags\tfunction\tclock cycle\n";

	// not uint8_t otherwise, infinite loop
	for (unsigned int i = 0; i < 256; ++i)
	{
		if (InstructionSet[i] == nullptr)
		{
			fmt::print("{0:#02x}\t{1:10}\n", i,'-');
			continue;
		}


		auto isl = InstructionSet[i];
		fmt::print("{0:#02x}\t{1:10}\t{2}\t\t\t{3}\n",i,isl->Mnemonic,isl->Size,isl->ClockCycle.toString());
	}
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

void Processor::Initialize(const std::vector<std::filesystem::path>& pathToRomFiles, const uint16_t totalRam, const uint16_t workRamAddress, const uint16_t videoRamAddress, const uint16_t mirrorRamAddress)
{
	std::vector<uint8_t> buffer;

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

	if (InstructionSet[opCode[0]])
	{
		auto isl = InstructionSet[opCode[0]];
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
	auto isl = InstructionSet[opCode[0]];
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
			Utilities::DCR(this->m_State.B, this->m_State);
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
			Utilities::DCR(this->m_State.C, this->m_State);
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
			Utilities::DCR(this->m_State.D, this->m_State);
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
			Utilities::DCR(this->m_State.E, this->m_State);
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
			Utilities::DCR(this->m_State.H, this->m_State);
			m_State.PC += isl->Size;
			break;
		}

		case 0x2D:
		{
			// DCR L
			Utilities::DCR(this->m_State.L, this->m_State);
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
			Utilities::DCR(this->m_State.A, this->m_State);
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

		case 0xCD:
			// CALL adr
			p_MemoryMap->Poke(m_State.SP - 1, (m_State.PC & 0xFF00) >> 8);
			p_MemoryMap->Poke(m_State.SP - 2, m_State.PC & 0x00FF);
			m_State.SP -= 2;
			m_State.PC = (opCode[2] << 8) + opCode[1];
			break;

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

		case 0xC9:
		{
			// RET
			auto spc = p_MemoryMap->Peek(m_State.SP);
			auto spc1 = p_MemoryMap->Peek(m_State.SP + 1);
			m_State.PC = (spc1 << 8) + spc;
			m_State.SP += 2;
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

const State& Processor::getState() const
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