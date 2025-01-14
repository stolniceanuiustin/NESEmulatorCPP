#include "cpu_header.h"

void CPU::ASL(uint16_t address, bool accumulator)
{
    // Arithmetic shift left
    // Carry = old bit 7
    // bit 0 = 0
    int lookup[] = {-1, 5, 2, 6, -1, 6, -1, 7};
    if (accumulator)
    {
        uint16_t carry_flag = A & (1 << 7);
        if (carry_flag)
            C = 1;
        else
            C = 0;

        A = A << 1;
        set_ZN(A);
    }
    else
    {
        uint16_t carry_flag = ram[address] & (1 << 7);
        if (carry_flag)
            C = 1;
        else
            C = 0;
        ram[address] = ram[address] << 1;
        set_ZN(ram[address]);
    }
    cycles += lookup[inst.bbb];
}
void CPU::ROL(uint16_t address, bool accumulator)
{
    // rotate left
    int lookup[] = {-1, 5, 2, 6, -1, 6, -1, 7};
    if (accumulator)
    {
        uint16_t carry_flag = A & (1 << 7);
        A = A << 1;
        A &= ~1;   // Clear last bit;
        A = A | C; // Set last bit to carry (the old carry, the documentation says so)

        if (carry_flag)
            C = 1;
        else
            C = 0;

        set_ZN(A);
    }
    else
    {
        uint16_t carry_flag = ram[address] & (1 << 7);
        ram[address] = ram[address] << 1;
        ram[address] &= ~1;
        ram[address] |= C;

        if (carry_flag)
            C = 1;
        else
            C = 0;
        set_ZN(ram[address]);
    }
    cycles += lookup[inst.bbb];
}
void CPU::LSR(uint16_t address, bool accumulator)
{
    // Logical shift right
    int lookup[] = {-1, 5, 2, 6, -1, 6, -1, 7};
    if (accumulator)
    {
        uint16_t carry_flag = A & 1;
        A = A >> 1;
        A &= ~(1 << 7); // clear first bit(should be clear already);

        if (carry_flag)
            C = 1;
        else
            C = 0;
        set_ZN(A);
    }
    else
    {
        uint16_t carry_flag = ram[address] & 1;
        ram[address] = ram[address] >> 1;
        ram[address] &= ~(1 << 7);

        if (carry_flag)
            C = 1;
        else
            C = 0;
        set_ZN(ram[address]);
    }
    cycles += lookup[inst.bbb];
}
void CPU::ROR(uint16_t address, bool accumulator)
{
    // Rotate right
    int lookup[] = {-1, 5, 2, 6, -1, 6, -1, 7};
    if (accumulator)
    {
        int carry_flag = A & 1;
        A = A >> 1;
        A &= ~(1 << 7); // clear first bit(should be clear already);
        A |= C << 7;    // set first bit to carry flag

        if (carry_flag)
            C = 1;
        else
            C = 0;
        set_ZN(A);
    }
    else
    {
        int carry_flag = ram[address] & 1;

        ram[address] = ram[address] >> 1;
        ram[address] &= ~(1 << 7); // clear first bit(should be clear already);
        ram[address] |= C << 7;    // set first bit to carry flag

        if (carry_flag)
            C = 1;
        else
            C = 0;
        set_ZN(ram[address]);
    }

    cycles += lookup[inst.bbb];
}
void CPU::STX(uint16_t address)
{
    int lookup[] = {-1, 3, -1, 4, -1, 4, -1, -1};
    ram[address] = X;
    cycles += lookup[inst.bbb];
}
void CPU::LDX(uint16_t address, bool page_cross)
{
    int lookup[] = {2, 3, -1, 4, -1, 4, -1, 4};
    X = ram[address];
    set_ZN(X);
    cycles += lookup[inst.bbb] + (int)page_cross;
}
void CPU::DEC(uint16_t address)
{
    int lookup[] = {-1, 5, -1, 6, -1, 6, -1, 7};
    ram[address] -= 1;
    set_ZN(ram[address]);
    cycles += lookup[inst.bbb];
}
void CPU::INC(uint16_t address)
{
    int lookup[] = {-1, 5, -1, 6, -1, 6, -1, 7};
    ram[address] += 1;
    set_ZN(ram[address]);
    cycles += lookup[inst.bbb];
}