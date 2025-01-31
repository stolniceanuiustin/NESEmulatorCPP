#include "../include/ppu.h"
#include "../include/emulator_config.h"
#include <iostream>
#include <fstream>
std::ofstream ppu_log("ppu_log.log");
void clear_status_register(byte &x)
{
    x &= 0b00011111;
}

void PPU::reset()
{
    PPUCTRL = 0x0000;
    PPUMASK = 0x0000;
    PPUSTATUS = PPUSTATUS & 0b10000000;
    PPUSCROLL = 0x0000;
    PPUDATA = 0x00;
}
byte PPU::get_status()
{
    byte status = PPUSTATUS & 0b11100000;
    clear_vblank(); // VBLANK is cleared whenever PPUSTATUS IS READ;
    first_write = true;
    return status;
}
void PPU::execute()
{
    switch (pipeline_state)
    {
    case PRE_RENDER:
        ppu_log << "PRERENDER";
        if (dots == 1)
        {
            PPUSTATUS &= 0b00011111;
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
            if (PPUCTRL & (1 << 7))
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
    PPUSTATUS |= 1 << 7;
}

void PPU::clear_vblank()
{
    PPUSTATUS &= ~(1 << 7);
}