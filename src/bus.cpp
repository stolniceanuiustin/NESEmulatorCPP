#include "../include/bus.h"
#include "../include/ppu.h"
#include "../include/cartridge.h"
#include <iostream>

void BUS::cpu_write(uint16_t addr, byte data)
{
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        cartridge.cpu_write(addr, data);
    }
    else if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        cpu_ram[addr & 0x07FF] = data;
    }
    else if (addr >= 0x2000 && addr <= 0x3FFF) // those are the PPU registers mirrored every 8 bites
    {
        ppu.write_from_cpu(addr & 0x0007, data);
    }
    else if (addr >= 0x4000 && addr <= 0x4013)
    {
        //std::cerr << "APU REGISTERS\n";
    }
    else if (addr == 0x4014) //OAMDMA
    {
        oam_dma_page = data;
        oam_dma_addr = 0x00;
        dma_transfer = true;
    }
    else if (addr == 0x4016 || addr == 0x4017)
    {
        static byte prev_write = 0;
        // std::cout << (int)prev_write << " " << (int)data << '\n';
        if ((prev_write & 1) == 1 && (data & 1) == 0)
        {
            controller_state[0] = controller[0];
            controller_state[1] = 0x00;
        }
        prev_write = data;
    }
}

byte BUS::cpu_read(uint16_t addr)
{
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        return cartridge.cpu_read(addr);
    }
    if (addr >= 0x0000 && addr <= 0x1FFF)
    {
        return cpu_ram[addr & 0x07FF];
    }
    else if (addr >= 0x2000 && addr <= 0x3FFF)
    {
        byte data = ppu.read_from_cpu(addr & 0x0007, false);
        return data;
    }
    else if (addr >= 0x4000 && addr <= 0x4015)
    {
        // std::cerr << "APU REGISTERS\n";
        return 0;
    }
    else if (addr == 0x4016 || addr == 0x4017)
    {
        byte data = (controller_state[addr & 1] & 0x80) > 0 ? 1 : 0;
        controller_state[addr & 1] <<= 1;
        if (data)
            std::cout << (int)data << '\n';
        // std::cerr << "INPUT REGISTERS READ\n";
        return data; // bit 6 shjould always be set to 1 due to open bus behaviour
    }
    else
        return 0;
}

void BUS::reset()
{
    cpu_ram.reset();
    ppu_ram.reset();
    controller[0] = 0x00;
    controller[1] = 0x00;
    controller_state[0] = 0x00;
    controller_state[1] = 0x00;
    dma_transfer = false;
    oam_dma_page = 0x00;
    oam_dma_addr = 0x00;
}

void BUS::hexdump()
{
    cpu_ram.hexdump("cpu_hexdump", 0x07FF);
    ppu_ram.hexdump("ppu_hexdump", 0x3FFF);
}