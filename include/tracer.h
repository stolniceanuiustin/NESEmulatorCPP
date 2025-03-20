#ifndef tracer_headerguard
#define tracer_headerguard
#include "cpu_header.h"
#include <string>
class TRACER
{
private:
	CPU& cpu;
public:
	TRACER(CPU& cpu_pointer) : cpu(cpu_pointer)
	{
	}
	std::string tracer(uint16_t PC, byte FLAGS, byte A, byte X, byte Y, byte SP, int cycles);
	void print_flags_group1();
	void trace_instruction_group3(uint16_t address);
	void trace_instruction_group2(uint16_t address);
	void trace_instruction_group1(uint16_t address);
	uint16_t read_address_tracer(uint16_t address);
};


#endif