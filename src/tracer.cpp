// #include "cpu_header.h"
// #include <iostream>

// using std::cout;
// // WILL TRACE


// //TODO: CHANGE TRACER SO LOGS ARE MORE COMPACT
// byte read_byte_tracer(byte *address)
// {
//     // sleep(CLOCK_TIME);
//     return (*address);
// }

// //Reads an address(16 bits)
// uint16_t TRACER::read_address_tracer(uint16_t address)
// {
//     uint16_t val = cpu.ram_at(address+1);
//     val <<= 8;
//     val = cpu.ram_at(address);
//     return val;
// }

// void TRACER::print_flags_group1()
// {
//     cout << "C: " << cpu.C << " O: " << cpu.O << " Z: " << cpu.Z << " N: " << cpu.N;
// }

// void TRACER::trace_instruction_group1(uint16_t address)
// {
//     // PCR STANDS FOR PAGE CROSS, booolean
//     switch (cpu.inst.aaa)
//     {
//     case 0x0:
//         printf("ORA - OR with accumulator. Cycles:%01lu. Flags: ", cpu.get_cycles());
//         print_flags_group1();
//         break;
//     case 0x1:
//         printf("AND - A(%02d) and M(%02d). Cycles:%01lu. Flags: ", cpu.get_A(), cpu.ram_at(address), cpu.get_cycles());
//         print_flags_group1();
//         break;
//     case 0x2:
//         printf("EOR - XOR with accumulator. Cycles:%01lu. Flags: ", cpu.get_cycles());
//         print_flags_group1();
//         break;
//     case 0x3:
//         // ADC();
//         printf("ADC - Add A + M with C. Cycles:%01lu. Flags: ", cpu.get_cycles());
//         print_flags_group1();
//         break;
//     case 0x4:
//         // STA();
//         printf("STA - Store A at M. Cycles:%01lu. Flags: ", cpu.get_cycles());
//         print_flags_group1();
//         break;
//     case 0x5:
//         // LDA();
//         printf("LDA - Load A(%02X) from M. Cycles:%01lu. Flags: ", cpu.get_A(), cpu.get_cycles());
//         print_flags_group1();
//         break;
//     case 0x6:
//         // CMP();
//         printf("CMP - CMP A(%02X) with M(%02X). Cycles:%01lu. Flags: ", cpu.get_A(), cpu.ram_at(address), cpu.get_cycles());
//         print_flags_group1();
//         break;
//     case 0x7:
//         // SBC();
//         printf("SBC - Sub A with M with borrow. Cycles:%01lu. Flags: ", cpu.get_cycles());
//         print_flags_group1();
//         break;
//     default:
//         printf("Unimplemented opcode\n");
//     }
// }

// void TRACER::trace_instruction_group2(uint16_t address, bool onaddress_group2)
// {
//     // TODO: could add results but unsure if worth it. flags are more significant here i think.
//     switch (cpu.inst.aaa)
//     {
//     case 0x0:
//         if (onaddress_group2)
//         {
//             printf("ASL - Shift left arithmetic. Value(%02X), Result(%02X), Cycles:%01lu, Flags: ", cpu.ram_at(address), (cpu.ram_at(address) << 1), cpu.get_cycles());
//             print_flags_group1();
//         }
//         else
//         {
//             printf("ASL - Shift left arithmetic. A(%02X), Result(%02X), Cycles:%01lu, Flags: ", cpu.get_A(), cpu.get_A() << 1, cpu.get_cycles());
//             print_flags_group1();
//         }
//         break;
//     case 0x1:
//         if (onaddress_group2)
//         {
//             printf("ROL - Rotate left. Value(%02X), Cycles:%01lu, Flags: ", cpu.ram_at(address), cpu.get_cycles());
//             print_flags_group1();
//         }
//         else
//         {
//             printf("ROL - Rotate left. A(%02X), Cycles:%01lu, Flags: ", cpu.get_A(), cpu.get_cycles());
//             print_flags_group1();
//         }
//         break;
//     case 0x2:
//         if (onaddress_group2)
//         {
//             printf("LSR - Logic shift right. Value(%02X), Cycles:%01lu, Flags: ", cpu.ram_at(address), cpu.get_cycles());
//             print_flags_group1();
//         }
//         else
//         {
//             printf("LSR - Logic shift right. A(%02X), Cycles:%01lu, Flags: ", cpu.get_A(), cpu.get_cycles());
//             print_flags_group1();
//         }
//         break;
//     case 0x3:
//         if (onaddress_group2)
//         {
//             printf("ROR - Rotate right. Value(%02X), Cycles:%01lu, Flags: ", cpu.ram_at(address), cpu.get_cycles());
//             print_flags_group1();
//         }
//         else
//         {
//             printf("ROR - Rotate right. A(%02X), Cycles:%01lu, Flags: ", cpu.get_A(), cpu.get_cycles());
//             print_flags_group1();
//         }
//         break;
//     case 0x4:
//         printf("STX - Store X at address. X(%02X), Cycles:%01lu, Flags: Not affected\n", cpu.get_X(), cpu.get_cycles());
//         break;
//     case 0x5:
//         printf("LDX - Load X from address. X(%02X), Cycles:%01lu, Flags: ", cpu.get_X(), cpu.get_cycles());
//         print_flags_group1();
//         break;
//     case 0x6:
//         printf("DEC - Decrement one. Value before(%02X), Cycles:%01lu, Flags: ", cpu.ram_at(address), cpu.get_cycles());
//         print_flags_group1();
//         break;
//     case 0x7:
//         printf("INC - Increment one. Value before(%02X), Cycles:%01lu, Flags: ", cpu.ram_at(address), cpu.get_cycles());
//         print_flags_group1();
//         break;
//     }
// }

// void TRACER::trace_instruction_group3(uint16_t address)
// {
//     switch (cpu.inst.aaa)
//     {
//     case 0x0:
//         // printf("INVALID OPCODE \n");
//         break;
//     case 0x1:
//         printf("AND Test - A(%02X), Value(%02X), Cycles:%01lu, Flags: ", cpu.get_A(), cpu.ram_at(address), cpu.get_cycles());
//         print_flags_group1();
//         break;
//     case 0x2:
//         printf("JMP(Absolut) to (%04X). Cycles:%01lu, Flags: not affected\n", address, cpu.get_cycles());
//         break;
//     case 0x3:
//         printf("JMP(Indirect) to (%04X). Cycles:%01lu, Flags: not affected\n", read_address_tracer(address), cpu.get_cycles());
//         break;
//     case 0x4:
//         // STY
//         break;
//     case 0x5:
//         // LDY
//         break;
//     case 0x6:
//         // CPY
//         break;
//     case 0x7:
//         // CPX
//         break;
//     }
// }

// void TRACER::tracer(uint16_t address, bool page_cross, uint16_t original_pc,bool onaddress_group2)
// {
//     // ADDRESSING MODES AT https://llx.com/Neil/a2/opcodes.html
//     //printf("Address: %04X, Opcode: %02X, Instruction: ", PC, cpu.inst.opcode);
//     cout << cpu.get_pc() << " " <<  cpu.get_inst_opcode();
//     byte low_nibble = cpu.get_inst_opcode() & 0x0F;
//     byte high_nibble = cpu.get_inst_opcode() >> 4;
//     if (low_nibble == 0x08)
//     {
//         // Single byte instruction type 1
//         // run_insturction_sb1(cpu);
//     }
//     if (low_nibble == 0x0A && high_nibble >= 0x08)
//     {
//         // Single byte instruction type 2
//         // run_instruction_sb2(cpu);
//     }
//     switch (cpu.inst.cc)
//     {
//     case 0x01: // cc = 01
//         trace_instruction_group1(address);
//         break;

//     case 0x02:
//         trace_instruction_group2(address, onaddress_group2);
//         // printf("Unimplemented opcode\n");
//         //  address = decode_addrmode_g2(cpu);
//         //  run_insturction_group2(address, cpu);
//         break;

//     case 0x0:
//         trace_instruction_group3(address);
//         break;
//     default:
//         cout << "Unimplemented opcode\n";
//         break;
//     }
// }