#include "InstructionSetTest.h"

TEST_F(InstructionSetTest, NOP)
{
	const std::vector<uint8_t> rom = {
		0x00
	};

	State state{};
	auto pMemoryMap=std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
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

TEST_F(InstructionSetTest, LXI_B)
{
	const std::vector<uint8_t> rom = {
		0x01,
		0x03,
		0x01
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0x01);
	EXPECT_EQ(state.C, 0x03);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 10);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, LXI_D)
{
	const std::vector<uint8_t> rom = {
		0x11,
		0x03,
		0x01
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x01);
	EXPECT_EQ(state.E, 0x03);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 10);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, LXI_H)
{
	const std::vector<uint8_t> rom = {
		0x21,
		0x03,
		0x01
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0x01);
	EXPECT_EQ(state.L, 0x03);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 10);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, LXI_SP)
{
	const std::vector<uint8_t> rom = {
		0x31,
		0xBC,
		0x3A
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
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
	EXPECT_EQ(state.Cycles, 10);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0x03ABC);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, STAX_B)
{
	const std::vector<uint8_t> rom = {
		0x02
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	state.B = 0x3F;
	state.C = 0x16;
	state.A = 0xAC;
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	auto value=pMemoryMap->Peek(0x03F16);

	EXPECT_EQ(value, 0xAC);
	EXPECT_EQ(state.A, 0xAC);
	EXPECT_EQ(state.B, 0x3F);
	EXPECT_EQ(state.C, 0x16);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, STAX_D)
{
	const std::vector<uint8_t> rom = {
		0x12
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	state.D = 0x3F;
	state.E = 0x16;
	state.A = 0xAC;
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	auto value = pMemoryMap->Peek(0x03F16);

	EXPECT_EQ(value, 0xAC);
	EXPECT_EQ(state.A, 0xAC);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x3F);
	EXPECT_EQ(state.E, 0x16);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, INX_B)
{
	const std::vector<uint8_t> rom = {
		0x03
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	state.B = 0x38;
	state.C = 0xFF;
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0x39);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 5);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, INX_D)
{
	const std::vector<uint8_t> rom = {
		0x13
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	state.D = 0x38;
	state.E = 0xFF;
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x39);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 5);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, INX_H)
{
	const std::vector<uint8_t> rom = {
		0x23
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	state.H = 0x38;
	state.L = 0xFF;
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0x39);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 5);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, INX_SP)
{
	const std::vector<uint8_t> rom = {
		0x33
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	state.SP = 0xFFFF;
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
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
	EXPECT_EQ(state.Cycles, 5);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, INR_B)
{
	const std::vector<uint8_t> rom = {
		0x04,
		0x04,
		0x04
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	opCode = &pMemoryMap->Peek(state.PC);
	isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	opCode = &pMemoryMap->Peek(state.PC);
	isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0x03);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x04);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 15);
	EXPECT_TRUE(state.P);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 3);
}

TEST_F(InstructionSetTest, INR_C)
{
	const std::vector<uint8_t> rom = {
		0x0C,
		0x0C,
		0x0C
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	opCode = &pMemoryMap->Peek(state.PC);
	isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	opCode = &pMemoryMap->Peek(state.PC);
	isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0x03);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x04);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 15);
	EXPECT_TRUE(state.P);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 3);
}

TEST_F(InstructionSetTest, INR_D)
{
	const std::vector<uint8_t> rom = {
		0x14,
		0x14,
		0x14
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	opCode = &pMemoryMap->Peek(state.PC);
	isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	opCode = &pMemoryMap->Peek(state.PC);
	isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x03);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x04);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 15);
	EXPECT_TRUE(state.P);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 3);
}

TEST_F(InstructionSetTest, INR_E)
{
	const std::vector<uint8_t> rom = {
		0x1C,
		0x1C,
		0x1C
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	opCode = &pMemoryMap->Peek(state.PC);
	isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	opCode = &pMemoryMap->Peek(state.PC);
	isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0x03);
	EXPECT_EQ(state.F, 0x04);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 15);
	EXPECT_TRUE(state.P);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 3);
}

TEST_F(InstructionSetTest, INR_H)
{
	const std::vector<uint8_t> rom = {
		0x24,
		0x24,
		0x24
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	opCode = &pMemoryMap->Peek(state.PC);
	isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	opCode = &pMemoryMap->Peek(state.PC);
	isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x04);
	EXPECT_EQ(state.H, 0x03);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 15);
	EXPECT_TRUE(state.P);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 3);
}

TEST_F(InstructionSetTest, INR_L)
{
	const std::vector<uint8_t> rom = {
		0x2C,
		0x2C,
		0x2C
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	opCode = &pMemoryMap->Peek(state.PC);
	isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	opCode = &pMemoryMap->Peek(state.PC);
	isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x04);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0x03);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 15);
	EXPECT_TRUE(state.P);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 3);
}

TEST_F(InstructionSetTest, INR_M)
{
	const std::vector<uint8_t> rom = {
		0x34,
		0x34,
		0x34
	};

	State state{};
	state.H = 0x25;
	state.L = 0x34;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	opCode = &pMemoryMap->Peek(state.PC);
	isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	opCode = &pMemoryMap->Peek(state.PC);
	isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	uint16_t adr = (state.H << 8) + state.L;
	auto value = pMemoryMap->Peek(adr);
	EXPECT_EQ(value, 3);
	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x04);
	EXPECT_EQ(state.H, 0x25);
	EXPECT_EQ(state.L, 0x34);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 30);
	EXPECT_TRUE(state.P);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 3);
}

TEST_F(InstructionSetTest, INR_A)
{
	const std::vector<uint8_t> rom = {
		0x3C,
		0x3C,
		0x3C
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode,isl->Size, isl->ClockCycle);
	opCode = &pMemoryMap->Peek(state.PC);
	isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	opCode = &pMemoryMap->Peek(state.PC);
	isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0x03);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x04);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 15);
	EXPECT_TRUE(state.P);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 3);
}

TEST_F(InstructionSetTest, RLC)
{
	const std::vector<uint8_t> rom = {
		0x07
	};

	State state{};
	state.A = 0xF2;

	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0xE5);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 1);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_TRUE(state.CY);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, RRC)
{
	const std::vector<uint8_t> rom = {
		0x0F
	};

	State state{};
	state.A = 0xF2;

	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0x79);
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
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, RAL)
{
	const std::vector<uint8_t> rom = {
		0x17
	};

	State state{};
	state.A = 0xB5;

	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0x6A);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 1);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_TRUE(state.CY);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, RAR)
{
	const std::vector<uint8_t> rom = {
		0x1F
	};

	State state{};
	state.A = 0x6A;
	state.CY = 1;

	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0xB5);
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
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, DAD_B)
{
	const std::vector<uint8_t> rom = {
		0x09
	};

	State state{};
	state.B = (0x339F & 0b1111111100000000) >> 8;
	state.C =  0x339F & 0b0000000011111111;
	state.H = (0xA17B & 0b1111111100000000) >> 8;
	state.L =  0xA17B & 0b0000000011111111;

	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, (0x339F & 0b1111111100000000) >> 8);
	EXPECT_EQ(state.C,  0x339F & 0b0000000011111111);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0xD5);
	EXPECT_EQ(state.L, 0x1A);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 10);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, DAD_D)
{
	const std::vector<uint8_t> rom = {
		0x19
	};

	State state{};
	state.D = (0x339F & 0b1111111100000000) >> 8;
	state.E= 0x339F & 0b0000000011111111;
	state.H = (0xA17B & 0b1111111100000000) >> 8;
	state.L = 0xA17B & 0b0000000011111111;

	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, (0x339F & 0b1111111100000000) >> 8);
	EXPECT_EQ(state.E, 0x339F & 0b0000000011111111);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0xD5);
	EXPECT_EQ(state.L, 0x1A);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 10);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, DAD_H)
{
	const std::vector<uint8_t> rom = {
		0x29
	};

	State state{};
	state.H = (0xA17B & 0b1111111100000000) >> 8;
	state.L = 0xA17B & 0b0000000011111111;

	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 1);
	EXPECT_EQ(state.H, (0x42F6 & 0b1111111100000000) >> 8);
	EXPECT_EQ(state.L, 0x42F6 & 0b0000000011111111);
	EXPECT_FALSE(state.AC);
	EXPECT_TRUE(state.CY);
	EXPECT_EQ(state.Cycles, 10);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, LDAX_B)
{
	const std::vector<uint8_t> rom = {
		0x0A
	};

	State state{};
	state.B = 0x93;
	state.C = 0x8B;

	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	pMemoryMap->Poke(0x938B, 0xAA);
	
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0xAA);
	EXPECT_EQ(state.B, 0x93);
	EXPECT_EQ(state.C, 0x8B);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, LDAX_D)
{
	const std::vector<uint8_t> rom = {
		0x1A
	};

	State state{};
	state.D = 0x93;
	state.E = 0x8B;

	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	pMemoryMap->Poke(0x938B, 0xAA);

	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0xAA);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x93);
	EXPECT_EQ(state.E, 0x8B);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, MVI_A)
{
	const std::vector<uint8_t> rom = {
		0x3E,
		0x3C
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0x3C);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 2);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, MVI_B)
{
	const std::vector<uint8_t> rom = {
		0x06,
		0x3C
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0x3C);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 2);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, MVI_C)
{
	const std::vector<uint8_t> rom = {
		0x0E,
		0x3C
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0x3C);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 2);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, MVI_D)
{
	const std::vector<uint8_t> rom = {
		0x16,
		0x3C
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x3C);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 2);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, MVI_E)
{
	const std::vector<uint8_t> rom = {
		0x1E,
		0x3C
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0x3C);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 2);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, MVI_H)
{
	const std::vector<uint8_t> rom = {
		0x26,
		0x3C
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0x3C);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 2);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, MVI_L)
{
	const std::vector<uint8_t> rom = {
		0x2E,
		0x3C
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0x3C);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 2);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, MVI_M)
{
	const std::vector<uint8_t> rom = {
		0x36,
		0xFF
	};

	State state{};
	state.H = 0x3C;
	state.L = 0xF4;

	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	auto value= pMemoryMap->Peek(0x3CF4);
	EXPECT_EQ(value, 0xFF);

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0x3C);
	EXPECT_EQ(state.L, 0xF4);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 10);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 2);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, DCX_B)
{
	const std::vector<uint8_t> rom = {
		0x0B
	};

	State state{};
	state.B = 0x98;
	state.C = 0x00;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0x97);
	EXPECT_EQ(state.C, 0xFF);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 5);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, DCX_D)
{
	const std::vector<uint8_t> rom = {
		0x1B
	};

	State state{};
	state.D = 0x98;
	state.E = 0x00;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x97);
	EXPECT_EQ(state.E, 0xFF);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 5);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, DCX_H)
{
	const std::vector<uint8_t> rom = {
		0x2B
	};

	State state{};
	state.H = 0x98;
	state.L= 0x00;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0x97);
	EXPECT_EQ(state.L, 0xFF);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 5);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, DCX_SP)
{
	const std::vector<uint8_t> rom = {
		0x3B
	};

	State state{};
	state.SP = 0xFF;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
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
	EXPECT_EQ(state.Cycles, 5);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0xFE);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, SHLD_adr)
{
	const std::vector<uint8_t> rom = {
		0x22,
		0x0A,
		0x01
	};

	State state{};
	state.H = 0xAE;
	state.L = 0x29;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	auto value1 = pMemoryMap->Peek(0x010A);
	auto value2 = pMemoryMap->Peek(0x010B);

	EXPECT_EQ(value1, 0x29);
	EXPECT_EQ(value2, 0xAE);


	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0xAE);
	EXPECT_EQ(state.L, 0x29);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 16);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, DAA)
{
	const std::vector<uint8_t> rom = {
		0x27
	};

	State state{};
	state.A = 0x9B;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 1);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0b00010001);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.AC);
	EXPECT_TRUE(state.CY);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, LHLD_adr)
{
	const std::vector<uint8_t> rom = {
		0x2A,
		0x5B,
		0x02
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	pMemoryMap->Poke(0x025B, 0xFF);
	pMemoryMap->Poke(0x025C, 0x03);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0x03);
	EXPECT_EQ(state.L, 0xFF);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 16);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, CMA)
{
	const std::vector<uint8_t> rom = {
		0x2F
	};

	State state{};
	state.A = 0b01010001;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0b10101110);
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
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, STA_adr)
{
	const std::vector<uint8_t> rom = {
		0x32,
		0xB3,
		0x05
	};

	State state{};
	state.A = 0x45;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	auto value = pMemoryMap->Peek(0x05B3);

	EXPECT_EQ(value, 0x45);
	EXPECT_EQ(state.A, 0x45);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 13);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, DCR_M)
{
	const std::vector<uint8_t> rom = {
		0x35
	};

	State state{};
	state.H = 0x3A;
	state.L = 0x7C;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	pMemoryMap->Poke(0x3A7C, 0x40);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	auto value = pMemoryMap->Peek(0x3A7C);

	EXPECT_EQ(value, 0x3F);
	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 4);
	EXPECT_EQ(state.H, 0x3A);
	EXPECT_EQ(state.L, 0x7C);
	EXPECT_FALSE(state.AC);
	EXPECT_FALSE(state.CY);
	EXPECT_EQ(state.Cycles, 10);
	EXPECT_TRUE(state.P);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, STC)
{
	const std::vector<uint8_t> rom = {
		0x37
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.setF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 1);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.AC);
	EXPECT_TRUE(state.CY);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.P);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.S);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Z);
	EXPECT_EQ(state.Steps, 1);
}