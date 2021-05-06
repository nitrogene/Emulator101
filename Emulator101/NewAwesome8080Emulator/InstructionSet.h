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

public:
	InstructionSet(const std::filesystem::path& pathToInstructionSet);

	std::shared_ptr<const InstructionSetLine> operator[](const uint8_t idx) const;

	const std::string DisplayInstructionSet() const;
};