#include "Processor.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <fmt/core.h>

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

void Processor::disassembleRom()
{
	std::cout << "disassembleRom:\n";
	PC = 0;
	while (PC < Rom.size())
	{
		disassemble();
	}
}

void Processor::disassemble()
{
	if (PC >= Rom.size())
	{
		std::cout << "overflow" << std::endl;
		return;
	}

	auto code = &Rom[PC];
	unsigned short opbytes = 1;

	fmt::print("{0:04x}\t", PC);

	if (InstructionSet[code[0]])
	{
		auto isl = InstructionSet[code[0]];
		auto instruction = isl->Mnemonic;
		if (isl->Mnemonic.ends_with("adr"))
		{
			unsigned  short d = (code[2] << 8) + code[1];
			instruction = fmt::format("{0}${1:04x}", instruction.substr(0, instruction.size() - 3), d);
		}
		else if (isl->Mnemonic.ends_with("D8"))
		{
			unsigned  short d = code[1];
			instruction = fmt::format("{0}#${1:02x}", instruction.substr(0, instruction.size() - 2), d);
		}
		else if (isl->Mnemonic.ends_with("D16"))
		{
			unsigned  short d = (code[2] << 8) + code[1];
			instruction = fmt::format("{0}#${1:02x}", instruction.substr(0, instruction.size() - 3), d);
		}
		fmt::print("{0:10}\n", instruction);
		opbytes = isl->Size;
	}
	else
	{
		fmt::print("not implemented\n");
	}

	PC += opbytes;
}

void Processor::run()
{
}