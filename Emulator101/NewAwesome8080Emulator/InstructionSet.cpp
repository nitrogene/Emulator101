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

		this->setInstructions();
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

void InstructionSet::setInstruction(const uint8_t adr, std::function<void(State&, MemoryMap&, const uint8_t*, const uint16_t, const ::ClockCycle&)> exec)
{
	if (m_InstructionSet[adr]->exec != nullptr)
	{
		throw std::exception("already set");
	}

	m_InstructionSet[adr]->exec = exec;
}


void InstructionSet::setInstructions()
{

	// TODO: how to deduce following from the instruction set file ?
	// 
	// NOP
	setInstruction(
		0x00,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// LXI B,D16
	setInstruction(
		0x01,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.B = opCode[2];
			state.C = opCode[1];
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// STAX B
	setInstruction(
		0x02,
		[](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.B << 8) + state.C;
			map.Poke(adr, state.A);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// INX B
	setInstruction(
		0x03,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t value = (state.B << 8) + state.C;
			value++;
			state.B = (value & 0b1111111100000000) >> 8;
			state.C = value & 0b0000000011111111;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// INR B
	setInstruction(
		0x04,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::INR(state, state.B);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);
	

	// DCR B
	setInstruction(
		0x05,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::DCR(state, state.B);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MVI B, D8
	setInstruction(
		0x06,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.B = opCode[1];
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// RLC
	setInstruction(
		0x07,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::RLC(state);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// DAD B
	setInstruction(
		0x09,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::DAD(state, state.B, state.C);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// LDAX B
	setInstruction(
		0x0A,
		[](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.B << 8) + state.C;
			state.A = map.Peek(adr);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// DCX B
	setInstruction(
		0x0B,
		[](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t value = (state.B << 8) + state.C;
			value--;
			state.B = (value & 0b1111111100000000) >> 8;
			state.C = value & 0b0000000011111111;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// INR C
	setInstruction(
		0x0c,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::INR(state, state.C);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// DCR C
	setInstruction(
		0x0d,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::DCR(state, state.C);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MVI C, D8
	setInstruction(
		0x0E,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.C = opCode[1];
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// RRC
	setInstruction(
		0x0F,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::RRC(state);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// LXI D,16
	setInstruction(
		0x11,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.D = opCode[2];
			state.E = opCode[1];
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// STAX D
	setInstruction(
		0x12,
		[](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.D << 8) + state.E;
			map.Poke(adr, state.A);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// INX D
	setInstruction(
		0x13,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t value = (state.D << 8) + state.E;
			value++;
			state.D = (value & 0b1111111100000000) >> 8;
			state.E = value & 0b0000000011111111;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// INR D
	setInstruction(
		0x14,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::INR(state, state.D);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// DCR D
	setInstruction(
		0x15,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::DCR(state, state.D);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MVI D, D8
	setInstruction(
		0x16,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.D = opCode[1];
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// RAL
	setInstruction(
		0x17,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::RAL(state);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// DAD D
	setInstruction(
		0x19,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::DAD(state, state.D, state.E);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// LDAX D
	setInstruction(
		0x1A,
		[](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.D << 8) + state.E;
			state.A = map.Peek(adr);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// DCX D
	setInstruction(
		0x1B,
		[](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t value = (state.D << 8) + state.E;
			value--;
			state.D = (value & 0b1111111100000000) >> 8;
			state.E = value & 0b0000000011111111;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// INR E
	setInstruction(
		0x1C,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::INR(state, state.E);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// DCR E
	setInstruction(
		0x1D,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::DCR(state, state.E);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);
	

	// MVI E, D8
	setInstruction(
		0x1E,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.E = opCode[1];
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// RAR
	setInstruction(
		0x1F,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::RAR(state);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// LXI D,16
	setInstruction(
		0x21,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.H = opCode[2];
			state.L = opCode[1];
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// SHLD adr
	setInstruction(
		0x22,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (opCode[2] << 8) + opCode[1];
			map.Poke(adr, state.L);
			map.Poke(adr + 1, state.H);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// INX H
	setInstruction(
		0x23,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t value = (state.H << 8) + state.L;
			value++;
			state.H = (value & 0b1111111100000000) >> 8;
			state.L = value & 0b0000000011111111;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// INR H
	setInstruction(
		0x24,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::INR(state, state.H);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// DCR H
	setInstruction(
		0x25,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::DCR(state, state.H);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MVI H, D8
	setInstruction(
		0x26,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.H = opCode[1];
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// DAA
	setInstruction(
		0x27,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::DAA(state);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// DAD H
	setInstruction(
		0x29,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::DAD(state, state.H, state.L);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// LHLD adr
	setInstruction(
		0x2A,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			auto adr = (opCode[2] << 8) + opCode[1];
			state.L = map.Peek(adr);
			state.H = map.Peek(adr + 1);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// DCX H
	setInstruction(
		0x2B,
		[](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t value = (state.H << 8) + state.L;
			value--;
			state.H = (value & 0b1111111100000000) >> 8;
			state.L = value & 0b0000000011111111;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// INR L
	setInstruction(
		0x2C,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::INR(state, state.L);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// DCR L
	setInstruction(
		0x2D,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::DCR(state, state.L);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MVI L, D8
	setInstruction(
		0x2E,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.L = opCode[1];
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// CMA
	setInstruction(
		0x2F,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.A = ~state.A;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// LXI SP,D16
	setInstruction(
		0x31,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.SP = (opCode[2] << 8) + opCode[1];
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// STA adr
	setInstruction(
		0x32,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			auto adr = (opCode[2] << 8) + opCode[1];
			map.Poke(adr, state.A);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// INX SP
	setInstruction(
		0x33,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			state.SP += 1;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// INR M
	setInstruction(
		0x34,
		[](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			auto value = map.Peek(adr);
			Utilities::INR(state, value);
			map.Poke(adr, value);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// DCR M
	setInstruction(
		0x35,
		[](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			auto value = map.Peek(adr);
			Utilities::DCR(state, value);
			map.Poke(adr, value);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MVI M, D8
	setInstruction(
		0x36,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			map.Poke(adr, opCode[1]);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// STC
	setInstruction(
		0x37,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.Flags.Carry = true;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// DAD SP
	setInstruction(
		0x39,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			uint8_t b = (state.SP & 0b1111111100000000) >> 8;
			uint8_t c = state.SP & 0b0000000011111111;

			Utilities::DAD(state, b, c);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// LDA adr
	setInstruction(
		0x3A,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (opCode[2] << 8) + opCode[1];
			state.A = map.Peek(adr);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// DCX SP
	setInstruction(
		0x3B,
		[](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			state.SP--;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// INR A
	setInstruction(
		0x3C,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::INR(state, state.A);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// DCR A
	setInstruction(
		0x3D,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::DCR(state, state.A);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MVI A, D8
	setInstruction(
		0x3E,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.A = opCode[1];
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// CMC
	setInstruction(
		0x3F,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.Flags.Carry = !state.Flags.Carry;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV B,B
	setInstruction(
		0x40,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV B,C
	setInstruction(
		0x41,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.B = state.C;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV B,D
	setInstruction(
		0x42,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.B = state.D;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV B,E
	setInstruction(
		0x43,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.B = state.E;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV B,H
	setInstruction(
		0x44,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.B = state.H;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV B,L
	setInstruction(
		0x45,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.B = state.L;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV B,M
	setInstruction(
		0x46,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			state.B = map.Peek(adr);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV B,A
	setInstruction(
		0x47,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.B = state.A;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV C,B
	setInstruction(
		0x48,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.C = state.B;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV C,C
	setInstruction(
		0x49,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV C,D
	setInstruction(
		0x4A,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.C = state.D;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV C,E
	setInstruction(
		0x4B,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.C = state.E;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV C,H
	setInstruction(
		0x4C,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.C = state.H;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV C,L
	setInstruction(
		0x4D,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.C = state.L;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV C,M
	setInstruction(
		0x4E,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			state.C = map.Peek(adr);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV C,A
	setInstruction(
		0x4F,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.C = state.A;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV D,B
	setInstruction(
		0x50,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.D = state.B;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV D,C
	setInstruction(
		0x51,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.D = state.C;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV D,D
	setInstruction(
		0x52,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV D,E
	setInstruction(
		0x53,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.D = state.E;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV D,H
	setInstruction(
		0x54,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.D = state.H;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV D,L
	setInstruction(
		0x55,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.D = state.L;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV D,M
	setInstruction(
		0x56,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			state.D = map.Peek(adr);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV D,A
	setInstruction(
		0x57,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.D = state.A;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV E,B
	setInstruction(
		0x58,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.E = state.B;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV E,C
	setInstruction(
		0x59,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.E = state.C;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV E,D
	setInstruction(
		0x5A,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.E = state.D;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV E,E
	setInstruction(
		0x5B,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV E,H
	setInstruction(
		0x5C,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.E = state.H;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV E,L
	setInstruction(
		0x5D,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.E = state.L;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV E,M
	setInstruction(
		0x5E,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			state.E = map.Peek(adr);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV E,A
	setInstruction(
		0x5F,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.E = state.F;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV H,B
	setInstruction(
		0x60,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.H = state.B;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV H,C
	setInstruction(
		0x61,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.H = state.C;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV H,D
	setInstruction(
		0x62,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.H = state.D;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV H,E
	setInstruction(
		0x63,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.H = state.E;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV H,H
	setInstruction(
		0x64,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV H,L
	setInstruction(
		0x65,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.H = state.L;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV H,M
	setInstruction(
		0x66,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			state.H = map.Peek(adr);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV H,A
	setInstruction(
		0x67,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.H = state.A;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV L,B
	setInstruction(
		0x68,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.L = state.B;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV L,C
	setInstruction(
		0x69,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.L = state.C;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV L,D
	setInstruction(
		0x6A,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.L = state.D;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV L,E
	setInstruction(
		0x6B,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.L = state.E;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV L,H
	setInstruction(
		0x6C,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.L = state.H;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV L,L
	setInstruction(
		0x6D,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV L,M
	setInstruction(
		0x6E,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			state.L = map.Peek(adr);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV L,A
	setInstruction(
		0x6F,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.L = state.A;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV M,B
	setInstruction(
		0x70,
		[](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			map.Poke(adr, state.B);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV M,C
	setInstruction(
		0x71,
		[](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			map.Poke(adr, state.C);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV M,D
	setInstruction(
		0x72,
		[](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			map.Poke(adr, state.D);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV M,E
	setInstruction(
		0x73,
		[](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			map.Poke(adr, state.E);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV M,H
	setInstruction(
		0x74,
		[](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			map.Poke(adr, state.H);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV M,L
	setInstruction(
		0x75,
		[](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			map.Poke(adr, state.H);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// HLT
	setInstruction(
		0x76,
		[](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			state.HLT = true;
		}
	);

	// MOV M,A
	setInstruction(
		0x77,
		[](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			map.Poke(adr, state.A);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV A,B
	setInstruction(
		0x78,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.A = state.B;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV A,C
	setInstruction(
		0x79,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.A = state.C;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV A,D
	setInstruction(
		0x7A,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.A = state.D;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV A,E
	setInstruction(
		0x7B,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.A = state.E;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV A,H
	setInstruction(
		0x7C,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.A = state.H;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV A,L
	setInstruction(
		0x7D,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.A = state.L;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV A,M
	setInstruction(
		0x7E,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			state.A = map.Peek(adr);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// MOV A,A
	setInstruction(
		0x7F,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ADD B
	setInstruction(
		0x80,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ADD(state, state.B);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ADD C
	setInstruction(
		0x81,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ADD(state, state.C);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ADD D
	setInstruction(
		0x82,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ADD(state, state.D);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ADD E
	setInstruction(
		0x83,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ADD(state, state.E);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ADD H
	setInstruction(
		0x84,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ADD(state, state.H);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ADD L
	setInstruction(
		0x85,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ADD(state, state.L);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ADD M
	setInstruction(
		0x86,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			Utilities::ADD(state, map.Peek(adr));
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ADD A
	setInstruction(
		0x87,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ADD(state, state.A);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ADC B
	setInstruction(
		0x88,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ADC(state, state.B);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ADC C
	setInstruction(
		0x89,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ADC(state, state.C);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ADC D
	setInstruction(
		0x8A,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ADC(state, state.D);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ADC E
	setInstruction(
		0x8B,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ADC(state, state.E);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ADC H
	setInstruction(
		0x8C,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ADC(state, state.H);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ADC L
	setInstruction(
		0x8D,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ADC(state, state.L);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ADC M
	setInstruction(
		0x8E,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			Utilities::ADC(state, map.Peek(adr));
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ADC A
	setInstruction(
		0x8F,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ADC(state, state.A);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// SUB B
	setInstruction(
		0x90,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::SUB(state, state.B);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// SUB C
	setInstruction(
		0x91,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::SUB(state, state.C);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// SUB D
	setInstruction(
		0x92,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::SUB(state, state.D);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// SUB E
	setInstruction(
		0x93,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::SUB(state, state.E);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// SUB H
	setInstruction(
		0x94,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::SUB(state, state.H);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// SUB L
	setInstruction(
		0x95,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::SUB(state, state.L);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// SUB M
	setInstruction(
		0x96,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			Utilities::SUB(state, map.Peek(adr));
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// SUB A
	setInstruction(
		0x97,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::SUB(state, state.A);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// SBB B
	setInstruction(
		0x98,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::SBB(state, state.B);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// SBB C
	setInstruction(
		0x99,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::SBB(state, state.C);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// SBB D
	setInstruction(
		0x9A,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::SBB(state, state.D);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// SBB E
	setInstruction(
		0x9B,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::SBB(state, state.E);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// SBB H
	setInstruction(
		0x9C,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::SBB(state, state.H);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// SBB L
	setInstruction(
		0x9D,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::SBB(state, state.L);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// SBB M
	setInstruction(
		0x9E,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			Utilities::SBB(state, map.Peek(adr));
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// SBB A
	setInstruction(
		0x9F,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::SBB(state, state.A);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ANA B
	setInstruction(
		0xA0,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ANA(state, state.B);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ANA C
	setInstruction(
		0xA1, [](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ANA(state, state.C);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ANA D
	setInstruction(
		0xA2,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ANA(state, state.D);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ANA E
	setInstruction(
		0xA3,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ANA(state, state.E);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ANA H
	setInstruction(
		0xA4,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ANA(state, state.H);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ANA L
	setInstruction(
		0xA5,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ANA(state, state.L);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ANA M
	setInstruction(
		0xA6,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			Utilities::ANA(state, map.Peek(adr));
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ANA A
	setInstruction(
		0xA7,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ANA(state, state.A);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// XRA B
	setInstruction(
		0xA8,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::XRA(state, state.B);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// XRA C
	setInstruction(
		0xA9,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::XRA(state, state.C);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// XRA D
	setInstruction(
		0xAA,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::XRA(state, state.D);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// XRA E
	
	setInstruction(
		0xAB,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::XRA(state, state.E);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// XRA H
	setInstruction(
		0xAC,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::XRA(state, state.H);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// XRA L
	setInstruction(
		0xAD,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::XRA(state, state.L);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// XRA M
	setInstruction(
		0xAE,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			Utilities::XRA(state, map.Peek(adr));
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// XRA A
	setInstruction(
		0xAF,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::XRA(state, state.A);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ORA B
	setInstruction(
		0xB0,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ORA(state, state.B);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ORA C
	setInstruction(
		0xB1,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ORA(state, state.C);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ORA D
	setInstruction(
		0xB2,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ORA(state, state.D);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ORA E
	setInstruction(
		0xB3,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ORA(state, state.E);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ORA H
	setInstruction(
		0xB4,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ORA(state, state.H);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ORA L
	setInstruction(
		0xB5,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ORA(state, state.L);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ORA M
	setInstruction(
		0xB6,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			Utilities::ORA(state, map.Peek(adr));
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ORA A
	setInstruction(
		0xB7,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ORA(state, state.A);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// CMP B
	setInstruction(
		0xB8,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::CMP(state, state.B);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// CMP C
	setInstruction(
		0xB9,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::CMP(state, state.C);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// CMP D
	setInstruction(
		0xBA,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::CMP(state, state.D);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// CMP E
	setInstruction(
		0xBB,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::CMP(state, state.E);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// CMP H
	setInstruction(
		0xBC,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::CMP(state, state.H);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// CMP L
	setInstruction(
		0xBD,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::CMP(state, state.L);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// CMP M
	setInstruction(
		0xBE,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			uint16_t adr = (state.H << 8) + state.L;
			Utilities::CMP(state, map.Peek(adr));
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// CMP A
	setInstruction(
		0xBF,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::CMP(state, state.A);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// RNZ
	setInstruction(
		0xC0,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (state.Flags.Zero)
			{
				state.PC += size;
			}
			else
			{
				auto spc = map.Peek(state.SP);
				auto spc1 = map.Peek(state.SP + 1);
				state.PC = (spc1 << 8) + spc;
				state.SP += 2;
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// POP B
	setInstruction(
		0xC1,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.B = map.Peek(state.SP + 1);
			state.C = map.Peek(state.SP);
			state.SP += 2;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// JNZ adr
	setInstruction(
		0xC2,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (state.Flags.Zero)
			{
				state.PC += size;
			}
			else
			{
				state.PC = (opCode[2] << 8) + opCode[1];
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// JMP adr
	setInstruction(
		0xC3,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.PC = (opCode[2] << 8) + opCode[1];
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// CNZ adr
	setInstruction(
		0xC4,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (!state.Flags.Zero)
			{
				map.Poke(state.SP - 1, (state.PC & 0xFF00) >> 8);
				map.Poke(state.SP - 2, state.PC & 0x00FF);
				state.SP -= 2;
				state.PC = (opCode[2] << 8) + opCode[1];
			}
			else
			{
				state.PC += size;
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// PUSH B
	setInstruction(
		0xC5,
		[](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			map.Poke(state.SP - 1, state.B);
			map.Poke(state.SP - 2, state.C);
			state.PC += size;
			state.SP -= 2;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ADI D8
	setInstruction(
		0xC6,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ADI(state, opCode[1]);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// RZ
	setInstruction(
		0xC8,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (!state.Flags.Zero)
			{
				state.PC += size;
			}
			else
			{
				auto spc = map.Peek(state.SP);
				auto spc1 = map.Peek(state.SP + 1);
				state.PC = (spc1 << 8) + spc;
				state.SP += 2;
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// RET
	setInstruction(
		0xC9,
		[](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			auto spc = map.Peek(state.SP);
			auto spc1 = map.Peek(state.SP + 1);
			state.PC = (spc1 << 8) + spc;
			state.SP += 2;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// JZ adr
	setInstruction(
		0xCA,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (state.Flags.Zero)
			{
				state.PC = (opCode[2] << 8) + opCode[1];
			}
			else
			{
				state.PC += size;
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// CZ adr
	setInstruction(
		0xCC,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (state.Flags.Zero)
			{
				map.Poke(state.SP - 1, (state.PC & 0xFF00) >> 8);
				map.Poke(state.SP - 2, state.PC & 0x00FF);
				state.SP -= 2;
				state.PC = (opCode[2] << 8) + opCode[1];
			}
			else
			{
				state.PC += size;
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// CALL adr
	setInstruction(
		0xCD,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.PC += 3;
			map.Poke(state.SP - 1, (state.PC & 0xFF00) >> 8);
			map.Poke(state.SP - 2, state.PC & 0x00FF);
			state.SP -= 2;
			state.PC = (opCode[2] << 8) + opCode[1];
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ACI D8
	setInstruction(
		0xCE,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ACI(state, opCode[1]);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// RNC
	setInstruction(
		0xD0,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (state.Flags.Carry)
			{
				state.PC += size;
			}
			else
			{
				auto spc = map.Peek(state.SP);
				auto spc1 = map.Peek(state.SP + 1);
				state.PC = (spc1 << 8) + spc;
				state.SP += 2;
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// POP D
	setInstruction(
		0xD1,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.D = map.Peek(state.SP + 1);
			state.E = map.Peek(state.SP);
			state.SP += 2;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// JNC adr
	setInstruction(
		0xD2,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (state.Flags.Carry)
			{
				state.PC += size;
			}
			else
			{
				state.PC = (opCode[2] << 8) + opCode[1];
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// OUT adr
	setInstruction(
		0xD3,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::machineOUT(state, opCode[1]);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// CNC adr
	setInstruction(
		0xD4,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (!state.Flags.Carry)
			{
				map.Poke(state.SP - 1, (state.PC & 0xFF00) >> 8);
				map.Poke(state.SP - 2, state.PC & 0x00FF);
				state.SP -= 2;
				state.PC = (opCode[2] << 8) + opCode[1];
			}
			else
			{
				state.PC += size;
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// PUSH D
	setInstruction(
		0xD5,
		[](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			map.Poke(state.SP - 1, state.D);
			map.Poke(state.SP - 2, state.E);
			state.PC += size;
			state.SP -= 2;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// SUI D8
	setInstruction(
		0xD6,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::SUI(state, opCode[1]);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// RC
	setInstruction(
		0xD8,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (!state.Flags.Carry)
			{
				state.PC += size;
			}
			else
			{
				auto spc = map.Peek(state.SP);
				auto spc1 = map.Peek(state.SP + 1);
				state.PC = (spc1 << 8) + spc;
				state.SP += 2;
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// JC adr
	setInstruction(
		0xDA,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (state.Flags.Carry)
			{
				state.PC = (opCode[2] << 8) + opCode[1];
			}
			else
			{
				state.PC += size;
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// IN adr
	setInstruction(
		0xDB,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::machineIN(state, opCode[1]);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// CC adr
	setInstruction(
		0xDC,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (state.Flags.Carry)
			{
				map.Poke(state.SP - 1, (state.PC & 0xFF00) >> 8);
				map.Poke(state.SP - 2, state.PC & 0x00FF);
				state.SP -= 2;
				state.PC = (opCode[2] << 8) + opCode[1];
			}
			else
			{
				state.PC += size;
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// SBI D8
	setInstruction(
		0xDE,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::SBI(state, opCode[1]);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// XCHG
	setInstruction(
		0xEB,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			std::swap(state.H, state.D);
			std::swap(state.L, state.E);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// RPO
	setInstruction(
		0xE0,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (state.Flags.Parity)
			{
				state.PC += size;
			}
			else
			{
				auto spc = map.Peek(state.SP);
				auto spc1 = map.Peek(state.SP + 1);
				state.PC = (spc1 << 8) + spc;
				state.SP += 2;
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// POP H
	setInstruction(
		0xE1,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.H = map.Peek(state.SP + 1);
			state.L = map.Peek(state.SP);
			state.SP += 2;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// JPO adr
	setInstruction(
		0xE2,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (state.Flags.Parity)
			{
				state.PC += size;
			}
			else
			{
				state.PC = (opCode[2] << 8) + opCode[1];
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// XTHL
	setInstruction(
		0xE3,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			auto value = state.L;
			state.L = map.Peek(state.SP);
			map.Poke(state.SP, value);

			value = state.H;
			state.H = map.Peek(state.SP+1);
			map.Poke(state.SP+1, value);

			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// CPO adr
	setInstruction(
		0xE4,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (state.Flags.Parity)
			{
				map.Poke(state.SP - 1, (state.PC & 0xFF00) >> 8);
				map.Poke(state.SP - 2, state.PC & 0x00FF);
				state.SP -= 2;
				state.PC = (opCode[2] << 8) + opCode[1];
			}
			else
			{
				state.PC += size;
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// PUSH H
	setInstruction(
		0xE5,
		[](State& state, MemoryMap& map, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			map.Poke(state.SP - 1, state.H);
			map.Poke(state.SP - 2, state.L);
			state.PC += size;
			state.SP -= 2;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ANI D8
	setInstruction(
		0xE6,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ANI(state, opCode[1]);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// RPE
	setInstruction(
		0xE8,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (!state.Flags.Parity)
			{
				state.PC += size;
			}
			else
			{
				auto spc = map.Peek(state.SP);
				auto spc1 = map.Peek(state.SP + 1);
				state.PC = (spc1 << 8) + spc;
				state.SP += 2;
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// PCHL
	setInstruction(
		0xE9,
		[](State& state, MemoryMap&, const uint8_t*, const uint16_t size, const ClockCycle& cycle)
		{
			state.PC = (state.H << 8) + state.L;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// JPE adr
	setInstruction(
		0xEA,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (state.Flags.Parity)
			{
				state.PC = (opCode[2] << 8) + opCode[1];
			}
			else
			{
				state.PC += size;
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// CPE adr
	setInstruction(
		0xEC,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (!state.Flags.Parity)
			{
				map.Poke(state.SP - 1, (state.PC & 0xFF00) >> 8);
				map.Poke(state.SP - 2, state.PC & 0x00FF);
				state.SP -= 2;
				state.PC = (opCode[2] << 8) + opCode[1];
			}
			else
			{
				state.PC += size;
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// XRI D8
	setInstruction(
		0xEE,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::XRI(state, opCode[1]);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// RP
	setInstruction(
		0xF0,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (state.Flags.Sign)
			{
				state.PC += size;
			}
			else
			{
				auto spc = map.Peek(state.SP);
				auto spc1 = map.Peek(state.SP + 1);
				state.PC = (spc1 << 8) + spc;
				state.SP += 2;
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// POP PSW
	setInstruction(
		0xF1,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.A = map.Peek(state.SP + 1);
			state.F = map.Peek(state.SP);
			state.Flags.setF(state.F);
			state.SP += 2;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// JP adr
	setInstruction(
		0xF2,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (state.Flags.Sign)
			{
				state.PC += size;
			}
			else
			{
				state.PC = (opCode[2] << 8) + opCode[1];
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// DI
	setInstruction(
		0xF3,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.EI = false;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// CP adr
	setInstruction(
		0xF4,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (!state.Flags.Sign)
			{
				map.Poke(state.SP - 1, (state.PC & 0xFF00) >> 8);
				map.Poke(state.SP - 2, state.PC & 0x00FF);
				state.SP -= 2;
				state.PC = (opCode[2] << 8) + opCode[1];
			}
			else
			{
				state.PC += size;
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// PUSH PSW
	setInstruction(
		0xF5,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			map.Poke(state.SP - 1, state.A);
			map.Poke(state.SP - 2, state.F);
			state.PC += size;
			state.SP -= 2;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// ORI D8
	setInstruction(
		0xF6,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::ORI(state, opCode[1]);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// RM
	setInstruction(
		0xF8,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (!state.Flags.Sign)
			{
				state.PC += size;
			}
			else
			{
				auto spc = map.Peek(state.SP);
				auto spc1 = map.Peek(state.SP + 1);
				state.PC = (spc1 << 8) + spc;
				state.SP += 2;
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// SPHL
	setInstruction(
		0xF9,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.SP = (state.H << 8) + state.L;
			state.PC += 1;
			state.Cycles += 5;
			state.Steps++;
		}
	);

	// JM adr
	setInstruction(
		0xFA,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (state.Flags.Sign)
			{
				state.PC = (opCode[2] << 8) + opCode[1];
			}
			else
			{
				state.PC += size;
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// EI
	setInstruction(
		0xFB,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			state.EI = true;
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// CM adr
	setInstruction(
		0xFC,
		[](State& state, MemoryMap& map, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			if (state.Flags.Sign)
			{
				map.Poke(state.SP - 1, (state.PC & 0xFF00) >> 8);
				map.Poke(state.SP - 2, state.PC & 0x00FF);
				state.SP -= 2;
				state.PC = (opCode[2] << 8) + opCode[1];
			}
			else
			{
				state.PC += size;
			}
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);

	// CPI D8
	setInstruction(
		0xFE,
		[](State& state, MemoryMap&, const uint8_t* opCode, const uint16_t size, const ClockCycle& cycle)
		{
			Utilities::CPI(state, opCode[1]);
			state.PC += size;
			state.Steps++;
			state.Cycles += cycle.A;
		}
	);
}