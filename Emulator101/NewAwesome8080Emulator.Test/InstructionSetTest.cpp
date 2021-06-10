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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0x01);
	EXPECT_EQ(state.C, 0x03);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 10);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x01);
	EXPECT_EQ(state.E, 0x03);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 10);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0x01);
	EXPECT_EQ(state.L, 0x03);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 10);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 10);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0x03ABC);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	auto value=pMemoryMap->Peek(0x03F16);

	EXPECT_EQ(value, 0xAC);
	EXPECT_EQ(state.A, 0xAC);
	EXPECT_EQ(state.B, 0x3F);
	EXPECT_EQ(state.C, 0x16);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	auto value = pMemoryMap->Peek(0x03F16);

	EXPECT_EQ(value, 0xAC);
	EXPECT_EQ(state.A, 0xAC);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x3F);
	EXPECT_EQ(state.E, 0x16);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0x39);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 5);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x39);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 5);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0x39);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 5);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 5);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0x03);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 15);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0x03);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 15);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x03);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 15);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0x03);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 15);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0x03);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 15);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0x03);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 15);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	uint16_t adr = (state.H << 8) + state.L;
	auto value = pMemoryMap->Peek(adr);
	EXPECT_EQ(value, 3);
	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0x25);
	EXPECT_EQ(state.L, 0x34);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 30);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x03);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 15);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0xE5);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x03);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_TRUE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x79);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x6A);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x03);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_TRUE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, RAR)
{
	const std::vector<uint8_t> rom = {
		0x1F
	};

	State state{};
	state.A = 0x6A;
	state.Flags.Carry = 1;

	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0xB5);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, (0x339F & 0b1111111100000000) >> 8);
	EXPECT_EQ(state.C,  0x339F & 0b0000000011111111);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0xD5);
	EXPECT_EQ(state.L, 0x1A);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 10);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, (0x339F & 0b1111111100000000) >> 8);
	EXPECT_EQ(state.E, 0x339F & 0b0000000011111111);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0xD5);
	EXPECT_EQ(state.L, 0x1A);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 10);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x03);
	EXPECT_EQ(state.H, (0x42F6 & 0b1111111100000000) >> 8);
	EXPECT_EQ(state.L, 0x42F6 & 0b0000000011111111);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_TRUE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 10);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0xAA);
	EXPECT_EQ(state.B, 0x93);
	EXPECT_EQ(state.C, 0x8B);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0xAA);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x93);
	EXPECT_EQ(state.E, 0x8B);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x3C);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 2);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0x3C);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 2);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0x3C);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 2);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x3C);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 2);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0x3C);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 2);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0x3C);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 2);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0x3C);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 2);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	auto value= pMemoryMap->Peek(0x3CF4);
	EXPECT_EQ(value, 0xFF);

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0x3C);
	EXPECT_EQ(state.L, 0xF4);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 10);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 2);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0x97);
	EXPECT_EQ(state.C, 0xFF);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 5);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x97);
	EXPECT_EQ(state.E, 0xFF);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 5);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0x97);
	EXPECT_EQ(state.L, 0xFF);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 5);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 5);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0xFE);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	auto value1 = pMemoryMap->Peek(0x010A);
	auto value2 = pMemoryMap->Peek(0x010B);

	EXPECT_EQ(value1, 0x29);
	EXPECT_EQ(value2, 0xAE);


	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0xAE);
	EXPECT_EQ(state.L, 0x29);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 16);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 1);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x13);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_TRUE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0x03);
	EXPECT_EQ(state.L, 0xFF);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 16);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0b10101110);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	auto value = pMemoryMap->Peek(0x05B3);

	EXPECT_EQ(value, 0x45);
	EXPECT_EQ(state.A, 0x45);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 13);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, DCR_B)
{
	const std::vector<uint8_t> rom = {
		0x05
	};

	State state{};
	state.B = 0X40;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F = state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0x3F);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 5);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, DCR_C)
{
	const std::vector<uint8_t> rom = {
		0x0D
	};

	State state{};
	state.C = 0X40;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F = state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0x3F);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 5);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, DCR_D)
{
	const std::vector<uint8_t> rom = {
		0x15
	};

	State state{};
	state.D = 0X40;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F = state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x3F);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 5);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, DCR_E)
{
	const std::vector<uint8_t> rom = {
		0x1D
	};

	State state{};
	state.E = 0X40;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F = state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0x3F);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 5);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, DCR_H)
{
	const std::vector<uint8_t> rom = {
		0x25
	};

	State state{};
	state.H = 0X40;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F = state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0x3F);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 5);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, DCR_L)
{
	const std::vector<uint8_t> rom = {
		0x2D
	};

	State state{};
	state.L = 0X40;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F = state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0x3F);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 5);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	auto value = pMemoryMap->Peek(0x3A7C);

	EXPECT_EQ(value, 0x3F);
	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0x3A);
	EXPECT_EQ(state.L, 0x7C);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 10);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, DCR_A)
{
	const std::vector<uint8_t> rom = {
		0x3D
	};

	State state{};
	state.A = 0X40;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F = state.Flags.getF();

	EXPECT_EQ(state.A, 0x3F);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 5);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
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
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x03);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_TRUE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, LDA_adr)
{
	const std::vector<uint8_t> rom = {
		0x3A,
		0x00,
		0x03
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);

	pMemoryMap->Poke(0x0300, 0x34);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x34);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 13);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 3);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, DAD_SP)
{
	const std::vector<uint8_t> rom = {
		0x39
	};

	State state{};
	state.SP = 0xA17B;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0xA1);
	EXPECT_EQ(state.L, 0x7B);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 10);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0xA17B);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, CMC)
{
	const std::vector<uint8_t> rom = {
		0x3F
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x03);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_TRUE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ADD_B)
{
	const std::vector<uint8_t> rom = {
		0x80
	};

	State state{};
	state.A = 0x6C;
	state.B = 0x2E;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x9A);
	EXPECT_EQ(state.B, 0x2E);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x96);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ADD_C)
{
	const std::vector<uint8_t> rom = {
		0x81
	};

	State state{};
	state.A = 0x6C;
	state.C = 0x2E;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x9A);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0x2E);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x96);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ADD_D)
{
	const std::vector<uint8_t> rom = {
		0x82
	};

	State state{};
	state.A = 0x6C;
	state.D = 0x2E;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x9A);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x2E);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x96);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ADD_E)
{
	const std::vector<uint8_t> rom = {
		0x83
	};

	State state{};
	state.A = 0x6C;
	state.E = 0x2E;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x9A);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0x2E);
	EXPECT_EQ(state.F, 0x96);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ADD_H)
{
	const std::vector<uint8_t> rom = {
		0x84
	};

	State state{};
	state.A = 0x6C;
	state.H = 0x2E;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x9A);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x96);
	EXPECT_EQ(state.H, 0x2E);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ADD_L)
{
	const std::vector<uint8_t> rom = {
		0x85
	};

	State state{};
	state.A = 0x6C;
	state.L = 0x2E;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x9A);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x96);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0x2E);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ADD_M)
{
	const std::vector<uint8_t> rom = {
		0x86
	};

	State state{};
	state.A = 0x6C;
	state.H = 0x2E;
	state.L = 0x2E;
	uint16_t adr = (state.H << 8) + state.L;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	pMemoryMap->Poke(adr, 0x2E);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x9A);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x96);
	EXPECT_EQ(state.H, 0x2E);
	EXPECT_EQ(state.L, 0x2E);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ADD_A)
{
	const std::vector<uint8_t> rom = {
		0x87
	};

	State state{};
	state.A = 0x6C;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0xD8);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x96);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_B)
{
	const std::vector<uint8_t> rom = {
		0x88
	};

	State state{};
	state.B = 0x3D;
	state.A = 0x42;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x7F);
	EXPECT_EQ(state.B, 0x3D);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0X02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_B2)
{
	const std::vector<uint8_t> rom = {
		0x88
	};

	State state{};
	state.B = 0x3D;
	state.A = 0x42;
	state.Flags.Carry = true;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x80);
	EXPECT_EQ(state.B, 0x3D);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x92);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_C)
{
	const std::vector<uint8_t> rom = {
		0x89
	};

	State state{};
	state.C = 0x3D;
	state.A = 0x42;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x7F);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0x3D);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_C2)
{
	const std::vector<uint8_t> rom = {
		0x89
	};

	State state{};
	state.C = 0x3D;
	state.A = 0x42;
	state.Flags.Carry = true;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x80);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0x3D);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x92);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_D)
{
	const std::vector<uint8_t> rom = {
		0x8A
	};

	State state{};
	state.D = 0x3D;
	state.A = 0x42;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x7F);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x3D);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_D2)
{
	const std::vector<uint8_t> rom = {
		0x8A
	};

	State state{};
	state.D = 0x3D;
	state.A = 0x42;
	state.Flags.Carry = true;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x80);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x3D);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x92);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_E)
{
	const std::vector<uint8_t> rom = {
		0x8B
	};

	State state{};
	state.E = 0x3D;
	state.A = 0x42;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x7F);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0x3D);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_E2)
{
	const std::vector<uint8_t> rom = {
		0x8B
	};

	State state{};
	state.E = 0x3D;
	state.A = 0x42;
	state.Flags.Carry = true;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x80);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0x3D);
	EXPECT_EQ(state.F, 0x92);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_H)
{
	const std::vector<uint8_t> rom = {
		0x8C
	};

	State state{};
	state.H = 0x3D;
	state.A = 0x42;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x7F);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0x3D);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_H2)
{
	const std::vector<uint8_t> rom = {
		0x8C
	};

	State state{};
	state.H = 0x3D;
	state.A = 0x42;
	state.Flags.Carry = true;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x80);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x92);
	EXPECT_EQ(state.H, 0x3D);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_L)
{
	const std::vector<uint8_t> rom = {
		0x8D
	};

	State state{};
	state.L = 0x3D;
	state.A = 0x42;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x7F);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0x3D);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_L2)
{
	const std::vector<uint8_t> rom = {
		0x8D
	};

	State state{};
	state.L = 0x3D;
	state.A = 0x42;
	state.Flags.Carry = true;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x80);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x92);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0x3D);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_A)
{
	const std::vector<uint8_t> rom = {
		0x8F
	};

	State state{};
	state.A = 0x42;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x84);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x86);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}


TEST_F(InstructionSetTest, ADC_M)
{
	const std::vector<uint8_t> rom = {
		0x8E
	};

	State state{};
	state.A = 0x42;
	state.H = 0x2E;
	state.L = 0x2E;
	uint16_t adr = (state.H << 8) + state.L;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	pMemoryMap->Poke(adr, 0x3D);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x7F);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0x2E);
	EXPECT_EQ(state.L, 0x2E);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_M2)
{
	const std::vector<uint8_t> rom = {
		0x8E
	};

	State state{};
	state.A = 0x42;
	state.H = 0x2E;
	state.L = 0x2E;
	state.Flags.Carry = true;
	uint16_t adr = (state.H << 8) + state.L;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	pMemoryMap->Poke(adr, 0x3D);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x80);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x92);
	EXPECT_EQ(state.H, 0x2E);
	EXPECT_EQ(state.L, 0x2E);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, SUB_B)
{
	const std::vector<uint8_t> rom = {
		0x90
	};

	State state{};
	state.B = 0x3D;
	state.A = 0x42;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x05);
	EXPECT_EQ(state.B, 0x3D);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, SUB_C)
{
	const std::vector<uint8_t> rom = {
		0x91
	};

	State state{};
	state.C = 0x3D;
	state.A = 0x42;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x05);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0x3D);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, SUB_D)
{
	const std::vector<uint8_t> rom = {
		0x92
	};

	State state{};
	state.D = 0x3D;
	state.A = 0x42;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x05);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x3D);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, SUB_E)
{
	const std::vector<uint8_t> rom = {
		0x93
	};

	State state{};
	state.E = 0x3D;
	state.A = 0x42;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x05);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0x3D);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, SUB_H)
{
	const std::vector<uint8_t> rom = {
		0x94
	};

	State state{};
	state.H = 0x3D;
	state.A = 0x42;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x05);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0x3D);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, SUB_L)
{
	const std::vector<uint8_t> rom = {
		0x95
	};

	State state{};
	state.L = 0x3D;
	state.A = 0x42;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x05);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0x3D);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, SUB_A)
{
	const std::vector<uint8_t> rom = {
		0x97
	};

	State state{};
	state.A = 0x42;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x56);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_TRUE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, SUB_M)
{
	const std::vector<uint8_t> rom = {
		0x96
	};

	State state{};
	state.A = 0x42;
	state.H = 0x12;
	state.L = 0x12;
	uint16_t adr = (state.H << 8) + state.L;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	pMemoryMap->Poke(adr, 0x3D);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x05);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0x12);
	EXPECT_EQ(state.L, 0x12);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, SBB_B)
{
	const std::vector<uint8_t> rom = {
		0x98
	};

	State state{};
	state.A = 0x04;
	state.B = 0x02;
	state.Flags.Carry = true;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x01);
	EXPECT_EQ(state.B, 0x02);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x12);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, SBB_C)
{
	const std::vector<uint8_t> rom = {
		0x99
	};

	State state{};
	state.A = 0x04;
	state.C = 0x02;
	state.Flags.Carry = true;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x01);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0x02);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x12);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, SBB_D)
{
	const std::vector<uint8_t> rom = {
		0x9A
	};

	State state{};
	state.A = 0x04;
	state.D = 0x02;
	state.Flags.Carry = true;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x01);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x02);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x12);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, SBB_E)
{
	const std::vector<uint8_t> rom = {
		0x9B
	};

	State state{};
	state.A = 0x04;
	state.E = 0x02;
	state.Flags.Carry = true;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x01);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0x02);
	EXPECT_EQ(state.F, 0x12);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, SBB_H)
{
	const std::vector<uint8_t> rom = {
		0x9C
	};

	State state{};
	state.A = 0x04;
	state.H = 0x02;
	state.Flags.Carry = true;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x01);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x12);
	EXPECT_EQ(state.H, 0x02);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, SBB_L)
{
	const std::vector<uint8_t> rom = {
		0x9D
	};

	State state{};
	state.A = 0x04;
	state.L = 0x02;
	state.Flags.Carry = true;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x01);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x12);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0x02);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, SBB_M)
{
	const std::vector<uint8_t> rom = {
		0x9E
	};

	State state{};
	state.A = 0x04;
	state.H = 0x12;
	state.L = 0x12;
	state.Flags.Carry = true;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	uint16_t adr = (state.H << 8) + state.L;
	pMemoryMap->Poke(adr, 0x02);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x01);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x12);
	EXPECT_EQ(state.H, 0x12);
	EXPECT_EQ(state.L, 0x12);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, SBB_A)
{
	const std::vector<uint8_t> rom = {
		0x9F
	};

	State state{};
	state.A = 0x04;
	state.Flags.Carry = true;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0xFF);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x87);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_TRUE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ANA_B)
{
	const std::vector<uint8_t> rom = {
		0xA0
	};

	State state{};
	state.A = 0xFC;
	state.B = 0x0F;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x0C);
	EXPECT_EQ(state.B, 0x0F);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ANA_C)
{
	const std::vector<uint8_t> rom = {
		0xA1
	};

	State state{};
	state.A = 0xFC;
	state.C = 0x0F;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x0C);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0x0F);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ANA_D)
{
	const std::vector<uint8_t> rom = {
		0xA2
	};

	State state{};
	state.A = 0xFC;
	state.D = 0x0F;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x0C);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x0F);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ANA_E)
{
	const std::vector<uint8_t> rom = {
		0xA3
	};

	State state{};
	state.A = 0xFC;
	state.E = 0x0F;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x0C);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0x0F);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}


TEST_F(InstructionSetTest, ANA_H)
{
	const std::vector<uint8_t> rom = {
		0xA4
	};

	State state{};
	state.A = 0xFC;
	state.H = 0x0F;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x0C);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0x0F);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ANA_L)
{
	const std::vector<uint8_t> rom = {
		0xA5
	};

	State state{};
	state.A = 0xFC;
	state.L = 0x0F;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x0C);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0x0F);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ANA_M)
{
	const std::vector<uint8_t> rom = {
		0xA6
	};

	State state{};
	state.A = 0xFC;
	state.H = 0x12;
	state.L = 0x12;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	uint16_t adr = (state.H << 8) + state.L;
	pMemoryMap->Poke(adr, 0x0F);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x0C);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0x12);
	EXPECT_EQ(state.L, 0x12);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ANA_A)
{
	const std::vector<uint8_t> rom = {
		0xA7
	};

	State state{};
	state.A = 0xFC;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0xFC);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x86);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, XRA_B)
{
	const std::vector<uint8_t> rom = {
		0xA8
	};

	State state{};
	state.A = 0xFC;
	state.B = 0x0F;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0xF3);
	EXPECT_EQ(state.B, 0x0F);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x86);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, XRA_C)
{
	const std::vector<uint8_t> rom = {
		0xA9
	};

	State state{};
	state.A = 0xFC;
	state.C = 0x0F;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0xF3);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0x0F);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x86);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, XRA_D)
{
	const std::vector<uint8_t> rom = {
		0xAA
	};

	State state{};
	state.A = 0xFC;
	state.D = 0x0F;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0xF3);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x0F);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x86);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, XRA_E)
{
	const std::vector<uint8_t> rom = {
		0xAB
	};

	State state{};
	state.A = 0xFC;
	state.E = 0x0F;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0xF3);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0x0F);
	EXPECT_EQ(state.F, 0x86);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, XRA_H)
{
	const std::vector<uint8_t> rom = {
		0xAC
	};

	State state{};
	state.A = 0xFC;
	state.H = 0x0F;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0xF3);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x86);
	EXPECT_EQ(state.H, 0x0F);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, XRA_L)
{
	const std::vector<uint8_t> rom = {
		0xAD
	};

	State state{};
	state.A = 0xFC;
	state.L = 0x0F;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0xF3);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x86);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0x0F);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, XRA_M)
{
	const std::vector<uint8_t> rom = {
		0xAE
	};

	State state{};
	state.A = 0xFC;
	state.H = 0x12;
	state.L = 0x12;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	uint16_t adr = (state.H << 8) + state.L;
	pMemoryMap->Poke(adr, 0x0F);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0xF3);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x86);
	EXPECT_EQ(state.H, 0x12);
	EXPECT_EQ(state.L, 0x12);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ORA_B)
{
	const std::vector<uint8_t> rom = {
		0xB0
	};

	State state{};
	state.A = 0x33;
	state.B = 0x0F;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x3F);
	EXPECT_EQ(state.B, 0x0F);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ORA_C)
{
	const std::vector<uint8_t> rom = {
		0xB1
	};

	State state{};
	state.A = 0x33;
	state.C = 0x0F;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x3F);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0x0F);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ORA_D)
{
	const std::vector<uint8_t> rom = {
		0xB2
	};

	State state{};
	state.A = 0x33;
	state.D = 0x0F;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x3F);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x0F);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ORA_E)
{
	const std::vector<uint8_t> rom = {
		0xB3
	};

	State state{};
	state.A = 0x33;
	state.E = 0x0F;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x3F);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0x0F);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ORA_H)
{
	const std::vector<uint8_t> rom = {
		0xB4
	};

	State state{};
	state.A = 0x33;
	state.H = 0x0F;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x3F);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0x0F);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ORA_L)
{
	const std::vector<uint8_t> rom = {
		0xB5
	};

	State state{};
	state.A = 0x33;
	state.L = 0x0F;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x3F);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0x0F);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ORA_M)
{
	const std::vector<uint8_t> rom = {
		0xB6
	};

	State state{};
	state.A = 0x33;
	state.H = 0x12;
	state.L = 0x12;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	uint16_t adr = (state.H << 8) + state.L;
	pMemoryMap->Poke(adr, 0x0F);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x3F);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0x12);
	EXPECT_EQ(state.L, 0x12);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ORA_A)
{
	const std::vector<uint8_t> rom = {
		0xB7
	};

	State state{};
	state.A = 0x33;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x33);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_B)
{
	const std::vector<uint8_t> rom = {
		0xB8
	};

	State state{};
	state.A = 0x0A;
	state.B = 0x05;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x0A);
	EXPECT_EQ(state.B, 0x05);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x16);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_B2)
{
	const std::vector<uint8_t> rom = {
		0xB8
	};

	State state{};
	state.A = 0x02;
	state.B = 0x05;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x02);
	EXPECT_EQ(state.B, 0x05);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x83);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_TRUE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_C)
{
	const std::vector<uint8_t> rom = {
		0xB9
	};

	State state{};
	state.A = 0x0A;
	state.C = 0x05;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x0A);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0x05);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x16);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_C2)
{
	const std::vector<uint8_t> rom = {
		0xB9
	};

	State state{};
	state.A = 0x02;
	state.C = 0x05;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x02);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0x05);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x83);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_TRUE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_D)
{
	const std::vector<uint8_t> rom = {
		0xBA
	};

	State state{};
	state.A = 0x0A;
	state.D = 0x05;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x0A);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x05);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x16);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_D2)
{
	const std::vector<uint8_t> rom = {
		0xBA
	};

	State state{};
	state.A = 0x02;
	state.D = 0x05;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x02);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x05);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x83);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_TRUE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_E)
{
	const std::vector<uint8_t> rom = {
		0xBB
	};

	State state{};
	state.A = 0x0A;
	state.E = 0x05;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x0A);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0x05);
	EXPECT_EQ(state.F, 0x16);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_E2)
{
	const std::vector<uint8_t> rom = {
		0xBB
	};

	State state{};
	state.A = 0x02;
	state.E = 0x05;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x02);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0x05);
	EXPECT_EQ(state.F, 0x83);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_TRUE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_H)
{
	const std::vector<uint8_t> rom = {
		0xBC
	};

	State state{};
	state.A = 0x0A;
	state.H = 0x05;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x0A);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x16);
	EXPECT_EQ(state.H, 0x05);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_H2)
{
	const std::vector<uint8_t> rom = {
		0xBC
	};

	State state{};
	state.A = 0x02;
	state.H = 0x05;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x02);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x83);
	EXPECT_EQ(state.H, 0x05);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_TRUE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_L)
{
	const std::vector<uint8_t> rom = {
		0xBD
	};

	State state{};
	state.A = 0x0A;
	state.L = 0x05;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x0A);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x16);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0x05);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_L2)
{
	const std::vector<uint8_t> rom = {
		0xBD
	};

	State state{};
	state.A = 0x02;
	state.L = 0x05;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x02);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x83);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0x05);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_TRUE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_M)
{
	const std::vector<uint8_t> rom = {
		0xBE
	};

	State state{};
	state.A = 0x0A;
	state.H = 0x12;
	state.L = 0x12;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	uint16_t adr = (state.H << 8) + state.L;
	pMemoryMap->Poke(adr, 0x05);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x0A);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x16);
	EXPECT_EQ(state.H, 0x12);
	EXPECT_EQ(state.L, 0x12);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_M2)
{
	const std::vector<uint8_t> rom = {
		0xBE
	};

	State state{};
	state.A = 0x02;
	state.H = 0x12;
	state.L = 0x12;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	uint16_t adr = (state.H << 8) + state.L;
	pMemoryMap->Poke(adr, 0x05);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x02);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x83);
	EXPECT_EQ(state.H, 0x12);
	EXPECT_EQ(state.L, 0x12);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_TRUE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_A)
{
	const std::vector<uint8_t> rom = {
		0xBF
	};

	State state{};
	state.A = 0x0A;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x0A);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x56);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_TRUE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_A2)
{
	const std::vector<uint8_t> rom = {
		0xBF
	};

	State state{};
	state.A = 0x02;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0x02);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x56);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_TRUE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, SUI_D8)
{
	const std::vector<uint8_t> rom = {
		0xD6,
		0x01
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0xFF);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x87);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_TRUE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 2);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, SBI_D8)
{
	const std::vector<uint8_t> rom = {
		0xD6,
		0x01
	};

	State state{};
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0xFF);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x87);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_TRUE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 2);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, SBI_D8_2)
{
	const std::vector<uint8_t> rom = {
		0xDE,
		0x01
	};

	State state{};
	state.Flags.Carry = true;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0xFE);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x83);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_TRUE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 2);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ORI_D8)
{
	const std::vector<uint8_t> rom = {
		0xF6,
		0x0F
	};

	State state{};
	state.A = 0xB5;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0xBF);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x82);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 2);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, XRI_D8)
{
	const std::vector<uint8_t> rom = {
		0xEE,
		0x81
	};

	State state{};
	state.A = 0x3B;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F=state.Flags.getF();

	EXPECT_EQ(state.A, 0xBA);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x82);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 2);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, PUSH_B)
{
	const std::vector<uint8_t> rom = {
		0xC5
	};

	State state{};
	state.B = 0x8F;
	state.C = 0x9D;
	state.SP = 0x3A2C;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F = state.Flags.getF();

	EXPECT_EQ(pMemoryMap->Peek(0x3A2A), 0x9D);
	EXPECT_EQ(pMemoryMap->Peek(0x3A2B), 0x8F);

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0x8F);
	EXPECT_EQ(state.C, 0x9D);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 11);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0x3A2A);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, PUSH_D)
{
	const std::vector<uint8_t> rom = {
		0xD5
	};

	State state{};
	state.D = 0x8F;
	state.E = 0x9D;
	state.SP = 0x3A2C;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F = state.Flags.getF();

	EXPECT_EQ(pMemoryMap->Peek(0x3A2A), 0x9D);
	EXPECT_EQ(pMemoryMap->Peek(0x3A2B), 0x8F);

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x8F);
	EXPECT_EQ(state.E, 0x9D);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 11);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0x3A2A);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, PUSH_H)
{
	const std::vector<uint8_t> rom = {
		0xE5
	};

	State state{};
	state.H = 0x8F;
	state.L = 0x9D;
	state.SP = 0x3A2C;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F = state.Flags.getF();

	EXPECT_EQ(pMemoryMap->Peek(0x3A2A), 0x9D);
	EXPECT_EQ(pMemoryMap->Peek(0x3A2B), 0x8F);

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0x8F);
	EXPECT_EQ(state.L, 0x9D);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 11);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0x3A2A);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, PUSH_PSW)
{
	const std::vector<uint8_t> rom = {
		0xF5
	};

	State state{};
	state.A = 0x1F;
	state.SP = 0x502A;
	state.Flags.Carry = true;
	state.Flags.Zero = true;
	state.Flags.Parity = true;
	state.F = state.Flags.getF();
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	
	EXPECT_EQ(pMemoryMap->Peek(0x5029), 0x1F);
	EXPECT_EQ(pMemoryMap->Peek(0x5028), 0x47);

	EXPECT_EQ(state.A, 0x1F);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x47);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_TRUE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 11);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0x5028);
	EXPECT_TRUE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, CALL)
{
	std::vector<uint8_t> rom = {
		0xCD, 0x66, 0x06
	};
	rom.resize(65536);

	State state{};
	state.F = state.Flags.getF();

	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);

	EXPECT_EQ(pMemoryMap->Peek(0xFFFF), 0x00);
	EXPECT_EQ(pMemoryMap->Peek(0xFFFE), 0x03);

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 17);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 0x666);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0xFFFE);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, XCHG)
{
	std::vector<uint8_t> rom = {
		0xEB
	};

	State state{};
	state.F = state.Flags.getF();
	state.D = 0x33;
	state.E = 0x55;
	state.H = 0x00;
	state.L = 0xFF;

	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x00);
	EXPECT_EQ(state.E, 0xFF);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0x33);
	EXPECT_EQ(state.L, 0x55);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 4);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, POP_B)
{
	std::vector<uint8_t> rom = {
		0xC1
	};

	State state{};
	state.F = state.Flags.getF();
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];

	rom.resize(65536);
	pMemoryMap->Poke(0x1239, 0x3D);
	pMemoryMap->Poke(0x123A, 0x93);
	state.SP = 0x1239;

	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0x93);
	EXPECT_EQ(state.C, 0x3D);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 10);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0x123B);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, POP_D)
{
	std::vector<uint8_t> rom = {
		0xD1
	};

	State state{};
	state.F = state.Flags.getF();
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];

	rom.resize(65536);
	pMemoryMap->Poke(0x1239, 0x3D);
	pMemoryMap->Poke(0x123A, 0x93);
	state.SP = 0x1239;

	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0x93);
	EXPECT_EQ(state.E, 0x3D);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 10);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0x123B);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, POP_H)
{
	std::vector<uint8_t> rom = {
		0xE1
	};

	State state{};
	state.F = state.Flags.getF();
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];

	rom.resize(65536);
	pMemoryMap->Poke(0x1239, 0x3D);
	pMemoryMap->Poke(0x123A, 0x93);
	state.SP = 0x1239;

	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0x93);
	EXPECT_EQ(state.L, 0x3D);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 10);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0x123B);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, POP_PSW)
{
	std::vector<uint8_t> rom = {
		0xF1
	};
	State state{};
	state.F = state.Flags.getF();
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];

	rom.resize(65536);
	pMemoryMap->Poke(0x2C00, 0xC3);
	pMemoryMap->Poke(0x2C01, 0xFF);
	state.SP = 0x2C00;

	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);

	EXPECT_EQ(state.A, 0xFF);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0xC3);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_TRUE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 10);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0x2C02);
	EXPECT_TRUE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}


TEST_F(InstructionSetTest, CPI)
{
	const std::vector<uint8_t> rom = {
		0xFE, 0x40
	};

	State state{};
	state.A = 0x4A;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F = state.Flags.getF();

	EXPECT_EQ(state.A, 0x4A);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x16);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 2);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, CPI_1)
{
	const std::vector<uint8_t> rom = {
		0xFE, 0x4A
	};

	State state{};
	state.A = 0x4A;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F = state.Flags.getF();

	EXPECT_EQ(state.A, 0x4A);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x46);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 2);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_TRUE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, CPI_2)
{
	const std::vector<uint8_t> rom = {
		0xFE, 0x4B
	};

	State state{};
	state.A = 0x4A;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F = state.Flags.getF();

	EXPECT_EQ(state.A, 0x4A);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x97);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_TRUE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 2);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, CPI_3)
{
	const std::vector<uint8_t> rom = {
		0xFE, 0x00
	};

	State state{};
	state.A = 0xF5;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F = state.Flags.getF();

	EXPECT_EQ(state.A, 0xF5);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x86);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 2);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, XTHL)
{
	std::vector<uint8_t> rom = {
		0xE3
	};
	rom.resize(65536);
	State state{};
	state.SP = 0x10AD;
	state.H = 0x0B;
	state.L = 0x3C;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];

	pMemoryMap->Poke(0x10AD, 0xF0);
	pMemoryMap->Poke(0x10AE, 0x0D);
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F = state.Flags.getF();


	EXPECT_EQ(pMemoryMap->Peek(0x10AD), 0x3C);
	EXPECT_EQ(pMemoryMap->Peek(0x10AE), 0x0B);

	EXPECT_EQ(state.A, 0);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0x0D);
	EXPECT_EQ(state.L, 0xF0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 18);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 1);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0x10AD);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ADI)
{
	std::vector<uint8_t> rom = {
		0xC6, 0x42, 0xC6, 0xBE
	};
	State state{};
	state.A = 0x14;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F = state.Flags.getF();

	EXPECT_EQ(state.A, 0x56);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 2);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);

	opCode = &pMemoryMap->Peek(state.PC);
	isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F = state.Flags.getF();

	EXPECT_EQ(state.A, 0x14);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x17);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_TRUE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 14);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 4);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 2);
}

TEST_F(InstructionSetTest, ACI)
{
	std::vector<uint8_t> rom = {
		0xCE, 0xBE, 0xCE, 0x42
	};
	State state{};
	state.A = 0x56;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F = state.Flags.getF();

	EXPECT_EQ(state.A, 0x14);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x17);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_TRUE(state.Flags.AuxiliaryCarry);
	EXPECT_TRUE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 2);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);

	opCode = &pMemoryMap->Peek(state.PC);
	isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F = state.Flags.getF();

	EXPECT_EQ(state.A, 0x57);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x02);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 14);
	EXPECT_FALSE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 4);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 2);
}

TEST_F(InstructionSetTest, SUI)
{
	std::vector<uint8_t> rom = {
		0xD6, 0x01
	};
	State state{};
	state.A = 0x00;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F = state.Flags.getF();

	EXPECT_EQ(state.A, 0xFF);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x87);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_TRUE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 2);
	EXPECT_TRUE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}

TEST_F(InstructionSetTest, ANI)
{
	std::vector<uint8_t> rom = {
		0xE6, 0x4F
	};
	State state{};
	state.A = 0x3A;
	auto pMemoryMap = std::make_shared<MemoryMap>(rom, (uint16_t)0xFFFF, (uint16_t)0x2000, (uint16_t)0x2400, (uint16_t)0x4000);
	auto opCode = &pMemoryMap->Peek(state.PC);
	auto isl = (*p_InstructionSet)[opCode[0]];
	isl->exec(state, *pMemoryMap, opCode, isl->Size, isl->ClockCycle);
	state.F = state.Flags.getF();

	EXPECT_EQ(state.A, 0x0A);
	EXPECT_EQ(state.B, 0);
	EXPECT_EQ(state.C, 0);
	EXPECT_EQ(state.D, 0);
	EXPECT_EQ(state.E, 0);
	EXPECT_EQ(state.F, 0x06);
	EXPECT_EQ(state.H, 0);
	EXPECT_EQ(state.L, 0);
	EXPECT_FALSE(state.Flags.AuxiliaryCarry);
	EXPECT_FALSE(state.Flags.Carry);
	EXPECT_EQ(state.Cycles, 7);
	EXPECT_TRUE(state.Flags.Parity);
	EXPECT_EQ(state.PC, 2);
	EXPECT_FALSE(state.Flags.Sign);
	EXPECT_EQ(state.SP, 0);
	EXPECT_FALSE(state.Flags.Zero);
	EXPECT_EQ(state.Steps, 1);
}