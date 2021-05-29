#include <iostream>
#include <iomanip>
#include "MemoryMap.h"

MemoryMap::MemoryMap(const std::vector<uint8_t>& rom, const uint16_t totalRam,
	const uint16_t workRamAddress, const uint16_t videoRamAddress, const uint16_t mirrorRamAddress) :m_MemoryBuffer(rom), m_WorkRamAddress(workRamAddress), m_VideoRamAddress(videoRamAddress), m_MirrorRamAddress(mirrorRamAddress),
	m_RomSize((uint16_t)rom.size())
{
	// TODO:
	// Check size and addresses coherence
	// Split ROM and RAM ??
	// Check get / set
	// How to simulate a part of memory read only, another part writable, while providing pointer 
	m_MemoryBuffer.resize(totalRam+1);
}

void MemoryMap::Hexdump(const MemoryMapPart mmPart)
{
	uint8_t* buffer = nullptr;
	uint16_t size = 0;

	switch (mmPart)
	{
	case MemoryMapPart::ROM:
		buffer = m_MemoryBuffer.data();
		size=m_RomSize;
		break;

	case MemoryMapPart::WORK_RAM:
		buffer = m_MemoryBuffer.data()+ m_WorkRamAddress;
		size = m_VideoRamAddress- m_WorkRamAddress;
		break;

	case MemoryMapPart::VIDEO_RAM:
		buffer = m_MemoryBuffer.data() + m_VideoRamAddress;
		size = m_MirrorRamAddress - m_VideoRamAddress;
		break;

	case MemoryMapPart::MIRROR_RAM:
		buffer = m_MemoryBuffer.data() + m_MirrorRamAddress;
		size = (uint16_t)m_MemoryBuffer.size() - m_MirrorRamAddress;
		break;

	case MemoryMapPart::FULL:
		buffer = m_MemoryBuffer.data();
		size = (uint16_t)m_MemoryBuffer.size();
		break;
	}

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


const uint8_t& MemoryMap::Peek(const uint16_t idx) const
{
	return m_MemoryBuffer[idx];
}

void MemoryMap::Poke(const uint16_t idx, const uint8_t value)
{
	if (idx < m_RomSize)
	{
		throw new std::exception("Writing to ROM!!!!");
	}
#ifdef _DEBUG
	else if (idx >= m_RomSize && idx < m_VideoRamAddress)
	{
		std::cout << "writing to work ram" << std::endl;
	}
	else if (idx >= m_VideoRamAddress && idx < m_MirrorRamAddress)
	{
		std::cout << "writing to video ram" << std::endl;
	}
	else
	{
		std::cout << "writing above mirror ram" << std::endl;
	}

#endif // DEBUG
		
	m_MemoryBuffer[idx] = value;
}