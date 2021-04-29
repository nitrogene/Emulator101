#include "test.h"

TEST_F(ProcessorTest, Step1)
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

TEST_F(ProcessorTest, Step10)
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

TEST_F(ProcessorTest, Step100)
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

TEST_F(ProcessorTest, Step1000)
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

TEST_F(ProcessorTest, Step10000)
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