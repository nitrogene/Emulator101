#include <iostream>
#include <fstream>
#include <windows.h> 
#ifdef max 
#undef max
#endif
#include <fmt/core.h>
#include "Processor.h"
#include <SDL.h>

using namespace std::chrono_literals;

const static std::vector<std::filesystem::path> roms
{
	"CPUTEST.COM"
};

const static std::filesystem::path instructions = "instructions.set";

bool quit = false;
bool trace = false;
bool file = false;

void handleInput(bool& quit)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_d)
			{
				trace = !trace;
			}
			else if (event.key.keysym.sym == SDLK_f)
			{
				file = !file;
			}
			break;
		}
	}
}

int main(int /*argc*/, char** /*argv*/)
{
	SDL_Event event{};
	SDL_Renderer* renderer = nullptr;
	SDL_Window* window = nullptr;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(224, 256, 0, &window, &renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// This roms should start at 0x100, thus we need empty NOP vector
	// and settinf PC to 0x100.
	auto bytes = std::vector<uint8_t>(0x100, 0x0);

	auto processor = std::make_shared<Processor>(instructions);
	auto& state = processor->getState();

	// This rom use temporary variables:
	// THus, we have to allow writing to ROM
	processor->Initialize(roms, 0xFFFF, bytes, true);

	auto& map = processor->getMemoryMap();

	// the original assembly code has a ORG 00100H, thus we set PC to 0x100
	processor->setPC(0x100);

	//{
	//	std::ofstream outs("CPUTEST.HEX");
	//	processor->DisassembleRomStacksize(0x100, map.romSize() + 0x100, outs);
	//	outs.close();
	//}

	std::ofstream debugFile("debug.log");

	while (!processor->getState().HLT && !quit)
	{
		handleInput(quit);

		// as explained in http://emulator101.com/, CPUDIAG is a binary for 8080 processor and CP/M
		// running the binary without tweaks won't be usefull, because it is looking for CP/M instructions
		// at lower address $0005 to display test informations.
		// After each emulator step, we'll detect such a call
		/*
		Here is an excert os CPUDIAG assembled code:
				;***********************************************************************
				; MICROCOSM ASSOCIATES  8080/8085 CPU DIAGNOSTIC VERSION 1.0  (C) 1980
				;***********************************************************************
				;
				;DONATED TO THE "SIG/M" CP/M USER'S GROUP BY:
				;KELLY SMITH, MICROCOSM ASSOCIATES
				;3055 WACO AVENUE
				;SIMI VALLEY, CALIFORNIA, 93065
				;(805) 527-9321 (MODEM, CP/M-NET (TM))
				;(805) 527-0518 (VERBAL)
				;
				;
				;
				;
				;
				;

						ORG	00100H
				;
				;
				;
					JMP	CPU	;JUMP TO 8080 CPU DIAGNOSTIC
				;
				;
				;
					DB	'MICROCOSM ASSOCIATES 8080/8085 CPU DIAGNOSTIC'
					DB	' VERSION 1.0  (C) 1980'
				;
				;
				;
				BDOS	EQU	00005H	;BDOS ENTRY TO CP/M
				WBOOT	EQU	00000H	;RE-ENTRY TO CP/M WARM BOOT
		*/
		/*
		This is the print function:
				MSG:	PUSH	D	;EXILE D REG.
				XCHG				;SWAP H&L REGS. TO D&E REGS.
				MVI	C,9				;LET BDOS KNOW WE WANT TO SEND A MESSAGE
				CALL	BDOS
				POP	D				;BACK FROM EXILE
				RET

		You can find more information on what it does here https://www.seasip.info/Cpm/bdos.html
			BDOS function 9 (C_WRITESTR) - Output string
			Supported by: All versions
			Entered with C=9, DE=address of string.
			Display a string of ASCII characters, terminated with the $ character. Thus the string may not contain $ characters - so, for example, the VT52 cursor positioning command ESC Y y+32 x+32 will not be able to
			use row 4.
			Under CP/M 3 and above, the terminating character can be changed using BDOS function 110.
		*/

		if (trace || file)
		{
			auto stateStr = state.toString();
			std::stringstream ss;
			processor->DisassembleRomStacksize(state.PC, 1, ss);
			auto dissStr = ss.str();

			if (trace)
			{
				SetConsoleTextAttribute(hConsole, 10);
				std::cout << stateStr;

				SetConsoleTextAttribute(hConsole, 11);
				std::cout << ss.str();
			}

			if (debugFile)
			{
				debugFile << stateStr << ss.str();
			}


		}


		auto opCode = &processor->Peek(state.PC);
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

					OutputDebugStringA(output.c_str());
				}
				else
				{
					// C_WRITE
					std::string output = { (char)state.E };
					OutputDebugStringA(output.c_str());
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

	debugFile.close();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}