#ifndef I8080_I8080_H_
#define I8080_I8080_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct i8080 {
  // memory + io interface
  uint8_t (*read_byte)(void*, uint16_t); // user function to read from memory
  void (*write_byte)(void*, uint16_t, uint8_t); // same for writing to memory
  uint8_t (*port_in)(void*, uint8_t); // user function to read from port
  void (*port_out)(void*, uint8_t, uint8_t); // same for writing to port
  void* userdata; // user custom pointer

  uint64_t cyc; // cycle count

  uint16_t pc, sp; // program counter, stack pointer
  uint8_t a, b, c, d, e, h, l; // registers
  // flags: sign, zero, half-carry, parity, carry, interrupt flip-flop
  bool sf : 1, zf : 1, hf : 1, pf : 1, cf : 1, iff : 1;
  bool halted : 1;

  bool interrupt_pending : 1;
  uint8_t interrupt_vector;
  uint8_t interrupt_delay;
} i8080;

void i8080_init(i8080* const c);
void i8080_step(i8080* const c);
void i8080_interrupt(i8080* const c, uint8_t opcode);
void i8080_debug_output(i8080* const c, bool print_disassembly);

void i8080_cmp(i8080* const c, uint8_t val);
void i8080_add(i8080* const c, uint8_t* const reg, uint8_t val, bool cy);
void i8080_ana(i8080* const c, uint8_t val);
uint8_t i8080_inr(i8080* const c, uint8_t val);
void i8080_rlc(i8080* const c);
void i8080_rrc(i8080* const c);
void i8080_ral(i8080* const c);
void i8080_rar(i8080* const c);
void i8080_dad(i8080* const c, uint16_t val);
uint8_t i8080_dcr(i8080* const c, uint8_t val);
void i8080_daa(i8080* const c);
void i8080_sub(i8080* const c, uint8_t* const reg, uint8_t val, bool cy);
void i8080_xra(i8080* const c, uint8_t val);
void i8080_ora(i8080* const c, uint8_t val);
#endif // I8080_I8080_H_
