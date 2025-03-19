#ifndef HEADERGUARD_PPU
#define HEADERGUARD_PPU

#include <stdint.h>
#include "virtual_screen.h"
#include "cartridge.h"
#include "log.h"
class CARTRIDGE;
class BUS;
class Memory;
class CPU;
class LOG;
// #define PPUCTRL shared_ram[0x2000]
// #define PPUMASK shared_ram[0x2001]
// #define PPUSTATUS shared_ram[0x2002]
// #define OAMADDR shared_ram[0x2003]
// #define OAMDATA shared_ram[0x2004]
// #define PPUSCROLL shared_ram[0x2005]
// #define PPUADDR shared_ram[0x2006]
// #define PPUDATA shared_ram[0x2007]
// #define OAMDMA shared_ram[0x4014]
#define VISIBLE_SCANLINES 240
#define VISIBLE_DOTS 256
#define FRAME_END_SCANLINE 261
#define DOTS_PER_SCANLINE 341
#define LAST_SCANLINE_DOT 340

#define SHOW_BG PPUMASK & 1 << 3
#define VBLANK (PPUCTRL & 1 << 7) >> 7;
typedef uint8_t byte;

union
{
    struct 
    {
        uint8_t unused : 5;
        uint8_t sprite_overflow : 1;
        uint8_t sprite_zero_hit : 1;
        uint8_t vertical_blank : 1;
    };
    uint8_t reg;
}status;

union
{
    struct
    {
        uint8_t grayscale : 1;
        uint8_t render_background_left : 1;
        uint8_t render_sprites_left : 1;
        uint8_t render_backgorund : 1;
        uint8_t render_sprites : 1;
        uint8_t enhance_red : 1;
        uint8_t enhance_green : 1;
        uint8_t enhance_blue : 1;
    };

    uint8_t reg;
} mask;

union 
{
    struct 
    {
        uint8_t nametable_x : 1;
        uint8_t nametable_y : 1;
        uint8_t increment_mode : 1;
        uint8_t pattern_sprite : 1;
        uint8_t pattern_background : 1;
        uint8_t sprite_size : 1;
        uint8_t slave_mode : 1; //UNUSED
        uint8_t enable_nmi : 1;
    };
    uint8_t reg;
} control;

union loopy
{
    struct
    {
        uint16_t coarse_x : 5;
        uint16_t coarse_y : 5;
        uint16_t nametable_x : 1;
        uint16_t nametable_y : 1;
        uint16_t fine_y : 3;
        uint16_t unused : 1;
    };
    uint16_t reg = 0x0000;
};

class PPU
{
public:
    CPU &cpu;
    Screen &screen;
    CARTRIDGE* cartridge;

    uint16_t current_frame;
    bool odd_frame;
    BUS* bus;
    //uint8_t bus;
    loopy vaddr; //in render = scrolling position, outside of rendering = the current VRAM address
    loopy taddr; //During rendering it does something. Outsdie of rendering, holds the VRAM address before transfering it to v
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
    byte PPUDATA;
    byte OAMDMA;
    byte PPU_BUFFER;
    bool even_frame = false;

    PPU(CPU& cpu, Screen& screen) : cpu(cpu), screen(screen), ppu_log("ppu_log.txt"){
        current_frame = 0;
        status.reg = 0b10100000;
        control.reg = 0x00;
        mask.reg = 0x00;
        // OAMADDR = 0x00;
        // PPUSCROLL = 0x00;
        // PPUADDR = 0x00;
        // PPUDATA = 0x00;
        odd_frame = false;
        bus = 0;
        //v = 0;
        scanline = 0;
        dots = 0;
        pipeline_state = PRE_RENDER;
        
    }
    
    byte OAM[256];
    byte OAMADDR;
    byte OAMDATA;
    byte PPUSCROLL;

    byte nametable[2][0x03FF]; //mirrored
    byte patterntable[2][0x0FFF];
    byte pallete_table[32];

    void reset();
    void execute();
    void set_vblank_nmi();
    void clear_vblank_nmi();
    void set_vblank();
    void clear_vblank();
    byte get_status();
    byte get_control();
    //For main bus
    void connect_bus(BUS* new_bus)
    {
        bus = new_bus;
    }

    void connect_cartridge(CARTRIDGE* new_cartridge)
    {
        cartridge = new_cartridge;
    }
    byte read_from_cpu(byte addr, bool read_only);
    void write_from_cpu(byte addr, byte data);
    //for internal ppu bus
    byte ppu_read(uint16_t addr, bool read_only = false);
    void ppu_write(uint16_t addr, byte data);
    LOG ppu_log;
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