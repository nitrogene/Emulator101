#include <fmt/core.h>
#include <fstream>
#include "InstructionSet.h"
#include "Utilities.h"

InstructionSet::InstructionSet(const std::filesystem::path& pathToInstructionSet)
{
	std::ifstream fs(pathToInstructionSet);

	if (fs.is_open())
	{
		std::string line;
		std::getline(fs, line);
		// skip header row

		m_InstructionSet.fill(nullptr);

		// read line by line
		for (line; std::getline(fs, line); )
		{
			auto isl = std::make_shared<InstructionSetLine>();

			auto t = Utilities::Split(line, ';');

			if (t.size() != 12)
			{
				throw new std::exception("bad instruction set format");
			}

			isl->Mnemonic = t[0];
			isl->Description = t[1];

			for (auto i = 0; i < 8; ++i)
			{
				isl->Bits[i] = t[i + 2][0];
			}

			auto periods = Utilities::Split(t[10], '/');
			isl->ClockCycle.A = (uint8_t)std::atoi(periods[0].c_str());
			if (periods.size() > 1)
			{
				isl->ClockCycle.B = (uint8_t)std::atoi(periods[1].c_str());
			}
			isl->Size = (uint8_t)atoi(t[11].c_str());

			uint8_t r;
			if (isl->TryConvertBits(r))
			{
				if (m_InstructionSet[r] != nullptr)
				{
					throw new std::exception("instruction already in map");
				}
				m_InstructionSet[r] = isl;
			}
		}
		fs.close();

		return;
	}

	throw new std::exception("Unable to load instruction set");
}

std::shared_ptr<const InstructionSetLine> InstructionSet::operator[](const uint8_t idx) const
{
	return m_InstructionSet[idx];
}

const std::string InstructionSet::DisplayInstructionSet() const
{
	std::string result="Opcode\tInstruction\tsize\tflags\tfunction\tclock cycle\n";

	// not uint8_t otherwise, infinite loop
	for (unsigned int i = 0; i < 256; ++i)
	{
		if (m_InstructionSet[i] == nullptr)
		{
			result+=fmt::format("{0:#02x}\t{1:10}\n", i, '-');
			continue;
		}

		auto isl = m_InstructionSet[i];
		result += fmt::format("{0:#02x}\t{1:10}\t{2}\t\t\t{3}\n", i, isl->Mnemonic, isl->Size, isl->ClockCycle.toString());
	}

	return result;
}