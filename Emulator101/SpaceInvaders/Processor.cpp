#include "Processor.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <fmt/core.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef max

void clear() 
{
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}

std::string Registers::toString()
{
	return fmt::format("B={0:02x}, C={1:02x}, D={2:02x}, E={3:02x}, H={4:02x}, L={5:02x}, A={6:02x}", B, C, D, E, H, L, A);
}


// To be replaced by using ranges
std::vector<std::string> split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

bool InstructionSetLine::TryConvertBits(unsigned char& r)
{
	unsigned char base = 1;
	r = 0;
	for (auto i = 7; i >= 0; --i)
	{
		if (Bits[i] != '1' && Bits[i] != '0')
		{
			r = 0;
			return false;
		}

		if (Bits[i] == '1')
		{
			r += base;
		}

		base<<=1;
	}

	return true;
}

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

			auto t=split(line, ';');

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

void Processor::hexdump()
{
	auto buffer = Rom.data();
	unsigned int c = 0;

	std::cout << "hexDump:\n";

	while (buffer < Rom.data() + Rom.size())
	{
		std::cout << std::setw(7) << std::setfill('0') << std::hex << c << " ";

		auto buffer_ = buffer;
		while (buffer_ < buffer+16 && buffer_< Rom.data() + Rom.size())
		{
			std::cout << std::setw(2) << std::setfill('0') << std::hex << (unsigned int)*buffer_ << " ";
			++buffer_;
		}

		buffer = buffer_;
		std::cout << "\n";
		c += 16;
	}
}

bool Processor::TryLoadIntoBuffer(const std::filesystem::path& pathToRomFile)
{
	std::ifstream fs(pathToRomFile, std::ios_base::binary);

	if (fs.is_open())
	{
		auto n = (size_t)std::filesystem::file_size(pathToRomFile);
		auto offset = this->Rom.size();
		constexpr auto limit = std::numeric_limits<unsigned short>().max();

		if (n + offset > limit)
		{
			throw new std::exception("overflow");
		}

		this->Rom.resize(n+offset);
		fs.read((char*)this->Rom.data()+offset, n);
		fs.close();
		return true;
	}
	
	return false;
}

bool Processor::TryLoadIntoBuffer(const std::vector<std::filesystem::path>& pathToRomFiles)
{

	for (auto& pathToRomFile : pathToRomFiles)
	{
		if (!TryLoadIntoBuffer(pathToRomFile))
		{
			return false;
		}
	}

	return true;
}

void Processor::disassembleRom(const unsigned short offset, const unsigned short size)
{
	auto pc= offset;
	while (pc < offset+size)
	{
		disassemble(pc);
	}
}

void Processor::disassemble(unsigned short& pc)
{
	if (pc >= Rom.size())
	{
		std::cout << "overflow" << std::endl;
		return;
	}

	auto opCode = &Rom[pc];
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

void Processor::run(const unsigned short stackSize)
{ 
	PC = 0;

	while (true)
	{
		this->showState(stackSize);

		auto opCode = &Rom[PC];
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

			case 0xc3:
				// JMP adr
				PC= (opCode[2] << 8) + opCode[1];
				//PC += isl->Size;
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

unsigned short Processor::romSize()
{
	return (unsigned short)Rom.size();
}

void Processor::showState(const unsigned short stackSize)
{
	clear();
	fmt::print("PC={0:04x}, SP={1:04x}, {2}\n", PC, SP, this->Registers.toString());
	fmt::print("Next {1} instructions:\n", PC, stackSize);
	this->disassembleRom(PC, stackSize);
}