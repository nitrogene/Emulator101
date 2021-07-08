#include "InstructionSetTest.h"

TEST_F(InstructionSetTest, NOP)
{
	const std::vector<uint8_t> rom = {
		0x00
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps,1);
}

TEST_F(InstructionSetTest, LXI_B)
{
	const std::vector<uint8_t> rom = {
		0x01,
		0x03,
		0x01
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0x01);
	EXPECT_EQ(m_State.C, 0x03);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 10);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 3);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, LXI_D)
{
	const std::vector<uint8_t> rom = {
		0x11,
		0x03,
		0x01
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0x01);
	EXPECT_EQ(m_State.E, 0x03);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 10);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 3);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, LXI_H)
{
	const std::vector<uint8_t> rom = {
		0x21,
		0x03,
		0x01
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0x01);
	EXPECT_EQ(m_State.L, 0x03);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 10);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 3);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, LXI_SP)
{
	const std::vector<uint8_t> rom = {
		0x31,
		0xBC,
		0x3A
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 10);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 3);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0x03ABC);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, STAX_B)
{
	const std::vector<uint8_t> rom = {
		0x02
	};
	p_Processor->Initialize(rom);
	m_State.B = 0x3F;
	m_State.C = 0x16;
	m_State.A = 0xAC;
	p_Processor->RunStep();
	auto value=m_MemoryMap.Peek(0x03F16);

	EXPECT_EQ(value, 0xAC);
	EXPECT_EQ(m_State.A, 0xAC);
	EXPECT_EQ(m_State.B, 0x3F);
	EXPECT_EQ(m_State.C, 0x16);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, STAX_D)
{
	const std::vector<uint8_t> rom = {
		0x12
	};
	p_Processor->Initialize(rom);
	m_State.D = 0x3F;
	m_State.E = 0x16;
	m_State.A = 0xAC;
	p_Processor->RunStep();
	auto value = m_MemoryMap.Peek(0x03F16);

	EXPECT_EQ(value, 0xAC);
	EXPECT_EQ(m_State.A, 0xAC);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0x3F);
	EXPECT_EQ(m_State.E, 0x16);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, INX_B)
{
	const std::vector<uint8_t> rom = {
		0x03
	};
	p_Processor->Initialize(rom);
	m_State.B = 0x38;
	m_State.C = 0xFF;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0x39);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 5);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, INX_D)
{
	const std::vector<uint8_t> rom = {
		0x13
	};
	p_Processor->Initialize(rom);
	m_State.D = 0x38;
	m_State.E = 0xFF;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0x39);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 5);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, INX_H)
{
	const std::vector<uint8_t> rom = {
		0x23
	};
	p_Processor->Initialize(rom);
	m_State.H = 0x38;
	m_State.L = 0xFF;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0x39);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 5);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, INX_SP)
{
	const std::vector<uint8_t> rom = {
		0x33
	};
	p_Processor->Initialize(rom);
	m_State.SP = 0xFFFF;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 5);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, INR_B)
{
	const std::vector<uint8_t> rom = {
		0x04,
		0x04,
		0x04
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();
	p_Processor->RunStep();
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0x03);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 15);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 3);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 3);
}

TEST_F(InstructionSetTest, INR_C)
{
	const std::vector<uint8_t> rom = {
		0x0C,
		0x0C,
		0x0C
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();
	p_Processor->RunStep();
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0x03);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 15);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 3);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 3);
}

TEST_F(InstructionSetTest, INR_D)
{
	const std::vector<uint8_t> rom = {
		0x14,
		0x14,
		0x14
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();
	p_Processor->RunStep();
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0x03);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 15);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 3);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 3);
}

TEST_F(InstructionSetTest, INR_E)
{
	const std::vector<uint8_t> rom = {
		0x1C,
		0x1C,
		0x1C
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();
	p_Processor->RunStep();
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0x03);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 15);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 3);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 3);
}

TEST_F(InstructionSetTest, INR_H)
{
	const std::vector<uint8_t> rom = {
		0x24,
		0x24,
		0x24
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();
	p_Processor->RunStep();
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0x03);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 15);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 3);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 3);
}

TEST_F(InstructionSetTest, INR_L)
{
	const std::vector<uint8_t> rom = {
		0x2C,
		0x2C,
		0x2C
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();
	p_Processor->RunStep();
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0x03);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 15);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 3);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 3);
}

TEST_F(InstructionSetTest, INR_M)
{
	const std::vector<uint8_t> rom = {
		0x34,
		0x34,
		0x34
	};
	p_Processor->Initialize(rom);
	m_State.H = 0x25;
	m_State.L = 0x34;
	p_Processor->RunStep();
	p_Processor->RunStep();
	p_Processor->RunStep();
	uint16_t adr = (m_State.H << 8) + m_State.L;
	auto value = m_MemoryMap.Peek(adr);

	EXPECT_EQ(value, 3);
	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0x25);
	EXPECT_EQ(m_State.L, 0x34);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 30);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 3);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 3);
}

TEST_F(InstructionSetTest, INR_A)
{
	const std::vector<uint8_t> rom = {
		0x3C,
		0x3C,
		0x3C
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();
	p_Processor->RunStep();
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x03);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 15);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 3);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 3);
}

TEST_F(InstructionSetTest, RLC)
{
	const std::vector<uint8_t> rom = {
		0x07
	};
	p_Processor->Initialize(rom);
	m_State.A = 0xF2;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0xE5);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x03);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, RRC)
{
	const std::vector<uint8_t> rom = {
		0x0F
	};
	p_Processor->Initialize(rom);
	m_State.A = 0xF2;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x79);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, RAL)
{
	const std::vector<uint8_t> rom = {
		0x17
	};
	p_Processor->Initialize(rom);
	m_State.A = 0xB5;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x6A);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x03);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, RAR)
{
	const std::vector<uint8_t> rom = {
		0x1F
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x6A;
	m_State.Flags.Carry = 1;	
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0xB5);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, DAD_B)
{
	const std::vector<uint8_t> rom = {
		0x09
	};
	p_Processor->Initialize(rom);
	m_State.B = (0x339F & 0b1111111100000000) >> 8;
	m_State.C = 0x339F & 0b0000000011111111;
	m_State.H = (0xA17B & 0b1111111100000000) >> 8;
	m_State.L = 0xA17B & 0b0000000011111111;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, (0x339F & 0b1111111100000000) >> 8);
	EXPECT_EQ(m_State.C,  0x339F & 0b0000000011111111);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0xD5);
	EXPECT_EQ(m_State.L, 0x1A);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 10);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, DAD_D)
{
	const std::vector<uint8_t> rom = {
		0x19
	};
	p_Processor->Initialize(rom);
	m_State.D = (0x339F & 0b1111111100000000) >> 8;
	m_State.E = 0x339F & 0b0000000011111111;
	m_State.H = (0xA17B & 0b1111111100000000) >> 8;
	m_State.L = 0xA17B & 0b0000000011111111;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, (0x339F & 0b1111111100000000) >> 8);
	EXPECT_EQ(m_State.E, 0x339F & 0b0000000011111111);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0xD5);
	EXPECT_EQ(m_State.L, 0x1A);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 10);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, DAD_H)
{
	const std::vector<uint8_t> rom = {
		0x29
	};
	p_Processor->Initialize(rom);
	m_State.H = (0xA17B & 0b1111111100000000) >> 8;
	m_State.L = 0xA17B & 0b0000000011111111;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x03);
	EXPECT_EQ(m_State.H, (0x42F6 & 0b1111111100000000) >> 8);
	EXPECT_EQ(m_State.L, 0x42F6 & 0b0000000011111111);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 10);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, LDAX_B)
{
	const std::vector<uint8_t> rom = {
		0x0A
	};
	p_Processor->Initialize(rom);
	m_State.B = 0x93;
	m_State.C = 0x8B;
	m_MemoryMap.Poke(0x938B, 0xAA);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0xAA);
	EXPECT_EQ(m_State.B, 0x93);
	EXPECT_EQ(m_State.C, 0x8B);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, LDAX_D)
{
	const std::vector<uint8_t> rom = {
		0x1A
	};
	p_Processor->Initialize(rom);
	m_State.D = 0x93;
	m_State.E = 0x8B;
	m_MemoryMap.Poke(0x938B, 0xAA);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0xAA);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0x93);
	EXPECT_EQ(m_State.E, 0x8B);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, MVI_A)
{
	const std::vector<uint8_t> rom = {
		0x3E,
		0x3C
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x3C);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 2);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, MVI_B)
{
	const std::vector<uint8_t> rom = {
		0x06,
		0x3C
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0x3C);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 2);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, MVI_C)
{
	const std::vector<uint8_t> rom = {
		0x0E,
		0x3C
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0x3C);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 2);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, MVI_D)
{
	const std::vector<uint8_t> rom = {
		0x16,
		0x3C
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0x3C);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 2);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, MVI_E)
{
	const std::vector<uint8_t> rom = {
		0x1E,
		0x3C
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();	

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0x3C);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 2);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, MVI_H)
{
	const std::vector<uint8_t> rom = {
		0x26,
		0x3C
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();	

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0x3C);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 2);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, MVI_L)
{
	const std::vector<uint8_t> rom = {
		0x2E,
		0x3C
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0x3C);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 2);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, MVI_M)
{
	const std::vector<uint8_t> rom = {
		0x36,
		0xFF
	};
	p_Processor->Initialize(rom);
	m_State.H = 0x3C;
	m_State.L = 0xF4;
	p_Processor->RunStep();
	auto value= m_MemoryMap.Peek(0x3CF4);
	
	EXPECT_EQ(value, 0xFF);
	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0x3C);
	EXPECT_EQ(m_State.L, 0xF4);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 10);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 2);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, DCX_B)
{
	const std::vector<uint8_t> rom = {
		0x0B
	};
	p_Processor->Initialize(rom);
	m_State.B = 0x98;
	m_State.C = 0x00;	
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0x97);
	EXPECT_EQ(m_State.C, 0xFF);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 5);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, DCX_D)
{
	const std::vector<uint8_t> rom = {
		0x1B
	};
	p_Processor->Initialize(rom);
	m_State.D = 0x98;
	m_State.E = 0x00;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0x97);
	EXPECT_EQ(m_State.E, 0xFF);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 5);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, DCX_H)
{
	const std::vector<uint8_t> rom = {
		0x2B
	};
	p_Processor->Initialize(rom);
	m_State.H = 0x98;
	m_State.L = 0x00;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0x97);
	EXPECT_EQ(m_State.L, 0xFF);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 5);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, DCX_SP)
{
	const std::vector<uint8_t> rom = {
		0x3B
	};
	p_Processor->Initialize(rom);
	m_State.SP = 0xFF;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 5);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0xFE);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, SHLD_adr)
{
	const std::vector<uint8_t> rom = {
		0x22,
		0x0A,
		0x01
	};
	p_Processor->Initialize(rom);
	m_State.H = 0xAE;
	m_State.L = 0x29;
	p_Processor->RunStep();
	auto value1 = m_MemoryMap.Peek(0x010A);
	auto value2 = m_MemoryMap.Peek(0x010B);

	EXPECT_EQ(value1, 0x29);
	EXPECT_EQ(value2, 0xAE);
	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0xAE);
	EXPECT_EQ(m_State.L, 0x29);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 16);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 3);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, DAA)
{
	const std::vector<uint8_t> rom = {
		0x27
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x9B;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 1);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x13);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, LHLD_adr)
{
	const std::vector<uint8_t> rom = {
		0x2A,
		0x5B,
		0x02
	};
	p_Processor->Initialize(rom);
	m_MemoryMap.Poke(0x025B, 0xFF);
	m_MemoryMap.Poke(0x025C, 0x03);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0x03);
	EXPECT_EQ(m_State.L, 0xFF);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 16);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 3);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, CMA)
{
	const std::vector<uint8_t> rom = {
		0x2F
	};
	p_Processor->Initialize(rom);
	m_State.A = 0b01010001;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0b10101110);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, STA_adr)
{
	const std::vector<uint8_t> rom = {
		0x32,
		0xB3,
		0x05
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x45;
	p_Processor->RunStep();
	auto value = m_MemoryMap.Peek(0x05B3);

	EXPECT_EQ(value, 0x45);
	EXPECT_EQ(m_State.A, 0x45);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 13);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 3);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, DCR_B)
{
	const std::vector<uint8_t> rom = {
		0x05
	};
	p_Processor->Initialize(rom);
	m_State.B = 0X40;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0x3F);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 5);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, DCR_C)
{
	const std::vector<uint8_t> rom = {
		0x0D
	};
	p_Processor->Initialize(rom);
	m_State.C = 0X40;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0x3F);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 5);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, DCR_D)
{
	const std::vector<uint8_t> rom = {
		0x15
	};
	p_Processor->Initialize(rom);
	m_State.D = 0X40;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0x3F);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 5);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, DCR_E)
{
	const std::vector<uint8_t> rom = {
		0x1D
	};
	p_Processor->Initialize(rom);
	m_State.E = 0X40;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0x3F);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 5);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, DCR_H)
{
	const std::vector<uint8_t> rom = {
		0x25
	};
	p_Processor->Initialize(rom);
	m_State.H = 0X40;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0x3F);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 5);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, DCR_L)
{
	const std::vector<uint8_t> rom = {
		0x2D
	};
	p_Processor->Initialize(rom);
	m_State.L = 0X40;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0x3F);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 5);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, DCR_M)
{
	const std::vector<uint8_t> rom = {
		0x35
	};
	p_Processor->Initialize(rom);
	m_State.H = 0x3A;
	m_State.L = 0x7C;
	m_MemoryMap.Poke(0x3A7C, 0x40);
	p_Processor->RunStep();
	auto value = m_MemoryMap.Peek(0x3A7C);

	EXPECT_EQ(value, 0x3F);
	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0x3A);
	EXPECT_EQ(m_State.L, 0x7C);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 10);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, DCR_A)
{
	const std::vector<uint8_t> rom = {
		0x3D
	};
	p_Processor->Initialize(rom);
	m_State.A = 0X40;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x3F);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 5);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, STC)
{
	const std::vector<uint8_t> rom = {
		0x37
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x03);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, LDA_adr)
{
	const std::vector<uint8_t> rom = {
		0x3A,
		0x00,
		0x03
	};
	p_Processor->Initialize(rom);
	m_MemoryMap.Poke(0x0300, 0x34);
	p_Processor->RunStep();
	
	EXPECT_EQ(m_State.A, 0x34);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 13);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 3);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, DAD_SP)
{
	const std::vector<uint8_t> rom = {
		0x39
	};
	p_Processor->Initialize(rom);
	m_State.SP = 0xA17B;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0xA1);
	EXPECT_EQ(m_State.L, 0x7B);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 10);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0xA17B);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, CMC)
{
	const std::vector<uint8_t> rom = {
		0x3F
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x03);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADD_B)
{
	const std::vector<uint8_t> rom = {
		0x80
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x6C;
	m_State.B = 0x2E;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x9A);
	EXPECT_EQ(m_State.B, 0x2E);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x96);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADD_C)
{
	const std::vector<uint8_t> rom = {
		0x81
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x6C;
	m_State.C = 0x2E;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x9A);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0x2E);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x96);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADD_D)
{
	const std::vector<uint8_t> rom = {
		0x82
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x6C;
	m_State.D = 0x2E;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x9A);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0x2E);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x96);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADD_E)
{
	const std::vector<uint8_t> rom = {
		0x83
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x6C;
	m_State.E = 0x2E;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x9A);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0x2E);
	EXPECT_EQ(m_State.F, 0x96);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADD_H)
{
	const std::vector<uint8_t> rom = {
		0x84
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x6C;
	m_State.H = 0x2E;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x9A);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x96);
	EXPECT_EQ(m_State.H, 0x2E);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADD_L)
{
	const std::vector<uint8_t> rom = {
		0x85
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x6C;
	m_State.L = 0x2E;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x9A);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x96);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0x2E);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADD_M)
{
	const std::vector<uint8_t> rom = {
		0x86
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x6C;
	m_State.H = 0x2E;
	m_State.L = 0x2E;
	uint16_t adr = (m_State.H << 8) + m_State.L;
	m_MemoryMap.Poke(adr, 0x2E);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x9A);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x96);
	EXPECT_EQ(m_State.H, 0x2E);
	EXPECT_EQ(m_State.L, 0x2E);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADD_A)
{
	const std::vector<uint8_t> rom = {
		0x87
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x6C;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0xD8);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x96);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_B)
{
	const std::vector<uint8_t> rom = {
		0x88
	};
	p_Processor->Initialize(rom);
	m_State.B = 0x3D;
	m_State.A = 0x42;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x7F);
	EXPECT_EQ(m_State.B, 0x3D);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0X02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_B2)
{
	const std::vector<uint8_t> rom = {
		0x88
	};
	p_Processor->Initialize(rom);
	m_State.B = 0x3D;
	m_State.A = 0x42;
	m_State.Flags.Carry = true;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x80);
	EXPECT_EQ(m_State.B, 0x3D);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x92);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_C)
{
	const std::vector<uint8_t> rom = {
		0x89
	};
	p_Processor->Initialize(rom);
	m_State.C = 0x3D;
	m_State.A = 0x42;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x7F);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0x3D);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_C2)
{
	const std::vector<uint8_t> rom = {
		0x89
	};
	p_Processor->Initialize(rom);
	m_State.C = 0x3D;
	m_State.A = 0x42;
	m_State.Flags.Carry = true;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x80);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0x3D);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x92);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_D)
{
	const std::vector<uint8_t> rom = {
		0x8A
	};
	p_Processor->Initialize(rom);
	m_State.D = 0x3D;
	m_State.A = 0x42;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x7F);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0x3D);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_D2)
{
	const std::vector<uint8_t> rom = {
		0x8A
	};
	p_Processor->Initialize(rom);
	m_State.D = 0x3D;
	m_State.A = 0x42;
	m_State.Flags.Carry = true;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x80);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0x3D);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x92);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_E)
{
	const std::vector<uint8_t> rom = {
		0x8B
	};
	p_Processor->Initialize(rom);
	m_State.E = 0x3D;
	m_State.A = 0x42;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x7F);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0x3D);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_E2)
{
	const std::vector<uint8_t> rom = {
		0x8B
	};
	p_Processor->Initialize(rom);
	m_State.E = 0x3D;
	m_State.A = 0x42;
	m_State.Flags.Carry = true;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x80);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0x3D);
	EXPECT_EQ(m_State.F, 0x92);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_H)
{
	const std::vector<uint8_t> rom = {
		0x8C
	};
	p_Processor->Initialize(rom);
	m_State.H = 0x3D;
	m_State.A = 0x42;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x7F);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0x3D);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_H2)
{
	const std::vector<uint8_t> rom = {
		0x8C
	};
	p_Processor->Initialize(rom);
	m_State.H = 0x3D;
	m_State.A = 0x42;
	m_State.Flags.Carry = true;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x80);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x92);
	EXPECT_EQ(m_State.H, 0x3D);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_L)
{
	const std::vector<uint8_t> rom = {
		0x8D
	};
	p_Processor->Initialize(rom);
	m_State.L = 0x3D;
	m_State.A = 0x42;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x7F);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0x3D);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_L2)
{
	const std::vector<uint8_t> rom = {
		0x8D
	};
	p_Processor->Initialize(rom);
	m_State.L = 0x3D;
	m_State.A = 0x42;
	m_State.Flags.Carry = true;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x80);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x92);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0x3D);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_A)
{
	const std::vector<uint8_t> rom = {
		0x8F
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x42;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x84);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x86);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_M)
{
	const std::vector<uint8_t> rom = {
		0x8E
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x42;
	m_State.H = 0x2E;
	m_State.L = 0x2E;
	uint16_t adr = (m_State.H << 8) + m_State.L;
	m_MemoryMap.Poke(adr, 0x3D);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x7F);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0x2E);
	EXPECT_EQ(m_State.L, 0x2E);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADC_M2)
{
	const std::vector<uint8_t> rom = {
		0x8E
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x42;
	m_State.H = 0x2E;
	m_State.L = 0x2E;
	m_State.Flags.Carry = true;
	uint16_t adr = (m_State.H << 8) + m_State.L;
	m_MemoryMap.Poke(adr, 0x3D);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x80);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x92);
	EXPECT_EQ(m_State.H, 0x2E);
	EXPECT_EQ(m_State.L, 0x2E);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, SUB_B)
{
	const std::vector<uint8_t> rom = {
		0x90
	};
	p_Processor->Initialize(rom);
	m_State.B = 0x3D;
	m_State.A = 0x42;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x05);
	EXPECT_EQ(m_State.B, 0x3D);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, SUB_C)
{
	const std::vector<uint8_t> rom = {
		0x91
	};
	p_Processor->Initialize(rom);
	m_State.C = 0x3D;
	m_State.A = 0x42;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x05);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0x3D);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, SUB_D)
{
	const std::vector<uint8_t> rom = {
		0x92
	};
	p_Processor->Initialize(rom);
	m_State.D = 0x3D;
	m_State.A = 0x42;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x05);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0x3D);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, SUB_E)
{
	const std::vector<uint8_t> rom = {
		0x93
	};
	p_Processor->Initialize(rom);
	m_State.E = 0x3D;
	m_State.A = 0x42;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x05);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0x3D);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, SUB_H)
{
	const std::vector<uint8_t> rom = {
		0x94
	};
	p_Processor->Initialize(rom);
	m_State.H = 0x3D;
	m_State.A = 0x42;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x05);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0x3D);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, SUB_L)
{
	const std::vector<uint8_t> rom = {
		0x95
	};
	p_Processor->Initialize(rom);
	m_State.L = 0x3D;
	m_State.A = 0x42;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x05);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0x3D);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, SUB_A)
{
	const std::vector<uint8_t> rom = {
		0x97
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x42;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x56);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_TRUE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, SUB_M)
{
	const std::vector<uint8_t> rom = {
		0x96
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x42;
	m_State.H = 0x12;
	m_State.L = 0x12;
	uint16_t adr = (m_State.H << 8) + m_State.L;
	m_MemoryMap.Poke(adr, 0x3D);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x05);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0x12);
	EXPECT_EQ(m_State.L, 0x12);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, SBB_B)
{
	const std::vector<uint8_t> rom = {
		0x98
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x04;
	m_State.B = 0x02;
	m_State.Flags.Carry = true;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x01);
	EXPECT_EQ(m_State.B, 0x02);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x12);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, SBB_C)
{
	const std::vector<uint8_t> rom = {
		0x99
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x04;
	m_State.C = 0x02;
	m_State.Flags.Carry = true;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x01);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0x02);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x12);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, SBB_D)
{
	const std::vector<uint8_t> rom = {
		0x9A
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x04;
	m_State.D = 0x02;
	m_State.Flags.Carry = true;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x01);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0x02);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x12);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, SBB_E)
{
	const std::vector<uint8_t> rom = {
		0x9B
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x04;
	m_State.E = 0x02;
	m_State.Flags.Carry = true;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x01);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0x02);
	EXPECT_EQ(m_State.F, 0x12);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, SBB_H)
{
	const std::vector<uint8_t> rom = {
		0x9C
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x04;
	m_State.H = 0x02;
	m_State.Flags.Carry = true;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x01);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x12);
	EXPECT_EQ(m_State.H, 0x02);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, SBB_L)
{
	const std::vector<uint8_t> rom = {
		0x9D
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x04;
	m_State.L = 0x02;
	m_State.Flags.Carry = true;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x01);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x12);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0x02);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, SBB_M)
{
	const std::vector<uint8_t> rom = {
		0x9E
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x04;
	m_State.H = 0x12;
	m_State.L = 0x12;
	m_State.Flags.Carry = true;
	uint16_t adr = (m_State.H << 8) + m_State.L;
	m_MemoryMap.Poke(adr, 0x02);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x01);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x12);
	EXPECT_EQ(m_State.H, 0x12);
	EXPECT_EQ(m_State.L, 0x12);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, SBB_A)
{
	const std::vector<uint8_t> rom = {
		0x9F
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x04;
	m_State.Flags.Carry = true;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0xFF);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x87);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ANA_B)
{
	const std::vector<uint8_t> rom = {
		0xA0
	};
	p_Processor->Initialize(rom);
	m_State.A = 0xFC;
	m_State.B = 0x0F;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x0C);
	EXPECT_EQ(m_State.B, 0x0F);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ANA_C)
{
	const std::vector<uint8_t> rom = {
		0xA1
	};
	p_Processor->Initialize(rom);
	m_State.A = 0xFC;
	m_State.C = 0x0F;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x0C);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0x0F);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ANA_D)
{
	const std::vector<uint8_t> rom = {
		0xA2
	};
	p_Processor->Initialize(rom);
	m_State.A = 0xFC;
	m_State.D = 0x0F;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x0C);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0x0F);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ANA_E)
{
	const std::vector<uint8_t> rom = {
		0xA3
	};
	p_Processor->Initialize(rom);
	m_State.A = 0xFC;
	m_State.E = 0x0F;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x0C);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0x0F);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}


TEST_F(InstructionSetTest, ANA_H)
{
	const std::vector<uint8_t> rom = {
		0xA4
	};
	p_Processor->Initialize(rom);
	m_State.A = 0xFC;
	m_State.H = 0x0F;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x0C);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0x0F);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ANA_L)
{
	const std::vector<uint8_t> rom = {
		0xA5
	};
	p_Processor->Initialize(rom);
	m_State.A = 0xFC;
	m_State.L = 0x0F;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x0C);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0x0F);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ANA_M)
{
	const std::vector<uint8_t> rom = {
		0xA6
	};
	p_Processor->Initialize(rom);
	m_State.A = 0xFC;
	m_State.H = 0x12;
	m_State.L = 0x12;
	uint16_t adr = (m_State.H << 8) + m_State.L;
	m_MemoryMap.Poke(adr, 0x0F);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x0C);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0x12);
	EXPECT_EQ(m_State.L, 0x12);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ANA_A)
{
	const std::vector<uint8_t> rom = {
		0xA7
	};
	p_Processor->Initialize(rom);
	m_State.A = 0xFC;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0xFC);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x86);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, XRA_B)
{
	const std::vector<uint8_t> rom = {
		0xA8
	};

	p_Processor->Initialize(rom);
	m_State.A = 0xFC;
	m_State.B = 0x0F;
	p_Processor->RunStep();
	EXPECT_EQ(m_State.A, 0xF3);
	EXPECT_EQ(m_State.B, 0x0F);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x86);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, XRA_C)
{
	const std::vector<uint8_t> rom = {
		0xA9
	};
	p_Processor->Initialize(rom);
	m_State.A = 0xFC;
	m_State.C = 0x0F;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0xF3);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0x0F);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x86);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, XRA_D)
{
	const std::vector<uint8_t> rom = {
		0xAA
	};
	p_Processor->Initialize(rom);
	m_State.A = 0xFC;
	m_State.D = 0x0F;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0xF3);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0x0F);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x86);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, XRA_E)
{
	const std::vector<uint8_t> rom = {
		0xAB
	};
	p_Processor->Initialize(rom);
	m_State.A = 0xFC;
	m_State.E = 0x0F;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0xF3);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0x0F);
	EXPECT_EQ(m_State.F, 0x86);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, XRA_H)
{
	const std::vector<uint8_t> rom = {
		0xAC
	};
	p_Processor->Initialize(rom);
	m_State.A = 0xFC;
	m_State.H = 0x0F;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0xF3);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x86);
	EXPECT_EQ(m_State.H, 0x0F);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, XRA_L)
{
	const std::vector<uint8_t> rom = {
		0xAD
	};
	p_Processor->Initialize(rom);
	m_State.A = 0xFC;
	m_State.L = 0x0F;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0xF3);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x86);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0x0F);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, XRA_M)
{
	const std::vector<uint8_t> rom = {
		0xAE
	};
	p_Processor->Initialize(rom);
	m_State.A = 0xFC;
	m_State.H = 0x12;
	m_State.L = 0x12;
	uint16_t adr = (m_State.H << 8) + m_State.L;
	m_MemoryMap.Poke(adr, 0x0F);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0xF3);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x86);
	EXPECT_EQ(m_State.H, 0x12);
	EXPECT_EQ(m_State.L, 0x12);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ORA_B)
{
	const std::vector<uint8_t> rom = {
		0xB0
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x33;
	m_State.B = 0x0F;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x3F);
	EXPECT_EQ(m_State.B, 0x0F);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ORA_C)
{
	const std::vector<uint8_t> rom = {
		0xB1
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x33;
	m_State.C = 0x0F;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x3F);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0x0F);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ORA_D)
{
	const std::vector<uint8_t> rom = {
		0xB2
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x33;
	m_State.D = 0x0F;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x3F);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0x0F);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ORA_E)
{
	const std::vector<uint8_t> rom = {
		0xB3
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x33;
	m_State.E = 0x0F;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x3F);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0x0F);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ORA_H)
{
	const std::vector<uint8_t> rom = {
		0xB4
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x33;
	m_State.H = 0x0F;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x3F);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0x0F);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ORA_L)
{
	const std::vector<uint8_t> rom = {
		0xB5
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x33;
	m_State.L = 0x0F;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x3F);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0x0F);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ORA_M)
{
	const std::vector<uint8_t> rom = {
		0xB6
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x33;
	m_State.H = 0x12;
	m_State.L = 0x12;
	uint16_t adr = (m_State.H << 8) + m_State.L;
	m_MemoryMap.Poke(adr, 0x0F);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x3F);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0x12);
	EXPECT_EQ(m_State.L, 0x12);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ORA_A)
{
	const std::vector<uint8_t> rom = {
		0xB7
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x33;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x33);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_B)
{
	const std::vector<uint8_t> rom = {
		0xB8
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x0A;
	m_State.B = 0x05;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x0A);
	EXPECT_EQ(m_State.B, 0x05);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x16);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_B2)
{
	const std::vector<uint8_t> rom = {
		0xB8
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x02;
	m_State.B = 0x05;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x02);
	EXPECT_EQ(m_State.B, 0x05);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x83);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_C)
{
	const std::vector<uint8_t> rom = {
		0xB9
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x0A;
	m_State.C = 0x05;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x0A);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0x05);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x16);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_C2)
{
	const std::vector<uint8_t> rom = {
		0xB9
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x02;
	m_State.C = 0x05;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x02);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0x05);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x83);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_D)
{
	const std::vector<uint8_t> rom = {
		0xBA
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x0A;
	m_State.D = 0x05;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x0A);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0x05);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x16);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_D2)
{
	const std::vector<uint8_t> rom = {
		0xBA
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x02;
	m_State.D = 0x05;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x02);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0x05);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x83);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_E)
{
	const std::vector<uint8_t> rom = {
		0xBB
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x0A;
	m_State.E = 0x05;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x0A);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0x05);
	EXPECT_EQ(m_State.F, 0x16);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_E2)
{
	const std::vector<uint8_t> rom = {
		0xBB
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x02;
	m_State.E = 0x05;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x02);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0x05);
	EXPECT_EQ(m_State.F, 0x83);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_H)
{
	const std::vector<uint8_t> rom = {
		0xBC
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x0A;
	m_State.H = 0x05;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x0A);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x16);
	EXPECT_EQ(m_State.H, 0x05);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_H2)
{
	const std::vector<uint8_t> rom = {
		0xBC
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x02;
	m_State.H = 0x05;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x02);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x83);
	EXPECT_EQ(m_State.H, 0x05);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_L)
{
	const std::vector<uint8_t> rom = {
		0xBD
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x0A;
	m_State.L = 0x05;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x0A);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x16);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0x05);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_L2)
{
	const std::vector<uint8_t> rom = {
		0xBD
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x02;
	m_State.L = 0x05;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x02);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x83);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0x05);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_M)
{
	const std::vector<uint8_t> rom = {
		0xBE
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x0A;
	m_State.H = 0x12;
	m_State.L = 0x12;
	uint16_t adr = (m_State.H << 8) + m_State.L;
	m_MemoryMap.Poke(adr, 0x05);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x0A);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x16);
	EXPECT_EQ(m_State.H, 0x12);
	EXPECT_EQ(m_State.L, 0x12);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_M2)
{
	const std::vector<uint8_t> rom = {
		0xBE
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x02;
	m_State.H = 0x12;
	m_State.L = 0x12;
	uint16_t adr = (m_State.H << 8) + m_State.L;
	m_MemoryMap.Poke(adr, 0x05);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x02);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x83);
	EXPECT_EQ(m_State.H, 0x12);
	EXPECT_EQ(m_State.L, 0x12);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_A)
{
	const std::vector<uint8_t> rom = {
		0xBF
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x0A;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x0A);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x56);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_TRUE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, CMP_A2)
{
	const std::vector<uint8_t> rom = {
		0xBF
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x02;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x02);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x56);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_TRUE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, SUI_D8)
{
	const std::vector<uint8_t> rom = {
		0xD6,
		0x01
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0xFF);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x87);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 2);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, SBI_D8)
{
	const std::vector<uint8_t> rom = {
		0xD6,
		0x01
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0xFF);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x87);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 2);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, SBI_D8_2)
{
	const std::vector<uint8_t> rom = {
		0xDE,
		0x01
	};
	p_Processor->Initialize(rom);
	m_State.Flags.Carry = true;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0xFE);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x83);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 2);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ORI_D8)
{
	const std::vector<uint8_t> rom = {
		0xF6,
		0x0F
	};
	p_Processor->Initialize(rom);
	m_State.A = 0xB5;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0xBF);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x82);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 2);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, XRI_D8)
{
	const std::vector<uint8_t> rom = {
		0xEE,
		0x81
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x3B;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0xBA);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x82);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 2);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, PUSH_B)
{
	const std::vector<uint8_t> rom = {
		0xC5
	};
	p_Processor->Initialize(rom);
	m_State.B = 0x8F;
	m_State.C = 0x9D;
	m_State.SP = 0x3A2C;
	p_Processor->RunStep();

	EXPECT_EQ(m_MemoryMap.Peek(0x3A2A), 0x9D);
	EXPECT_EQ(m_MemoryMap.Peek(0x3A2B), 0x8F);
	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0x8F);
	EXPECT_EQ(m_State.C, 0x9D);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 11);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0x3A2A);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, PUSH_D)
{
	const std::vector<uint8_t> rom = {
		0xD5
	};
	p_Processor->Initialize(rom);
	m_State.D = 0x8F;
	m_State.E = 0x9D;
	m_State.SP = 0x3A2C;
	p_Processor->RunStep();

	EXPECT_EQ(m_MemoryMap.Peek(0x3A2A), 0x9D);
	EXPECT_EQ(m_MemoryMap.Peek(0x3A2B), 0x8F);
	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0x8F);
	EXPECT_EQ(m_State.E, 0x9D);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 11);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0x3A2A);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, PUSH_H)
{
	const std::vector<uint8_t> rom = {
		0xE5
	};
	p_Processor->Initialize(rom);
	m_State.H = 0x8F;
	m_State.L = 0x9D;
	m_State.SP = 0x3A2C;
	p_Processor->RunStep();

	EXPECT_EQ(m_MemoryMap.Peek(0x3A2A), 0x9D);
	EXPECT_EQ(m_MemoryMap.Peek(0x3A2B), 0x8F);
	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0x8F);
	EXPECT_EQ(m_State.L, 0x9D);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 11);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0x3A2A);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, PUSH_PSW)
{
	const std::vector<uint8_t> rom = {
		0xF5
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x1F;
	m_State.SP = 0x502A;
	m_State.Flags.Carry = true;
	m_State.Flags.Zero = true;
	m_State.Flags.Parity = true;
	m_State.F = m_State.Flags.getF();
	p_Processor->RunStep();

	EXPECT_EQ(m_MemoryMap.Peek(0x5029), 0x1F);
	EXPECT_EQ(m_MemoryMap.Peek(0x5028), 0x47);
	EXPECT_EQ(m_State.A, 0x1F);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x47);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 11);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0x5028);
	EXPECT_TRUE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, CALL)
{
	std::vector<uint8_t> rom = {
		0xCD, 0x66, 0x06
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();

	EXPECT_EQ(m_MemoryMap.Peek(0xFFFF), 0x00);
	EXPECT_EQ(m_MemoryMap.Peek(0xFFFE), 0x03);
	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 17);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 0x666);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0xFFFE);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, XCHG)
{
	std::vector<uint8_t> rom = {
		0xEB
	};
	p_Processor->Initialize(rom);
	m_State.D = 0x33;
	m_State.E = 0x55;
	m_State.H = 0x00;
	m_State.L = 0xFF;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0x00);
	EXPECT_EQ(m_State.E, 0xFF);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0x33);
	EXPECT_EQ(m_State.L, 0x55);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 4);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, POP_B)
{
	std::vector<uint8_t> rom = {
		0xC1
	};
	p_Processor->Initialize(rom);
	m_MemoryMap.Poke(0x1239, 0x3D);
	m_MemoryMap.Poke(0x123A, 0x93);
	m_State.SP = 0x1239;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0x93);
	EXPECT_EQ(m_State.C, 0x3D);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 10);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0x123B);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, POP_D)
{
	std::vector<uint8_t> rom = {
		0xD1
	};
	p_Processor->Initialize(rom);
	m_MemoryMap.Poke(0x1239, 0x3D);
	m_MemoryMap.Poke(0x123A, 0x93);
	m_State.SP = 0x1239;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0x93);
	EXPECT_EQ(m_State.E, 0x3D);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 10);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0x123B);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, POP_H)
{
	std::vector<uint8_t> rom = {
		0xE1
	};
	p_Processor->Initialize(rom);
	m_MemoryMap.Poke(0x1239, 0x3D);
	m_MemoryMap.Poke(0x123A, 0x93);
	m_State.SP = 0x1239;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0x93);
	EXPECT_EQ(m_State.L, 0x3D);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 10);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0x123B);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, POP_PSW)
{
	std::vector<uint8_t> rom = {
		0xF1
	};
	p_Processor->Initialize(rom);
	m_MemoryMap.Poke(0x2C00, 0xC3);
	m_MemoryMap.Poke(0x2C01, 0xFF);
	m_State.SP = 0x2C00;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0xFF);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0xC3);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 10);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0x2C02);
	EXPECT_TRUE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}


TEST_F(InstructionSetTest, CPI)
{
	const std::vector<uint8_t> rom = {
		0xFE, 0x40
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x4A;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x4A);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x16);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 2);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, CPI_1)
{
	const std::vector<uint8_t> rom = {
		0xFE, 0x4A
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x4A;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x4A);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x46);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 2);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_TRUE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, CPI_2)
{
	const std::vector<uint8_t> rom = {
		0xFE, 0x4B
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x4A;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x4A);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x97);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 2);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, CPI_3)
{
	const std::vector<uint8_t> rom = {
		0xFE, 0x00
	};
	p_Processor->Initialize(rom);
	m_State.A = 0xF5;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0xF5);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x97);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 2);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, XTHL)
{
	std::vector<uint8_t> rom = {
		0xE3
	};
	p_Processor->Initialize(rom);
	m_State.SP = 0x10AD;
	m_State.H = 0x0B;
	m_State.L = 0x3C;
	m_MemoryMap.Poke(0x10AD, 0xF0);
	m_MemoryMap.Poke(0x10AE, 0x0D);
	p_Processor->RunStep();

	EXPECT_EQ(m_MemoryMap.Peek(0x10AD), 0x3C);
	EXPECT_EQ(m_MemoryMap.Peek(0x10AE), 0x0B);
	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0x0D);
	EXPECT_EQ(m_State.L, 0xF0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 18);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0x10AD);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ADI)
{
	std::vector<uint8_t> rom = {
		0xC6, 0x42, 0xC6, 0xBE
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x14;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x56);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 2);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);

	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x14);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x17);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 14);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 4);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 2);
}

TEST_F(InstructionSetTest, ACI)
{
	std::vector<uint8_t> rom = {
		0xCE, 0xBE, 0xCE, 0x42
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x56;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x14);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x17);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_TRUE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 2);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);

	p_Processor->RunStep();	

	EXPECT_EQ(m_State.A, 0x57);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 14);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 4);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 2);
}

TEST_F(InstructionSetTest, SUI)
{
	std::vector<uint8_t> rom = {
		0xD6, 0x01
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x00;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0xFF);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x87);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_TRUE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 2);
	EXPECT_TRUE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, ANI)
{
	std::vector<uint8_t> rom = {
		0xE6, 0x4F
	};
	p_Processor->Initialize(rom);
	m_State.A = 0x3A;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0x0A);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x06);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 7);
	EXPECT_TRUE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 2);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, SPHL)
{
	std::vector<uint8_t> rom = {
		0xF9
	};
	p_Processor->Initialize(rom);
	m_State.H = 0x50;
	m_State.L = 0x6C;
	p_Processor->RunStep();

	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0x50);
	EXPECT_EQ(m_State.L, 0x6C);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 5);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 1);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0x506C);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
}

TEST_F(InstructionSetTest, CALL_RET_HLT)
{
	std::vector<uint8_t> rom = {
		0xCD,	// CALL 0X0004
		0x04,
		0x00,
		0x76,	// HLT
		0xC9	// RET
	};
	p_Processor->Initialize(rom);
	p_Processor->RunStep();

	EXPECT_EQ(m_MemoryMap.Peek(0xFFFE), 0x03);
	EXPECT_EQ(m_MemoryMap.Peek(0xFFFF), 0x00);
	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 17);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 0x0004);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0xFFFE);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 1);
	EXPECT_EQ(m_State.HLT, false);

	p_Processor->RunStep();

	EXPECT_EQ(m_MemoryMap.Peek(0xFFFE), 0x03);
	EXPECT_EQ(m_MemoryMap.Peek(0xFFFF), 0x00);
	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 27);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 0x003);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0x0000);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 2);
	EXPECT_EQ(m_State.HLT, false);

	p_Processor->RunStep();

	EXPECT_EQ(m_MemoryMap.Peek(0xFFFE), 0x03);
	EXPECT_EQ(m_MemoryMap.Peek(0xFFFF), 0x00);
	EXPECT_EQ(m_State.A, 0);
	EXPECT_EQ(m_State.B, 0);
	EXPECT_EQ(m_State.C, 0);
	EXPECT_EQ(m_State.D, 0);
	EXPECT_EQ(m_State.E, 0);
	EXPECT_EQ(m_State.F, 0x02);
	EXPECT_EQ(m_State.H, 0);
	EXPECT_EQ(m_State.L, 0);
	EXPECT_FALSE(m_State.Flags.AuxiliaryCarry);
	EXPECT_FALSE(m_State.Flags.Carry);
	EXPECT_EQ(m_State.Cycles, 34);
	EXPECT_FALSE(m_State.Flags.Parity);
	EXPECT_EQ(m_State.PC, 0x004);
	EXPECT_FALSE(m_State.Flags.Sign);
	EXPECT_EQ(m_State.SP, 0x0000);
	EXPECT_FALSE(m_State.Flags.Zero);
	EXPECT_EQ(m_State.Steps, 3);
	EXPECT_EQ(m_State.HLT, true);
}
