#include "test.h"

TEST_F(ProcessorTest, FirstStep)
{
	p_Processor->RunStep();
	const auto& state = p_Processor->getState();

	int cycles = 0;
	p_MachineTemplate->doCPUStep(cycles);
	const auto& state8080 = p_MachineTemplate->getState();

	// Registers
	EXPECT_EQ(state.B, state8080.b);
	EXPECT_EQ(state.C, state8080.c);
	EXPECT_EQ(state.D, state8080.d);
	EXPECT_EQ(state.E, state8080.e);
	EXPECT_EQ(state.H, state8080.h);
	EXPECT_EQ(state.L, state8080.l);
	EXPECT_EQ(state.A, state8080.a);

	// Flags
	EXPECT_EQ(state.S, state8080.cc.s);
	EXPECT_EQ(state.Z, state8080.cc.z);
	EXPECT_EQ(state.AC, state8080.cc.ac);
	EXPECT_EQ(state.P, state8080.cc.p);
	EXPECT_EQ(state.CY, state8080.cc.cy);

	// Program counter
	EXPECT_EQ(state.PC, state8080.pc);
	EXPECT_EQ(state.SP, state8080.sp);
}

TEST_F(ProcessorTest, TenStep)
{
	for (auto i = 0; i < 10; ++i)
	{
		p_Processor->RunStep();
		const auto& state = p_Processor->getState();

		int cycles = 0;
		p_MachineTemplate->doCPUStep(cycles);
		const auto& state8080 = p_MachineTemplate->getState();

		// Registers
		EXPECT_EQ(state.B, state8080.b);
		EXPECT_EQ(state.C, state8080.c);
		EXPECT_EQ(state.D, state8080.d);
		EXPECT_EQ(state.E, state8080.e);
		EXPECT_EQ(state.H, state8080.h);
		EXPECT_EQ(state.L, state8080.l);
		EXPECT_EQ(state.A, state8080.a);

		// Flags
		EXPECT_EQ(state.S, state8080.cc.s);
		EXPECT_EQ(state.Z, state8080.cc.z);
		EXPECT_EQ(state.AC, state8080.cc.ac);
		EXPECT_EQ(state.P, state8080.cc.p);
		EXPECT_EQ(state.CY, state8080.cc.cy);

		// Program counter
		EXPECT_EQ(state.PC, state8080.pc);
		EXPECT_EQ(state.SP, state8080.sp);
	}

}