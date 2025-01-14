// #include <iostream>
// #include <unistd.h>
// #include "cpu_header.h"
// #include "unittest.h"
// using std::cout;

// int main(int argc, char *argv[])
// {
//     CPU cpu;
//     cpu.reset();
//     // if(argc < 2)
//     // {
//     //     std::cerr << "Usage: " << argv[0] << "rom_name\n";
//     //     return -1;
//     // }

//     // Config config = Config(argv[1]);
//     Config config = Config("..\\roms\\6502_functional_test.bin");
//     config.code_segment = 0x0400;
//     cpu.init(config);
//     int t = 0;
//     while(true)
//     {
//         cpu.execute();
//         usleep(0.558659218);
//     }

//     // Assert assert;
//     // assert.unit_test();
// }