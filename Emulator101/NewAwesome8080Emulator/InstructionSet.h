#pragma once
#include <stdint.h>
#include <filesystem>
#include <array>
#include "InstructionSetLine.h"

class InstructionSet
{
private:
	std::array<std::shared_ptr<const InstructionSetLine>, std::numeric_limits<uint8_t>::max()+1> m_InstructionSet{};

public:
	InstructionSet(const std::filesystem::path& pathToInstructionSet);

	std::shared_ptr<const InstructionSetLine> operator[](const uint8_t idx) const;

	const std::string DisplayInstructionSet() const;
};