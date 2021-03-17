#include "Processor.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <fmt/core.h>

void Processor::hexdump(const char* filename)
{
	std::ifstream fs(filename, std::ios_base::binary);

	if (fs.is_open())
	{
		size_t t = 0;
		while (fs.good())
		{
			// 0xFF & c: left bits are 0 filled
			std::cout << std::setw(7) << std::setfill('0') << std::hex << t << " ";

			char buf[16];
			fs.read(buf, sizeof(buf));
			for (auto c : buf)
			{
				std::cout << std::setw(2) << std::setfill('0') << std::hex << (0xFF & c) << " ";
			}
			t += 16;
			std::cout << std::endl;
		}

		fs.close();
	}
	else
	{
		std::cout << "Error loading " << filename;
	}
}

void Processor::addToBuffer(const char* filename)
{
	std::ifstream fs(filename, std::ios_base::binary);

	if (fs.is_open())
	{
		auto n = (size_t)std::filesystem::file_size(filename);
		auto offset = this->Buffer.size();
		this->Buffer.resize(n+offset);
		fs.read(this->Buffer.data()+offset, n);
		fs.close();
	}
}

Processor Processor::loadIntoBuffer(const char* filename)
{
	Processor processor{};
	processor.addToBuffer(filename);
	return processor;
}

Processor Processor::loadIntoBuffer(const char* filenames[])
{
	auto n = sizeof(filenames);
	Processor processor{};

	for (size_t i = 0; i < n; ++i)
	{
		auto filename = filenames[i];
		processor.addToBuffer(filename);
	}

	return processor;
}

int Processor::disassemble()
{
	if (PC >= Buffer.size())
	{
		std::cout << "overflow" << std::endl;
		return 0;
	}

	auto code = &Buffer[PC];
	unsigned short opbytes = 1;

	fmt::print("{0:04x} {1:#04x} ", PC, 0xFF & code[0]);

	switch (0xFF & code[0])
	{
	case 0x00: 
		fmt::print("{0:10}\n","NOP");
		break;

	case 0x0f:
		fmt::print("{0:10}\n", "RRC");
		break;

	case 0x21:
		fmt::print("{0:10}{1},#${2:02x}{3:02x}\n", "LXI", "H", 0xFF & code[2], 0xFF & code[1]);
		opbytes = 3;
		break;

	case 0x27:
		fmt::print("{0:10}\n", "DAA");
		break;

	case 0x32:
		fmt::print("{0:10}${1:02x}{2:02x}\n", "STA", 0xFF & code[2], 0xFF & code[1]);
		opbytes = 3;
		break;

	case 0x35:
		fmt::print("{0:10}{1}\n", "DCR", "M");
		opbytes = 1;
		break;

	case 0x3a:
		fmt::print("{0:10}${1:02x}{2:02x}\n", "LDA", 0xFF & code[2], 0xFF & code[1]);
		opbytes = 3;
		break;

	case 0x3e:
		fmt::print("{0:10}{1},#${2:02x}\n", "MVI", "A", 0xFF & code[1]);
		opbytes = 2;
		break;

	case 0xa7:
		fmt::print("{0:10}{1:10}\n", "ANA", "A");
		break;

	case 0xaf:
		fmt::print("{0:10}{1:10}\n", "XRA", "A");
		break;

	case 0xc3:
		fmt::print("{0:10}${1:02x}{2:02x}\n", "JMP", 0xFF & code[2], 0xFF & code[1]);
		opbytes = 3;
		break;

	case 0xc5:
		fmt::print("{0:10}{1:10}\n", "PUSH", "B");
		break;

	case 0xc6:
		fmt::print("{0:10}#${1:02x}\n", "ADI", 0xFF & code[1]);
		opbytes = 2;
		break;

	case 0xca:
		fmt::print("{0:10}${1:02x}{2:02x}\n", "JZ", 0xFF & code[2], 0xFF & code[1]);
		opbytes = 3;
		break;

	case 0xcd:
		fmt::print("{0:10}${1:02x}{2:02x}\n", "CALL", 0xFF & code[2], 0xFF & code[1]);
		opbytes = 3;
		break;

	case 0xd5:
		fmt::print("{0:10}{1:10}\n", "PUSH", "D");
		break;

	case 0xda:
		fmt::print("{0:10}${1:02x}{2:02x}\n", "JC", 0xFF & code[2], 0xFF & code[1]);
		opbytes = 3;
		break;

	case 0xdb:
		fmt::print("{0:10}#${1:02x}\n", "IN", 0xFF & code[1]);
		opbytes = 2;
		break;

	case 0xe5:
		fmt::print("{0:10}{1:10}\n", "PUSH", "H");
		break;

	case 0xf5:
		fmt::print("{0:10}{1:10}\n", "PUSH","PSW");
		break;

	case 0xfe:
		fmt::print("{0:10}#${1:02x}\n", "CPI", 0xFF & code[1]);
		opbytes = 2;
		break;

	default:
		fmt::print("not implemented\n");
	}

	return opbytes;
}