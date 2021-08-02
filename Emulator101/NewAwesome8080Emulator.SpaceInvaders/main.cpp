#include <SDL.h>
#include "SpaceInvaders.h"

using namespace std::chrono_literals;

SDL_Renderer* renderer;

void refreshScreen(const uint8_t* const buffer, void* data)
{
	const uint32_t pitch = sizeof(uint8_t) * 4 * SpaceInvaders::SCREEN_WIDTH;
	SDL_Texture* texture = (SDL_Texture*)data;
	if (0 != SDL_UpdateTexture(texture, NULL, buffer, pitch))
	{
		SDL_Log("SDL_UpdateTexture error: %s\n", SDL_GetError());
	}
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void handleInput(bool& quit, bool& doDisassemble, uint8_t& in_port1, uint8_t& in_port2)
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
				doDisassemble = !doDisassemble;
			}
			else if (event.key.keysym.sym == SDLK_c)
			{
				// Port 1
				// bit 0 = CREDIT (1 if deposit)
				in_port1 |= 0b00000001;
			}
			else if (event.key.keysym.sym == SDLK_RETURN)
			{
				// Port 1
				// bit 1 = 2P start (1 if pressed)
				in_port1 |= 0b00000010;
			}
			else if (event.key.keysym.sym == SDLK_2)
			{
				// Port 1
				// bit 2 = 1P start(1 if pressed)
				in_port1 |= 0b00000100;
			}
			else if (event.key.keysym.sym == SDLK_SPACE)
			{
				// Port 1
				// bit 4 = 1P shot (1 if pressed)
				in_port1 |= 0b00010000;

				// Port 2
				// bit 4 = 2P shot (1 if pressed)
				in_port2 |= 0b00010000;
			}
			else if (event.key.keysym.sym == SDLK_q)
			{
				// Port 1
				// bit 5 = 1P left (1 if pressed)
				in_port1 |= 0b00100000;

				// Port 2
				// bit 5 = 2P left (1 if pressed)
				in_port2 |= 0b00100000;
			}
			else if (event.key.keysym.sym == SDLK_s)
			{
				// Port 1
				// bit 6 = 1P right (1 if pressed)
				in_port1 |= 0b01000000;

				// Port 2
				// bit 6 = 2P right (1 if pressed)
				in_port2 |= 0b01000000;
			}
			break;

		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_c)
			{
				// Port 1
				// bit 0 = CREDIT (1 if deposit)
				in_port1 &=~ 0b00000001;
			}
			else if (event.key.keysym.sym == SDLK_RETURN)
			{
				// Port 1
				// bit 1 = 2P start (1 if pressed)
				in_port1 &=~ 0b00000010;
			}
			else if (event.key.keysym.sym == SDLK_2)
			{
				// Port 1
				// bit 2 = 1P start(1 if pressed)
				in_port1 &=~ 0b00000100;
			}
			else if (event.key.keysym.sym == SDLK_SPACE)
			{
				// Port 1
				// bit 4 = 1P shot (1 if pressed)
				in_port1 &=~ 0b00010000;

				// Port 2
				// bit 4 = 2P shot (1 if pressed)
				in_port2 &=~ 0b00010000;
			}
			else if (event.key.keysym.sym == SDLK_q)
			{
				// Port 1
				// bit 5 = 1P left (1 if pressed)
				in_port1 &=~ 0b00100000;

				// Port 2
				// bit 5 = 2P left (1 if pressed)
				in_port2 &=~ 0b00100000;
			}
			else if (event.key.keysym.sym == SDLK_s)
			{
				// Port 1
				// bit 6 = 1P right (1 if pressed)
				in_port1 &=~ 0b01000000;

				// Port 2
				// bit 6 = 2P right (1 if pressed)
				in_port2 &=~ 0b01000000;
			}
			break;
		}
	}
}

int main(int /*argc*/, char** /*argv*/)
{
	SpaceInvaders si;

	si.setHandleInput(handleInput);
	si.setRefreshScreen(refreshScreen);

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	auto window = SDL_CreateWindow(
		"Space Invaders",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SpaceInvaders::SCREEN_WIDTH * 2,
		SpaceInvaders::SCREEN_HEIGHT * 2,
		SDL_WINDOW_SHOWN);

	// Check that the window was successfully created
	if (window == NULL) 
	{
		// In the case that the window could not be made...
		SDL_Log("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		// In the case that the renderer could not be made...
		SDL_Log("Could not create renderer: %s\n", SDL_GetError());
		return 1;
	}

	SDL_RendererInfo info;
	SDL_GetRendererInfo(renderer, &info);
	std::cout << "Renderer name: " << info.name << std::endl;
	std::cout << "Texture formats: " << std::endl;
	for (Uint32 i = 0; i < info.num_texture_formats; i++)
	{
		std::cout << SDL_GetPixelFormatName(info.texture_formats[i]) << std::endl;
	}


	if (SDL_RenderSetLogicalSize(renderer, SpaceInvaders::SCREEN_WIDTH, SpaceInvaders::SCREEN_HEIGHT) != 0)
	{
		SDL_Log("SDL_RenderSetLogicalSize error %s\n", SDL_GetError());
		return 1;
	}

	auto texture= SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SpaceInvaders::SCREEN_WIDTH,
		SpaceInvaders::SCREEN_HEIGHT);

	if (texture == NULL)
	{
		// In the case that the texture could not be made...
		SDL_Log("Could not create texture: %s\n", SDL_GetError());
		return 1;
	}

	si.setData(texture);

	si.run();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}