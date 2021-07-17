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

	void Initialize(const std::vector<uint8_t>& rom, const uint16_t totalMemorySize, const bool allowWritingToRom);

	void Hexdump();

	size_t size() const { return m_MemoryBuffer.size(); };

	uint16_t romSize() const { return m_RomSize; };

	const uint8_t Peek(const uint16_t adr) const;
	void Poke(const uint16_t adr, const uint8_t value);

	const uint16_t Peek16(const uint16_t adr) const;
	void Poke16(const uint16_t adr, const uint16_t value);

	const uint8_t* getOpCode(const uint16_t adr) const;
};
