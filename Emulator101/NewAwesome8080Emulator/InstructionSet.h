#pragma once
#include <stdint.h>
#include <filesystem>
#include <array>
#include "InstructionSetLine.h"

class InstructionSet
{
private:
	std::array<std::shared_ptr<InstructionSetLine>, std::numeric_limits<uint8_t>::max()+1> m_InstructionSet{};

	// State affecting operations
	static void DAA(State& state);
	static void RLC(State& state);
	static void RAL(State& state);
	static void RAR(State& state);
	static void RRC(State& state);
	static void DCR(State& state, uint8_t& value);
	static void INR(State& state, uint8_t& value);
	static void ANI(State& state, uint8_t& value, const uint8_t& opCode1);
	static void ADI(State& state, const uint8_t& opCode1);
	static void CPI(State& state, const uint8_t& opCode1);
	static void ACI(State& state, const uint8_t& opCode1);
	static void DAD(State&, uint8_t& rh, uint8_t& rl);
	static void ADD(State&, const uint8_t value);
	static void ADC(State&, const uint8_t value);
	static void SUB(State&, const uint8_t value);
	static void SBB(State&, const uint8_t value);
	static void ANA(State&, const uint8_t value);
	static void XRA(State&, const uint8_t value);
	static void ORA(State&, const uint8_t value);
	static void CMP(State&, const uint8_t value);
	static void SUI(State&, const uint8_t value);
	static void SBI(State&, const uint8_t value);

public:
	InstructionSet(const std::filesystem::path& pathToInstructionSet);

	std::shared_ptr<const InstructionSetLine> operator[](const uint8_t idx) const;

	const std::string DisplayInstructionSet() const;
};