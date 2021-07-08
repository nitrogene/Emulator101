#include <iostream>
#include <windows.h> 
#ifdef max 
#undef max
#endif
#include <fmt/core.h>
#include "Processor.h"

const static std::vector<std::filesystem::path> roms
{
	"TST8080.COM"
};

const static std::filesystem::path instructions = "InstructionSet.csv";

int main(int /*argc*/, char** /*argv*/)
{
	// This roms should start at 0x100, thus we need empty NOP vector
	// and settinf PC to 0x100.
	auto bytes = std::vector<uint8_t>(0x100, 0x0);

	auto processor = std::make_shared<Processor>(instructions);

	processor->Initialize(roms, 0xFFFF, bytes, true);

	auto& map = processor->getMemoryMap();

	// the original assembly code has a ORG 00100H, thus we set PC to 0x100
	processor->setPC(0x100);


	while (!processor->getState().HLT)
	{
		// THis test is a binary for 8080 processor and CP/M
		// running the binary without tweaks won't be usefull, because it is looking for CP/M instructions
		// at lower address $0005 to display test informations.
		// After each emulator step, we'll detect such a call
		/*
		You can find more information on what it does here https://www.seasip.info/Cpm/bdos.html
			BDOS function 9 (C_WRITESTR) - Output string
			Supported by: All versions
			Entered with C=9, DE=address of string.
			Display a string of ASCII characters, terminated with the $ character. Thus the string may not contain $ characters - so, for example, the VT52 cursor positioning command ESC Y y+32 x+32 will not be able to
			use row 4.
			Under CP/M 3 and above, the terminating character can be changed using BDOS function 110.
		*/

		auto opCode = &processor->Peek(processor->getState().PC);
		auto& state = processor->getState();
		const auto& isl = processor->getIsl(opCode[0]);

		if (opCode[0] == 0xCD)
		{
			// Detect CALL	BDOS
			if (5 == ((opCode[2] << 8) | opCode[1]))
			{
				if (state.C == 9)
				{
					// C_WRITESTR
					std::string output;
					uint16_t adr = (state.D << 8) | state.E;
					auto str = (const char*)&processor->Peek(adr);
					while (*str != '$')
					{
						output += *str++;
					}

					// Remove line feed (for printer?)
					output.erase(std::remove(output.begin(), output.end(), '\f'), output.end());

					std::cout << output;
				}
				else
				{
					// C_WRITE
					std::cout << (char)state.E;
				}

				// With real CP/M, there is a RET, let's just go to next instruction
				state.PC += isl.Size;
				state.Cycles += 17;
				continue;
			}
		}
		else if (opCode[0] == 0xC3)
		{
			if (opCode[1] == 0x00 && opCode[2] == 0x00)
			{
				// Detect EXIT TO CP/M WARM BOOT (JMP 0x0000)
				std::cout << "\nCP/M WARM BOOT exit" << std::endl;
				processor->getState().HLT = true;
				continue;
			}
		}

		processor->RunStep();
	}

	return 0;
}