#include "cpu_header.h"
#include "unittest.h"
#include <iostream>

using std::cout;

void test_ora(Assert& assert, CPU& cpu, byte operand1, byte operand2, byte expected_result)
{
    cpu.A = operand1;
    cpu.ram[0] = operand2;
    cpu.ORA(0, 0);
    assert.are_equal(expected_result, cpu.A);
}

void test_and(Assert& assert, CPU& cpu, byte operand1, byte operand2, byte expected_result)
{
    cpu.A = operand1;
    cpu.ram[0] = operand2;
    cpu.AND(0, 0);
    assert.are_equal(expected_result, cpu.A);
}

void test_adc(Assert& assert, CPU& cpu, byte operand1, byte operand2, byte expected_result, byte expected_overflow, byte expected_carry)
{
    cpu.reset();
    cpu.A = operand1;
    cpu.ram[0] = operand2;
    cpu.ADC(0, 0);
    assert.are_equal_flags(expected_result, cpu.A, expected_overflow, expected_carry, cpu);
}

void test_cmp(Assert& assert, CPU& cpu, byte operand1, byte operand2, byte expected_zero, byte expected_neg, byte expected_carry)
{
    cpu.reset();
    cpu.A = operand1;
    cpu.ram[0] = operand2;
    cpu.CMP(0, 0);
    assert.are_equal_flags(1, 1, expected_zero, expected_neg, expected_carry, cpu);
}

void test_sbc(Assert& assert, CPU& cpu, byte operand1, byte operand2, byte expected_result, byte expected_overflow, byte expected_carry)
{
    cpu.reset();
    cpu.A = operand1;
    cpu.ram[0] = operand2;
    cpu.SBC(0, 0);
    assert.are_equal_flags(expected_result, cpu.A, expected_overflow, expected_carry, cpu);
}

void test_ror(Assert& assert, CPU& cpu, byte operand, byte expected_result, byte expected_zero, byte expected_neg, byte expected_carry)
{
    //cpu.reset();
    cpu.A = operand;
    cpu.ROR(0, true);
    //assert.are_equal(expected_result, cpu.A);
    assert.are_equal_flags(expected_result, cpu.A, expected_zero, expected_neg, expected_carry, cpu);
}
void test_rol(Assert& assert, CPU& cpu, byte operand, byte expected_result, byte expected_zero, byte expected_neg, byte expected_carry)
{
    //cpu.reset();
    cpu.A = operand;
    cpu.ROL(0, true);
    //assert.are_equal(expected_result, cpu.A);
    assert.are_equal_flags(expected_result, cpu.A, expected_zero, expected_neg, expected_carry, cpu);
}

void test_asl(Assert& assert, CPU& cpu, byte operand, byte expected_result, byte expected_zero, byte expected_neg, byte expected_carry)
{
    //cpu.reset();
    cpu.A = operand;
    cpu.ASL(0, true);
    //assert.are_equal(expected_result, cpu.A);
    assert.are_equal_flags(expected_result, cpu.A, expected_zero, expected_neg, expected_carry, cpu);
}

void test_lsr(Assert& assert, CPU& cpu, byte operand, byte expected_result, byte expected_zero, byte expected_neg, byte expected_carry)
{
    //cpu.reset();
    cpu.A = operand;
    cpu.LSR(0, true);
    //assert.are_equal(expected_result, cpu.A);
    assert.are_equal_flags(expected_result, cpu.A, expected_zero, expected_neg, expected_carry, cpu);
}

void test_bit(Assert& assert, CPU& cpu, byte mask, byte operand, byte expected_zero, byte expected_neg, byte expected_overflow)
{
    //cpu.reset();
    cpu.A = mask;
    cpu.ram[0] = operand;
    cpu.BIT(0);
    assert.are_equal_flags(expected_zero, expected_neg, expected_overflow, cpu);
}

void test_cpx(Assert& assert, CPU& cpu, byte Y, byte operand, byte expected_zero, byte expected_neg, byte expected_carry, bool CPY)
{
    cpu.reset();
    if(CPY)
    {
        cpu.Y = Y;
        cpu.ram[0] = operand;
        cpu.CPY(0);
        assert.are_equal_flags_c(expected_zero, expected_neg, expected_carry, cpu);
    }
    else 
    {
        cpu.X = Y;
        cpu.ram[0] = operand;
        cpu.CPX(0);
        assert.are_equal_flags_c(expected_zero, expected_neg, expected_carry, cpu);
    }
}

void test_group_1(Assert& assert, CPU& cpu)
{
    cout << "ORA Tests\n";
    //Test 1
    test_ora(assert, cpu, 0b00000000, 0b00001111, 0b00001111);
    test_ora(assert, cpu, 0b00000000, 0b00001111, 0b00001111);
    test_ora(assert, cpu, 0b00000000, 0b00011111, 0b00011111);
    test_ora(assert, cpu, 0b00000000, 0b00111111, 0b00111111);
    
    assert.reset();
    cout << "AND Tests\n";
    test_and(assert, cpu, 0b00000000, 0b00001111, 0b00000000);
    test_and(assert, cpu, 0b00000000, 0b00001111, 0b00000000);
    test_and(assert, cpu, 0b00000000, 0b00011111, 0b00000000);
    test_and(assert, cpu, 0b00000000, 0b00111111, 0b00000000);


    //skipping over EOR
    
    assert.reset();
    cout << "ADC Tests\n";
    test_adc(assert, cpu, 0x50, 0x10, 0x60, 0, 0);
    test_adc(assert, cpu, 0x50, 0x50, 0xA0, 1, 0);
    test_adc(assert, cpu, 0x50, 0x90, 0xE0, 0, 0);
    test_adc(assert, cpu, 0x50, 0xd0, 0x20, 0, 1);
    test_adc(assert, cpu, 0xd0, 0x10, 0xe0, 0, 0);
    test_adc(assert, cpu, 0xd0, 0x50, 0x20, 0, 1);
    test_adc(assert, cpu, 0xd0, 0x90, 0x60, 1, 1);
    test_adc(assert, cpu, 0xd0, 0xd0, 0xa0, 0, 1);

    assert.reset();
    cout << "CMP Tests\n";
	test_cmp(assert, cpu, 0x50, 0x60, 0, 1, 0);
    test_cmp(assert, cpu, 0x50, 0x50, 1, 0, 1);
    test_cmp(assert, cpu, 0xFF, 0xFF, 1, 0, 1);
    test_cmp(assert, cpu, 0x50, 0x40, 0, 0, 1);
    
    assert.reset();
    cout << "SBC Tests\n";
    test_sbc(assert, cpu, 0x50, 0xf0, 0x60, 0, 1);
    test_sbc(assert, cpu, 0x50, 0xb0, 0xa0, 1, 1);
    test_sbc(assert, cpu, 0x50, 0x70, 0xe0, 0, 1);
    test_sbc(assert, cpu, 0x50, 0x30, 0x20, 0, 0);
    test_sbc(assert, cpu, 0xd0, 0xf0, 0xe0, 0, 1);
    test_sbc(assert, cpu, 0xd0, 0xb0, 0x20, 0, 0);
    test_sbc(assert, cpu, 0xd0, 0x70, 0x60, 1, 0);
    test_sbc(assert, cpu, 0xd0, 0x30, 0xa0, 0, 0);
	//void SBC(uint16_t address, bool page_cross);
}

void  test_group_2(Assert& assert, CPU& cpu)
{
    cout << "ROR Tests\n";
    assert.reset();
    cpu.reset();
    test_ror(assert, cpu, 0b11111111, 0b01111111, 0, 0, 1);
    test_ror(assert, cpu, 0b01111111, 0b10111111, 0, 1, 1);
    test_ror(assert, cpu, 0b10111111, 0b11011111, 0, 1, 1);
    test_ror(assert, cpu, 0b00000000, 0b10000000, 0, 1, 0);
    test_ror(assert, cpu, 0b10000000, 0b01000000, 0, 0, 0);
    assert.reset();
    cout << "ROL Tests\n";
    test_rol(assert, cpu, 0b11111111, 0b11111110, 0, 1, 1);
    test_rol(assert, cpu, 0b11111110, 0b11111101, 0, 1, 1);
    test_rol(assert, cpu, 0b11111101, 0b11111011, 0, 1, 1);
    test_rol(assert, cpu, 0b00000000, 0b00000001, 0, 0, 0);
    test_rol(assert, cpu, 0b00000001, 0b00000010, 0, 0, 0);

    assert.reset();

    cout << "ASL Tests\n";
    test_asl(assert, cpu, 0b11111111, 0b11111110, 0, 1, 1);
    test_asl(assert, cpu, 0b11111110, 0b11111100, 0, 1, 1);
    test_asl(assert, cpu, 0b11111100, 0b11111000, 0, 1, 1);
    test_asl(assert, cpu, 0b11110000, 0b11100000, 0, 1, 1);
    test_asl(assert, cpu, 0b00000000, 0b00000000, 1, 0, 0);
    test_asl(assert, cpu, 0b01111110, 0b11111100, 0, 1, 0);

    assert.reset();

    cout << "LSR Tests\n";
    test_lsr(assert, cpu, 0b11111111, 0b01111111, 0, 0, 1);
    test_lsr(assert, cpu, 0b01111111, 0b00111111, 0, 0, 1);
    test_lsr(assert, cpu, 0b00000000, 0b00000000, 1, 0, 0);
    test_lsr(assert, cpu, 0b00000001, 0b00000000, 1, 0, 1);
    test_lsr(assert, cpu, 0b00000011, 0b00000001, 0, 0, 1);
    test_lsr(assert, cpu, 0b10000000, 0b01000000, 0, 0, 0);

    //Skiping over the simple ones
}

void test_group_3(Assert& assert, CPU& cpu)
{
    cpu.reset();
    assert.reset();
    cout << "BIT tests\n";
    test_bit(assert, cpu, 0b11000000, 0b00000000, 1, 0, 0);
    test_bit(assert, cpu, 0b11000000, 0b01000000, 0, 0, 1);
    test_bit(assert, cpu, 0b11000000, 0b10000000, 0, 1, 0);
    test_bit(assert, cpu, 0b11000000, 0b11000000, 0, 1, 1);
    test_bit(assert, cpu, 0b01000000, 0b11111111, 0, 0, 1);

    assert.reset();
    cout << "CPX tests\n";
    test_cpx(assert, cpu, 0x00, 0x00, 1, 0, 1, false);
    test_cpx(assert, cpu, 0x7F, 0x00, 0, 0, 1, false);
    test_cpx(assert, cpu, 0x00, 0x90, 0, 0, 1, false);
    test_cpx(assert, cpu, 0x7F, 0x7F, 1, 0, 1, false);
    test_cpx(assert, cpu, 0x7F, 0x10, 0, 0, 1, false);

    assert.reset();
    cout << "CPY tests\n";
    test_cpx(assert, cpu, 0x00, 0x00, 1, 0, 1, true);
    test_cpx(assert, cpu, 0x7F, 0x00, 0, 0, 1, true);
    test_cpx(assert, cpu, 0x00, 0x90, 0, 0, 1, true);
    test_cpx(assert, cpu, 0x7F, 0x7F, 1, 0, 1, true);
    test_cpx(assert, cpu, 0x7F, 0x10, 0, 0, 1, true);
    
}
void Assert::unit_test()
{
    Memory ram = Memory();
    CPU cpu(ram);
    cpu.reset();
    test_group_1(*this, cpu);
    test_group_2(*this, cpu);
    test_group_3(*this, cpu);
}
