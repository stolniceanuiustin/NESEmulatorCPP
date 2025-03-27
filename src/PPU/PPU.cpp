#include "../include/ppu.h"
#include "../include/emulator_config.h"
#include "../include/cpu_header.h"
#include <iostream>
#include <fstream>
void PPU::hexdump()
{
    FILE *file = fopen("ppunametable", "wb");
    if (file)
    {
        fwrite(nametable, sizeof(byte), 0x3FFF, file);
        fclose(file);
    }
    FILE *pallete = fopen("ppupallete", "wb");
    if (pallete)
    {
        fwrite(pallete_table, sizeof(byte), 0x32, pallete);
        fclose(pallete);
    }
}
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
    bgs_pattern_l = 0x0000;
    bgs_pattern_h = 0x0000;
    bgs_attribute_l = 0x0000;
    bgs_attribute_h = 0x0000;
    for (int i = 0; i < 32; i++)
        pallete_table[i] = 0;
}

/*
This function sohul dbe complete. PPU_Write remains! Then i need to see how to actually draw something on the screen
*/
byte PPU::ppu_read(uint16_t addr, bool read_only)
{
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
            else if (addr >= 0x0C00 && addr <= 0x0FFF)
                return nametable[1][addr & 0x03FF];
        }
    }
    else if (addr >= 0x3F00 && addr <= 0x3FFF)
    {
        addr &= 0x001F;
        // mirroring:
        if (addr == 0x10)
            addr = 0x00;
        else if (addr == 0x14)
            addr = 0x04;
        else if (addr == 0x18)
            addr = 0x08;
        else if (addr == 0x1C)
            addr = 0x0C;
        return pallete_table[addr] & (mask.grayscale ? 0x30 : 0x3F);
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
        // std::cout << "Writing to nametable memory\n";
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
            else if (addr >= 0x0C00 && addr <= 0x0FFF)
                nametable[1][addr & 0x03FF] = data;
        }
    }
    else if (addr >= 0x3F00 && addr <= 0x3FFF)
    {
        std::cout << "writing to pallete table\n";
        addr &= 0x001F;
        // mirroring:
        if (addr == 0x10)
            addr = 0x00;
        else if (addr == 0x14)
            addr = 0x04;
        else if (addr == 0x18)
            addr = 0x08;
        else if (addr == 0x1C)
            addr = 0x0C;
        pallete_table[addr] = data;
    }
}

byte PPU::read_from_cpu(byte addr, bool read_only)
{
    byte data = 0x00;
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
            // TODO: CHECK THIS
            status.reg = (status.reg & 0b11100000) | (PPU_BUFFER & 0b00011111); // last 5 bits of the last ppu bus transaction
            data = status.reg;
            // i think clearing vblank is messing up with timing!
            clear_vblank();
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
                data = PPU_BUFFER;                          // separate memory on the ppu
            vaddr.reg += (control.increment_mode ? 32 : 1); // either vertical or horizontal jumps in nametable
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
    case 2: // STATUS - read only
        break;
    case 3:
        return; // OAM ADDR
    case 4:
        return; // OAMDATA
    case 5:     // Scroll Register
        if (PPUADDR_latch == false)
        {
            fine_x = data & 0x07;
            taddr.coarse_x = data >> 3;
            PPUADDR_latch = true;
        }
        else
        {
            taddr.fine_y = data & 0x07;
            taddr.coarse_y = data >> 3;
            PPUADDR_latch = false;
        }
        return;
    case 6:
        if (PPUADDR_latch == false)
        {
            taddr.reg = (uint16_t)((data & 0b00111111) << 8) | (taddr.reg & 0x00FF);
            PPUADDR_latch = true;
        }
        else if (PPUADDR_latch == true)
        {
            taddr.reg = (taddr.reg & 0xFF00) | data;
            vaddr = taddr;
            PPUADDR_latch = false;
        }
        return;
    case 7:
        ppu_write(vaddr.reg, data); // PPUDATA
        vaddr.reg += (control.increment_mode ? 32 : 1);
    }
}

void PPU::execute()
{
    auto clock_shifters = [&]()
    {
        // Every clock while rendering is enabled the internal shift registers shift one bit to the left
        if (mask.render_background)
        {
            bgs_pattern_l <<= 1;
            bgs_pattern_h <<= 1;
            bgs_attribute_h <<= 1;
            bgs_attribute_l <<= 1;
        }
    };

    auto load_bg_shifters = [&]()
    {
        bgs_pattern_l = (bgs_pattern_l & 0xFF00) | bg_next_tile_lsb;
        bgs_pattern_h = (bgs_pattern_h & 0xFF00) | bg_next_tile_msb;
        bgs_attribute_l = (bgs_attribute_l & 0xFF00) | ((bg_next_tile_attrib & 0b01) ? 0xFF : 0x00); // The lsbit of the attribute is mirrored 8 times
        bgs_attribute_h = (bgs_attribute_h & 0xFF00) | ((bg_next_tile_attrib & 0b10) ? 0xFF : 0x00);
    };

    auto transfer_address_x = [&]()
    {
        if (mask.render_background)
        {
            vaddr.nametable_x = taddr.nametable_x;
            vaddr.coarse_x = taddr.coarse_x;
        }
    };

    auto increment_scroll_x = [&]()
    {
        if (mask.render_background)
        {
            if (vaddr.coarse_x == 31)
            {
                vaddr.coarse_x = 0;
                vaddr.nametable_x = !vaddr.nametable_x;
            }
            else
                vaddr.coarse_x++;
        }
    };

    auto increment_scroll_y = [&]()
    {
        if (mask.render_background)
        {
            if (vaddr.fine_y < 7)
                vaddr.fine_y++;
            else
            {
                vaddr.fine_y = 0;
                if (vaddr.coarse_y == 29)
                {
                    vaddr.coarse_y = 0;
                    vaddr.nametable_y = !vaddr.nametable_y;
                }
                else if (vaddr.coarse_y == 31)
                {
                    vaddr.coarse_y = 0;
                }
                else
                    vaddr.coarse_y++;
            }
        }
    };
    auto transfer_address_y = [&]()
    {
        if (mask.render_background)
        {
            vaddr.nametable_y = taddr.nametable_y;
            vaddr.coarse_y = taddr.coarse_y;
            vaddr.fine_y = taddr.fine_y;
        }
    };
    if (scanline >= -1 && scanline < 240)
    {
        if (scanline == 0 && dots == 0)
            dots = 1;

        if (scanline == -1 && dots == 1)
            clear_vblank();

        if ((dots >= 2 && dots < 258) || (dots >= 321 && dots < 338))
        {
            clock_shifters();
            switch ((dots - 1) % 8)
            {
            case 0:
                // Nametable fetch. The ppu has to fetch the next tile id from the nametable. Using the tile id it will fetch the bits from the nametable
                load_bg_shifters();
                // only 12 bit addressing for the ppu memory
                bg_next_tile_id = ppu_read(0x2000 | vaddr.reg & 0x0FFF, false);

                break;
            case 2:
                // atribute table fetch
                // https://www.nesdev.org/wiki/PPU_scrolling#Tile_and_attribute_fetching
                bg_next_tile_attrib = ppu_read(0x23C0 | (vaddr.nametable_y << 11) | (vaddr.nametable_x << 10) | ((vaddr.coarse_y >> 2) << 3) | (vaddr.coarse_x >> 2));
                if (vaddr.coarse_y & 0x02)
                    bg_next_tile_attrib >>= 4;
                if (vaddr.coarse_x & 0x02)
                    bg_next_tile_attrib >>= 2;
                bg_next_tile_attrib &= 0x03;
                // todo: figure what those bits actually mean
                break;
            case 4:
                // BG lsbits : least significant bits. fetch lsb for background tile
                bg_next_tile_lsb = ppu_read((control.pattern_background << 12) + ((uint16_t)bg_next_tile_id << 4) + (vaddr.fine_y) + 0);
                break;
            case 6:
                bg_next_tile_msb = ppu_read((control.pattern_background << 12) + ((uint16_t)bg_next_tile_id << 4) + (vaddr.fine_y) + 8);
                break;
            case 7:
                increment_scroll_x();
                break;
            }
        }
        // on last visible dot increment scroll y
        if (dots == VISIBLE_DOTS)
        {
            increment_scroll_y();
        }
        if (dots == VISIBLE_DOTS + 1)
        {
            load_bg_shifters();
            transfer_address_x();
        }
        if (dots == 338 || dots == 340)
        {
            // does an unused memory fetch
            bg_next_tile_id = ppu_read(0x2000 | (vaddr.reg & 0x00FF));
        }
        if (scanline == -1 && dots >= 280 && dots < 305)
        {
            transfer_address_y();
        }
    }
    if (scanline == VISIBLE_SCANLINES) // post render scanline, nothing happens here
        ;
    if (scanline > VISIBLE_SCANLINES && scanline < FRAME_END_SCANLINE)
    {
        if (scanline == VISIBLE_SCANLINES + 1 && dots == 1)
        {
            set_vblank();
            screen.RENDER_ENABLED = true;
            std::cout << "=====BLANKING PERIOD======\n";
            SDL::state = PAUSED;
            if (control.enable_nmi == 1)
            {
                cpu.enqueue_nmi();
            }
        }
    }

    byte bg_pixel = 0x00;
    byte bg_pallete = 0x00;
    if (mask.render_background)
    {
        // TODO: Check this, need to figure out scrolling, ignore for now
        uint16_t bit_mux = 0x8000 >> fine_x;

        byte p0_pixel = (bgs_pattern_l & bit_mux) > 0;
        byte p1_pixel = (bgs_pattern_h & bit_mux) > 0;
        bg_pixel = (p1_pixel << 1) | p0_pixel;

        byte bg_pallete0 = (bgs_attribute_l & bit_mux) > 0;
        byte bg_pallete1 = (bgs_attribute_h & bit_mux) > 0;
        bg_pallete = (bg_pallete1 << 1) | bg_pallete0;
        if (scanline >= 0 && scanline < 240 && dots >= 0 && dots <= 256)
        {
            byte color = 0;
            if (bg_pixel != 0)
            {
                color = ppu_read(0x3F00 + (bg_pallete << 2) + bg_pixel, false) & 0x3F;
            }
            else
                color = ppu_read(0x3F00, false) & 0x3F;
            screen.set_pixel(scanline, dots, color);
        }
    }

    dots++;
    if (dots > LAST_SCANLINE_DOT)
    {
        dots = 0;
        scanline++;
        if (scanline > FRAME_END_SCANLINE)
        {
            scanline = -1;
        }
    }
}

void PPU::set_vblank()
{
    status.vertical_blank = 1;
}

void PPU::clear_vblank()
{
    status.vertical_blank = 0;
}