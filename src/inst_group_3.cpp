#include "cpu_header.h"

void CPU::BIT(uint16_t address)
{
    // bit test, test if one or more bits are in a target memory location
    // mask patern in A is & with memory to keep zero, overflow, negative etc.
    int lookup[] = {-1, 3, -1, 4, -1, -1, -1, -1};
    uint8_t result = A & ram[address];
    if (result == 0)
        Z = 1;
    else
        Z = 0;
    uint8_t negative = result & (1 << 7);
    if (negative)
        N = 1;
    else
        N = 0;

    uint8_t overflow = result & (1 << 6);
    if (overflow)
        O = 1;
    else
        O = 0;
}

void CPU::JMP_abs(uint16_t jump_address)
{
    PC = jump_address;
    cycles += 3;
}

void CPU::JMP_indirect(uint16_t jump_address)
{
    // TODO: ADDRESS BUG FROM ORIGINAL 6502(not a bug in my code)
    PC = read_abs_address(jump_address);
    cycles += 5;
}

// void CPU::STX(uint16_t address)
// {                 //immediate, zeropage, nothig, absolut,nothing, zero page x, nothing, absolut x
//     int lookup[] = {-1, 3, -1, 4, -1, 4, -1, -1};
//     ram[address] = X;
//     cycles += lookup[inst.bbb];
// }

void CPU::STY(uint16_t address)
{
    int lookup[] = {-1, 3, -1, 4, -1, 4, -1, -1};
    ram[address] = Y;
    cycles += lookup[inst.bbb];
}

void CPU::LDY(uint16_t address, bool page_cross)
{ // immediate, zeropage, nothig, absolut,nothing, zero page x, nothing, absolut x
    int lookup[] = {2, 3, -1, 4, -1, 4, -1, 4};
    Y = ram[address];
    cycles += lookup[inst.bbb] + (int)page_cross;
}

void CPU::CPY(uint16_t address)
{
    int lookup[] = {2, 3, -1, 4, -1, -1, -1, -1};
    byte result = Y - ram[address];
    set_ZN(result);
    if (result >= 0)
        C = 1;
    else
        C = 0;

    cycles += lookup[inst.bbb];
}

void CPU::CPX(uint16_t address)
{
    int lookup[] = {2, 3, -1, 4, -1, -1, -1, -1};
    byte result = X - ram[address];
    set_ZN(result);
    if (result >= 0)
        C = 1;
    else
        C = 0;
    cycles += lookup[inst.bbb];
}