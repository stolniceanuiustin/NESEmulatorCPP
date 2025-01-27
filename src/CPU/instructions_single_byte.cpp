#include "../include/cpu_header.h"

byte CPU::pack_flags()
{
    byte to_return = 0;
    to_return |= N << 7;
    to_return |= O << 6;
    to_return |= 1 << 5;
    to_return |= B << 4;
    to_return |= D << 3;
    to_return |= I << 2;
    to_return |= Z << 1;
    to_return |= C;

    return to_return;

}


void CPU::unpack_flags(byte flags)
{
    N = ((1 << 7) & flags) >> 7;
    O = ((1 << 6) & flags) >> 6;
    B = 0;                      //break flag shouldnt change when loded with PLP
    D = ((1 << 3) & flags) >> 3;
    I = ((1 << 2) & flags) >> 2;
    Z = ((1 << 1) & flags) >> 1;
    C = 1 & flags;
    return;
}
void CPU::PHP()
{
    byte to_push = pack_flags();
    push(to_push);
    cycles += 3;
}

void CPU::PLP()
{
    byte flags = pop();
    unpack_flags(flags);

    cycles += 4;

}

void CPU::PHA()
{
    push(A);
    cycles += 3;
}

void CPU::PLA()
{
    A = pop();
    set_ZN(A);
    cycles += 4;
}

void CPU::DEY()
{
    Y = Y-1;
    set_ZN(Y);
    cycles += 2;
}

void CPU::TAY()
{
    Y = A;
    set_ZN(Y);
    cycles += 2;
}

void CPU::INY()
{
    Y = Y+1;
    set_ZN(Y);
    cycles += 2;
}

void CPU::INX()
{
    X = X+1;
    set_ZN(X);
    cycles += 2;
}

void CPU::CLC()
{
    C = 0;
    cycles += 2;
}

void CPU::SEC()
{
    C = 1;
    cycles += 2;
}

void CPU::CLI()
{
    I = 0;
    cycles += 2;
}

void CPU::SEI()
{
    I = 1;
    cycles += 2;   
}

void CPU::TYA()
{
    A = Y;
    set_ZN(A);
    cycles += 2;
}

void CPU::CLV()
{
    O = 0;
    cycles += 2;   
}

void CPU::CLD()
{
    //SHOULDNT USE IN NES EMU
    D = 0;
    cycles += 2; 
}

void CPU::SED()
{
    D = 1;
    cycles += 2;
}

void CPU::TXA()
{
    A = X;
    set_ZN(A);
    cycles += 2;
}

void CPU::TXS()
{
    SP = X;
    cycles += 2;
}

void CPU::TAX()
{
    X = A;
    set_ZN(X);
    cycles += 2;
}

void CPU::TSX()
{
    X = SP;
    set_ZN(X);
    cycles += 2;
}

void CPU::DEX()
{
    X = X-1;
    set_ZN(X);
    cycles += 2;
}