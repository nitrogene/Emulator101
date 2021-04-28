#include "test.h"

TEST_F(ProcessorTest, FirstStep)
{
	p_Processor->RunStep();

	const auto& state = p_Processor->getState();

	// Registers
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_EQ(state.A, 0);

	// Flags
	EXPECT_FALSE(state.S);
	EXPECT_FALSE(state.Z);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.P);
	EXPECT_FALSE(state.CY);

	// Program counter
	EXPECT_EQ(state.PC, 1);
	EXPECT_EQ(state.SP, 0);
}