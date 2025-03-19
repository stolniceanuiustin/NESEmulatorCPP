#include "../include/ppu.h"
#include "../include/emulator_config.h"
#include "../include/cpu_header.h"
#include <iostream>
#include <fstream>
void clear_status_register(byte &x)
{
    x &= 0b00011111;
}

void PPU::reset()
{
    control.reg = 0x0000;
    mask.reg = 0x0000;
    status.reg = status.reg & 0b10000000;
    PPU_BUFFER = 0;
    PPUSCROLL_latch = false;
    PPUADDR_latch = false;
    first_write = false;
    vaddr.reg = 0x0000;
    taddr.reg = 0x0000;
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

/*
This function sohul dbe complete. PPU_Write remains! Then i need to see how to actually draw something on the screen
*/
byte PPU::ppu_read(uint16_t addr, bool read_only)
{
    byte data = 0x00;
    addr &= 0x3FFF;
    if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        return cartridge->ppu_read(addr);
    }
    else if (addr >= 0x2000 && addr <= 0x3EFF)
    {
        /*
            https://www.nesdev.org/wiki/Mirroring
        */
        addr &= 0x0FFF;
        if (cartridge->mirroring == VERTICAL)
        {
            if (addr >= 0x0000 && addr <= 0x03FF)
                return nametable[0][addr & 0x03FF];
            if (addr >= 0x0400 && addr <= 0x07FF)
                return nametable[1][addr & 0x03FF];
            if (addr >= 0x0800 && addr <= 0x0BFF)
                return nametable[0][addr & 0x03FF];
            if (addr >= 0x0C00 && addr <= 0x0FFF)
                return nametable[1][addr & 0x03FF];
        }
        else if (cartridge->mirroring == HORIZONTAL)
        {
            if (addr >= 0x0000 && addr <= 0x03FF)
                return nametable[0][addr & 0x03FF];
            else if (addr >= 0x0400 && addr <= 0x07FF)
                return nametable[0][addr & 0x03FF];
            else if (addr >= 0x0800 && addr <= 0x0BFF)
                return nametable[1][addr & 0x03FF];
            else if(addr >= 0x0C00 && addr <= 0x0FFF)
                return nametable[1][addr & 0x03FF];
        }
    }
    else if(addr >= 0x3F00 && addr <= 0x3FFF)
    {
        addr &= 0x001F;
        //mirroring: 
        if(addr == 0x10) 
            addr = 0x00;
        else if(addr == 0x14)
            addr = 0x04;
        else if(addr == 0x18)
            addr = 0x08;
        else if(addr == 0x1C)
            addr = 0x0C;
        return pallete_table[addr];
    }
}

void PPU::ppu_write(uint16_t addr, uint8_t data)
{
    addr &= 0x3FFF;
    if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        cartridge->ppu_write(addr, data);
        ppu_log.write("Writing to cartridge ROM. should not get here in normal circumstances\n");
    }
    else if (addr >= 0x2000 && addr <= 0x3EFF)
    {
        /*
            https://www.nesdev.org/wiki/Mirroring
        */
        addr &= 0x0FFF;
        if (cartridge->mirroring == VERTICAL)
        {
            if (addr >= 0x0000 && addr <= 0x03FF)
                nametable[0][addr & 0x03FF] = data;
            if (addr >= 0x0400 && addr <= 0x07FF)
                nametable[1][addr & 0x03FF] = data;
            if (addr >= 0x0800 && addr <= 0x0BFF)
                nametable[0][addr & 0x03FF] = data;
            if (addr >= 0x0C00 && addr <= 0x0FFF)
                nametable[1][addr & 0x03FF] = data;
        }
        else if (cartridge->mirroring == HORIZONTAL)
        {
            if (addr >= 0x0000 && addr <= 0x03FF)
                nametable[0][addr & 0x03FF] = data;
            else if (addr >= 0x0400 && addr <= 0x07FF)
                nametable[0][addr & 0x03FF] = data;
            else if (addr >= 0x0800 && addr <= 0x0BFF)
                nametable[1][addr & 0x03FF] = data;
            else if(addr >= 0x0C00 && addr <= 0x0FFF)
                nametable[1][addr & 0x03FF] = data;
        }
    }
    else if(addr >= 0x3F00 && addr <= 0x3FFF)
    {
        addr &= 0x001F;
        //mirroring: 
        if(addr == 0x10) 
            addr = 0x00;
        else if(addr == 0x14)
            addr = 0x04;
        else if(addr == 0x18)
            addr = 0x08;
        else if(addr == 0x1C)
            addr = 0x0C;
        pallete_table[addr] = data;
    }
}

byte PPU::read_from_cpu(byte addr, bool read_only)
{
    byte data;
    if (read_only == true)
    {
        switch (addr)
        {
        case 0:
            data = control.reg;
            break;
        case 1:
            data = mask.reg;
            break;
        case 2:
            data = status.reg;
            break;
        case 3:
            data = OAMADDR;
            break;
        case 4:
            data = OAMDATA; // OAMDATA
        case 5:
            return -1;
        case 6:
            return -1; /// PPUADDR
        case 7:
            return -1;
        }
    }
    else if (read_only == false) // default mode
    {
        switch (addr)
        {
        case 0:
            break; // control not readable usually
        case 1:
            break; // mask not readable usually
        case 2:
            status.reg & 0b11100000 | PPU_BUFFER & 0b00011111; // last 5 bits of the last ppu bus transaction
            status.vertical_blank = 0;
            PPUADDR_latch = false;
        case 3:
            // oam addr;
            break;
        case 4:
            // oam data;
            break;
        case 5:
            // scroll is not readable
            break;
        case 6:
            // ppu address is not readable
            break;
        case 7: // reads from memory but it is delayed one cycle
            data = PPU_BUFFER;
            PPU_BUFFER = ppu_read(vaddr.reg); 
            if (vaddr.reg >= 0x3F00)
                data = PPU_BUFFER; // separate memory on the ppu
            vaddr.reg += (control.increment_mode ? 32 : 1); //either vertical or horizontal jumps in nametable
            break;
        }
    }
    return data;
}

void PPU::write_from_cpu(byte addr, byte data)
{
    switch (addr)
    {
    case 0:
        control.reg = data;
        taddr.nametable_x = control.nametable_x;
        taddr.nametable_y = control.nametable_y;
        break;
    case 1:
        mask.reg = data;
        break;
    case 2:     // STATUS - read only
        break;
    case 3:
        return; // OAM ADDR
    case 4:
        return; // OAMDATA
    case 5:
        return; // PPUSCROLL
    case 6:
        if(PPUADDR_latch == false)
        {
            taddr.reg = (uint16_t)((data & 0b00111111) << 8) | (taddr.reg & 0x00FF);
            PPUADDR_latch = true;
        }
        else (PPUADDR_latch = true)
        {
            taddr.reg = (taddr.reg & 0xFF00) | data;
            vaddr = taddr;
            PPUADDR_latch = 0;
        }
        return;
    case 7:
        ppu_write(vaddr.reg, data); // PPUDATA
        vaddr.reg += (control.increment_mode ? 32:1);
    }
}

void PPU::execute()
{
    switch (pipeline_state)
    {
    case PRE_RENDER:
        ppu_log .write( "PRERENDER";
        if (dots == 1)
        {
            // PPUSTATUS &= 0b00011111;
        }
        if (dots >= LAST_SCANLINE_DOT)
        {
            pipeline_state = RENDER;
            dots = 0;
            scanline = 0;
        }
        break;
    case RENDER:
        ppu_log.write("RENDER");
        if (dots > 0 && dots <= VISIBLE_DOTS)
        {
            // do something someday
            //TODO: check set_pixelfunction, check how the color is calculated!
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
        ppu_log.write( "POSTRENDER");
        if (dots >= LAST_SCANLINE_DOT)
        {
            scanline++;
            dots = 0;
            pipeline_state = VERTICAL_BLANK;
        }
        break;
    case VERTICAL_BLANK:
        ppu_log.write("VBLANK");
        if (dots == 1 && scanline == VISIBLE_SCANLINES + 1)
        {
            set_vblank();
            screen.RENDER_ENABLED = true;
            std::cout << "=====BLANKING PERIOD======\n";
            ppu_log.write("GOT HERE BLANKING PERIOD!!");
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
    ppu_log.write("\n");
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