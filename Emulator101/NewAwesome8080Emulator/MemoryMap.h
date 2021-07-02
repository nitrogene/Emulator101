#pragma once
#include <vector>
#include <string>

class MemoryMap
{
private:
	// 8 bits buffer
	std::vector<uint8_t> m_MemoryBuffer{};
	uint16_t m_RomSize = 0;
	bool m_AllowWritingToRom = false;

public:
	MemoryMap(const std::vector<uint8_t>& rom, const uint16_t totalMemorySize, const bool allowWritingToRom=false);

	MemoryMap() = default;
	MemoryMap(const MemoryMap&) = default;
	//MemoryMap& operator=(const MemoryMap&) = default;

	void Hexdump();

	size_t size() const { return m_MemoryBuffer.size(); };

	uint16_t romSize() const { return m_RomSize; };

	const uint8_t& Peek(const uint16_t idx) const;
	void Poke(const uint16_t idx, const uint8_t value);
};
