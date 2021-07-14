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
	static constexpr uint16_t getAddrFromHighLow(const uint8_t high, const uint8_t low)
	{
		return (high << 8) | low;
	}
	static void setZeroSignAndParity(Flags& state, uint8_t value);

	// State affecting operations
	static void DAA(State& state);
	static void RLC(State& state);
	static void RAL(State& state);
	static void RAR(State& state);
	static void RRC(State& state);
	static void DCR(State& state, uint8_t& value);
	static void INR(State& state, uint8_t& value);
	static void ANI(State& state, const uint8_t value);
	static void DAD(State&, const uint8_t h, const uint8_t l);
	static void ADD(State&, const uint8_t value, const bool carry);
	static void SUB(State&, const uint8_t value);
	static void SBB(State&, const uint8_t value);
	static void ANA(State&, const uint8_t value);
	static void XRA(State&, const uint8_t value);
	static void ORA(State&, const uint8_t value);
	static void CMP(State&, const uint8_t value);
	static void SUI(State&, const uint8_t value);
	static void SBI(State&, const uint8_t value);
	static void ORI(State&, const uint8_t value);
	static void XRI(State&, const uint8_t value);
	static void RST(State& state, const uint8_t num, MemoryMap& pMap);
};

