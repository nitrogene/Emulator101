#pragma once
#include <stdint.h>
#include <filesystem>
#include <array>
#include "InstructionSetLine.h"

class InstructionSet
{
private:
	std::array<std::shared_ptr<InstructionSetLine>, std::numeric_limits<uint8_t>::max()+1> m_InstructionSet{};

	void setInstructions();

	void setInstruction(const uint8_t adr, std::function<void(State&, MemoryMap&, const uint8_t*, const uint16_t, const ::ClockCycle&)> exec);

public:
	InstructionSet(const std::filesystem::path& pathToInstructionSet);

	std::shared_ptr<const InstructionSetLine> operator[](const uint8_t idx) const;

	const std::string DisplayInstructionSet() const;
};