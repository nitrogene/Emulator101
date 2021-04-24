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
			isl->Periods = t[10];
			isl->Size = (unsigned char)atoi(t[11].c_str());

			unsigned char r;
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

	std::cout << "Opcode\tInstruction\tsize\tflags\tfunction\n";

	// not unsigned char otherwise, infinite loop
	for (unsigned int i = 0; i < 256; ++i)
	{
		if (InstructionSet[i] == nullptr)
		{
			fmt::print("{0:#02x}\t{1:10}\n", i,'-');
			continue;
		}


		auto isl = InstructionSet[i];
		fmt::print("{0:#02x}\t{1:10}\t{2}\n",i,isl->Mnemonic,isl->Size);
	}
}

void Processor::Hexdump(MemoryMapPart mmPart)
{
	p_MemoryMap->Hexdump(mmPart);
}

void Processor::LoadIntoBuffer(const std::filesystem::path& pathToRomFile, std::vector<unsigned char>& buffer)
{
	std::ifstream fs(pathToRomFile, std::ios_base::binary);

	if (fs.is_open())
	{
		auto n = (size_t)std::filesystem::file_size(pathToRomFile);
		auto offset = buffer.size();
		constexpr auto limit = std::numeric_limits<unsigned short>().max();

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

void Processor::Initialize(const std::vector<std::filesystem::path>& pathToRomFiles, const unsigned short totalRam, const unsigned short workRamAddress, const unsigned short videoRamAddress, const unsigned short mirrorRamAddress)
{
	std::vector<unsigned char> buffer;

	for (auto& pathToRomFile : pathToRomFiles)
	{
		LoadIntoBuffer(pathToRomFile, buffer);
	}

	this->p_MemoryMap = std::make_shared<MemoryMap>(buffer, totalRam, workRamAddress, videoRamAddress, mirrorRamAddress);
}

void Processor::DisassembleRomStacksize(const unsigned short offset, const unsigned short stackSize)
{
	auto pc = offset;
	auto count = 0;
	while (count < stackSize)
	{
		Disassemble(pc);
		count++;
	}
}

void Processor::Disassemble(unsigned short& pc)
{
	auto opCode = &p_MemoryMap->Peek(pc);
	unsigned short opbytes = 1;

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

void Processor::Run(const unsigned short stackSize)
{ 
	PC = 0;

	while (true)
	{
		this->ShowState(stackSize);

		// TODO: Create ROM "read only" throw exception whenever we write in rom part or out of ram
		// Simplify
		auto opCode = &p_MemoryMap->Peek(PC);
		auto isl = InstructionSet[opCode[0]];
		if (isl!=nullptr)
		{
			// TODO: replace by script stored in nstruction set ???
			switch (opCode[0])
			{
			case 0x00:
				// NOP
				PC += isl->Size;
				break;

			case 0x06:
				// MVI B, D8
				this->Registers.B = opCode[1];
				PC += isl->Size;
				break;

			case 0x31:
				// LXI SP,D16
				SP = (opCode[2] << 8) + opCode[1];
				PC += isl->Size;
				break;

			case 0xCD:
				// CALL adr
				p_MemoryMap->Poke(SP - 1, (PC & 0xFF00) >> 8);
				p_MemoryMap->Poke(SP - 2,PC & 0x00FF);
				SP -= 2;
				PC = (opCode[2] << 8) + opCode[1];
				break;

			case 0xC3:
				// JMP adr
				PC= (opCode[2] << 8) + opCode[1];
				break;

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
}

void Processor::ShowState(const unsigned short stackSize)
{
	Utilities::ClearScreen();
	fmt::print("PC={0:04x}, SP={1:04x}, {2}\n", PC, SP, this->Registers.toString());
	fmt::print("Next {1} instructions in rom:\n", PC, stackSize);
	this->DisassembleRomStacksize(PC, stackSize);
}