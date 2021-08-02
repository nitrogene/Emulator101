#ifndef SPACEI_INVADERS_H
#define SPACEI_INVADERS_H

#include <string.h>
#include "i8080.h"

#define SCREEN_WIDTH 224
#define SCREEN_HEIGHT 256
#define FPS 60

#define CYCLES_PER_FRAME 2000000 / FPS // 2Mhz at 60 fps
#define HALF_CYCLES_PER_FRAME CYCLES_PER_FRAME / 2

#define VRAM_ADDR 0x2400

typedef struct invaders invaders;
struct invaders {
    i8080 cpu;
    uint8_t memory[0x10000];

    uint8_t next_interrupt;
    bool colored_screen;

    // SI-specific ports & shift registers that are used in IN/OUT opcodes
    uint8_t port1, port2;
    uint8_t shift0, shift1, shift_offset;
    uint8_t last_out_port3, last_out_port5;

    // screen pixel buffer
    uint8_t screen_buffer[SCREEN_HEIGHT][SCREEN_WIDTH][4];
    // function pointer provided by the user that will be called every time
    // the screen must be updated:
    void (*update_screen)(invaders* const si);
};

void invaders_init(invaders* const si);
void invaders_update(invaders* const si);
void invaders_gpu_update(invaders* const si);
void invaders_play_sound(invaders* const si, uint8_t bank);

// memory handling
uint8_t invaders_rb(void* userdata, uint16_t addr);
void invaders_wb(void* userdata, uint16_t addr, uint8_t val);
int invaders_load_rom(invaders* const si, const char* filename,
                      uint16_t start_addr);

#endif  // SPACEI_INVADERS_H
