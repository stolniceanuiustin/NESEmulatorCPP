#include "cpu_header.h"
#include "tracer.h"
#include <iostream>
#include <stdint.h>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
using std::cout;
using std::string;
using std::getline;

std::ifstream logs("nestest.log");

// retruns length of operand!
byte compute_operand_length_g1(byte bbb)
{
    switch (bbb)
    {
    case 0x0: //(ZERO PAGE, X). Next byte + X represents an adress in zero page that needs to be refferenced
    {
        return 1;
        break;
    }
    case 0x01: // ZERO PAGE.
    {
        return 1;
        break;
    }
    case 0x02: // Immediate. The opperand is in the next byte of memory
    {
        return 1;
        break;
    }
    case 0x03: // Absolute. The full 16-bit address
    {
        return 2;
        break;
    }
    case 0x04: //(zero page), Y; Takes an address from zero page as a pointer then adds Y to that address
    {          // CAN CROSS PAGES
        return 1;
        break;
    }
    case 0x05: // zero page, X; basically zeropage_addres + x BUT CANT CROSS PAGE
    {
        return 1;
        break;
    }
    case 0x06: // absolute, Y; The full 16-bit address is in memory and we have to add Y
    {          // CAN CROSS PAGES
        return 2;
        break;
    }
    case 0x07: // Absolute, X. the same as before
        // CAN CROSS PAGES
        {
            return 2;
            break;
        }
    }
    return 0;
}

string compute_instruction_name_group1(byte aaa)
{
    switch (aaa)
    {
    case 0x0:
        return "ORA";
        break;
    case 0x1:
        return "AND";
        break;
    case 0x2:
        return "EOR";
        break;
    case 0x3:
        return "ADC";
        break;
    case 0x4:
        return "STA";
        break;
    case 0x5:
        return "LDA";
        break;
    case 0x6:
        return "CMP";
        break;
    case 0x7:
        return "SBC";
        break;
    }
}

byte compute_operand_length_g23(byte bbb, byte opcode)
{
    if (opcode != 0x004C && opcode != 0x0060) // EXCLUDE THE JUMPS;
    {
        switch (bbb)
        {
        case 0x0: // #immediate: literally the next byte in memory
        {
            return 1;
        }
        case 0x1: // zero page: an adress in range 0x0000 - 0x0FFF
        {
            return 1;
            break;
        }
        case 0x2: // accumulator. there is no address
            return 1;

        case 0x3: // absolute
        {
            return 2;
            break;
        }
        case 0x4:
        {
            std::cerr << "INVALID OPCODE\n";
            break;
        }
        case 0x5: // zero page, x
        {         // STX, LDX use zero page, y not x
            return 1;
            break;
        }
        case 0x6:
        {
            std::cerr << "INVALID OPCODE\n";
            break;
        }
        case 0x7: // absolute, x
        {         // THIS could be more compact but i don't want to mess with the pagecross logic
            return 2;
        }
        }
    }
    if (opcode == 0x004C || opcode == 0x006C)
        return 2;

    return 0;
}

string compute_instruction_name_group2(byte aaa)
{
    switch (aaa)
    {
    case 0x0:
        return "ASL";
        break;
    case 0x1:
        return "ROL";
        break;
    case 0x2:
        return "LSR";
        break;
    case 0x3:
        return "ROR";
        break;
    case 0x4:
        return "STX";
        break;
    case 0x5:
        return "LDX";
        break;
    case 0x6:
        return "DEC";
        break;
    case 0x7:
        return "INC";
        break;
    }
    return "";
}

string compute_instruction_name_group3(byte aaa, string &observations)
{
    uint16_t jump_address = 0;
    switch (aaa)
    {
    case 0x0:
        return "INVALID OPCODE";
        break;
    case 0x1:
        return "BIT";
        break;
    case 0x2:
        observations = "JMPabs";
        return "JMP";
        break;
    case 0x3:
        observations = "JMPind";
        return "JMP";
        break;
    case 0x4:
        return "STY";
        break;
    case 0x5:
        return "LDY";
        break;
    case 0x6:
        return "CPY";
        break;
    case 0x7:
        return "CPX";
        ;
        break;
    }
    return "";
}

void TRACER::tracer(uint16_t PC, byte FLAGS)
{
    // ADDRESSING MODES AT https://llx.com/Neil/a2/opcodes.html
    // LOG: Address, OPCODE, INST, A:, X:, Y:, P:, SP:
    // cout << std::hex << cpu.get_pc() << " " <<  cpu.get_inst_opcode();
    
    string observations = "";
    Instruction inst;
    inst.opcode = cpu.ram_at(PC);
    inst.aaa = (0xE0 & inst.opcode) >> 5;      // first 3 bits of the opcode
    inst.bbb = (0x1C & inst.opcode) >> 2;      // second 3 bits
    inst.cc = (0x03 & inst.opcode);            // last 2 bits
    inst.xx = (0b11000000 & inst.opcode) >> 6; // first 2 bits(xx)
    inst.y = (0b00100000 & inst.opcode) >> 5;  // third bit from the left;
    byte last_5_bits = (0b00011111 & inst.opcode);
    byte low_nibble = inst.opcode & 0x0F;
    byte high_nibble = inst.opcode >> 4;
    uint16_t address = 0;

    std::string instruction_name = "UNKNOWN";

    byte instruction_length = 1; // IT CAN BE 1, 2, 3 AT MOST
    bool branch_instruction = false;
    if (low_nibble == 0x00 && last_5_bits == 0b00010000)
    {
        branch_instruction = true;
        instruction_length = 2;
        // here we have branching
        int8_t branch_position = (int8_t)cpu.ram_at(PC + 1);
        bool branch_succeded = false;
        bool page_cross = false;
        uint16_t aux_pc = PC;
        if (inst.xx == 0b00)
        {
            if (cpu.N == inst.y)
            {
                branch_succeded = true;
            }
            instruction_name = (inst.y == 0) ? "BPL" : "BMI";
        }
        else if (inst.xx == 0b01)
        {
            if (cpu.O == inst.y)
            {
                branch_succeded = true;
            }
            instruction_name = (inst.y == 0) ? "BVC" : "BVS";
        }
        else if (inst.xx == 0b10)
        {
            if (cpu.C == inst.y)
            {
                branch_succeded = true;
            }
            instruction_name = (inst.y == 0) ? "BCC" : "BCS";
        }
        else if (inst.xx == 0b11)
        {
            if (cpu.Z == inst.y)
            {
                branch_succeded = true;
            }
            instruction_name = (inst.y == 0) ? "BNE" : "BEQ";
        }
        // if (branch_succeded)
        // {
        //     PC += branch_position;
        // }
    }

    else if (inst.opcode == 0x00)
    {
        instruction_name = "BRK";
        instruction_length = 1;
    }
    else if (inst.opcode == 0x20)
    {
        // TODO CHECK THIS: the address on the stack points to the last byte of jump instruction or so im told
        instruction_name = "JSR";
        instruction_length = 1;
    }
    else if (inst.opcode == 0x40)
    {
        instruction_name = "RTI";
        instruction_length = 1;
    }
    else if (inst.opcode == 0x60)
    {
        instruction_name = "RTS";
        instruction_length = 1;
    }
    // for single byte instructions !
    else if (low_nibble == 0x08)
    {
        instruction_length = 1;
        switch (inst.opcode)
        {
        case 0x08:
            instruction_name = "PHP";
            break;
        case 0x28:
            instruction_name = "PLP";
            break;
        case 0x48:
            instruction_name = "PHA";
            break;
        case 0x68:
            instruction_name = "PLA";
            break;
        case 0x88:
            instruction_name = "DEY";
            break;
        case 0xA8:
            instruction_name = "TAY";
            break;
        case 0xC8:
            instruction_name = "INY";
            break;
        case 0xE8:
            instruction_name = "INX";
            break;
        case 0x18:
            instruction_name = "CLC";
            break;
        case 0x38:
            instruction_name = "SEC";
            break;
        case 0x58:
            instruction_name = "CLI";
            break;
        case 0x78:
            instruction_name = "SEI";
            break;
        case 0x98:
            instruction_name = "TYA";
            break;
        case 0xB8:
            instruction_name = "CLV";
            break;
        case 0xD8:
            instruction_name = "CLD";
            break;
        case 0xF8:
            instruction_name = "SED";
            break;
        default:
            std::cerr << "ILLEGAL";
        }
    }
    else if (low_nibble == 0x0A && high_nibble >= 0x08)
    {
        instruction_length = 1;
        switch (inst.opcode)
        {
        case 0x8a:
            instruction_name = "TXA";
            break;
        case 0x9A:
            instruction_name = "TXS";
            break;
        case 0xAA:
            instruction_name = "TAX";
            break;
        case 0xBA:
            instruction_name = "TSX";
            break;
        case 0xCA:
            instruction_name = "DEX";
            break;
        case 0xEA:
            instruction_name = "NOP";
            break;
        default:
            break;
        }
    }
    else
        switch (inst.cc)
        {
        // compute_addr_mode DOES return an address via reffrence(&)
        case 0x01: // cc = 1
            instruction_length += compute_operand_length_g1(inst.bbb);
            instruction_name = compute_instruction_name_group1(inst.aaa);
            break;
        case 0x02: // cc = 10
            // Will return address via pointer, the function returns a boolean.
            instruction_length += compute_operand_length_g23(inst.bbb, inst.opcode);
            instruction_name = compute_instruction_name_group2(inst.aaa);
            break;
        case 0x0: // cc = 00
            instruction_length += compute_operand_length_g23(inst.bbb, inst.opcode);
            instruction_name = compute_instruction_name_group3(inst.aaa, observations);
            break;
        }

    cout << std::hex << PC << " ";
    std::stringstream ss;
    ss << std::uppercase << std::hex << PC;

    string address_string = ss.str();
    if (instruction_length == 1)
    {
        cout << std::hex << std::uppercase
                   << (int)cpu.ram_at(PC) << std::setw(7) << std::setfill(' ');
    }
    else if (instruction_length == 2)
    {
        cout << std::hex << std::uppercase
                  << std::setw(2) << std::setfill(' ') << (int)cpu.ram_at(PC) << " "
                  << std::setw(2) << std::setfill(' ') << (int)cpu.ram_at(PC + 1)
                  << std::setw(4) << std::setfill(' ');
    }
    else if (instruction_length == 3)
    {
        cout << std::hex << std::uppercase
                  << std::setw(2) << std::setfill(' ') << (int)cpu.ram_at(PC) << " "
                  << std::setw(2) << std::setfill(' ') << (int)cpu.ram_at(PC + 1) << " "
                  << std::setw(2) << std::setfill(' ') << (int)cpu.ram_at(PC + 2);
    }
    cout << " ";
    cout << instruction_name << " ";
    cout << std::hex << std::uppercase << "A:" << std::setw(2) << (int)cpu.get_A() << " X:" 
         << std::setw(2) << (int)cpu.get_X() << " Y:" << std::setw(2) << (int)cpu.get_Y() << " P:"
         << std::setw(2) << (int)FLAGS << " SP:" << (int)cpu.get_SP();
    if(!logs)
    {
        cout << "could not open correct log file";
    }

    

    string log_entry;
    getline(logs, log_entry);
    int flag_pos = log_entry.find("P:");
    string log_address = log_entry.substr(0, log_entry.find(" "));
    if(flag_pos == string::npos)
    {
        cout << "Invalid log entry" << '\n';
        return;
    }
    flag_pos += 2;
    string p_value_str = log_entry.substr(flag_pos, 2);
    int p_value;
    std::stringstream(p_value_str) >> std::hex >> p_value;
    
    if(log_address.compare(address_string))
    {
        cout << " address problem here!";
    }
    if(p_value != FLAGS)
    {
        cout << " flag problem here!";
    }

    cout << '\n';
    int t = 0;
}