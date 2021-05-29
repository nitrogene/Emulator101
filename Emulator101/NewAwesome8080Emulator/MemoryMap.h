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
	std::vector<uint8_t> m_MemoryBuffer{};
	const uint16_t m_RomSize = 0;
	const uint16_t m_WorkRamAddress = 0;
	const uint16_t m_VideoRamAddress = 0;
	const uint16_t m_MirrorRamAddress = 0;

public:
	MemoryMap(const std::vector<uint8_t>& rom, const uint16_t totalRam,
		const uint16_t workRamAddress, const uint16_t videoRamAddress, const uint16_t mirrorRamAddress);

	void Hexdump(const MemoryMapPart mmPart);

	size_t size() const { return m_MemoryBuffer.size(); };

	const uint8_t& Peek(const uint16_t idx) const;
	void Poke(const uint16_t idx, const uint8_t value);
};
