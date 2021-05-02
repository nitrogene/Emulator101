#include "SpaceInvadersTest.h"

TEST_F(SpaceInvadersTest, Step1)
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

TEST_F(SpaceInvadersTest, Step10)
{
	for (auto i = 0; i < 10; ++i)
	{
		p_Processor->RunStep();
		const auto& state = p_Processor->getState();

		int cycles = 0;
		p_MachineTemplate->doCPUStep(cycles);
		const auto& state8080 = p_MachineTemplate->getState();

		// Registers
		EXPECT_EQ(state.B, state8080.b) << "step: " << std::to_string(i);
		EXPECT_EQ(state.C, state8080.c) << "step: " << std::to_string(i);
		EXPECT_EQ(state.D, state8080.d) << "step: " << std::to_string(i);
		EXPECT_EQ(state.E, state8080.e) << "step: " << std::to_string(i);
		EXPECT_EQ(state.H, state8080.h) << "step: " << std::to_string(i);
		EXPECT_EQ(state.L, state8080.l) << "step: " << std::to_string(i);
		EXPECT_EQ(state.A, state8080.a) << "step: " << std::to_string(i);

		// Flags
		EXPECT_EQ(state.S, state8080.cc.s) << "step: " << std::to_string(i);
		EXPECT_EQ(state.Z, state8080.cc.z) << "step: " << std::to_string(i);
		EXPECT_EQ(state.AC, state8080.cc.ac) << "step: " << std::to_string(i);
		EXPECT_EQ(state.P, state8080.cc.p) << "step: " << std::to_string(i);
		EXPECT_EQ(state.CY, state8080.cc.cy) << "step: " << std::to_string(i);

		// Program counter
		EXPECT_EQ(state.PC, state8080.pc) << "step: " << std::to_string(i);
		EXPECT_EQ(state.SP, state8080.sp) << "step: " << std::to_string(i);
	}

}

TEST_F(SpaceInvadersTest, Step100)
{
	for (auto i = 0; i < 100; ++i)
	{
		p_Processor->RunStep();
		const auto& state = p_Processor->getState();

		int cycles = 0;
		p_MachineTemplate->doCPUStep(cycles);
		const auto& state8080 = p_MachineTemplate->getState();

		// Registers
		EXPECT_EQ(state.B, state8080.b) << "step: " << std::to_string(i);
		EXPECT_EQ(state.C, state8080.c) << "step: " << std::to_string(i);
		EXPECT_EQ(state.D, state8080.d) << "step: " << std::to_string(i);
		EXPECT_EQ(state.E, state8080.e) << "step: " << std::to_string(i);
		EXPECT_EQ(state.H, state8080.h) << "step: " << std::to_string(i);
		EXPECT_EQ(state.L, state8080.l) << "step: " << std::to_string(i);
		EXPECT_EQ(state.A, state8080.a) << "step: " << std::to_string(i);

		// Flags
		EXPECT_EQ(state.S, state8080.cc.s) << "step: " << std::to_string(i);
		EXPECT_EQ(state.Z, state8080.cc.z) << "step: " << std::to_string(i);
		EXPECT_EQ(state.AC, state8080.cc.ac) << "step: " << std::to_string(i);
		EXPECT_EQ(state.P, state8080.cc.p) << "step: " << std::to_string(i);
		EXPECT_EQ(state.CY, state8080.cc.cy) << "step: " << std::to_string(i);

		// Program counter
		EXPECT_EQ(state.PC, state8080.pc) << "step: " << std::to_string(i);
		EXPECT_EQ(state.SP, state8080.sp) << "step: " << std::to_string(i);
	}

}

TEST_F(SpaceInvadersTest, Step1000)
{
	for (auto i = 0; i < 1000; ++i)
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

TEST_F(SpaceInvadersTest, Step10000)
{
	for (auto i = 0; i < 10000; ++i)
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

TEST_F(SpaceInvadersTest, TestSetF)
{
	// https://www.tramm.li/i8080/emu8080.html
	// type x f[flag] 0/1

	//	s	S
	//	z	Z
	//	i
	//	h	AC
	//	p	P
	//	c	C

	struct s
	{
		bool S;
		bool Z;
		bool AC;
		bool P;
		bool CY;

		uint8_t F;
	};

	s f[]{
		{false, false,	false,	false,	false,	0x00},
		{false, false,	false,	false,	true,	0x01},
		{false, false,	false,	true,	false,	0x04},
		{false, false,	false,	true,	true,	0x05},
		{false, false,	true,	false,	false,	0x10},
		{false, false,	true,	false,	true,	0x11},
		{false, false,	true,	true,	false,	0x14},
		{false, false,	true,	true,	true,	0x15},
		{false, true,	false,	false,	false,	0x40},
		{false, true,	false,	false,	true,	0x41},
		{false, true,	false,	true,	false,	0x44},
		{false, true,	false,	true,	true,	0x45},
		{false, true,	true,	false,	false,	0x50},
		{false, true,	true,	false,	true,	0x51},
		{false, true,	true,	true,	false,	0x54},
		{false, true,	true,	true,	true,	0x55},

		{true, false,	false,	false,	false,	0x80},
		{true, false,	false,	false,	true,	0x81},
		{true, false,	false,	true,	false,	0x84},
		{true, false,	false,	true,	true,	0x85},
		{true, false,	true,	false,	false,	0x90},
		{true, false,	true,	false,	true,	0x91},
		{true, false,	true,	true,	false,	0x94},
		{true, false,	true,	true,	true,	0x95},
		{true, true,	false,	false,	false,	0xC0},
		{true, true,	false,	false,	true,	0xC1},
		{true, true,	false,	true,	false,	0xC4},
		{true, true,	false,	true,	true,	0xC5},
		{true, true,	true,	false,	false,	0xD0},
		{true, true,	true,	false,	true,	0xD1},
		{true, true,	true,	true,	false,	0xD4},
		{true, true,	true,	true,	true,	0xD5}

	};

	for(const auto flags:f)
	{
		State state{};

		state.S = flags.S;
		state.Z = flags.Z;
		state.AC = flags.AC;
		state.P = flags.P;
		state.CY = flags.CY;

		state.setF();

		EXPECT_EQ(state.F, flags.F) << state.toString();
	}
}
