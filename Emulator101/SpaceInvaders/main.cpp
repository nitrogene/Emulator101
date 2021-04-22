#include <iostream>
#include "Processor.h"

const static std::vector<std::filesystem::path> roms
{
	"invaders.h",
	"invaders.g",
	"invaders.f",
	"invaders.e"
};

const static std::filesystem::path instructions = "instuctions.set";

int main(int /*argc*/, char** /*argv*/)
{
	auto processor = std::make_shared<Processor>(instructions);

	//processor->DisplayInstructionSet();

	if (!processor->TryLoadIntoBuffer(roms))
	{
		std::cout << "loading roms has failed\n";
	}

	//processor->hexdump();

	//processor->disassembleRom(0,processor->romSize());

	processor->run(16);

	return 0;
}