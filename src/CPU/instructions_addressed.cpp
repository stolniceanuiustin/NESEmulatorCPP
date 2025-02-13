#include "../include/cpu_header.h"

// Here will be all the helper functions for emulating the instructions

void CPU::set_ZN(byte value)
{
    Z = (value == 0) ? 1 : 0;
    N = (value & 0x80) ? 1 : 0; //Checks bit 7 if it's set or not
}

// GROUP 1 INSTRUCTIOS
void CPU::ORA(uint16_t address, bool page_cross)
{
    // OR between accumulator and the contents at the given address
    const int lookup[] = {6, 3, 2, 4, 5, 4, 4, 4};
    A = A | read(address);
    set_ZN(A);

    cycles += lookup[inst.bbb] + (int)page_cross;
}

void CPU::AND(uint16_t address, bool page_cross)
{
    const int lookup[] = {6, 3, 2, 4, 5, 4, 4, 4};
    byte operand = read(address);
    A = A & operand;
    set_ZN(A);

    cycles += lookup[inst.bbb] + (int)page_cross;
}

void CPU::EOR(uint16_t address, bool page_cross)
{
    const int lookup[] = {6, 3, 2, 4, 5, 4, 4, 4};
    A = A ^ read(address);
    set_ZN(A);

    cycles += lookup[inst.bbb] + (int)page_cross;
}

void CPU::ADC(uint16_t address, bool page_cross)
{
    // TODO: TEST THIS
    const int lookup[] = {6, 3, 2, 4, 5, 4, 4, 4};
    byte operand = read(address);
    uint16_t result = (uint16_t)A + (uint16_t)(operand) + (uint16_t)(C);
    C = result > 0x00FF ? 1 : 0; 
    bool overflow = ~(A ^ operand) & (A ^ result) & 0x80;
    O = overflow ? 1 : 0;
    A = (byte)(result & 0x00FF);
    set_ZN(A);

    cycles += lookup[inst.bbb] + (int)page_cross;
}

void CPU::STA(uint16_t address)
{
    // Can't use IMEDIATE ADDRESSING
    // Stores the contents of the accumulator in memory
    // Doesnt change flags
    const int lookup[] = {6, 3, -1, 4, 6, 4, 5, 5};
    write(address, A);
    cycles += lookup[inst.bbb];
}

void CPU::LDA(uint16_t address, bool page_cross)
{
    //Loads A from memory
    const int lookup[] = {6, 3, 2, 4, 5, 4, 4, 4};
    A = read(address);
    set_ZN(A);
    cycles += lookup[inst.bbb] + (int)page_cross;
}

void CPU::CMP(uint16_t address, bool page_cross)
{
    //TODO not sure if the flags are set correctly but i think they are, will find out in unit testing
    const int lookup[] = {6, 3, 2, 4, 5, 4, 4, 4};

    uint16_t result = A - read(address);
    //cpu->SR &= ~(CARRY | NEGATIVE | ZERO);
    C = !(result & 0xFF00) ? 1 : 0;
    set_ZN(result);
    cycles += lookup[inst.bbb] + (int)page_cross;
}

void CPU::SBC(uint16_t address, bool page_cross)
{
    // TODO: TEST THIS
    const int lookup[] = {6, 3, 2, 4, 5, 4, 4, 4};
    byte operand = read(address);
    operand = ~operand; 
    uint16_t result = (uint16_t)A + (uint16_t)(operand) + (uint16_t)(C);     //Accumulator + address + carry
    
    C = result > 0x00FF ? 1 : 0;    //if more than 1 byte => carry
    bool overflow = ~(A ^ operand) & (A ^ result) & 0x80;
    O = overflow ? 1 : 0;
    A = (byte)(result & 0x00FF);
    set_ZN(A);

    cycles += lookup[inst.bbb] + (int)page_cross;
}


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
        byte operand = read(address);
        uint16_t carry_flag = operand & (1 << 7);
        if (carry_flag)
            C = 1;
        else
            C = 0;
        write(address, operand << 1);
        set_ZN(read(address));
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
        byte operand = read(address);
        uint16_t carry_flag = operand & (1 << 7);
        operand = operand << 1;
        operand &= ~1;
        operand |= C;
        write(address, operand);
        if (carry_flag)
            C = 1;
        else
            C = 0;
        set_ZN(operand);
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
        byte operand = read(address);
        uint16_t carry_flag = operand & 1;
        operand = operand >> 1;
        operand &= ~(1 << 7);
        write(address, operand);
        if (carry_flag)
            C = 1;
        else
            C = 0;
        set_ZN(operand);
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
        byte operand = read(address);
        int carry_flag = operand & 1;

        operand = operand >> 1;
        operand &= ~(1 << 7);
        operand |= C << 7;
        write(address, operand);

        if (carry_flag)
            C = 1;
        else
            C = 0;
        set_ZN(operand);
    }

    cycles += lookup[inst.bbb];
}
void CPU::STX(uint16_t address)
{
    int lookup[] = {-1, 3, -1, 4, -1, 4, -1, -1};
    write(address, X);
    cycles += lookup[inst.bbb];
}
void CPU::LDX(uint16_t address, bool page_cross)
{
    int lookup[] = {2, 3, -1, 4, -1, 4, -1, 4};
    X = read(address);
    set_ZN(X);
    cycles += lookup[inst.bbb] + (int)page_cross;
}
void CPU::DEC(uint16_t address)
{
    int lookup[] = {-1, 5, -1, 6, -1, 6, -1, 7};
    byte operand = read(address);
    operand -= 1;
    write(address, operand);
    set_ZN(operand);
    cycles += lookup[inst.bbb];
}
void CPU::INC(uint16_t address)
{
    int lookup[] = {-1, 5, -1, 6, -1, 6, -1, 7};
    byte operand = read(address);
    operand += 1;
    write(address, operand);
    set_ZN(operand);
    cycles += lookup[inst.bbb];
}


void CPU::BIT(uint16_t address)
{
    // bit test, test if one or more bits are in a target memory location
    // mask patern in A is & with memory to keep zero, overflow, negative etc.
    int lookup[] = {-1, 3, -1, 4, -1, -1, -1, -1};
    uint8_t result = A & read(address);
    if (result == 0)
        Z = 1;
    else
        Z = 0;
    uint8_t negative = read(address) & (1 << 7);
    if (negative)
        N = 1;
    else
        N = 0;
    uint8_t overflow = read(address) & (1 << 6);
    if (overflow)
        O = 1;
    else
        O = 0;

    cycles += lookup[inst.bbb];
}

void CPU::JMP_abs(uint16_t jump_address)
{
    PC = jump_address;
    cycles += 3;
}

void CPU::JMP_indirect(uint16_t jump_address)
{
    // TODO: ADDRESS BUG FROM ORIGINAL 6502(not a bug in my code)
    //PC = read_abs_address(jump_address);
    uint16_t aux_address = 0;
    byte low_byte = read(jump_address);
    uint16_t high_byte_of_addr = jump_address & 0xFF00;
    byte high_byte = read(((jump_address + 1) & 0x00FF) | high_byte_of_addr);
    aux_address = (high_byte << 8) | low_byte;
    PC = aux_address;
    cycles += 5;
}

void CPU::STY(uint16_t address)
{
    int lookup[] = {-1, 3, -1, 4, -1, 4, -1, -1};
    write(address, Y);
    cycles += lookup[inst.bbb];
}

void CPU::LDY(uint16_t address, bool page_cross)
{ // immediate, zeropage, nothig, absolut,nothing, zero page x, nothing, absolut x
    int lookup[] = {2, 3, -1, 4, -1, 4, -1, 4};
    Y = read(address);
    cycles += lookup[inst.bbb] + (int)page_cross;
    set_ZN(Y);
}

void CPU::CPY(uint16_t address)
{
    int lookup[] = {2, 3, -1, 4, -1, -1, -1, -1};
    byte operand = read(address);  
    byte result_byte = Y - operand;
    C = Y >= operand ? 1 : 0;
    set_ZN(result_byte);
    

    cycles += lookup[inst.bbb];
}

void CPU::CPX(uint16_t address)
{
    //TODO FIX THIS THE SAME AS CPY
    int lookup[] = {2, 3, -1, 4, -1, -1, -1, -1};
    byte operand = read(address);
    byte result_byte = X - operand;
    C = X >= operand ? 1 : 0;
    set_ZN(result_byte);
    

    cycles += lookup[inst.bbb];
}