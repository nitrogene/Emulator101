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
			m_InstructionSet[0x00]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				state.PC += size;
				state.Steps++;
				state.Cycles+= cycle.A;
			};
		}

		// LXI B,D16
		{
			m_InstructionSet[0x01]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.B = opCode[2];
				state.C = opCode[1];
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// STAX B
		{
			m_InstructionSet[0x02]->exec = [](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (state.B << 8) + state.C;
				map.Poke(adr, state.A);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// INX B
		{
			m_InstructionSet[0x03]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				state.C += 1;
				if (state.C == 0)
				{
					state.B += 1;
				}

				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// INR B
		{
			m_InstructionSet[0x04]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				INR(state, state.B);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// DCR B
		{
			m_InstructionSet[0x05]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				DCR(state,state.B);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MVI B, D8
		{
			m_InstructionSet[0x06]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.B = opCode[1];
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// RLC
		{
			m_InstructionSet[0x07]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				RLC(state);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// INR C
		{
			m_InstructionSet[0x0c]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				INR(state, state.C);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// DCR C
		{
			m_InstructionSet[0x0d]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				DCR(state, state.C);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// RRC
		{
			m_InstructionSet[0x0F]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				RRC(state);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// LXI D,16
		{
			m_InstructionSet[0x11]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.D = opCode[2];
				state.E = opCode[1];
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// STAX D
		{
			m_InstructionSet[0x12]->exec = [](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (state.D << 8) + state.E;
				map.Poke(adr, state.A);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// INX C
		{
			m_InstructionSet[0x13]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				state.E += 1;
				if (state.E == 0)
				{
					state.D += 1;
				}

				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// INR D
		{
			m_InstructionSet[0x14]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				INR(state, state.D);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// DCR D
		{
			m_InstructionSet[0x15]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				DCR(state, state.D);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// RAL
		{
			m_InstructionSet[0x17]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				RAL(state);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// LDAX D
		{
			m_InstructionSet[0x1A]->exec = [](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (state.D << 8) + state.E;
				state.A = map.Peek(adr);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// INR E
		{
			m_InstructionSet[0x1C]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				INR(state, state.E);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// DCR E
		{
			m_InstructionSet[0x1D]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				DCR(state, state.E);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// RAR
		{
			m_InstructionSet[0x1F]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				RAR(state);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// LXI D,16
		{
			m_InstructionSet[0x21]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.H = opCode[2];
				state.L = opCode[1];
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// INX H
		{
			m_InstructionSet[0x23]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				state.L += 1;
				if (state.L == 0)
				{
					state.H += 1;
				}

				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// INR H
		{
			m_InstructionSet[0x24]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				INR(state, state.H);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// DCR H
		{
			m_InstructionSet[0x25]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				DCR(state, state.H);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// INR L
		{
			m_InstructionSet[0x2C]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				INR(state, state.L);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// DCR L
		{
			m_InstructionSet[0x2D]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				DCR(state, state.L);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// LXI SP,D16
		{
			m_InstructionSet[0x31]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.SP = (opCode[2] << 8) + opCode[1];
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// INX SP
		{
			m_InstructionSet[0x33]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				state.SP += 1;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// INR M
		{
			m_InstructionSet[0x34]->exec = [](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr= (state.H << 8) + state.L;
				auto value = map.Peek(adr);
				INR(state, value);
				map.Poke(adr,value);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// INR A
		{
			m_InstructionSet[0x3C]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				INR(state, state.A);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// DCR A
		{
			m_InstructionSet[0x3D]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				DCR(state, state.A);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV M,A
		{
			m_InstructionSet[0x77]->exec = [](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (state.H << 8) + state.L;
				map.Poke(adr, state.A);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// JNZ adr
		{
			m_InstructionSet[0xC2]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				if (state.Z)
				{
					state.PC += size;
				}
				else
				{
					state.PC = (opCode[2] << 8) + opCode[1];
				}
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// JMP adr
		{
			m_InstructionSet[0xC3]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.PC = (opCode[2] << 8) + opCode[1];
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// PUSH B
		{
			m_InstructionSet[0xC5]->exec = [](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				map.Poke(state.SP - 1, state.B);
				map.Poke(state.SP - 2, state.C);
				state.PC += size;
				state.SP -= 2;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// ADI D8
		{
			m_InstructionSet[0xC6]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				ADI(state, opCode[1]);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// RET
		{
			m_InstructionSet[0xC9]->exec = [](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				auto spc = map.Peek(state.SP);
				auto spc1 = map.Peek(state.SP + 1);
				state.PC = (spc1 << 8) + spc;
				state.SP += 2;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// JZ adr
		{
			m_InstructionSet[0xCA]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				if (state.Z)
				{
					state.PC = (opCode[2] << 8) + opCode[1];
				}
				else
				{
					state.PC += size;
				}
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// CALL adr
		{
			m_InstructionSet[0xCD]->exec = [](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				map.Poke(state.SP - 1, (state.PC & 0xFF00) >> 8);
				map.Poke(state.SP - 2, state.PC & 0x00FF);
				state.SP -= 2;
				state.PC = (opCode[2] << 8) + opCode[1];
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// ACI D8
		{
			m_InstructionSet[0xCE]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				ACI(state, opCode[1]);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}


		// JNC adr
		{
			m_InstructionSet[0xCA]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				if (state.C)
				{
					state.PC += size;
				}
				else
				{
					state.PC = (opCode[2] << 8) + opCode[1];
				}
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// PUSH D
		{
			m_InstructionSet[0xD5]->exec = [](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				map.Poke(state.SP - 1, state.D);
				map.Poke(state.SP - 2, state.E);
				state.PC += size;
				state.SP -= 2;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// JC adr
		{
			m_InstructionSet[0xDA]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				if (state.C)
				{
					state.PC = (opCode[2] << 8) + opCode[1];
				}
				else
				{
					state.PC += size;
				}
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// PUSH H
		{
			m_InstructionSet[0xE5]->exec = [](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				map.Poke(state.SP - 1, state.H);
				map.Poke(state.SP - 2, state.L);
				state.PC += size;
				state.SP -= 2;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// ANI D8
		{
			m_InstructionSet[0xE6]->exec = [](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				ANI(state,state.H, opCode[1]);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// PCHL
		{
			m_InstructionSet[0xE9]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				state.PC = (state.H << 8) + state.L;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// JPE adr
		{
			m_InstructionSet[0xEA]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				if (state.P)
				{
					state.PC = (opCode[2] << 8) + opCode[1];
				}
				else
				{
					state.PC += size;
				}
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// JPO adr
		{
			m_InstructionSet[0xE2]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				if (state.P)
				{
					state.PC += size;
				}
				else
				{
					state.PC = (opCode[2] << 8) + opCode[1];
				}
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// JP adr
		{
			m_InstructionSet[0xF2]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				if (state.S)
				{
					state.PC += size;
				}
				else
				{
					state.PC = (opCode[2] << 8) + opCode[1];
				}
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// PUSH PSW
		{
			m_InstructionSet[0xF5]->exec = [](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				map.Poke(state.SP - 1, state.A);
				map.Poke(state.SP - 2, state.F);
				state.PC += size;
				state.SP -= 2;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// JM adr
		{
			m_InstructionSet[0xFA]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				if (state.S)
				{
					state.PC = (opCode[2] << 8) + opCode[1];
				}
				else
				{
					state.PC += size;
				}
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// CPI D8
		{
			m_InstructionSet[0xFA]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				CPI(state, opCode[1]);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
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

void InstructionSet::INR(State& state, uint8_t& value)
{
	/*
	INR R (Increment register)
	0 0 r r r 1 0 0					R <- R + 1 Data in R is incremented by 1
									flags affected: Z, S, P, AC
	*/

	// Perform operation in higher precision
	uint16_t a = ((uint16_t)value);
	uint16_t b = ((uint16_t)1);
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

void InstructionSet::RLC(State& state)
{
	/*
	RLC (Rotate left / rotate logically left)
	0	0	0	0	0	1	1	1		Ai+1<-Ai, A0<-A7, CY<-A7 Bits in A shifted left,
										oldest bit copied to youngest bit and CY, flags affected: CY

	*/

	// CARRY BIT
	state.CY = state.A & 0x80;
	state.A = std::rotl(state.A,1);

	// change F 
	state.setF();
}

void InstructionSet::RRC(State& state)
{
	/*
	RRC (Rotate right / rotate logically right)
	0	0	0	0	1	1	1	1		Ai<-Ai+1, A7<-A0, CY<-SA0 Bits in A shifted right,
										youngest bit copied
	*/

	// CARRY BIT
	state.CY = state.A & 0x01;
	state.A = std::rotr(state.A, 1);

	// change F 
	state.setF();
}

void InstructionSet::RAL(State& state)
{
	/*
	RAL (Rotate left through carry / rotate arithmetically left)
	0	0	0	1	0	1	1	1		Ai+1<-Ai, A0<-CY, CY<-A7 Bits in A shifted left,
										CY copied to youngest bit, oldest bit copied to CY,
										flags affected: CY
	*/

	uint8_t oldCarry = state.CY ? 1 : 0;

	state.A = std::rotl(state.A, 1);

	// CARRY BIT
	state.CY = state.A & 0b00000001;
	state.A = state.A & 0b11111110 | oldCarry;

	// change F 
	state.setF();
}

void InstructionSet::RAR(State& state)
{
	/*
	RAR (Rotate right through carry / rotate arithmetically right)
	0	0	0	1	0	1	1	1		Ai<-Ai+1, A7<-CY, CY-<A0 Bits in A shifted right,
										CY copied to oldest bit, youngest bit copied to CY,
										flags affected: CY
	*/
	uint8_t oldCarry = state.CY ? 1 : 0;

	state.A = std::rotr(state.A, 1);
	
	// CARRY BIT
	state.CY = state.A & 0b10000000;
	state.A = state.A & 0b01111111 | oldCarry<<7;

	// change F 
	state.setF();
}