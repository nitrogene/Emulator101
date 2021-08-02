#include "invaders.h"

void invaders_init(invaders* const si) {
    i8080_init(&si->cpu);
    si->cpu.read_byte = invaders_rb;
    si->cpu.write_byte = invaders_wb;
    si->cpu.userdata = si;

    memset(si->memory, 0, sizeof si->memory);
    memset(si->screen_buffer, 0, sizeof si->screen_buffer);
    si->next_interrupt = 0x08;
    si->port1 = 1 << 3; // bit 3 is always set
    si->port2 = 0;
    si->shift0 = 0;
    si->shift1 = 0;
    si->shift_offset = 0;
    si->last_out_port3 = 0;
    si->last_out_port5 = 0;
    si->colored_screen = true;
    si->update_screen = NULL;
}

// emulates the correct number of cycles for one frame; this function should
// be called every 1/60s
void invaders_update(invaders* const si) {
    uint64_t count_cycles = 0;

    while (count_cycles <= CYCLES_PER_FRAME) {
        const uint64_t start_cyc = si->cpu.cyc;
        const uint8_t opcode = invaders_rb(si, si->cpu.pc);

        i8080_step(&si->cpu);

        count_cycles += si->cpu.cyc - start_cyc;

        // Space Invaders special opcodes (IN/OUT)
        switch (opcode) {
        case 0xDB: { // IN
            const uint8_t port = invaders_rb(si, si->cpu.pc++);
            uint8_t value = 0;

            if (port == 1) {
                value = si->port1;
            }
            else if (port == 2) {
                value = si->port2;
            }
            else if (port == 3) {
                const uint16_t v = (si->shift1 << 8) | si->shift0;
                value = (v >> (8 - si->shift_offset)) & 0xFF;
            }
            else {
                fprintf(stderr, "error: unknown IN port %02X\n", port);
            }
            si->cpu.a = value;
        } break;
        case 0xD3: { // OUT
            const uint8_t port = invaders_rb(si, si->cpu.pc++);
            const uint8_t value = si->cpu.a;

            if (port == 2) {
                si->shift_offset = value & 0x7;
            }
            else if (port == 3) {
            }
            else if (port == 4) {
                si->shift0 = si->shift1;
                si->shift1 = value;
            }
            else if (port == 5) {
            }
            else if (port == 6) {
                // debug port?
            }
            else {
                fprintf(stderr, "error: unknown OUT port %02X\n", port);
            }
        } break;
        }

        // interrupts handling: every HALF_CYCLES_PER_FRAME cycles, an
        // interrupt is requested (0x08 or 0x10)
        if (si->cpu.cyc >= HALF_CYCLES_PER_FRAME) {
            i8080_interrupt(&si->cpu, si->next_interrupt);
            si->cpu.cyc -= HALF_CYCLES_PER_FRAME;
            si->next_interrupt = si->next_interrupt == 0x08 ? 0x10 : 0x08;
        }
    }
}

// updates the screen buffer according to what is in the video ram
void invaders_gpu_update(invaders* const si) {
    // the screen is 256 * 224 pixels, and is rotated anti-clockwise.
    // these are the overlay dimensions:
    // ,_______________________________.
    // |WHITE            ^             |
    // |                32             |
    // |                 v             |
    // |-------------------------------|
    // |RED              ^             |
    // |                32             |
    // |                 v             |
    // |-------------------------------|
    // |WHITE                          |
    // |         < 224 >               |
    // |                               |
    // |                 ^             |
    // |                120            |
    // |                 v             |
    // |                               |
    // |                               |
    // |                               |
    // |-------------------------------|
    // |GREEN                          |
    // | ^                  ^          |
    // |56        ^        56          |
    // | v       72         v          |
    // |____      v      ______________|
    // |  ^  |          | ^            |
    // |<16> |  < 118 > |16   < 122 >  |
    // |  v  |          | v            |
    // |WHITE|          |         WHITE|
    // `-------------------------------'

    // the screen is 256 * 224 pixels, and 1 byte contains 8 pixels
    for (int i = 0; i < 256 * 224 / 8; i++) {
        const int y = i * 8 / 256;
        const int base_x = (i * 8) % 256;
        const uint8_t cur_byte = si->memory[VRAM_ADDR + i];

        for (uint8_t bit = 0; bit < 8; bit++) {
            int px = base_x + bit;
            int py = y;
            const bool is_pixel_lit = (cur_byte >> bit) & 1;
            uint8_t r = 0, g = 0, b = 0;

            // colour handling:
            if (!si->colored_screen && is_pixel_lit) {
                r = 255; g = 255; b = 255;
            }
            else if (si->colored_screen && is_pixel_lit) {
                if (px < 16) {
                    if (py < 16 || py > 118 + 16) {
                        r = 255; g = 255; b = 255;
                    }
                    else {
                        g = 255;
                    }
                }
                else if (px >= 16 && px <= 16 + 56) {
                    g = 255;
                }
                else if (px >= 16 + 56 + 120 && px < 16 + 56 + 120 + 32) {
                    r = 255;
                }
                else {
                    r = 255; g = 255; b = 255;
                }
            }

            // space invaders' screen is rotated 90 degrees anti-clockwise
            // so we invert the coordinates:
            const int temp_x = px;
            px = py;
            py = -temp_x + SCREEN_HEIGHT - 1;

            si->screen_buffer[py][px][0] = r;
            si->screen_buffer[py][px][1] = g;
            si->screen_buffer[py][px][2] = b;
        }
    }

    si->update_screen(si);
}

// memory handling

// reads a byte from memory
uint8_t invaders_rb(void* userdata, uint16_t addr) {
    invaders* const si = (invaders*) userdata;

    // if (addr >= 0x6000) return 0;
    if (addr >= 0x4000 && addr < 0x6000) addr -= 0x2000; // RAM mirror

    return si->memory[addr];
}

// writes a byte to memory
void invaders_wb(void* userdata, uint16_t addr, uint8_t val) {
    invaders* const si = (invaders*) userdata;

    if (addr < 0x2000) return; // cannot write to rom
    // if (addr >= 0x6000) return;
    if (addr >= 0x4000 && addr < 0x6000) addr -= 0x2000; // RAM mirror

    si->memory[addr] = val;
}


