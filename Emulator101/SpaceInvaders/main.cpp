#include "Processor.h"

const static char* roms[] = {
	"invaders.e",
	"invaders.f",
	"invaders.g",
	"invaders.h"
};

int main(int /*argc*/, char** /*argv*/)
{
	// Processor::hexdump(roms[3]);

	auto processor = Processor::loadIntoBuffer(roms[3]);

	int opbytes = 1;

	while (opbytes > 0)
	{
		opbytes=processor.disassemble();
		processor.PC += opbytes;
	}

	return 0;
}