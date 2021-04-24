#pragma once
#include <vector>
#include <string>

enum class MemoryMapPart
{
	ROM,
	WORK_RAM,
	VIDEO_RAM,
	MIRROR_RAM,
	FULL
};

class MemoryMap
{
private:
	// 8 bits buffer
	std::vector<unsigned char> m_MemoryBuffer{};
	const unsigned short m_RomSize = 0;
	const unsigned short m_WorkRamAddress = 0;
	const unsigned short m_VideoRamAddress = 0;
	const unsigned short m_MirrorRamAddress = 0;

public:
	MemoryMap(const std::vector<unsigned char>& rom, const unsigned short totalRam,
		const unsigned short workRamAddress, const unsigned short videoRamAddress, const unsigned short mirrorRamAddress);

	void Hexdump(const MemoryMapPart mmPart);

	const unsigned char& Peek(const unsigned short idx) const;
	void Poke(const unsigned short idx, const unsigned char value);
};
