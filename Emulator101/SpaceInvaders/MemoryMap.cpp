#include <iostream>
#include <iomanip>
#include "MemoryMap.h"

MemoryMap::MemoryMap(const std::vector<unsigned char>& rom, const unsigned short totalRam,
	const unsigned short workRamAddress, const unsigned short videoRamAddress, const unsigned short mirrorRamAddress) :m_MemoryBuffer(rom), m_WorkRamAddress(workRamAddress), m_VideoRamAddress(videoRamAddress), m_MirrorRamAddress(mirrorRamAddress),
	m_RomSize((unsigned short)rom.size())
{
	// TODO:
	// Check size and addresses coherence
	// Split ROM and RAM ??
	// Check get / set
	// How to simulate a part of memory read only, another part writable, while providing pointer 
	m_MemoryBuffer.resize(totalRam);
}

void MemoryMap::Hexdump(const MemoryMapPart mmPart)
{
	unsigned char* buffer = nullptr;
	unsigned short size = 0;

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
		size = (unsigned short)m_MemoryBuffer.size() - m_MirrorRamAddress;
		break;

	case MemoryMapPart::FULL:
		buffer = m_MemoryBuffer.data();
		size = (unsigned short)m_MemoryBuffer.size();
		break;
	}

	unsigned short c = 0;
	auto limit = buffer + size;

	std::cout << "hexDump:\n";

	while (buffer < limit)
	{
		std::cout << std::setw(7) << std::setfill('0') << std::hex << c << " ";

		auto buffer_ = buffer;
		while (buffer_ < buffer + 16 && buffer_ < limit)
		{
			std::cout << std::setw(2) << std::setfill('0') << std::hex << (unsigned short)*buffer_ << " ";
			++buffer_;
		}

		buffer = buffer_;
		std::cout << "\n";
		c += 16;
	}
}


const unsigned char& MemoryMap::Peek(const unsigned short idx) const
{
	return m_MemoryBuffer[idx];
}

void MemoryMap::Poke(const unsigned short idx, const unsigned char value)
{
	if (idx < m_RomSize)
	{
		throw new std::exception("Writing to ROM!!!!");
	}
	m_MemoryBuffer[idx] = value;
}