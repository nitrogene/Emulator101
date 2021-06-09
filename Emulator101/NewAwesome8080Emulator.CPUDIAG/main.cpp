#include <iostream>
#include "Processor.h"

const static std::vector<std::filesystem::path> roms
{
	"CPUDIAG.COM"
};

const static std::filesystem::path instructions = "instructions.set";


int main(int /*argc*/, char** /*argv*/)
{
	// This roms should start at 0x100, thus we need empty NOP vector
	// and settinf PC to 0x100.
	auto bytes = std::vector<uint8_t>(0x100, 0x0);

	auto processor = std::make_shared<Processor>(instructions);

	processor->Initialize(roms, 0xFFFF, 0x2000, 0x2400, 0x4000, bytes);

	auto& map = processor->getMemoryMap();

	//Fix the stack pointer from 0x6ad to 0x7ad    
	map.Poke(0x01AB+2, 0x7, true);

	// the original assembly code has a ORG 00100H, thus we set PC to 0x100
	processor->setPC(0x100);

	while (!processor->getState().HLT)
	{
		// as explained in http://emulator101.com/, CPUDIAG is a binary for 8080 processor and CP/M
		// running the binary without tweaks won't be usefull, because it is looking for CP/M instructions
		// at lower address $0005 to display test informations.
		// After each emulator step, we'll detect such a call
		/*
		Here is an excert os CPUDIAG assembled code:
				;***********************************************************************
				; MICROCOSM ASSOCIATES  8080/8085 CPU DIAGNOSTIC VERSION 1.0  (C) 1980
				;***********************************************************************
				;
				;DONATED TO THE "SIG/M" CP/M USER'S GROUP BY:
				;KELLY SMITH, MICROCOSM ASSOCIATES
				;3055 WACO AVENUE
				;SIMI VALLEY, CALIFORNIA, 93065
				;(805) 527-9321 (MODEM, CP/M-NET (TM))
				;(805) 527-0518 (VERBAL)
				;
				;
				;
				;
				;
				;

						ORG	00100H
				;
				;
				;
					JMP	CPU	;JUMP TO 8080 CPU DIAGNOSTIC
				;
				;
				;
					DB	'MICROCOSM ASSOCIATES 8080/8085 CPU DIAGNOSTIC'
					DB	' VERSION 1.0  (C) 1980'
				;
				;
				;
				BDOS	EQU	00005H	;BDOS ENTRY TO CP/M
				WBOOT	EQU	00000H	;RE-ENTRY TO CP/M WARM BOOT
		*/
		/*
		This is the print function:
				MSG:	PUSH	D	;EXILE D REG.
				XCHG				;SWAP H&L REGS. TO D&E REGS.
				MVI	C,9				;LET BDOS KNOW WE WANT TO SEND A MESSAGE
				CALL	BDOS
				POP	D				;BACK FROM EXILE
				RET

		You can find more information on what it does here https://www.seasip.info/Cpm/bdos.html
			BDOS function 9 (C_WRITESTR) - Output string
			Supported by: All versions
			Entered with C=9, DE=address of string.
			Display a string of ASCII characters, terminated with the $ character. Thus the string may not contain $ characters - so, for example, the VT52 cursor positioning command ESC Y y+32 x+32 will not be able to
			use row 4.
			Under CP/M 3 and above, the terminating character can be changed using BDOS function 110.
		*/

		//processor->ShowState(16);

		auto opCode = &processor->Peek(processor->getState().PC);
		auto& state = processor->getState();
		const auto& isl = processor->getIsl(opCode[0]);

		if (opCode[0] == 0xCD)
		{
			// Detect CALL	BDOS
			if (5 == ((opCode[2] << 8) | opCode[1]))
			{
				if (state.C == 9)
				{
					// C_WRITESTR
					std::string output;
					uint16_t adr = (state.D << 8) | state.E;
					auto str = (const char*)&processor->Peek(adr);
					while (*str != '$')
					{
						output += *str++;
					}
					std::cout << output;
				}
				else
				{
					throw std::exception("unexpected call");
				}

				// With real CP/M, there is a RET, let's just go to next instruction
				state.PC += isl.Size;
				continue;
			}
		}

		processor->RunStep();
	}

	return 0;
}