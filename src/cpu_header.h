#ifndef cpu_header
#define cpu_header
#include <stdint.h>
#define TEST
#include "emulator_config.h"

typedef uint8_t byte;


struct Mem
{
	int x;
};

struct Instruction
{
	byte aaa;
	byte bbb;
	byte cc;
	byte opcode;
	byte xx;
	byte y;
};

class CPU
{
public:
	struct Instruction inst;
	byte ram[65536];
#ifndef TEST
private:
#endif 
#ifdef TEST
public:
#endif
	//STACK IS at page 1 0100 - 01FF, sp goes from 
	byte A; // Registrii Accumulator, X, Y
	byte X;
	byte Y;
	uint16_t PC;
	byte SP; // Stack pointer
	int cycles;
public:
	// Cpu Flags
	byte C; // carry
	byte Z; // zero
	byte I; // interrupt
	byte D; // decimal
	byte B; // break
	byte O; // overflow //iN DOCUMENTATION IT'S V BUT O is more intuitive
	byte N; // negative
	
	bool init(Config config, bool NES);

	byte ram_at(uint16_t address)
	{	
		return ram[address];
	}
	byte get_A()
	{
		return A;
	}
	byte get_X()
	{
		return X;
	}
	byte get_Y()
	{
		return Y;
	}
	int get_cycles()
	{
		return cycles;
	}
	uint16_t get_pc()
	{
		return PC;
	}
	uint16_t get_inst_opcode()
	{
		return inst.opcode;
	}
	byte get_SP()
	{
		return SP;
	}
	bool reset();
	byte read_pc()
	{
		byte val = ram[PC];
		PC++;
		return val;
	}

	uint16_t read_address_from_pc();
	void write_byte(byte *address, byte value);
	uint16_t read_address(byte offset);
	int execute();
	uint16_t read_abs_address(uint16_t offset);
	void push(byte x);
	void push_address(uint16_t address);
	byte pop();
	uint16_t pop_address();
	byte pack_flags();
	void unpack_flags(byte flags);

	bool compute_addr_mode_g23(bool &page_cross, uint16_t &address_to_return);
	
	void JSR_abs(uint16_t address);
	void RTS();

	// First group of instructions
	void set_ZN(byte value);
	uint16_t compute_addr_mode_g1(bool &page_cross);
	void run_instruction_group1(uint16_t address, bool page_cross);
	void ORA(uint16_t address, bool page_cross);
	void AND(uint16_t address, bool page_cross);
	void EOR(uint16_t address, bool page_cross);
	void ADC(uint16_t address, bool page_cross);
	void STA(uint16_t address);
	void LDA(uint16_t address, bool page_cross);
	void CMP(uint16_t address, bool page_cross);
	void SBC(uint16_t address, bool page_cross);

	// Second group of instructions

	void run_instruction_group2(uint16_t address, bool page_cross, bool accumulator);
	void ASL(uint16_t address, bool accumulator);
	void ROL(uint16_t address, bool accumulator);
	void LSR(uint16_t address, bool accumulator);
	void ROR(uint16_t address, bool accumulator);
	void STX(uint16_t address);
	void LDX(uint16_t address, bool page_cross);
	void DEC(uint16_t address);
	void INC(uint16_t address);

	// Third group of instructions
	void run_instruction_group3(uint16_t address, bool page_cross);
	void JMP_abs(uint16_t jump_address);
	void JMP_indirect(uint16_t jump_address);
	void BIT(uint16_t address);
	void STY(uint16_t address);
	void CPY(uint16_t address);
	void CPX(uint16_t address);
	void LDY(uint16_t address, bool page_cross);

	//SINGLE BYTE INSTRUCTIONS
	void run_instruction_group_sb1();
	void PHP();
	void PLP();
	void PHA();
	void PLA();
	void DEY();
	void TAY();
	void INY();
	void INX();
	void CLC();
	void SEC();
	void CLI();
	void SEI();
	void TYA();
	void CLV();
	void CLD();
	void SED();
	void BRK();

	void run_instruction_group_sb2();
	void TXA();
	void TXS();
	void TAX();
	void TSX();
	void DEX();
	void NOP();
	
	//INTERRUPTS 
	const uint16_t NMI_vector = 0xFFFA;
	const uint16_t reset_vector = 0xFFFC;
	const uint16_t IRQ_vector = 0xFFFE;
	const uint16_t BRK_vector = 0xFFFE;

	void trigger_irq();
};


#endif