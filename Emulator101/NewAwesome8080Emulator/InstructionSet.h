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
	void DCR(State& state, uint8_t& value);
	void ANI(State& state, uint8_t& value, const uint8_t& opCode1);
	void ADI(State& state, const uint8_t& opCode1);
	void CPI(State& state, const uint8_t& opCode1);
	void ACI(State& state, const uint8_t& opCode1);

public:
	InstructionSet(const std::filesystem::path& pathToInstructionSet);

	std::shared_ptr<const InstructionSetLine> operator[](const uint8_t idx) const;

	const std::string DisplayInstructionSet() const;
};