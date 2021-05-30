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
	EXPECT_EQ(state.Flags.Sign, state8080.cc.s);
	EXPECT_EQ(state.Flags.Zero, state8080.cc.z);
	EXPECT_EQ(state.Flags.AuxiliaryCarry, state8080.cc.ac);
	EXPECT_EQ(state.Flags.Parity, state8080.cc.p);
	EXPECT_EQ(state.Flags.Carry, state8080.cc.cy);

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
		//EXPECT_EQ(state.Flags.Sign, state8080.cc.s) << "step: " << std::to_string(i);
		//EXPECT_EQ(state.Flags.Zero, state8080.cc.z) << "step: " << std::to_string(i);
		//EXPECT_EQ(state.Flags.AuxiliaryCarry, state8080.cc.ac) << "step: " << std::to_string(i);
		//EXPECT_EQ(state.Flags.Parity, state8080.cc.p) << "step: " << std::to_string(i);
		//EXPECT_EQ(state.Flags.Carry, state8080.cc.cy) << "step: " << std::to_string(i);

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
		//EXPECT_EQ(state.Flags.Sign, state8080.cc.s) << "step: " << std::to_string(i);
		//EXPECT_EQ(state.Flags.Zero, state8080.cc.z) << "step: " << std::to_string(i);
		//EXPECT_EQ(state.Flags.AuxiliaryCarry, state8080.cc.ac) << "step: " << std::to_string(i);
		//EXPECT_EQ(state.Flags.Parity, state8080.cc.p) << "step: " << std::to_string(i);
		//EXPECT_EQ(state.Flags.Carry, state8080.cc.cy) << "step: " << std::to_string(i);

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
		//EXPECT_EQ(state.Flags.Sign, state8080.cc.s);
		//EXPECT_EQ(state.Flags.Zero, state8080.cc.z);
		//EXPECT_EQ(state.Flags.AuxiliaryCarry, state8080.cc.ac);
		//EXPECT_EQ(state.Flags.Parity, state8080.cc.p);
		//EXPECT_EQ(state.Flags.Carry, state8080.cc.cy);

		// Program counter
		EXPECT_EQ(state.PC, state8080.pc);
		EXPECT_EQ(state.SP, state8080.sp);
	}

}

TEST_F(SpaceInvadersTest, Step1500)
{
	for (auto i = 0; i < 1500; ++i)
	{
		p_Processor->RunStep();
		auto map = p_Processor->getMemoryMap();
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
		//EXPECT_EQ(state.Flags.Sign, state8080.cc.s);
		//EXPECT_EQ(state.Flags.Zero, state8080.cc.z);
		//EXPECT_EQ(state.Flags.AuxiliaryCarry, state8080.cc.ac);
		//EXPECT_EQ(state.Flags.Parity, state8080.cc.p);
		//EXPECT_EQ(state.Flags.Carry, state8080.cc.cy);

		// Program counter
		EXPECT_EQ(state.PC, state8080.pc);
		EXPECT_EQ(state.SP, state8080.sp);
	}

}

TEST_F(SpaceInvadersTest, Step2000)
{
	int cycles = 0;
	const auto& state = p_Processor->getState();
	const auto& state8080 = p_MachineTemplate->getState();
	auto& map = p_Processor->getMemoryMap();
	auto adr = &map.Peek(0);
	auto adr8080 = state8080.memory;

	memcpy(adr8080, adr, map.size() * sizeof(uint8_t));

	for (auto i = 0; i < 2000; ++i)
	{
		if (i == 1552)
		{
			std::cout << i << std::endl;
		}

		p_Processor->RunStep();		
		p_MachineTemplate->doCPUStep(cycles);



		// Test work ram
		auto it=std::mismatch(adr + 0x2000, adr + 0x23FF, adr8080 + 0x2000);

		EXPECT_EQ(it, std::make_pair(adr + 0x23FF, adr8080 + 0x23FF)) << "Step " << std::to_string(i) << " " << it.first - adr - 0x2000;

		// Registers
		EXPECT_EQ(state.B, state8080.b) << "Step " << std::to_string(i);
		EXPECT_EQ(state.C, state8080.c) << "Step " << std::to_string(i);
		EXPECT_EQ(state.D, state8080.d) << "Step " << std::to_string(i);
		EXPECT_EQ(state.E, state8080.e) << "Step " << std::to_string(i);
		EXPECT_EQ(state.H, state8080.h) << "Step " << std::to_string(i);
		EXPECT_EQ(state.L, state8080.l) << "Step " << std::to_string(i);
		EXPECT_EQ(state.A, state8080.a) << "Step " << std::to_string(i);

		// Flags
		EXPECT_EQ(state.Flags.Sign, state8080.cc.s) << "Step " << std::to_string(i);
		EXPECT_EQ(state.Flags.Zero, state8080.cc.z) << "Step " << std::to_string(i);
		//EXPECT_EQ(state.Flags.AuxiliaryCarry, state8080.cc.ac) << "Step " << std::to_string(i);
		EXPECT_EQ(state.Flags.Parity, state8080.cc.p) << "Step " << std::to_string(i);
		EXPECT_EQ(state.Flags.Carry, state8080.cc.cy) << "Step " << std::to_string(i);

		// Program counter
		EXPECT_EQ(state.PC, state8080.pc) << "Step " << std::to_string(i);
		EXPECT_EQ(state.SP, state8080.sp) << "Step " << std::to_string(i);
		EXPECT_EQ(state.Cycles, cycles) << "Step " << std::to_string(i);
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
		EXPECT_EQ(state.Flags.Sign, state8080.cc.s);
		EXPECT_EQ(state.Flags.Zero, state8080.cc.z);
		EXPECT_EQ(state.Flags.AuxiliaryCarry, state8080.cc.ac);
		EXPECT_EQ(state.Flags.Parity, state8080.cc.p);
		EXPECT_EQ(state.Flags.Carry, state8080.cc.cy);

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

		state.Flags.Sign = flags.S;
		state.Flags.Zero = flags.Z;
		state.Flags.AuxiliaryCarry = flags.AC;
		state.Flags.Parity = flags.P;
		state.Flags.Carry = flags.CY;

		state.F=state.Flags.getF();

		EXPECT_EQ(state.F, flags.F) << state.toString();
	}
}
