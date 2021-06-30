#include <SDL.h>
#include <thread>
#include <iostream>
#include "Processor.h"
#include "Utilities.h"

using namespace std::chrono_literals;

// http://computerarcheology.com/Arcade/SpaceInvaders/Hardware.html

const static std::vector<std::filesystem::path> roms
{
	"invaders.h",
	"invaders.g",
	"invaders.f",
	"invaders.e"
};

const static std::filesystem::path instructions = "instructions.set";

/*
	;16 bit shift register:
   ;    f              0    bit
   ;    xxxxxxxxyyyyyyyy
   ;
   ;    Writing to port 4 shifts x into y, and the new value into x, eg.
   ;    $0000,
   ;    write $aa -> $aa00,
   ;    write $ff -> $ffaa,
   ;    write $12 -> $12ff, ..
   ;
   ;    Writing to port 2 (bits 0,1,2) sets the offset for the 8 bit result, eg.
   ;    offset 0:
   ;    rrrrrrrr        result=xxxxxxxx
   ;    xxxxxxxxyyyyyyyy
   ;
   ;    offset 2:
   ;      rrrrrrrr  result=xxxxxxyy
   ;    xxxxxxxxyyyyyyyy
   ;
   ;    offset 7:
   ;           rrrrrrrr result=xyyyyyyy
   ;    xxxxxxxxyyyyyyyy
   ;
   ;    Reading from port 3 returns said result.
*/

uint8_t x = 0, y = 0, shift_offset = 0;
uint8_t in_port0 = 0b00001110;
uint8_t in_port1 = 0b00001000;
uint8_t in_port2 = 0b00000000;

bool doDisassemble = false;

void machineIN(State& state, const uint8_t port)
{
	switch (port)
	{
		case 0:
		{
			state.A = in_port0;
			break;
		}

		case 1:
		{
			state.A = in_port1;
			break;
		}

		case 2:
		{
			state.A = in_port2;
			break;
		}

		case 3:
		{
			uint16_t v = (x << 8) | y;
			state.A = ((v >> (8 - shift_offset)) & 0xff);
			break;
		}
	}
}

void machineOUT(State& state, const uint8_t port)
{
	switch (port)
	{
	case 2:
	{
		shift_offset = state.A & 0x7;
		break;
	}

	case 4:
	{
		y = x;
		x = state.A;
		break;
	}
	}
}


void refreshScreen(const MemoryMap& map, SDL_Renderer* pRenderer)
{
	uint16_t px = 0, py = 0;

	for (uint16_t adr = 0x2400; adr <= 0x3FFF; ++adr)
	{
		auto pixel = map.Peek(adr);

		for (uint8_t i = 0; i < 8; ++i)
		{
			uint8_t value = ((pixel >> i) & 0b00000001) == 0 ? 0 : 255;
			SDL_SetRenderDrawColor(pRenderer, value, value, value, 255);
			SDL_RenderDrawPoint(pRenderer, py, 256-(px+i));
		}
		px+=8;
		if (px == 256)
		{
			px = 0;
			++py;
		}
	}

	SDL_RenderPresent(pRenderer);
}

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
			if (event.key.keysym.sym == SDLK_c)
			{
				// Port 1
				// bit 0 = CREDIT (1 if deposit)
				in_port1 |= 0b00000001;
			}
			else if (event.key.keysym.sym == SDLK_m)
			{
				// Port 1
				// bit 2 = 1P start (1 if pressed)
				in_port1 |= 0b00000100;
			}
			else if (event.key.keysym.sym == SDLK_SPACE)
			{
				// Port 1
				// bit 4 = 1P shot (1 if pressed)
				in_port1 |= 0b00010000;
			}
			else if (event.key.keysym.sym == SDLK_q)
			{
				// Port 1
				// bit 5 = 1P left (1 if pressed)
				in_port1 |= 0b00100000;
			}
			else if (event.key.keysym.sym == SDLK_s)
			{
				// Port 1
				// bit 6 = 1P right (1 if pressed)
				in_port1 |= 0b01000000;
			}
			break;

		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_c)
			{
				// Port 1
				// bit 0 = CREDIT (1 if deposit)
				in_port1 &= ~0b00000001;
			}
			else if (event.key.keysym.sym == SDLK_m)
			{
				// Port 1
				// bit 2 = 1P start (1 if pressed)
				in_port1 &= ~0b00000100;
			}
			else if (event.key.keysym.sym == SDLK_SPACE)
			{
				// Port 1
				// bit 4 = 1P shot (1 if pressed)
				in_port1 &= ~0b00010000;
			}
			else if (event.key.keysym.sym == SDLK_q)
			{
				// Port 1
				// bit 5 = 1P left (1 if pressed)
				in_port1 &= ~0b00100000;
			}
			else if (event.key.keysym.sym == SDLK_s)
			{
				// Port 1
				// bit 6 = 1P right (1 if pressed)
				in_port1 &= ~0b01000000;
			}
			break;
		}
	}
}

int main(int /*argc*/, char** /*argv*/)
{
	auto processor = std::make_shared<Processor>(instructions);

	processor->Initialize(roms, 0xFFFF);

	processor->setMachineIN(machineIN);
	processor->setMachineOUT(machineOUT);

	SDL_Event event{};
	SDL_Renderer* renderer=nullptr;
	SDL_Window* window = nullptr;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(224, 256, 0, &window, &renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	auto& map = processor->getMemoryMap();

	auto t=std::thread(
		[&processor,&map]() 
		{
			auto last_interrupt_time = std::chrono::high_resolution_clock::now();

			auto preProcessFunc = [&processor, &map]()
			{
				auto pc = processor->getState().PC;

				if (doDisassemble)
				{
					processor->DisassembleRomStacksize(pc, 1);
				}

				auto opCode = map.Peek(pc);

				if (opCode == 0xF3)
				{
					std::cout << "DI" << std::endl;
				}
				else if (opCode == 0xFB)
				{
					std::cout << "EI" << std::endl;
				}
				else if (opCode == 0xDB)
				{
					std::cout << "IN" << std::endl;
				}
				else if (opCode == 0xD3)
				{
					std::cout << "OUT" << std::endl;
				}
			};


			uint8_t currentInterrupt = 1;

			auto postProcessFunc = [&last_interrupt_time, &processor, &map, &currentInterrupt]()
			{
				// Should we trigger an interrupt ?
				auto current_time = std::chrono::high_resolution_clock::now();

				auto dt = std::chrono::duration_cast<std::chrono::microseconds>(current_time - last_interrupt_time);

				// https://www.cpu-world.com/Arch/8080.html
				// 
				// 60 Hz, 2 interrupts per frame
				if ( dt > 8333us)
				{
					// generate interrupt
					if (processor->getState().EI)
					{
						std::cout << "RST " + std::to_string(currentInterrupt) << std::endl;

						if (currentInterrupt == 1)
						{
							processor->setInterrupt({ 0xCF,0x00,0x00 });
						}
						else
						{
							processor->setInterrupt({ 0xD7,0x00,0x00 });
						}

						currentInterrupt = currentInterrupt == 1 ? 2 : 1;

						last_interrupt_time = current_time;
					}
				}
			};

			processor->Run(preProcessFunc, postProcessFunc);

		}
	
	);

	//https://github.com/DanielH4/spaceinvaders-emulator

	bool quit = false;
	while (!quit) 
	{
		refreshScreen(map, renderer);
		SDL_SetWindowTitle(window, std::to_string(processor->getState().Steps).c_str());

		handleInput(quit);
	}

	processor->getState().HLT = true;
	t.join();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}