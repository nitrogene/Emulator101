#pragma once
#include <string>
#include <vector>
#include <memory>
#include "State.h"
#include "MemoryMap.h"

class Utilities
{
public:
	static void ClearScreen();
	static std::vector<std::string> Split(const std::string& s, char delimiter);
	static bool isOddParity(const uint8_t value);
	static constexpr uint16_t getWordFromBytes(const uint8_t high, const uint8_t low)
	{
		return (high << 8) | low;
	}

	static constexpr void setBytesFromWord(const uint16_t adr, uint8_t& high, uint8_t& low)
	{
		high = adr >> 8;
		low = adr & 0xFF;
	}

	static void setZeroSignAndParity(Flags& state, uint8_t value);
	static void PushStack(State& state, const uint16_t value, MemoryMap& pMap);
	static uint16_t PopStack(State& state, MemoryMap& pMap);

	static void CALL(State& state, const uint16_t adr, MemoryMap& pMap);
	static void JMP(State& state, const uint16_t adr);
	static void RET(State& state, MemoryMap& pMap);

	// State affecting operations
	static void DAA(State& state);
	static void RLC(State& state);
	static void RAL(State& state);
	static void RAR(State& state);
	static void RRC(State& state);
	static void DCR(State& state, uint8_t& value);
	static void INR(State& state, uint8_t& value);
	static void DAD(State&, const uint8_t h, const uint8_t l);
	static void ADD(State&, const uint8_t value, const bool carry);
	static void SUB(State&, const uint8_t value, const bool carry);
	static void ANA(State&, const uint8_t value);
	static void XRA(State&, const uint8_t value);
	static void ORA(State&, const uint8_t value);
	static void CMP(State&, const uint8_t value);
};

