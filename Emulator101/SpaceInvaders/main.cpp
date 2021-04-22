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
	auto processor = new Processor(instructions);

	//processor->DisplayInstructionSet();

	if (!processor->TryLoadIntoBuffer(roms))
	{
		std::cout << "loading roms has failed\n";
	}

	//processor->hexdump();

	processor->disassembleRom();

	processor->run();

	return 0;
}