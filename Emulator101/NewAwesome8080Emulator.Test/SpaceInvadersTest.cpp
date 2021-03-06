#include <fmt/core.h>
#include "SpaceInvadersTest.h"
#include "../NewAwesome8080Emulator.SpaceInvaders/SpaceInvaders.h"

void SpaceInvadersTest::fun()
{
	auto& map = p_Processor->getMemoryMap();
	auto& state = p_Processor->getState();
	auto opCode = map.getOpCode(p_Processor->getState().PC);
	auto isl= p_Processor->getIsl(opCode[0]);

	p_Processor->RunStep();
	state = p_Processor->getState();

	i8080_step(p_i8080State.get());

	auto msg = isl.Mnemonic + "\n" + state.toString();
	

	// Registers
	EXPECT_EQ(state.B, p_i8080State->b) << msg;
	EXPECT_EQ(state.C, p_i8080State->c) << msg;
	EXPECT_EQ(state.D, p_i8080State->d) << msg;
	EXPECT_EQ(state.E, p_i8080State->e) << msg;
	EXPECT_EQ(state.H, p_i8080State->h) << msg;
	EXPECT_EQ(state.L, p_i8080State->l) << msg;
	EXPECT_EQ(state.A, p_i8080State->a) << msg;

	// Flags
	EXPECT_EQ(state.Flags.Sign, p_i8080State->sf) << msg;
	EXPECT_EQ(state.Flags.Zero, p_i8080State->zf) << msg;
	EXPECT_EQ(state.Flags.AuxiliaryCarry, p_i8080State->hf) << msg;
	EXPECT_EQ(state.Flags.Parity, p_i8080State->pf) << msg;
	EXPECT_EQ(state.Flags.Carry, p_i8080State->cf) << msg;

	// Program counter
	EXPECT_EQ(state.PC, p_i8080State->pc) << msg;

	// Stack pointer
	EXPECT_EQ(state.SP, p_i8080State->sp) << msg;

	// Cycles
	EXPECT_EQ(state.Cycles, p_i8080State->cyc) << msg;

	// Interrupt
	EXPECT_EQ(state.INTE, p_i8080State->iff) << msg;
}

TEST_F(SpaceInvadersTest, Step10)
{
	for (auto i = 0; i < 10; ++i)
	{
		fun();
	}
}

TEST_F(SpaceInvadersTest, Step1000)
{
	for (auto i = 0; i < 1000; ++i)
	{
		fun();
	}
}

TEST_F(SpaceInvadersTest, Step2000)
{
	for (auto i = 0; i < 2000; ++i)
	{
		fun();
	}
}

TEST_F(SpaceInvadersTest, Step5000)
{
	for (auto i = 0; i < 5000; ++i)
	{
		fun();
	}
}

TEST_F(SpaceInvadersTest, Step10000)
{
	for (auto i = 0; i < 10000; ++i)
	{
		fun();
	}
}

TEST_F(SpaceInvadersTest, Step50000)
{
	for (auto i = 0; i < 50000; ++i)
	{
		fun();
	}
}

TEST_F(SpaceInvadersTest, Step5000_interrupt)
{
	for (auto i = 0; i < 5000; ++i)
	{
		fun();
	}

	p_i8080State->iff = 1;
	i8080_interrupt(p_i8080State.get(), 0xcf);

	p_Processor->getState().INTE = true;
	p_Processor->setInterrupt({ 0xcf ,0x00,0x00 });

	for (auto i = 0; i < 1000; ++i)
	{
		fun();
	}
}
