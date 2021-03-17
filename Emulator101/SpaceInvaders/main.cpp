#include "Processor.h"

const static char* roms[] = {
	"invaders.h",
	"invaders.g",
	"invaders.f",
	"invaders.e"
	
	
};

int main(int /*argc*/, char** /*argv*/)
{
	// Processor::hexdump(roms[3]);

	//auto processor = Processor::loadIntoBuffer(roms);
	auto processor = Processor::loadIntoBuffer(roms);

	int opbytes = 1;

	while (opbytes > 0)
	{
		opbytes=processor.disassemble();
		processor.PC += opbytes;
	}

	return 0;
}