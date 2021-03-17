#include "Processor.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <filesystem>

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

Processor Processor::loadIntoBuffer(const char* filename)
{
	Processor processor{};

	std::ifstream fs(filename, std::ios_base::binary);

	if (fs.is_open())
	{
		auto n = (size_t)std::filesystem::file_size(filename);
		processor.Buffer.resize(n);
		fs.read(processor.Buffer.data(), n);
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

	std::cout << std::setw(2) << std::setfill('0') << std::hex << (0xFF & *code) << " ";

	switch (*code)
	{
	case 0x00: 
		std::cout << "NOP" << std::endl; 
		break;

	default:
		std::cout << "not implemented" << std::endl;
	}

	return opbytes;
}