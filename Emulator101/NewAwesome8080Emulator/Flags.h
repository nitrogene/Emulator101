#pragma once
#include <stdint.h>


struct Flags
{
/*
If register pair PSW is specified, the first byte of information
saved holds the contents of the A register; the
second byte holds the settings of the five condition bits,
i.e., Carry, Zero, Sign, Parity, and Auxiliary Carry. The format
of this byte is:

| | | |A| | | | |
|S|Z|0|C|0|P|1|C|

S  State of Sign bit
Z  State of Zero bit
0  always 0
AC State of auxiliary Carry bit
0  always 0
P  State of Parity bit
1  always 1
C  State of Carry bit
*/
	bool Sign = false;
	bool Zero = false;
	bool AuxiliaryCarry = false;
	bool Parity = false;
	bool Carry = false;


	uint8_t getF();
};