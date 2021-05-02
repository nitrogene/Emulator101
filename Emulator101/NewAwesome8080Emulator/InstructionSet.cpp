#include <fmt/core.h>
#include <fstream>
#include "InstructionSet.h"
#include "Utilities.h"

InstructionSet::InstructionSet(const std::filesystem::path& pathToInstructionSet)
{
	std::ifstream fs(pathToInstructionSet);

	if (fs.is_open())
	{
		std::string line;
		std::getline(fs, line);
		// skip header row

		m_InstructionSet.fill(nullptr);

		// read line by line
		for (line; std::getline(fs, line); )
		{
			auto isl = std::make_shared<InstructionSetLine>();

			auto t = Utilities::Split(line, ';');

			if (t.size() != 12)
			{
				throw new std::exception("bad instruction set format");
			}

			isl->Mnemonic = t[0];
			isl->Description = t[1];

			for (auto i = 0; i < 8; ++i)
			{
				isl->Bits[i] = t[i + 2][0];
			}

			auto periods = Utilities::Split(t[10], '/');
			isl->ClockCycle.A = (uint8_t)std::atoi(periods[0].c_str());
			if (periods.size() > 1)
			{
				isl->ClockCycle.B = (uint8_t)std::atoi(periods[1].c_str());
			}
			isl->Size = (uint8_t)atoi(t[11].c_str());

			uint8_t r;
			if (isl->TryConvertBits(r))
			{
				if (m_InstructionSet[r] != nullptr)
				{
					throw new std::exception("instruction already in map");
				}
				m_InstructionSet[r] = isl;
			}
		}
		fs.close();
		
		// TODO: how to deduce following from the instruction set file ?
		// 
		// NOP
		{
			auto isl = m_InstructionSet[0x00];
			isl->exec = [isl](State& state, MemoryMap&, const uint8_t*)
			{
				state.PC += isl->Size;
				state.Steps++;
				state.Cycles+=isl->ClockCycle.A;
			};
		}

		// LXI B,D16
		{
			auto isl = m_InstructionSet[0x01];
			isl->exec = [isl](State& state, MemoryMap&, const uint8_t* opCode)
			{
				state.B = opCode[2];
				state.C = opCode[1];
				state.PC += isl->Size;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// STAX B
		{
			auto isl = m_InstructionSet[0x02];
			isl->exec = [isl](State& state, MemoryMap& map, const uint8_t*)
			{
				uint16_t adr = (state.B << 8) + state.C;
				map.Poke(adr, state.A);
				state.PC += isl->Size;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// INX B
		{
			auto isl = m_InstructionSet[0x03];
			isl->exec = [isl](State& state, MemoryMap&, const uint8_t*)
			{
				state.C += 1;
				if (state.C == 0)
				{
					state.B += 1;
				}

				state.PC += isl->Size;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// DCR B
		{
			auto isl = m_InstructionSet[0x05];
			isl->exec = [&isl,this](State& state, MemoryMap&, const uint8_t*)
			{
				DCR(state,state.B);
				state.PC += isl->Size;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// MVI B, D8
		{
			auto isl = m_InstructionSet[0x06];
			isl->exec = [isl](State& state, MemoryMap&, const uint8_t* opCode)
			{
				state.B = opCode[1];
				state.PC += isl->Size;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// DCR C
		{
			auto isl = m_InstructionSet[0x0d];
			isl->exec = [&isl, this](State& state, MemoryMap&, const uint8_t*)
			{
				DCR(state, state.C);
				state.PC += isl->Size;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// LXI D,16
		{
			auto isl = m_InstructionSet[0x11];
			isl->exec = [isl](State& state, MemoryMap&, const uint8_t* opCode)
			{
				state.D = opCode[2];
				state.E = opCode[1];
				state.PC += isl->Size;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// INX C
		{
			auto isl = m_InstructionSet[0x13];
			isl->exec = [isl](State& state, MemoryMap&, const uint8_t*)
			{
				state.E += 1;
				if (state.E == 0)
				{
					state.D += 1;
				}

				state.PC += isl->Size;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// DCR D
		{
			auto isl = m_InstructionSet[0x15];
			isl->exec = [&isl, this](State& state, MemoryMap&, const uint8_t*)
			{
				DCR(state, state.D);
				state.PC += isl->Size;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// LDAX D
		{
			auto isl = m_InstructionSet[0x1A];
			isl->exec = [isl](State& state, MemoryMap& map, const uint8_t*)
			{
				uint16_t adr = (state.D << 8) + state.E;
				state.A = map.Peek(adr);
				state.PC += isl->Size;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// DCR E
		{
			auto isl = m_InstructionSet[0x1D];
			isl->exec = [&isl, this](State& state, MemoryMap&, const uint8_t*)
			{
				DCR(state, state.E);
				state.PC += isl->Size;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// LXI D,16
		{
			auto isl = m_InstructionSet[0x21];
			isl->exec = [isl](State& state, MemoryMap&, const uint8_t* opCode)
			{
				state.H = opCode[2];
				state.L = opCode[1];
				state.PC += isl->Size;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// INX H
		{
			auto isl = m_InstructionSet[0x23];
			isl->exec = [isl](State& state, MemoryMap&, const uint8_t*)
			{
				state.L += 1;
				if (state.L == 0)
				{
					state.H += 1;
				}

				state.PC += isl->Size;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// DCR H
		{
			auto isl = m_InstructionSet[0x25];
			isl->exec = [&isl, this](State& state, MemoryMap&, const uint8_t*)
			{
				DCR(state, state.H);
				state.PC += isl->Size;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// DCR L
		{
			auto isl = m_InstructionSet[0x2D];
			isl->exec = [&isl, this](State& state, MemoryMap&, const uint8_t*)
			{
				DCR(state, state.L);
				state.PC += isl->Size;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// LXI SP,D16
		{
			auto isl = m_InstructionSet[0x31];
			isl->exec = [isl](State& state, MemoryMap&, const uint8_t* opCode)
			{
				state.SP = (opCode[2] << 8) + opCode[1];
				state.PC += isl->Size;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// INX SP
		{
			auto isl = m_InstructionSet[0x33];
			isl->exec = [isl](State& state, MemoryMap&, const uint8_t*)
			{
				state.SP += 1;
				state.PC += isl->Size;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// DCR A
		{
			auto isl = m_InstructionSet[0x3D];
			isl->exec = [&isl, this](State& state, MemoryMap&, const uint8_t*)
			{
				DCR(state, state.A);
				state.PC += isl->Size;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// MOV M,A
		{
			auto isl = m_InstructionSet[0x77];
			isl->exec = [&isl, this](State& state, MemoryMap& map, const uint8_t*)
			{
				uint16_t adr = (state.H << 8) + state.L;
				map.Poke(adr, state.A);
				state.PC += isl->Size;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// JNZ adr
		{
			auto isl = m_InstructionSet[0xC2];
			isl->exec = [&isl, this](State& state, MemoryMap&, const uint8_t* opCode)
			{
				if (state.Z)
				{
					state.PC += isl->Size;
				}
				else
				{
					state.PC = (opCode[2] << 8) + opCode[1];
				}
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// JMP adr
		{
			auto isl = m_InstructionSet[0xC3];
			isl->exec = [&isl, this](State& state, MemoryMap&, const uint8_t* opCode)
			{
				state.PC = (opCode[2] << 8) + opCode[1];
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// PUSH B
		{
			auto isl = m_InstructionSet[0xC5];
			isl->exec = [&isl, this](State& state, MemoryMap& map, const uint8_t*)
			{
				map.Poke(state.SP - 1, state.B);
				map.Poke(state.SP - 2, state.C);
				state.PC += isl->Size;
				state.SP -= 2;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// ADI D8
		{
			auto isl = m_InstructionSet[0xC6];
			isl->exec = [&isl, this](State& state, MemoryMap&, const uint8_t* opCode)
			{
				ADI(state, opCode[1]);
				state.PC += isl->Size;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// RET
		{
			auto isl = m_InstructionSet[0xC9];
			isl->exec = [&isl, this](State& state, MemoryMap& map, const uint8_t*)
			{
				auto spc = map.Peek(state.SP);
				auto spc1 = map.Peek(state.SP + 1);
				state.PC = (spc1 << 8) + spc;
				state.SP += 2;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// JZ adr
		{
			auto isl = m_InstructionSet[0xCA];
			isl->exec = [&isl, this](State& state, MemoryMap&, const uint8_t* opCode)
			{
				if (state.Z)
				{
					state.PC = (opCode[2] << 8) + opCode[1];
				}
				else
				{
					state.PC += isl->Size;
				}
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// CALL adr
		{
			auto isl = m_InstructionSet[0xCD];
			isl->exec = [&isl, this](State& state, MemoryMap& map, const uint8_t* opCode)
			{
				map.Poke(state.SP - 1, (state.PC & 0xFF00) >> 8);
				map.Poke(state.SP - 2, state.PC & 0x00FF);
				state.SP -= 2;
				state.PC = (opCode[2] << 8) + opCode[1];
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// ACI D8
		{
			auto isl = m_InstructionSet[0xCE];
			isl->exec = [&isl, this](State& state, MemoryMap&, const uint8_t* opCode)
			{
				ACI(state, opCode[1]);
				state.PC += isl->Size;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}


		// JNC adr
		{
			auto isl = m_InstructionSet[0xCA];
			isl->exec = [&isl, this](State& state, MemoryMap&, const uint8_t* opCode)
			{
				if (state.C)
				{
					state.PC += isl->Size;
				}
				else
				{
					state.PC = (opCode[2] << 8) + opCode[1];
				}
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// PUSH D
		{
			auto isl = m_InstructionSet[0xD5];
			isl->exec = [&isl, this](State& state, MemoryMap& map, const uint8_t*)
			{
				map.Poke(state.SP - 1, state.D);
				map.Poke(state.SP - 2, state.E);
				state.PC += isl->Size;
				state.SP -= 2;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// JC adr
		{
			auto isl = m_InstructionSet[0xDA];
			isl->exec = [&isl, this](State& state, MemoryMap&, const uint8_t* opCode)
			{
				if (state.C)
				{
					state.PC = (opCode[2] << 8) + opCode[1];
				}
				else
				{
					state.PC += isl->Size;
				}
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// PUSH H
		{
			auto isl = m_InstructionSet[0xE5];
			isl->exec = [&isl, this](State& state, MemoryMap& map, const uint8_t*)
			{
				map.Poke(state.SP - 1, state.H);
				map.Poke(state.SP - 2, state.L);
				state.PC += isl->Size;
				state.SP -= 2;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// ANI D8
		{
			auto isl = m_InstructionSet[0xE6];
			isl->exec = [&isl, this](State& state, MemoryMap& map, const uint8_t* opCode)
			{
				ANI(state,state.H, opCode[1]);
				state.PC += isl->Size;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// PCHL
		{
			auto isl = m_InstructionSet[0xE9];
			isl->exec = [&isl, this](State& state, MemoryMap&, const uint8_t*)
			{
				state.PC = (state.H << 8) + state.L;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// JPE adr
		{
			auto isl = m_InstructionSet[0xEA];
			isl->exec = [&isl, this](State& state, MemoryMap&, const uint8_t* opCode)
			{
				if (state.P)
				{
					state.PC = (opCode[2] << 8) + opCode[1];
				}
				else
				{
					state.PC += isl->Size;
				}
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// JPO adr
		{
			auto isl = m_InstructionSet[0xE2];
			isl->exec = [&isl, this](State& state, MemoryMap&, const uint8_t* opCode)
			{
				if (state.P)
				{
					state.PC += isl->Size;
				}
				else
				{
					state.PC = (opCode[2] << 8) + opCode[1];
				}
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// JP adr
		{
			auto isl = m_InstructionSet[0xF2];
			isl->exec = [&isl, this](State& state, MemoryMap&, const uint8_t* opCode)
			{
				if (state.S)
				{
					state.PC += isl->Size;
				}
				else
				{
					state.PC = (opCode[2] << 8) + opCode[1];
				}
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// PUSH PSW
		{
			auto isl = m_InstructionSet[0xF5];
			isl->exec = [&isl, this](State& state, MemoryMap& map, const uint8_t* opCode)
			{
				map.Poke(state.SP - 1, state.A);
				map.Poke(state.SP - 2, state.F);
				state.PC += isl->Size;
				state.SP -= 2;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// JM adr
		{
			auto isl = m_InstructionSet[0xFA];
			isl->exec = [&isl, this](State& state, MemoryMap&, const uint8_t* opCode)
			{
				if (state.S)
				{
					state.PC = (opCode[2] << 8) + opCode[1];
				}
				else
				{
					state.PC += isl->Size;
				}
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}

		// CPI D8
		{
			auto isl = m_InstructionSet[0xFA];
			isl->exec = [&isl, this](State& state, MemoryMap&, const uint8_t* opCode)
			{
				CPI(state, opCode[1]);
				state.PC += isl->Size;
				state.Steps++;
				state.Cycles += isl->ClockCycle.A;
			};
		}
	}
	else
	{
		throw new std::exception("Unable to load instruction set");
	}
}

std::shared_ptr<const InstructionSetLine> InstructionSet::operator[](const uint8_t idx) const
{
	return m_InstructionSet[idx];
}

const std::string InstructionSet::DisplayInstructionSet() const
{
	std::string result="Opcode\tInstruction\tsize\tflags\tfunction\tclock cycle\n";

	// not uint8_t otherwise, infinite loop
	for (unsigned int i = 0; i < 256; ++i)
	{
		if (m_InstructionSet[i] == nullptr)
		{
			result+=fmt::format("{0:#02x}\t{1:10}\n", i, '-');
			continue;
		}

		auto isl = m_InstructionSet[i];
		result += fmt::format("{0:#02x}\t{1:10}\t{2}\t\t\t{3}\n", i, isl->Mnemonic, isl->Size, isl->ClockCycle.toString());
	}

	return result;
}

void InstructionSet::DCR(State& state, uint8_t& value)
{
	/*
	DCR R (Decrement register)
	0 0 r r r 1 0 1						R <- R - 1 Data in R is decremented by 1
										flags affected: Z, S, P, AC
	*/

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)value);
	uint16_t b = ((uint16_t)-1);
	uint16_t chp = a + b;

	// get result in normal precision
	value = chp & 0xFF;

	// ZERO CHECK
	state.Z = value == 0;

	// SIGN CHECK
	state.S = chp & 0x80;

	// PARITY CHECK
	state.P = Utilities::isOddParity(value);

	// AUXILIARY CARRY
	uint8_t a_ = a & 0x0F;
	uint8_t b_ = b & 0x0F;
	uint8_t c_ = a_ + b_;
	state.AC = c_ & 0x10;

	// change F 
	state.setF();
}

void InstructionSet::ANI(State& state, uint8_t& value, const uint8_t& opCode1)
{
	/*
	ANI data8 (AND immediate)
	1	1	1	0	0	1	1	0		A <- A ^ data8 Bits in A logically multiplied with bits from
	d7	d6	d5	d4	d3	d2	d1	d0		2nd byte of instruction, flags affected: Z, S, P, CY=0, AC=0

	*/

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)value);
	uint16_t b = ((uint16_t)opCode1);
	uint16_t chp = a & b;

	// get result in normal precision
	value = chp & 0xFF;

	// ZERO CHECK
	state.Z = value == 0;

	// SIGN CHECK
	state.S = chp & 0x80;

	// PARITY CHECK
	state.P = Utilities::isOddParity(value);

	state.CY = 0;
	state.AC = 0;

	// change F 
	state.setF();
}

void InstructionSet::ADI(State& state, const uint8_t& opCode1)
{
	/*
	ADI data8 (Add immediate)
	1	1	0	0	0	1	1	0		A<-A + data8 one byte (next to instruction) added to A
	d7	d6	d5	d4	d3	d2	d1	d0		flags affected: Z, S, P, CY, AC
	*/

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)state.A);
	uint16_t b = ((uint16_t)opCode1);
	uint16_t chp = a + b;

	// get result in normal precision
	state.A = chp & 0xFF;

	// ZERO CHECK
	state.Z = state.A == 0;

	// SIGN CHECK
	state.S = chp & 0x80;

	// PARITY CHECK
	state.P = Utilities::isOddParity(state.A);

	state.CY = 0;
	state.AC = 0;

	// change F 
	state.setF();
}

void InstructionSet::CPI(State& state, const uint8_t& opCode1)
{
	/*
	CPI data8 (Compare immediate)
	1	1	1	1	1	1	1	0		A - data8 Data in 2nd byte of instruction is subtracted
	d7	d6	d5	d4	d3	d2	d1	d0		from data in A, only flags are affected: Z, S, P, CY, AC
	*/

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)state.A);
	uint16_t b = ((uint16_t)opCode1);
	uint16_t chp = a - b;

	// get result in normal precision
	state.A = chp & 0xFF;

	// ZERO CHECK
	state.Z = state.A == 0;

	// SIGN CHECK
	state.S = chp & 0x80;

	// PARITY CHECK
	state.P = Utilities::isOddParity(state.A);

	// CARRY BIT
	state.CY = chp & 0x100;

	// AUXILIARY CARRY
	uint8_t a_ = a & 0x0F;
	uint8_t b_ = b & 0x0F;
	uint8_t c_ = a_ - b_;
	state.AC = c_ & 0x10;

	// change F 
	state.setF();
}

void InstructionSet::ACI(State& state, const uint8_t& opCode1)
{
	/*
	ACI data8 (Add immediate with carry)
	1	1	0	0	1	1	1	0		A <- A + data8 + CY one byte and CY added to A
	d7	d6	d5	d4	d3	d2	d1 d0		flags affected: Z, S, P, CY, AC
	*/

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)state.A);
	uint16_t b = ((uint16_t)opCode1);
	uint16_t chp = a + b;

	if (state.CY)
	{
		chp += 1;
	}

	// get result in normal precision
	state.A = chp & 0xFF;

	// ZERO CHECK
	state.Z = state.A == 0;

	// SIGN CHECK
	state.S = chp & 0x80;

	// PARITY CHECK
	state.P = Utilities::isOddParity(state.A);

	// CARRY BIT
	state.CY = chp & 0x100;

	// AUXILIARY CARRY
	uint8_t a_ = a & 0x0F;
	uint8_t b_ = b & 0x0F;
	uint8_t c_ = a_ - b_;
	state.AC = c_ & 0x10;

	// change F 
	state.setF();
}