#include "cpu_header.h"
#include <iostream>

using std::cout;

const byte last_byte = 0xFF; //00...011111111

uint16_t CPU::compute_addr_mode_g1(bool &page_cross)
{
    uint16_t address = 0; // The first byte must be 0
    page_cross = false;
    switch (inst.bbb)
    {
    case 0x0: //(ZERO PAGE, X). Next byte + X represents an adress in zero page that needs to be refferenced
    {
        address = read_pc() + X;
        address &= last_byte; // The first byte must be 0
        address = read_address(address);
        break;
    }
    case 0x01: // ZERO PAGE.
    {
        address = read_pc();
        break;
    }
    case 0x02: // Immediate. The opperand is in the next byte of memory
    {
        //FIXED THIS
        address = PC;
        PC += 1;
        break;
    }
    case 0x03: // Absolute. The full 16-bit address
    {
        address = read_abs_address(PC);
        PC += 2;
        break;
    }
    case 0x04: //(zero page), Y; Takes an address from zero page as a pointer then adds Y to that address
    {          // CAN CROSS PAGES
        uint16_t alligment = 0;
        address = read_pc() + alligment;
        address &= last_byte;
        address = read_address(address);
        if (address + Y > last_byte) // We crossed a page
            page_cross = true;
        address += Y;
        break;
    }
    case 0x05: // zero page, X; basically zeropage_addres + x
    {
        address = read_pc() + X;
        address &= last_byte;
        break;
    }
    case 0x06: // absolute, Y; The full 16-bit address is in memory and we have to add Y
    {          // CAN CROSS PAGES
        // TODO: im not sure this is right
        // IT SHOULD WORK BECAUSE IT READS AN ADDRESS(16 bits)
        address = read_address(PC);
        int first_digit_address = address >> 12; // We get the first 4 bits -> one digit in Hexa
        PC += 2;
        address += Y;
        int first_digit_address_after_increment = address >> 12;
        if (first_digit_address < first_digit_address_after_increment)
            page_cross = true;
        break;
    }
    case 0x07: // Absolute, X. the same as before
        // CAN CROSS PAGES
        {
            address = read_address(PC);
            int first_digit_address_1 = address >> 12;
            PC += 2;
            address += X;
            int first_digit_address_after_increment_1 = address >> 12;
            if (first_digit_address_1 < first_digit_address_after_increment_1)
                page_cross = true;
            break;
        }
    }
    return address;

}

bool CPU::compute_addr_mode_g23(bool &page_cross, uint16_t &address_to_return)
{
    uint16_t address = 0;
    page_cross = false;
    if (inst.opcode != 0x004C && inst.opcode != 0x0060) // EXCLUDE THE JUMPS;
    {
        switch (inst.bbb)
        {
        case 0x0: // #immediate: literally the next byte in memory
        {
            address = PC++;
            break;
        }
        case 0x1: // zero page: an adress in range 0x0000 - 0x0FFF
        {
            address = read_pc();
            break;
        }
        case 0x2: // accumulator. there is no address
            return false;

        case 0x3: // absolute
        {
            address = read_abs_address(PC);
            PC += 2;
            break;
        }
        case 0x4:
        {
            cout << "INVALID OPCODE\n";
            break;
        }
        case 0x5: // zero page, x
        {         // STX, LDX use zero page, y not x
            bool is_using_y = (inst.aaa == 0x4 || inst.aaa == 0x5) && inst.cc == 0x2;
            byte to_add = is_using_y ? Y : X;

            address = read_pc() + to_add;
            address &= last_byte;

            break;
        }
        case 0x6:
        {
            cout << "Invalid Opcode \n";
            break;
        }
        case 0x7: // absolute, x
        {         // THIS could be more compact but i don't want to mess with the pagecross logic
            address = read_address(PC);
            PC += 2;
            // LTX: absolute, y instead of X
            if (inst.aaa == 0x7 && inst.cc == 0x2)
            {
                int first_digit_address = address >> 12; // We get the first 4 bits -> one digit in Hexa
                address += Y;
                int first_digit_address_after_increment = address >> 12;
                if (first_digit_address < first_digit_address_after_increment)
                    page_cross = true;
                break;
            }
            else
                address += X;
            break;
        }
        }
    }
    address_to_return = address;
    return true;
}