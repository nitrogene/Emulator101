#include <SDL.h>
#include <thread>
#include "Processor.h"

const static std::vector<std::filesystem::path> roms
{
	"invaders.h",
	"invaders.g",
	"invaders.f",
	"invaders.e"
};

const static std::filesystem::path instructions = "instuctions.set";


void refreshScreen(const MemoryMap& map, SDL_Renderer* pRenderer)
{
	auto fb = &map.Peek(0x2400);

	for (uint16_t y = 0; y < 224; ++y)
	{
		for (uint16_t x = 0; x < 256; x += 8)
		{
			// 8 pixels per byte
			auto pixel=*(fb+y/8 + x * 28);

			for (uint16_t i = 0; i < 8; ++i) 
			{
				uint8_t value = pixel & (1 << i);
				
				if (value != 0) {
					value = 255;
				}

				SDL_SetRenderDrawColor(pRenderer, value, value, value, 255);
				SDL_RenderDrawPoint(pRenderer, x+i, y);
			}
		}
	}

	SDL_RenderPresent(pRenderer);
};

int main(int /*argc*/, char** /*argv*/)
{
	auto processor = std::make_shared<Processor>(instructions);

	processor->Initialize(roms, 0xFFFF, 0x2000, 0x2400, 0x4000);

	SDL_Event event{};
	SDL_Renderer* renderer=nullptr;
	SDL_Window* window = nullptr;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(256, 224, 0, &window, &renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	auto& map = processor->getMemoryMap();

	auto t=std::thread(
		[&processor]() 
		{
			processor->Run(); 
		}
	
	);

	while (1) 
	{
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
		{
			break;
		}

		std::this_thread::sleep_for(std::chrono::microseconds(16666));
		refreshScreen(map, renderer);
		SDL_SetWindowTitle(window, std::to_string(processor->getState().Steps).c_str());
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}