#include "SpaceInvaders.h"

const static std::filesystem::path instructions = "InstructionSet.csv";

const static std::vector<std::filesystem::path> roms
{
	"invaders.h",
	"invaders.g",
	"invaders.f",
	"invaders.e"
};

uint8_t SpaceInvaders::IN(const uint8_t port)
{
	uint8_t value;

	switch (port)
	{
	case 0:
	{
		value = in_port0;
		break;
	}

	case 1:
	{
		value = in_port1;
		break;
	}

	case 2:
	{
		value = in_port2;
		break;
	}

	case 3:
	{
		uint16_t v = (x << 8) | y;
		value = ((v >> (8 - shift_offset)) & 0xff);
		break;
	}

	default:
		throw new std::exception("Unexpected input port!");
	}

	return value;
}

void SpaceInvaders::OUT(const uint8_t a, const uint8_t port)
{
	switch (port)
	{
	case 2:
	{
		shift_offset = a & 0x7;
		break;
	}
	case 3:
	{
		//sound bits
		break;
	}
	case 4:
	{
		y = x;
		x = a;
		break;
	}
	case 5:
	{
		// sound bits
		break;
	}
	case 6:
	{
		// watch-dog 
		break;
	}
	default:
		throw new std::exception("Unexpected output port!");
	}
}

void SpaceInvaders::gameUpdate()
{
	uint64_t cycles = 0;
	const auto& state = p_Processor->getState();
	uint8_t opCodeInterrupt = 0xCF;
	uint16_t lastSP = state.SP;
	bool halfFrameInterruptDone = false;

	while (cycles <= CYCLES_PER_FRAME)
	{
		auto startCycles = state.Cycles;

		if (doDisassemble)
		{
			auto pc = p_Processor->getState().PC;
			p_Processor->DisassembleRomStacksize(pc, 1);
		}

		p_Processor->RunStep();

		if ((lastSP != 0) && (state.SP < 0x2300))
		{
			printf("Stack getting dangerously low %04x\n", state.SP);
		}

		// Alert If more than 2 bytes have changed in the stack since last time    
		if ((lastSP != 0) && (abs(lastSP - state.SP) > 2))
		{
			printf("Stack Squash?\n");
		}
		lastSP = state.SP;

		cycles += state.Cycles - startCycles;

		if (cycles>HALF_CYCLES_PER_FRAME && !halfFrameInterruptDone)
		{
			if (state.INTE)
			{
				p_Processor->setInterrupt({ opCodeInterrupt ,0x00,0x00 });
				opCodeInterrupt = opCodeInterrupt == 0xCF ? 0xD7 : 0xCF;
			}
			halfFrameInterruptDone = true;
		}
	}
	if (state.INTE)
	{
		p_Processor->setInterrupt({ opCodeInterrupt ,0x00,0x00 });
		opCodeInterrupt = opCodeInterrupt == 0xCF ? 0xD7 : 0xCF;
	}
}

SpaceInvaders::SpaceInvaders()
{
	p_Processor = std::make_shared<Processor>(instructions);
	p_Processor->Initialize(roms, 0xFFFF);
	p_Processor->setData(this);

	auto IN = [](const uint8_t port, void* data) -> uint8_t
	{
		auto si = (SpaceInvaders*)data;

		return si->IN(port);
	};

	p_Processor->setIN(IN);

	auto OUT = [this](const uint8_t port, void* data)
	{
		auto si = (SpaceInvaders*)data;

		si->OUT(p_Processor->getState().A, port);
	};

	p_Processor->setOUT(OUT);
}

void SpaceInvaders::run()
{
	bool quit = false;
	auto timer = std::chrono::steady_clock::now();
	auto timer2 = std::chrono::steady_clock::now();
	bool doRefresh = false;

	while (!quit)
	{
		m_HandleInput(quit, doDisassemble,in_port1, in_port2);
		// Using Superzazu code logic here
		// update the game for each frame (every 1/60 seconds)
		auto newTimer = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(newTimer - timer).count();
		auto elapsed2 = std::chrono::duration_cast<std::chrono::milliseconds>(newTimer - timer2).count();

		m_Cps = p_Processor->getState().Cycles/1000.0 / (double)elapsed2;

		if (elapsed > (1.f / FPS) * 1000) 
		{
			timer = newTimer;
			gameUpdate();
			doRefresh = true;

			std::cout << m_Cps << std::endl;
		}

		if (doRefresh)
		{
			refreshScreenBuffer();
			m_RefreshScreen(p_Buffer.data(), p_Data);
			doRefresh = false;
		}
	}
}

void SpaceInvaders::refreshScreenBuffer()
{
	uint16_t px = 0, py = 0;
	const auto& map = p_Processor->getMemoryMap();

	for (uint16_t adr = 0x2400; adr <= 0x3FFF; ++adr)
	{
		auto pixel = map.Peek(adr);

		for (uint8_t i = 0; i < 8; ++i)
		{
			uint8_t value = ((pixel >> i) & 0b00000001) == 0 ? 0 : 255;

			const unsigned int xx = SCREEN_HEIGHT-(px+i)-1;
			const unsigned int yy = py;

			const unsigned int offset = (SCREEN_WIDTH * 4 * xx) + yy * 4;
			p_Buffer[offset + 0] = value;        // b
			p_Buffer[offset + 1] = value;        // g
			p_Buffer[offset + 2] = value;        // r
			p_Buffer[offset + 3] = 255;    // a
		}
		px += 8;
		if (px == 256)
		{
			px = 0;
			++py;
		}
	}

}