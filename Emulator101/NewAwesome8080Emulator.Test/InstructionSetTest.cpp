#include "InstructionSetTest.h"

TEST_F(InstructionSetTest, NOP)
{
	const std::vector<uint8_t> rom = {
		0x00
	};

	State state{};
	auto pMemoryMap=std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	(*p_InstructionSet)[rom[0]]->exec(state, *pMemoryMap, opCode);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps,1);
}