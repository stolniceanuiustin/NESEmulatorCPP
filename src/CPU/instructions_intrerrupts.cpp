#include "cpu_header.h"

void CPU::NOP()
{
    cycles += 2;
}

void CPU::JSR_abs(uint16_t address)
{
    push_address(PC);
    PC = address;
    cycles += 6;
}

void CPU::RTS()
{
    uint16_t aux = pop_address();
    aux += 1;
    PC = aux;
    cycles += 6;
}

void CPU::BRK()
{
    trigger_irq();
    B = 1;
    cycles += 3;
}


void CPU::trigger_irq()
{
    if(I == 0) //interrupts enabled
    {
        SP = 0xFF;
        push_address(PC);
        // push((PC & 0xFF00) >> 8);
        // push((PC & 0x00FF));
        push(pack_flags());
        PC = read_abs_address(IRQ_vector);
        I = 1;
    }
}

void CPU::RTI()
{
    byte flags = pop();
    unpack_flags(flags);
    PC = pop_address();
    cycles += 6;
}