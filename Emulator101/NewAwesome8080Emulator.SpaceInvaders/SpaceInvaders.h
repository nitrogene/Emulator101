#pragma once
#include "Processor.h"

// http://computerarcheology.com/Arcade/SpaceInvaders/Hardware.html

class SpaceInvaders
{
public:
	const static uint16_t SCREEN_WIDTH = 224;
	const static uint16_t SCREEN_HEIGHT = 256;
	const static inline double FPS = 60.0;
	const static inline double CYCLES_PER_FRAME = 2000000.0 / FPS; // 2Mhz at 60 fps
	const static inline double HALF_CYCLES_PER_FRAME = CYCLES_PER_FRAME / 2.0;

private:
	uint8_t x = 0, y = 0, shift_offset = 0;
	uint8_t in_port0 = 0b00001110;
	uint8_t in_port1 = 0b00001000;
	uint8_t in_port2 = 0b00000000;
	bool doDisassemble = false;
	std::shared_ptr<Processor> p_Processor;
	std::function<void(bool& quit, bool& doDisassemble, uint8_t& in_port1, uint8_t& in_port2)> m_HandleInput;
	std::function<void(const uint8_t* const buffer, void* data)>  m_RefreshScreen;
	void* p_Data;
	double m_Cps; // Cycles/s 

	std::array<uint8_t,SCREEN_HEIGHT*SCREEN_WIDTH*4> p_Buffer;

	void gameUpdate();

public:
	SpaceInvaders();

	uint8_t IN(const uint8_t port);
	void OUT(const uint8_t a, const uint8_t port);

	void setHandleInput(std::function<void(bool& quit, bool& doDisassemble, uint8_t& in_port1, uint8_t& in_port2)> handleInput) 
	{
		m_HandleInput = handleInput;
	}

	void setRefreshScreen(std::function<void(const uint8_t* const buffer , void* data)> refreshScreen)
	{
		m_RefreshScreen = refreshScreen;
	}

	void setData(void* data)
	{
		p_Data = data;
	}

	void run();

	double getCps() const { return m_Cps; };

	void refreshScreenBuffer();
};