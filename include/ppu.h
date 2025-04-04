#ifndef HEADERGUARD_PPU
#define HEADERGUARD_PPU

#include <stdint.h>
#include "virtual_screen.h"
#include "cartridge.h"
#include "log.h"
#include "SDL_backend.h"
class CARTRIDGE;
class BUS;
class Memory;
class CPU;
class LOG;

#define VISIBLE_SCANLINES 240
#define VISIBLE_DOTS 256
#define FRAME_END_SCANLINE 261
#define DOTS_PER_SCANLINE 341
#define LAST_SCANLINE_DOT 340

typedef uint8_t byte;

struct _OAM
{
    byte Y_pos;
    byte index;
    byte attributes;
    byte X_pos;
}; //this is how a sprite is stored

union
{
    struct 
    {
        byte unused : 5;
        byte sprite_overflow : 1;
        byte sprite_zero_hit : 1;
        byte vertical_blank : 1;
    };
    byte reg;
}status;

union
{
    struct
    {
        byte grayscale : 1;
        byte render_background_left : 1;
        byte render_sprites_left : 1;
        byte render_background : 1;
        byte render_sprites : 1;
        byte enhance_red : 1;
        byte enhance_green : 1;
        byte enhance_blue : 1;
    };

    byte reg;
} mask;

union 
{
    struct 
    {
        byte nametable_x : 1;
        byte nametable_y : 1;
        byte increment_mode : 1;
        byte pattern_sprite : 1;
        byte pattern_background : 1;
        byte sprite_size : 1;
        byte slave_mode : 1; //UNUSED
        byte enable_nmi : 1;
    };
    byte reg;
} control;

union loopy
{   
    //THE X and T registers play more roles. Can either be used as address for vram or for selecting other stuff
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
    bool pause_after_frame = false;
    CPU &cpu;
    Screen &screen;
    CARTRIDGE* cartridge;

    uint16_t current_frame;
    bool odd_frame;
    BUS* bus;
    //byte bus;
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
    _OAM OAM[64];
    byte* pOAM = (byte*)OAM; //pointer to OAM for byte by byte access
    //TODO: here you can enable/disable ppu logging
    PPU(CPU& cpu, Screen& screen) : cpu(cpu), screen(screen), ppu_log("ppu_log.txt", false){
        current_frame = 0;
        status.reg = 0b10100000;
        control.reg = 0x00;
        mask.reg = 0x00;
        OAMADDR = 0x00;
        OAMDATA = 0x00;
        odd_frame = false;
        bus = 0;
        // v = 0;
        scanline = 0;
        dots = 0;
        fine_x = 0;
        for(int i=0; i<64; i++)
        {
            OAM[i].Y_pos = 0x00;
            OAM[i].X_pos = 0x00;
            OAM[i].index = 0x00;
            OAM[i].attributes = 0x00;
        }
    }
    
    byte OAMADDR;
    byte OAMDATA;
    byte PPUSCROLL;

    byte nametable[2][0x03FF]; //mirrored
    byte patterntable[2][0x0FFF];
    byte pallete_table[32];
    byte fine_x = 0; //3 bits wide!
    //rendering shift registers! they shift every PPU clock. There are 2 16bit registers
    //https://www.nesdev.org/wiki/PPU_rendering
    //Conceptually:
    //Pixels that are being drawn currently
    //So we always load only the last 8 bits of the register
    //msb xxxx-xxxx                            xxxx-xxxx 
    //lsb xxxx-xxxx                            xxxx-xxxx
    uint16_t bgs_pattern_l = 0x0000; 
    uint16_t bgs_pattern_h = 0x0000;
    uint16_t bgs_attribute_l = 0x0000;
    uint16_t bgs_attribute_h = 0x0000;
    byte bg_next_tile_id = 0x00;
    byte bg_next_tile_attrib = 0x00;
    byte bg_next_tile_lsb = 0x00;
    byte bg_next_tile_msb = 0x00;

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
    void hexdump();
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