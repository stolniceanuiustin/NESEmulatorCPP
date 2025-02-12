#include "../include/ppu.h"
#include "../include/emulator_config.h"
#include "../include/cpu_header.h"
#include <iostream>
#include <fstream>
std::ofstream ppu_log("ppu_log.log");
void clear_status_register(byte &x)
{
    x &= 0b00011111;
}

void PPU::reset()
{
    control.reg = 0x0000;
    mask.reg = 0x0000;
    status.reg = status.reg & 0b10000000;
}
byte PPU::get_status()
{
    byte state = status.reg & 0b11100000;
    clear_vblank(); // VBLANK is cleared whenever PPUSTATUS IS READ;
    first_write = true;
    return state;
}
byte PPU::get_control()
{
    return control.reg;
}
byte PPU::read_from_cpu(byte addr)
{
    switch(addr)
    {
        case 0:
            return control.reg;
        case 1:
            return mask.reg;
        case 2:
            return status.reg;
        case 3:
            return 0; //OAM ADDR
        case 4:
            return 0; //OAMDATA
        case 5:
            return 0; //PPUSCROLL
        case 6:
            return 0; ///PPUADDR
        case 7:
            return 0; //PPUDATA
    }
}

void PPU::write_from_cpu(byte addr, byte data)
{
    switch(addr)
    {
        case 0:
            control.reg = data;
            break;
        case 1:
            mask.reg = data;
            break;
        case 2:
            status.reg = data;
            break;
        case 3:
            return; //OAM ADDR
        case 4:
            return; //OAMDATA
        case 5:
            return; //PPUSCROLL
        case 6:
            return; ///PPUADDR
        case 7:
            return; //PPUDATA
    }
}
void PPU::execute()
{
    switch (pipeline_state)
    {
    case PRE_RENDER:
        ppu_log << "PRERENDER";
        if (dots == 1)
        {
            //PPUSTATUS &= 0b00011111;
        }
        if (dots >= LAST_SCANLINE_DOT)
        {
            pipeline_state = RENDER;
            dots = 0;
            scanline = 0;
        }
        break;
    case RENDER:
        ppu_log << "RENDER";
        if (dots > 0 && dots <= VISIBLE_DOTS)
        {
            // do something someday!
            if (scanline % 5 == 0 || scanline % 5 == 1 || scanline % 5 == 3)
            {
                screen.set_pixel(scanline, dots);
            }
        }

        if (dots >= LAST_SCANLINE_DOT)
        {
            scanline++;
            dots = 0;
        }
        if (scanline >= VISIBLE_SCANLINES)
        {
            pipeline_state = POST_RENDER;
        }
        break;
    case POST_RENDER:
        ppu_log << "POSTRENDER";
        if (dots >= LAST_SCANLINE_DOT)
        {
            scanline++;
            dots = 0;
            pipeline_state = VERTICAL_BLANK;
        }
        break;
    case VERTICAL_BLANK:
        ppu_log << "VBLANK";
        if (dots == 1 && scanline == VISIBLE_SCANLINES + 1)
        {
            set_vblank();
            screen.RENDER_ENABLED = true;
            std::cout << "=====BLANKING PERIOD======\n";
            ppu_log << "GOT HERE!!";
            if (control.enable_nmi == 1)
            {
                cpu.enqueue_nmi();
            }
        }
        if (dots >= LAST_SCANLINE_DOT)
        {
            scanline++;
            dots = 0;
        }
        if (scanline >= FRAME_END_SCANLINE)
        {
            pipeline_state = PRE_RENDER;
            scanline = 0;
            even_frame = !even_frame;
        }
    }
    ppu_log << '\n';
    dots++;
}

void PPU::set_vblank()
{
    status.vertical_blank = 1;
}

void PPU::clear_vblank()
{
    status.vertical_blank = 0;
}