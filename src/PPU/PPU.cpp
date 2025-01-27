#include "ppu.h"
#include "../emulator_config.h"

void PPU::reset()
{
    PPUCTRL = 0x0000;
    PPUMASK = 0x0000;
    PPUSTATUS = PPUSTATUS & 0b10000000;
    PPUSCROLL = 0x0000;
    PPUDATA = 0x00;
}

void PPU::execute()
{
    if(dots >= 0 && dots < VISIBLE_DOTS)
    {
        dots++;
        //DO SOMETHING
    }
    else if(dots == VISIBLE_DOTS)
    {
        dots++;
    }
    else if(dots > VISIBLE_DOTS && dots < END_DOT-1)
    {
        dots++;
    }
    else if(dots == END_DOT - 1)
    {
        if(scanline != NTSC_SCANLINES_PER_FRAME - 1)
            scanline++;
        else scanline = 0;
        dots = 0;
    }

    if (scanline < VISIBLE_SCANLINES)
    {
        clear_vblank_nmi();
    }
    else if(scanline == VISIBLE_SCANLINES && dots == 0)
    {
        set_vblank_nmi();
    }
    else if(scanline >= VISIBLE_SCANLINES && scanline < NTSC_SCANLINES_PER_FRAME-1)
    {
        clear_vblank_nmi();
    }
}


void PPU::set_vblank_nmi()
{a
    PPUCTRL |= 1 << 7;
}

void PPU::clear_vblank_nmi()
{
    PPUCTRL &= ~(1 << 7);
}

void PPU::set_vblank()
{
    PPUSTATUS |= 1 << 7;
}

void PPU::clear_vblank()
{
    PPUSTATUS &= ~(1 << 7);
}