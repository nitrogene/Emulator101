#include "UtilitiesTest.h"
#include "../NewAwesome8080Emulator/Utilities.h"
#include "superzazu-8080/i8080.h"

TEST_F(UtilitiesTest, CMP)
{
	for (uint16_t a = 0; a <= 0xFF; ++a)
	{
		for (uint16_t v = 0; v <= 0xFF; ++v)
		{
			State state{};
			state.A = (uint8_t)a;
			Utilities::CMP(state, (uint8_t)v);

			auto c = std::make_shared<i8080>();
			i8080_init(c.get());
			c->a = (uint8_t)a;
			i8080_cmp(c.get(), (uint8_t)v);

			auto msg = "A="+std::to_string(state.A)+"\t v=" + std::to_string(v);
			
			EXPECT_EQ(state.A, c->a) << msg;
			EXPECT_EQ(state.Flags.Sign, c->sf) << msg;
			EXPECT_EQ(state.Flags.Zero, c->zf) << msg;
			EXPECT_EQ(state.Flags.AuxiliaryCarry, c->hf) << msg;
			EXPECT_EQ(state.Flags.Parity, c->pf) << msg;
			EXPECT_EQ(state.Flags.Carry, c->cf) << msg;
		}
	}
}

TEST_F(UtilitiesTest, ADD)
{
	for (uint8_t cf = 0; cf <= 1; ++cf)
	{
		for (uint16_t a = 0; a <= 0xFF; ++a)
		{
			for (uint16_t v = 0; v <= 0xFF; ++v)
			{
				State state{};
				state.A = (uint8_t)a;
				state.Flags.Carry = cf;
				Utilities::ADD(state, (uint8_t)v, state.Flags.Carry);

				auto c = std::make_shared<i8080>();
				i8080_init(c.get());
				c->a = (uint8_t)a;
				c->cf = cf;
				i8080_add(c.get(), &c->a, (uint8_t)v, c->cf);

				auto msg = "A=" + std::to_string(state.A) + "\t v=" + std::to_string(v) + "\t cf=" + std::to_string(cf);

				EXPECT_EQ(state.A, c->a) << msg;
				EXPECT_EQ(state.Flags.Sign, c->sf) << msg;
				EXPECT_EQ(state.Flags.Zero, c->zf) << msg;
				EXPECT_EQ(state.Flags.AuxiliaryCarry, c->hf) << msg;
				EXPECT_EQ(state.Flags.Parity, c->pf) << msg;
				EXPECT_EQ(state.Flags.Carry, c->cf) << msg;
			}
		}
	}
}

TEST_F(UtilitiesTest, ANA)
{
	for (uint16_t a = 0; a <= 0xFF; ++a)
	{
		for (uint16_t v = 0; v <= 0xFF; ++v)
		{
			State state{};
			state.A = (uint8_t)a;
			Utilities::ANA(state, (uint8_t)v);

			auto c = std::make_shared<i8080>();
			i8080_init(c.get());
			c->a = (uint8_t)a;
			i8080_ana(c.get(), (uint8_t)v);

			auto msg = "A=" + std::to_string(state.A) + "\t v=" + std::to_string(v);

			EXPECT_EQ(state.A, c->a) << msg;
			EXPECT_EQ(state.Flags.Sign, c->sf) << msg;
			EXPECT_EQ(state.Flags.Zero, c->zf) << msg;
			EXPECT_EQ(state.Flags.AuxiliaryCarry, c->hf) << msg;
			EXPECT_EQ(state.Flags.Parity, c->pf) << msg;
			EXPECT_EQ(state.Flags.Carry, c->cf) << msg;
		}
	}
}

TEST_F(UtilitiesTest, XRA)
{
	for (uint16_t a = 0; a <= 0xFF; ++a)
	{
		for (uint16_t v = 0; v <= 0xFF; ++v)
		{
			State state{};
			state.A = (uint8_t)a;
			Utilities::XRA(state, (uint8_t)v);

			auto c = std::make_shared<i8080>();
			i8080_init(c.get());
			c->a = (uint8_t)a;
			i8080_xra(c.get(), (uint8_t)v);

			auto msg = "A=" + std::to_string(state.A) + "\t v=" + std::to_string(v);

			EXPECT_EQ(state.A, c->a) << msg;
			EXPECT_EQ(state.Flags.Sign, c->sf) << msg;
			EXPECT_EQ(state.Flags.Zero, c->zf) << msg;
			EXPECT_EQ(state.Flags.AuxiliaryCarry, c->hf) << msg;
			EXPECT_EQ(state.Flags.Parity, c->pf) << msg;
			EXPECT_EQ(state.Flags.Carry, c->cf) << msg;
		}
	}
}

TEST_F(UtilitiesTest, ORA)
{
	for (uint16_t a = 0; a <= 0xFF; ++a)
	{
		for (uint16_t v = 0; v <= 0xFF; ++v)
		{
			State state{};
			state.A = (uint8_t)a;
			Utilities::ORA(state, (uint8_t)v);

			auto c = std::make_shared<i8080>();
			i8080_init(c.get());
			c->a = (uint8_t)a;
			i8080_ora(c.get(), (uint8_t)v);

			auto msg = "A=" + std::to_string(state.A) + "\t v=" + std::to_string(v);

			EXPECT_EQ(state.A, c->a) << msg;
			EXPECT_EQ(state.Flags.Sign, c->sf) << msg;
			EXPECT_EQ(state.Flags.Zero, c->zf) << msg;
			EXPECT_EQ(state.Flags.AuxiliaryCarry, c->hf) << msg;
			EXPECT_EQ(state.Flags.Parity, c->pf) << msg;
			EXPECT_EQ(state.Flags.Carry, c->cf) << msg;
		}
	}
}

TEST_F(UtilitiesTest, INR)
{
	for (uint16_t v = 0; v <= 0xFF; ++v)
	{
		State state{};
		uint8_t value = (uint8_t)v;
		Utilities::INR(state, value);

		auto c = std::make_shared<i8080>();
		i8080_init(c.get());
		auto result=i8080_inr(c.get(), (uint8_t)v);

		auto msg = "v=" + std::to_string(v);

		EXPECT_EQ(value, result) << msg;
		EXPECT_EQ(state.Flags.Sign, c->sf) << msg;
		EXPECT_EQ(state.Flags.Zero, c->zf) << msg;
		EXPECT_EQ(state.Flags.AuxiliaryCarry, c->hf) << msg;
		EXPECT_EQ(state.Flags.Parity, c->pf) << msg;
		EXPECT_EQ(state.Flags.Carry, c->cf) << msg;
	}
}

TEST_F(UtilitiesTest, RLC)
{
	for (uint16_t a = 0; a <= 0xFF; ++a)
	{
		State state{};
		state.A = (uint8_t)a;
		Utilities::RLC(state);

		auto c = std::make_shared<i8080>();
		i8080_init(c.get());
		c->a = (uint8_t)a;
		i8080_rlc(c.get());

		auto msg = "A=" + std::to_string(state.A);

		EXPECT_EQ(state.A, c->a) << msg;
		EXPECT_EQ(state.Flags.Sign, c->sf) << msg;
		EXPECT_EQ(state.Flags.Zero, c->zf) << msg;
		EXPECT_EQ(state.Flags.AuxiliaryCarry, c->hf) << msg;
		EXPECT_EQ(state.Flags.Parity, c->pf) << msg;
		EXPECT_EQ(state.Flags.Carry, c->cf) << msg;
	}
}

TEST_F(UtilitiesTest, RRC)
{
	for (uint16_t a = 0; a <= 0xFF; ++a)
	{
		State state{};
		state.A = (uint8_t)a;
		Utilities::RRC(state);

		auto c = std::make_shared<i8080>();
		i8080_init(c.get());
		c->a = (uint8_t)a;
		i8080_rrc(c.get());

		auto msg = "A=" + std::to_string(state.A);

		EXPECT_EQ(state.A, c->a) << msg;
		EXPECT_EQ(state.Flags.Sign, c->sf) << msg;
		EXPECT_EQ(state.Flags.Zero, c->zf) << msg;
		EXPECT_EQ(state.Flags.AuxiliaryCarry, c->hf) << msg;
		EXPECT_EQ(state.Flags.Parity, c->pf) << msg;
		EXPECT_EQ(state.Flags.Carry, c->cf) << msg;
	}
}

TEST_F(UtilitiesTest, RAL)
{
	for (uint16_t a = 0; a <= 0xFF; ++a)
	{
		State state{};
		state.A = (uint8_t)a;
		Utilities::RAL(state);

		auto c = std::make_shared<i8080>();
		i8080_init(c.get());
		c->a = (uint8_t)a;
		i8080_ral(c.get());

		auto msg = "A=" + std::to_string(state.A);

		EXPECT_EQ(state.A, c->a) << msg;
		EXPECT_EQ(state.Flags.Sign, c->sf) << msg;
		EXPECT_EQ(state.Flags.Zero, c->zf) << msg;
		EXPECT_EQ(state.Flags.AuxiliaryCarry, c->hf) << msg;
		EXPECT_EQ(state.Flags.Parity, c->pf) << msg;
		EXPECT_EQ(state.Flags.Carry, c->cf) << msg;
	}
}

TEST_F(UtilitiesTest, RAR)
{
	for (uint16_t a = 0; a <= 0xFF; ++a)
	{
		State state{};
		state.A = (uint8_t)a;
		Utilities::RAR(state);

		auto c = std::make_shared<i8080>();
		i8080_init(c.get());
		c->a = (uint8_t)a;
		i8080_rar(c.get());

		auto msg = "A=" + std::to_string(state.A);

		EXPECT_EQ(state.A, c->a) << msg;
		EXPECT_EQ(state.Flags.Sign, c->sf) << msg;
		EXPECT_EQ(state.Flags.Zero, c->zf) << msg;
		EXPECT_EQ(state.Flags.AuxiliaryCarry, c->hf) << msg;
		EXPECT_EQ(state.Flags.Parity, c->pf) << msg;
		EXPECT_EQ(state.Flags.Carry, c->cf) << msg;
	}
}

TEST_F(UtilitiesTest, DAD)
{
	for (uint16_t h = 0; h <= 0xFF; ++h)
	{
		for (uint16_t l = 0; l <= 0xFF; ++l)
		{
			State state{};
			Utilities::DAD(state,(uint8_t)h, (uint8_t)l);

			uint16_t hl = ((uint16_t)h) << 8 | ((uint16_t)l);

			auto c = std::make_shared<i8080>();
			i8080_init(c.get());
			i8080_dad(c.get(),hl);

			auto msg = "h=" + std::to_string(h) + "\tl=" + std::to_string(l);

			EXPECT_EQ(state.H, c->h) << msg;
			EXPECT_EQ(state.L, c->l) << msg;
			EXPECT_EQ(state.Flags.Sign, c->sf) << msg;
			EXPECT_EQ(state.Flags.Zero, c->zf) << msg;
			EXPECT_EQ(state.Flags.AuxiliaryCarry, c->hf) << msg;
			EXPECT_EQ(state.Flags.Parity, c->pf) << msg;
			EXPECT_EQ(state.Flags.Carry, c->cf) << msg;
		}
	}
}

TEST_F(UtilitiesTest, DCR)
{
	for (uint16_t v = 0; v <= 0xFF; ++v)
	{
		State state{};
		uint8_t value = (uint8_t)v;
		Utilities::DCR(state, value);

		auto c = std::make_shared<i8080>();
		i8080_init(c.get());
		auto result = i8080_dcr(c.get(), (uint8_t)v);

		auto msg = "v=" + std::to_string(v);

		EXPECT_EQ(value, result) << msg;
		EXPECT_EQ(state.Flags.Sign, c->sf) << msg;
		EXPECT_EQ(state.Flags.Zero, c->zf) << msg;
		EXPECT_EQ(state.Flags.AuxiliaryCarry, c->hf) << msg;
		EXPECT_EQ(state.Flags.Parity, c->pf) << msg;
		EXPECT_EQ(state.Flags.Carry, c->cf) << msg;
	}
}

TEST_F(UtilitiesTest, DAA)
{
	for (auto hf = 0; hf<2; ++hf)
	{
		for (auto cf = 0; cf<2; ++cf)
		{
			for (uint16_t a = 0; a <= 0xFF; ++a)
			{
				State state{};
				state.A = (uint8_t)a;
				state.Flags.AuxiliaryCarry = hf;
				state.Flags.Carry = cf;
				Utilities::DAA(state);

				auto c = std::make_shared<i8080>();
				i8080_init(c.get());
				c->a = (uint8_t)a;
				c->hf = hf;
				c->cf = cf;
				i8080_daa(c.get());

				auto msg = "A=" + std::to_string(state.A) + " hf=" + std::to_string(hf) + " cf=" + std::to_string(cf);

				EXPECT_EQ(state.A, c->a) << msg;
				EXPECT_EQ(state.Flags.Sign, c->sf) << msg;
				EXPECT_EQ(state.Flags.Zero, c->zf) << msg;
				EXPECT_EQ(state.Flags.AuxiliaryCarry, c->hf) << msg;
				EXPECT_EQ(state.Flags.Parity, c->pf) << msg;
				EXPECT_EQ(state.Flags.Carry, c->cf) << msg;
			}
		}
	}
}

TEST_F(UtilitiesTest, SUB)
{
	for (uint16_t a = 0; a <= 0xFF; ++a)
	{
		for (uint16_t v = 0; v <= 0xFF; ++v)
		{
			State state{};
			state.A = (uint8_t)a;
			Utilities::SUB(state, (uint8_t)v,0);

			auto c = std::make_shared<i8080>();
			i8080_init(c.get());
			c->a = (uint8_t)a;
			i8080_sub(c.get(), &c->a, (uint8_t)v, false);

			auto msg = "A=" + std::to_string(state.A) + "\t v=" + std::to_string(v);

			EXPECT_EQ(state.A, c->a) << msg;
			EXPECT_EQ(state.Flags.Sign, c->sf) << msg;
			EXPECT_EQ(state.Flags.Zero, c->zf) << msg;
			EXPECT_EQ(state.Flags.AuxiliaryCarry, c->hf) << msg;
			EXPECT_EQ(state.Flags.Parity, c->pf) << msg;
			EXPECT_EQ(state.Flags.Carry, c->cf) << msg;
		}
	}
}