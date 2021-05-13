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
				uint16_t value = (state.B << 8) + state.C;
				value++;
				state.B = (value & 0b1111111100000000) >> 8;
				state.C = value & 0b0000000011111111;
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

		// DAD B
		{
			m_InstructionSet[0x09]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				DAD(state, state.B, state.C);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// LDAX B
		{
			m_InstructionSet[0x0A]->exec = [](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (state.B << 8) + state.C;
				state.A = map.Peek(adr);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// DCX B
		{
			m_InstructionSet[0x0B]->exec = [](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t value = (state.B << 8) + state.C;
				value--;
				state.B = (value & 0b1111111100000000) >> 8;
				state.C = value & 0b0000000011111111;
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

		// MVI C, D8
		{
			m_InstructionSet[0x0E]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.C = opCode[1];
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

		// INX D
		{
			m_InstructionSet[0x13]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t value = (state.D << 8) + state.E;
				value++;
				state.D = (value & 0b1111111100000000) >> 8;
				state.E = value & 0b0000000011111111;
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

		// MVI D, D8
		{
			m_InstructionSet[0x16]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.D = opCode[1];
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

		// DAD D
		{
			m_InstructionSet[0x19]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				DAD(state, state.D, state.E);
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

		// DCX D
		{
			m_InstructionSet[0x1B]->exec = [](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t value = (state.D << 8) + state.E;
				value--;
				state.D = (value & 0b1111111100000000) >> 8;
				state.E = value & 0b0000000011111111;
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

		// MVI E, D8
		{
			m_InstructionSet[0x1E]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.E = opCode[1];
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

		// SHLD adr
		{
			m_InstructionSet[0x22]->exec = [](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (opCode[2] << 8) + opCode[1];
				map.Poke(adr, state.L);
				map.Poke(adr+1, state.H);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// INX H
		{
			m_InstructionSet[0x23]->exec = [](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t value = (state.H << 8) + state.L;
				value++;
				state.H = (value & 0b1111111100000000) >> 8;
				state.L = value & 0b0000000011111111;
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

		// MVI H, D8
		{
			m_InstructionSet[0x26]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.H = opCode[1];
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// DAA
		{
			m_InstructionSet[0x27]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				DAA(state);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// DAD H
		{
			m_InstructionSet[0x29]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				DAD(state, state.H, state.L);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// LHLD adr
		{
			m_InstructionSet[0x2A]->exec = [](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				auto adr = (opCode[2] << 8) + opCode[1];
				state.L = map.Peek(adr);
				state.H = map.Peek(adr+1);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// DCX H
		{
			m_InstructionSet[0x2B]->exec = [](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t value = (state.H << 8) + state.L;
				value--;
				state.H = (value & 0b1111111100000000) >> 8;
				state.L = value & 0b0000000011111111;
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

		// MVI L, D8
		{
			m_InstructionSet[0x2E]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.L = opCode[1];
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// CMA
		{
			m_InstructionSet[0x2F]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.A = ~state.A;
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

		// STA adr
		{
			m_InstructionSet[0x32]->exec = [](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				auto adr= (opCode[2] << 8) + opCode[1];
				map.Poke(adr, state.A);
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

		// DCR M
		{
			m_InstructionSet[0x35]->exec = [](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (state.H << 8) + state.L;
				auto value = map.Peek(adr);
				DCR(state, value);
				map.Poke(adr, value);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MVI M, D8
		{
			m_InstructionSet[0x36]->exec = [](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (state.H << 8) + state.L;
				map.Poke(adr, opCode[1]);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// STC
		{
			m_InstructionSet[0x37]->exec = [](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.CY = true;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// DAD SP
		{
			m_InstructionSet[0x39]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				uint8_t b = (state.SP & 0b1111111100000000) >> 8;
				uint8_t c = state.SP &  0b0000000011111111;

				DAD(state, b, c);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// LDA adr
		{
			m_InstructionSet[0x3A]->exec = [](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (opCode[2] << 8) + opCode[1];
				state.A = map.Peek(adr);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// DCX SP
		{
			m_InstructionSet[0x3B]->exec = [](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				state.SP--;
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

		// MVI A, D8
		{
			m_InstructionSet[0x3E]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.A = opCode[1];
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// CMC
		{
			m_InstructionSet[0x3F]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.CY = !state.CY;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV B,B
		{
			m_InstructionSet[0x40]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV B,C
		{
			m_InstructionSet[0x41]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.B = state.C;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV B,D
		{
			m_InstructionSet[0x42]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.B = state.D;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV B,E
		{
			m_InstructionSet[0x43]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.B = state.E;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV B,H
		{
			m_InstructionSet[0x44]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.B = state.H;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV B,L
		{
			m_InstructionSet[0x45]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.B = state.L;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV B,M
		{
			m_InstructionSet[0x46]->exec = [](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (state.H << 8) + state.L;
				state.B = map.Peek(adr);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV B,A
		{
			m_InstructionSet[0x47]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.B = state.A;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV C,B
		{
			m_InstructionSet[0x48]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.C = state.B;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV C,C
		{
			m_InstructionSet[0x49]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV C,D
		{
			m_InstructionSet[0x4A]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.C = state.D;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV C,E
		{
			m_InstructionSet[0x4B]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.C = state.E;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV C,H
		{
			m_InstructionSet[0x4C]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.C = state.H;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV C,L
		{
			m_InstructionSet[0x4D]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.C = state.L;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV C,M
		{
			m_InstructionSet[0x4E]->exec = [](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (state.H << 8) + state.L;
				state.C = map.Peek(adr);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV C,A
		{
			m_InstructionSet[0x4F]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.C = state.A;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV D,B
		{
			m_InstructionSet[0x50]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.D = state.B;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV D,C
		{
			m_InstructionSet[0x51]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.D = state.C;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV D,D
		{
			m_InstructionSet[0x52]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV D,E
		{
			m_InstructionSet[0x53]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.D = state.E;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV D,H
		{
			m_InstructionSet[0x54]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.D = state.H;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV D,L
		{
			m_InstructionSet[0x55]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.D = state.L;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV D,M
		{
			m_InstructionSet[0x56]->exec = [](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (state.H << 8) + state.L;
				state.D = map.Peek(adr);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV D,A
		{
			m_InstructionSet[0x57]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.D = state.A;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV E,B
		{
			m_InstructionSet[0x58]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.E = state.B;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV E,C
		{
			m_InstructionSet[0x59]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.E = state.C;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV E,D
		{
			m_InstructionSet[0x5A]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.E = state.D;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV E,E
		{
			m_InstructionSet[0x5B]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV E,H
		{
			m_InstructionSet[0x5C]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.E = state.H;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV E,L
		{
			m_InstructionSet[0x5D]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.E = state.L;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV E,M
		{
			m_InstructionSet[0x5E]->exec = [](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (state.H << 8) + state.L;
				state.E = map.Peek(adr);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV E,A
		{
			m_InstructionSet[0x5F]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.E = state.F;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV H,B
		{
			m_InstructionSet[0x60]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.H = state.B;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV H,C
		{
			m_InstructionSet[0x61]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.H = state.C;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV H,D
		{
			m_InstructionSet[0x62]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.H = state.D;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV H,E
		{
			m_InstructionSet[0x63]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.H = state.E;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV H,H
		{
			m_InstructionSet[0x64]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV H,L
		{
			m_InstructionSet[0x65]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.H = state.L;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV H,M
		{
			m_InstructionSet[0x66]->exec = [](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (state.H << 8) + state.L;
				state.H = map.Peek(adr);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV H,A
		{
			m_InstructionSet[0x67]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.H = state.A;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV L,B
		{
			m_InstructionSet[0x68]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.L = state.B;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV L,C
		{
			m_InstructionSet[0x69]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.L = state.C;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV L,D
		{
			m_InstructionSet[0x6A]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.L = state.D;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV L,E
		{
			m_InstructionSet[0x6B]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.L = state.E;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV L,H
		{
			m_InstructionSet[0x6C]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.L = state.H;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV L,L
		{
			m_InstructionSet[0x6D]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV L,M
		{
			m_InstructionSet[0x6E]->exec = [](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (state.H << 8) + state.L;
				state.L = map.Peek(adr);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV L,A
		{
			m_InstructionSet[0x6F]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.L = state.A;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV M,B
		{
			m_InstructionSet[0x70]->exec = [](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (state.H << 8) + state.L;
				map.Poke(adr, state.B);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV M,C
		{
			m_InstructionSet[0x71]->exec = [](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (state.H << 8) + state.L;
				map.Poke(adr, state.C);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV M,D
		{
			m_InstructionSet[0x72]->exec = [](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (state.H << 8) + state.L;
				map.Poke(adr, state.D);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV M,E
		{
			m_InstructionSet[0x73]->exec = [](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (state.H << 8) + state.L;
				map.Poke(adr, state.E);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV M,H
		{
			m_InstructionSet[0x74]->exec = [](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (state.H << 8) + state.L;
				map.Poke(adr, state.H);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV M,L
		{
			m_InstructionSet[0x75]->exec = [](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (state.H << 8) + state.L;
				map.Poke(adr, state.H);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// HLT
		{
			m_InstructionSet[0x76]->exec = [](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
			{
				state.HLT = true;
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

		// MOV A,B
		{
			m_InstructionSet[0x78]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.A = state.B;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV A,C
		{
			m_InstructionSet[0x79]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.A = state.C;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV A,D
		{
			m_InstructionSet[0x7A]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.A = state.D;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV A,E
		{
			m_InstructionSet[0x7B]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.A = state.E;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV A,H
		{
			m_InstructionSet[0x7C]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.A = state.H;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV A,L
		{
			m_InstructionSet[0x7D]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.A = state.L;
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV A,M
		{
			m_InstructionSet[0x7E]->exec = [](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (state.H << 8) + state.L;
				state.A = map.Peek(adr);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// MOV A,A
		{
			m_InstructionSet[0x7F]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// ADD B
		{
			m_InstructionSet[0x80]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				ADD(state, state.B);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// ADD C
		{
			m_InstructionSet[0x81]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				ADD(state, state.C);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// ADD D
		{
			m_InstructionSet[0x82]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				ADD(state, state.D);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// ADD E
		{
			m_InstructionSet[0x83]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				ADD(state, state.E);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// ADD H
		{
			m_InstructionSet[0x84]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				ADD(state, state.H);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// ADD L
		{
			m_InstructionSet[0x85]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				ADD(state, state.L);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// ADD M
		{
			m_InstructionSet[0x86]->exec = [](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (state.H << 8) + state.L;
				ADD(state, map.Peek(adr));
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// ADD A
		{
			m_InstructionSet[0x87]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				ADD(state, state.A);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// ADC B
		{
			m_InstructionSet[0x88]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				ADC(state, state.B);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// ADC C
		{
			m_InstructionSet[0x89]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				ADC(state, state.C);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// ADC D
		{
			m_InstructionSet[0x8A]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				ADC(state, state.D);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// ADC E
		{
			m_InstructionSet[0x8B]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				ADC(state, state.E);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// ADC H
		{
			m_InstructionSet[0x8C]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				ADC(state, state.H);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// ADC L
		{
			m_InstructionSet[0x8D]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				ADC(state, state.L);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// ADC M
		{
			m_InstructionSet[0x8E]->exec = [](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (state.H << 8) + state.L;
				ADC(state, map.Peek(adr));
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// ADC A
		{
			m_InstructionSet[0x8F]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				ADC(state, state.A);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// SUB B
		{
			m_InstructionSet[0x90]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				SUB(state, state.B);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// SUB C
		{
			m_InstructionSet[0x91]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				SUB(state, state.C);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// SUB D
		{
			m_InstructionSet[0x92]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				SUB(state, state.D);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// SUB E
		{
			m_InstructionSet[0x93]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				SUB(state, state.E);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// SUB H
		{
			m_InstructionSet[0x94]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				SUB(state, state.H);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// SUB L
		{
			m_InstructionSet[0x95]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				SUB(state, state.L);
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// SUB M
		{
			m_InstructionSet[0x96]->exec = [](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				uint16_t adr = (state.H << 8) + state.L;
				SUB(state, map.Peek(adr));
				state.PC += size;
				state.Steps++;
				state.Cycles += cycle.A;
			};
		}

		// SUB A
		{
			m_InstructionSet[0x97]->exec = [](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
			{
				SUB(state, state.A);
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

void InstructionSet::DAD(State& state, uint8_t& rh, uint8_t& rl)
{
	/*
	DAD P (Decrement register pair)
	0 0 p p 1 0 0 1						HL<-HL + P Data in register pair P is added to HL
										flags affected: CY (from higher byte)
	*/

	uint16_t p = ((uint16_t)rh) << 8 | ((uint16_t)rl);
	uint16_t hl = ((uint16_t)state.H) << 8 | ((uint16_t)state.L);

	uint32_t value = ((uint32_t)p)+ ((uint32_t)hl);

	// CARRY BIT
	state.CY = value & 0b10000000000000000;
	state.H = (value & 0b1111111100000000) >> 8;
	state.L = value & 0b0000000011111111;
}

void InstructionSet::DAA(State& state)
{
	/*/
	DAA(Decimal adjust Accumulator)
	0	0	1	0	0	1	1	1		A<-adjustBCD(A) Data in A is adjusted as packed BCD :
										if (a3…a0) > 9 or AC = 1 then(a3…a0)<-(a3…a0) + 6
										if (a7…a4) > 9 or CY = 1 then(a7…a4)<-(a7…a4) + 6
										explanation: 6 is the 4 - bit U2 code of - 10
										flags affected : Z, S, P, CY, AC
	*/
	uint8_t a3_0 = state.A & 0b00001111;
	uint8_t a7_4 = (state.A & 0b11110000) >> 4;

	if (a3_0 > 9 || state.AC)
	{
		a3_0 += 6;
	}
	
	if (a3_0 & 0b00010000)
	{
		state.AC = true;
		a3_0 &= 0b00001111;
		a7_4++;
	}

	if (a7_4 > 9 || state.CY)
	{
		a7_4 += 6;
	}

	if (a7_4 & 0b00010000)
	{
		state.CY = true;
		a7_4 &= 0b00001111;
	}

	state.A = (a7_4 << 4) | a3_0;

	state.Z = state.A == 0;

	state.S = state.A & 0x80;

	state.P = Utilities::isOddParity(state.A);
}

void InstructionSet::ADD(State& state, const uint8_t value)
{
	/*
	ADD R (Add register)
	1	0	0	0	0	r	r	r		A <-A + R data from R is added to data in A
										flags affected: Z, S, P, CY, AC
	*/
	// Perform operation in higher precision
	uint16_t a = ((uint16_t)state.A);
	uint16_t b = ((uint16_t)value);
	uint16_t chp = a + b;

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
	uint8_t c_ = a_ + b_;
	state.AC = c_ & 0x10;

	// change F 
	state.setF();
}

void InstructionSet::ADC(State& state, const uint8_t value)
{
	/*
	ADC R (Add register with carry)
	1	0	0	0	1	r	r	r		A <- A + R + CY data from R and CY flag are added to A
										flags affected: Z, S, P, CY, AC
	*/
	// Perform operation in higher precision
	uint16_t a = ((uint16_t)state.A);
	uint16_t b = ((uint16_t)value) + state.CY;
	uint16_t chp = a + b;

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
	uint8_t c_ = a_ + b_;
	state.AC = c_ & 0x10;

	// change F 
	state.setF();
}

void InstructionSet::SUB(State& state, const uint8_t value)
{
	/*
	SUB R (Subtract register)
	1	0	0	1	0	r	r	r		A <- A - R data from R is subtracted from A
										flags affected: Z, S, P, CY, AC
	*/
	// Perform operation in higher precision
	uint16_t a = ((uint16_t)state.A);
	uint16_t b = ((uint16_t)(uint8_t(-value))) ;
	uint16_t chp = a + b;

	// get result in normal precision
	state.A = chp & 0xFF;

	// ZERO CHECK
	state.Z = state.A == 0;

	// SIGN CHECK
	state.S = chp & 0x80;

	// PARITY CHECK
	state.P = Utilities::isOddParity(state.A);

	// CARRY BIT
	state.CY = !(chp & 0x100);

	// AUXILIARY CARRY
	uint8_t a_ = a & 0x0F;
	uint8_t b_ = b & 0x0F;
	uint8_t c_ = a_ + b_;
	state.AC = c_ & 0x10;

	// change F 
	state.setF();
}