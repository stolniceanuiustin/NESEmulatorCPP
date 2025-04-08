#include "../include/cpu_header.h"

int CPU::estimate_cycles_group_sb1()
{
    switch (inst.opcode)
    {
    case 0x08:
        return 3;
    case 0x28:
        return 4;
    case 0x48:
        return 3;
    case 0x68:
        return 4;
    case 0x88:
        return 2;
    case 0xA8:
        return 2;
    case 0xC8:
        return 2;
        break;
    case 0xE8:
        return 2;
        break;
    case 0x18:
        return 2;
        break;
    case 0x38:
        return 2;
        break;
    case 0x58:
        return 2;
        break;
    case 0x78:
        return 2;
        break;
    case 0x98:
        return 2;
        break;
    case 0xB8:
        return 2;
        break;
    case 0xD8:
        return 2;
    case 0xF8:
        return 2;
        break;
    default:
        std::cerr << "ILLEGAL";
    }
}

int CPU::estimate_cycles_group_sb2()
{
    switch (inst.opcode)
    {
    case 0x8a:
        return 2;
        break;
    case 0x9A:
        return 2;
        break;
    case 0xAA:
        return 2;
        break;
    case 0xBA:
        return 2;
        break;
    case 0xCA:
        return 2;
        break;
    case 0xEA:
        return 2;
        break;
    default:
        return -1;
    }
}

int CPU::estimate_cycles()
{
    byte last_5_bits = (0b00011111 & inst.opcode);
    byte low_nibble = inst.opcode & 0x0F;
    byte high_nibble = inst.opcode >> 4;
    if (inst.opcode == 0x04 || inst.opcode == 0x44 || inst.opcode == 0x64)
    {
        return 3;
    }
    else if (inst.opcode == 0x0C)
    {
        return 4;
    }
    else if (inst.opcode == 0x14 || inst.opcode == 0x34 || inst.opcode == 0x54 || inst.opcode == 0x74 || inst.opcode == 0xD4 || inst.opcode == 0xF4)
    {
        return 4;
    }

    else if (low_nibble == 0x00 && last_5_bits == 0b00010000)
    {
        // Branching instructions
        // read but dont increment!
        uint16_t aux_pc2 = PC;
        int8_t branch_position = (int8_t)read(aux_pc2);
        aux_pc2++;
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
            aux_pc2 += branch_position;
            page_cross = (aux_pc & 0xFF00) != (aux_pc2 & 0xFF00);
        }
        return (2 + (int)branch_succeded + 2 * (int)page_cross);
    }

    else if (inst.opcode == 0x00)
    {
        return 7;
        // brk = forced interrupt.
    }
    else if (inst.opcode == 0x20)
    {
        return 6;
    }
    else if (inst.opcode == 0x40)
    {
        return 6;
    }
    else if (inst.opcode == 0x60)
    {
        return 6;
    }
    // for single byte instructions !
    else if (low_nibble == 0x08)
    {
        return estimate_cycles_group_sb1();
    }
    else if (low_nibble == 0x0A && high_nibble >= 0x08)
    {
        return estimate_cycles_group_sb2();
    }
    else
        switch (inst.cc)
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

    return 1;
}
return 1;
}
