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


void timer_start(std::function<void(void)> func, unsigned int interval)
{
	std::thread([func, interval]() {
		while (true)
		{
			func();
			std::this_thread::sleep_for(std::chrono::microseconds(interval));
		}
		}).detach();
}

int main(int /*argc*/, char** /*argv*/)
{
	auto processor = std::make_shared<Processor>(instructions);

	processor->Initialize(roms, 0xFFFF, 0x2000, 0x2400, 0x4000);

	// processor->Hexdump(MemoryMapPart::ROM);

	// processor->DisplayInstructionSet();

	//SDL_Event event{};
	//SDL_Renderer* renderer=nullptr;
	//SDL_Window* window = nullptr;

	//SDL_Init(SDL_INIT_VIDEO);
	//SDL_CreateWindowAndRenderer(256, 224, 0, &window, &renderer);
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	//SDL_RenderClear(renderer);
	//SDL_RenderPresent(renderer);

	//auto& map = processor->getMemoryMap();

	//std::function<void()> refreshScreen = [&map, &renderer]()
	//{
	//	auto fb = &map.Peek(0x2400);

	//	for (uint16_t y = 0; y < 224; ++y)
	//	{
	//		// 
	//		for (uint16_t x = 0; x < 256; x += 8)
	//		{
	//			// 8 pixels per byte
	//			auto pixel=*(fb+y/8 + x * 28);

	//			for (uint16_t i = 0; i < 8; ++i) 
	//			{
	//				uint8_t value = pixel & (1 << i);
	//				
	//				if (value != 0) {
	//					value = 255;
	//				}

	//				SDL_SetRenderDrawColor(renderer, value, value, value, 255);
	//				SDL_RenderDrawPoint(renderer, x, y+i);
	//			}
	//		}
	//	}
	//};

	//timer_start(refreshScreen, 16666);

	processor->Run();

	//while (1) 
	//{
	//	if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
	//	{
	//		break;
	//	}
	//}

	//SDL_DestroyRenderer(renderer);
	//SDL_DestroyWindow(window);
	//SDL_Quit();

	return EXIT_SUCCESS;
}