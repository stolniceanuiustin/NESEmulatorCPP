#ifndef HEADERGUARD_PPU
#define HEADERGUARD_PPU

#include <stdint.h>
#include "memory.h"
#include "cpu_header.h"
#define PPUCTRL shared_ram[0x2000]
#define PPUMASK shared_ram[0x2001]
#define PPUSTATUS shared_ram[0x2002]
#define OAMADDR shared_ram[0x2003]
#define OAMDATA shared_ram[0x2004]
#define PPUSCROLL shared_ram[0x2005]
#define PPUADDR shared_ram[0x2006]
#define PPUDATA shared_ram[0x2007]
#define OAMDMA shared_ram[0x4014]

#define VISIBLE_SCANLINES 240
#define VISIBLE_DOTS 256
#define FRAME_END_SCANLINE 261
#define DOTS_PER_SCANLINE 341
#define LAST_SCANLINE_DOT 340

#define SHOW_BG PPUMASK & 1 << 3
#define VBLANK (PPUCTRL & 1 << 7) >> 7;
typedef uint8_t byte;



class PPU
{
public:
    CPU &cpu;
    Memory &shared_ram; // SHARED RAM
    Memory &internal_ram; //internal RAM;
    uint16_t current_frame;
    bool odd_frame;
    uint8_t bus;
    uint16_t v; //in render = scrolling position, outside of rendering = the current VRAM address
    uint16_t t; //During rendering it does something. Outsdie of rendering, holds the VRAM address before transfering it to v
    uint16_t x; //FINE x position of the current scroll, sused during rendering alongside v
    uint16_t w; //Togles on each write to PPUSCROLL or PPUADDR, indicating whether it's the first or secnon dwrite. Clears on reads of PPUSTATUS
    //its also claled the write latch or write toggle
    uint16_t scanline;
    uint16_t dots;
    bool PPUSCROLL_latch = false;
    bool PPUADDR_latch = false;
    bool first_write = false;
    uint16_t PPUSCROLL16;
    uint16_t PPUADDR16;
    bool even_frame = false;
    PPU(Memory &ram, Memory &internal_ram, CPU& cpu) : shared_ram(ram), internal_ram(internal_ram), cpu(cpu) {
        current_frame = 0;
        PPUCTRL = 0x00;
        PPUMASK = 0x00;
        PPUSTATUS = 0b10100000;
        OAMADDR = 0x00;
        PPUSCROLL = 0x00;
        PPUADDR = 0x00;
        PPUDATA = 0x00;
        odd_frame = false;
        bus = 0;
        v = 0;
        scanline = 0;
        dots = 0;
        pipeline_state = PRE_RENDER;
    }
    
    byte OAM[256];

    void reset();
    void execute();
    void set_vblank_nmi();
    void clear_vblank_nmi();
    void set_vblank();
    void clear_vblank();
    byte get_status();

private:
    enum State
    {
        PRE_RENDER,
        RENDER,
        POST_RENDER,
        VERTICAL_BLANK
    } pipeline_state;
    int cycle;
    
};



#endif