#include <iostream>
#include <iomanip>
#include "MemoryMap.h"

MemoryMap::MemoryMap(const std::vector<uint8_t>& rom, const uint16_t totalMemorySize, const bool allowWritingToRom) 
{
	Initialize(rom, totalMemorySize, allowWritingToRom);
}

void MemoryMap::Initialize(const std::vector<uint8_t>& rom, const uint16_t totalMemorySize, const bool allowWritingToRom)
{
	if (rom.size() >= totalMemorySize)
	{
		throw new std::exception("totalMemorySize should be greater than rom size");
	}

	m_MemoryBuffer = rom;
	m_AllowWritingToRom = allowWritingToRom;
	m_RomSize = (uint16_t)rom.size();
	m_MemoryBuffer.resize(1 + (size_t)totalMemorySize, 0x00);
}

void MemoryMap::Hexdump()
{
	uint8_t* buffer = m_MemoryBuffer.data();
	uint16_t size = m_RomSize;
	uint16_t c = 0;
	auto limit = buffer + size;

	std::cout << "hexDump:\n";

	while (buffer < limit)
	{
		std::cout << std::setw(7) << std::setfill('0') << std::hex << c << " ";

		auto buffer_ = buffer;
		while (buffer_ < buffer + 16 && buffer_ < limit)
		{
			std::cout << std::setw(2) << std::setfill('0') << std::hex << (uint16_t)*buffer_ << " ";
			++buffer_;
		}

		buffer = buffer_;
		std::cout << "\n";
		c += 16;
	}
}

const uint8_t MemoryMap::Peek(const uint16_t adr) const
{
	return m_MemoryBuffer[adr];
}

const uint16_t MemoryMap::Peek16(const uint16_t adr) const
{
	return m_MemoryBuffer[adr+1]<<8 | m_MemoryBuffer[adr];
}

void MemoryMap::Poke(const uint16_t adr, const uint8_t value)
{
	if (adr < m_RomSize && !m_AllowWritingToRom)
	{
		std::cout << "Writing to ROM!!!!" << std::endl;
		return;
	}
		
	m_MemoryBuffer[adr] = value;
}

void MemoryMap::Poke16(const uint16_t adr, const uint16_t value)
{
	Poke(adr, value & 0xFF);
	Poke(adr+1, value >>8);
}

const uint8_t* MemoryMap::getOpCode(const uint16_t adr) const 
{
	return m_MemoryBuffer.data()+adr;
}