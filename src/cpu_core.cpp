#include "cpu_header.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <cstring>
using std::cout;

const uint16_t null_address = 0;
// byte CPU::read_byte(byte* address)
// {
//     // sleep(CLOCK_TIME);
//     return (*address);
// }

void CPU::push(byte x)
{
    //Stack overflow should handle itself
    ram[0x0100 + SP] = x;
    SP--;
}

byte CPU::pop()
{
    byte to_return = ram[0x0100 + SP];
    SP++;
    return to_return;
}

void CPU::write_byte(byte *address, byte value)
{
    // sleep(CLOCK_TIME);
    // sleep(0.1);
    *address = value;
    return;
}

uint16_t CPU::read_address(byte offset)
{
    uint16_t val = ram[offset + 1]; // little endian
    val <<= 8;
    val |= ram[offset];
    return val;
}

// The difference between read_address and read_abs_address is that read_abs_address takes a 16bit offset
uint16_t CPU::read_abs_address(uint16_t offset)
{
    uint16_t val = ram[offset + 1]; // little endian
    val <<= 8;
    val |= ram[offset];
    return val;
}

uint16_t CPU::read_address_from_pc()
{
    uint16_t address = read_abs_address(PC);
    PC += 2;
    return address;
}

bool CPU::reset()
{
    A = 0;
    X = 0;
    Y = 0;
    SP = 0xFF; //it decreases with pushing 
    PC = 0;
    C = 0; // carry
    Z = 0; // zero
    I = 1;
    D = 0;
    B = 0;
    O = 0;
    N = 0;
    for (int i = 0; i < 0xFFFF; i++)
        ram[i] = 0;
    cycles = 0;
    return true;
}

void CPU::run_instruction_group1(uint16_t address, bool page_cross)
{
    switch (inst.aaa)
    {
    case 0x0:
        ORA(address, page_cross);
        break;
    case 0x1:
        AND(address, page_cross);
        break;
    case 0x2:
        EOR(address, page_cross);
        break;
    case 0x3:
        ADC(address, page_cross);
        break;
    case 0x4:
        STA(address);
        break;
    case 0x5:
        LDA(address, page_cross);
        break;
    case 0x6:
        CMP(address, page_cross);
        break;
    case 0x7:
        SBC(address, page_cross);
        break;
    }
}

void CPU::run_instruction_group2(uint16_t address, bool page_cross, bool accumulator)
{
    switch (inst.aaa)
    {
    case 0x0:
        ASL(address, accumulator);
        break;
    case 0x1:
        ROL(address, accumulator);
        break;
    case 0x2:
        LSR(address, accumulator);
        break;
    case 0x3:
        ROR(address, accumulator);
        break;
    case 0x4:
        STX(address);
        break;
    case 0x5:
        LDX(address, page_cross);
        break;
    case 0x6:
        DEC(address);
        break;
    case 0x7:
        INC(address);
        break;
    }
    return;
}

void CPU::run_instruction_group3(uint16_t address, bool page_cross)
{
    uint16_t jump_address = 0;
    switch (inst.aaa)
    {
    case 0x0:
        printf("INVALID OPCODE \n");
        break;
    case 0x1:
        BIT(address);
        break;
    case 0x2:
        jump_address = read_address_from_pc();
        JMP_abs(jump_address);
        break;
    case 0x3:
        jump_address = read_address_from_pc();
        JMP_indirect(jump_address);
        break;
    case 0x4:
        STY(address);
        break;
    case 0x5:
        LDY(address, page_cross);
        break;
    case 0x6:
        CPY(address);
        break;
    case 0x7:
        CPX(address);
        break;
    }
    return;
}

void CPU::run_instruction_group_sb1()
{
    if(inst.opcode == 0x08)
            PHP();
        if(inst.opcode == 0x28)
            PLP();
        if(inst.opcode == 0x48)
            PHA();
        if(inst.opcode == 0x68)
            PLA();
        if(inst.opcode == 0x88)
            DEY();
        if(inst.opcode == 0xA8)
            TAY();
        if(inst.opcode == 0xC8)
            INY();
        if(inst.opcode == 0xE8)
            INX();
        if(inst.opcode ==0x18)
            CLC();
        if(inst.opcode == 0x38)
            SEC();
        if(inst.opcode == 0x58)
            CLI();
        if(inst.opcode == 0x78)
            SEI();
        if(inst.opcode == 0x98)
            TYA();
        if(inst.opcode == 0xB8)
            CLV();
        if(inst.opcode == 0xD8)
            CLD();
        if(inst.opcode == 0xF8)
            SED();
}

void CPU::run_instruction_group_sb2()
{
    switch (inst.opcode)
    {
        case 0x8a:
            TXA();
            break;
        case 0x9A:
            TXS();
            break;
        case 0xAA:
            TAX();
            break;
        case 0xBA:
            TSX();
            break;
        case 0xCA:
            DEX();
            break;
        case 0xEA:
            NOP();
            break;
        default:
            break;
    }
}

bool CPU::init(Config config)
{
    std::ifstream rom(config.rom_name, std::ios::binary);
    if(!rom)
    {
        std::cerr << "Could not open ROM FILE\n";
        return false;
    }
    NESHeader header;
    rom.read(reinterpret_cast<char*>(&header), sizeof(header));
    if (header.magic[0] != 'N' || header.magic[1] != 'E' || header.magic[2] != 'S' || header.magic[3] != 0x1A) {
        std::cerr << "Invalid NES file: Missing iNES header.\n";
        return 1;
    }

    int prg_size = header.prg_size * 16 * 1024;
    int chr_size = header.chr_size * 8 * 1024;

    rom.read(reinterpret_cast<char*>(&ram[0x8000]), prg_size);
    rom.close();

    FILE* hexdumpfile = fopen("hexdump", "wb");
    if(!hexdumpfile)
    {
        return false;
    }
    
    if(prg_size == 0x4000)
    {
        std::memcpy(&ram[0xC000], &ram[0x8000], 0x4000);
    }
    fwrite(ram, sizeof(byte), 0xFFFF, hexdumpfile);
    PC = config.code_segment;
    return true;
}

//RUNS ONE OPCODE
int CPU::execute()
{
    //TODO ADD ELSE IFS!
    bool onaddress_group2 = false;
    uint16_t original_pc = PC;
    bool page_cross = false;

    if (original_pc == 0xFFFF)
    {
        cout << "End of program";
        // cpu->state = QUIT;
        return -1;
    }
    inst.opcode = read_pc();
    //PC += 1; READ PC ALREADY DOES INCREMENT PC BY ONE!
    inst.aaa = (0xE0 & inst.opcode) >> 5;      // first 3 bits of the opcode
    inst.bbb = (0x1C & inst.opcode) >> 2;      // second 3 bits
    inst.cc = (0x03 & inst.opcode);            // last 2 bits
    inst.xx = (0b11000000 & inst.opcode) >> 6; // first 2 bits(xx)
    inst.y = (0b00100000 & inst.opcode) >> 5;  // third bit from the left;
    byte last_5_bits = (0b00011111 & inst.opcode);
    byte low_nibble = inst.opcode & 0x0F;
    byte high_nibble = inst.opcode >> 4;
    uint16_t address = 0; // Offset of ram. no longer doing pointer arithmetic
    
    cout << "Running opcode: " << std::hex << (int)inst.opcode << "\n";
    
    if (low_nibble == 0x00 && last_5_bits == 0b00010000)
    {
        // here we have branching
        int8_t branch_position = (int8_t)read_pc();
        bool branch_succeded = false;
        bool page_cross = false;
        uint16_t aux_pc = PC;
        if (inst.xx == 0b00 && N == inst.y)
            branch_succeded = true;
        else if (inst.xx == 0b01 && O == inst.y)
            branch_succeded = true;
        else if (inst.xx == 0b10 && C == inst.y)
            branch_succeded = true;
        else if (inst.xx == 0b11 && Z == inst.y)
            branch_succeded = true;
        
        if (branch_succeded)
        {
            PC += branch_position;
            // pagecross check
            page_cross = (aux_pc & 0xFF00) != (PC & 0xFF00);
        }
        cycles += 2 + (int)branch_succeded + 2 * (int)page_cross;
    }
    
    else if(inst.opcode == 0x00)
    {
        BRK();
        // brk = forced interrupt.
        // will handle interupts later, for now i have to do the single byte instrucftions !
    }
    else if(inst.opcode == 0x20)
    {
        //TODO CHECK THIS: the address on the stack points to the last byte of jump instruction or so im told
        address = read_abs_address(PC);
        PC += 1;
        JSR_abs(address);
    }
    else if(inst.opcode == 0x40)
    {
        //RTI() //return from interrupt;
    }
    else if(inst.opcode == 0x60)
    {
        RTS();//return from subroutine;
    }
    // for single byte instructions !
    else if (low_nibble == 0x08)
    {
        run_instruction_group_sb1();
    }
    else if (low_nibble == 0x0A && high_nibble >= 0x08)
    {
        run_instruction_group_sb2();
    }
    else switch (inst.cc)
    {
    // compute_addr_mode DOES return an address via reffrence(&)
    case 0x01: // cc = 1
        address = compute_addr_mode_g1(page_cross);
        run_instruction_group1(address, page_cross);
        break;
    case 0x02: // cc = 10
        // Will return address via pointer, the function returns a boolean.
        onaddress_group2 = compute_addr_mode_g23(page_cross, address);
        if (onaddress_group2 == true)
            run_instruction_group2(address, page_cross, 0); // Not accumulator, on address
        else
            run_instruction_group2(null_address, page_cross, 1); // On accumulator
        break;
    case 0x0: // cc = 00
        compute_addr_mode_g23(page_cross, address);
        run_instruction_group3(address, page_cross);
        break;
    }

    // TODO: check if this is the correct way to call tracer
    // TRACER my_tracer(*this);
    // my_tracer.tracer(offset_address, page_cross, original_pc, onaddress_group2);
    return 1;
}