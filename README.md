## NES Emulator – Personal Project in C++

I developed a functional emulator for the Nintendo Entertainment System (NES), allowing original games (ROMs) to run on any platform with a C++ compiler and support for SDL2.  


## Emulated Components


### CPU – Ricoh 2A03

Implemented a core compatible with the modified 6502 processor, without support for BCD instructions. The emulation includes:

    Fetch-decode-execute loop

    Interrupt handling (IRQ/NMI)

    Precise clock cycle synchronization according to the original specifications

### PPU – Ricoh 2C02

Recreated the graphics unit responsible for screen rendering. Features include:

    Scanline-based rendering

    Sprite and Background rendering

### NES Controller

Emulated the behavior of NES controller ports based on shift registers, allowing keyboard mapping to classic NES buttons (A, B, Start, Select, D-Pad).  

Memory Mapping – Mapper 0 (NROM) for now. Support for more complex mappers (e.g., MMC1, MMC3) is planned to expand game compatibility.

Implemented support for standard cartridges with no bank switching.
Currently runs well-tested games like Donkey Kong and Super Mario Bros.

Technologies Used

    C++

    SDL2 – for graphics rendering and input handling

    Meson – build system

Outcomes & Learning

This project gave me hands-on, in-depth understanding of:

    CPU architecture and machine code decoding

    Synchronization and data bus communication between simulated hardware components

    The fundamentals of 2D raster-based graphics rendering

The project was driven by my passion for emulators and the desire to deeply understand how classic consoles work by building one from scratch. 

## Build Instructions

### Prerequisites

- C++ compiler (e.g. `g++` or `clang++`)
- [Meson](https://mesonbuild.com/)
- [Ninja](https://ninja-build.org/)
- [SDL2](https://libsdl.org/) development libraries

On Debian/Ubuntu-based systems, you can install the dependencies using:

```bash
sudo apt install build-essential meson ninja-build libsdl2-dev
```
Step 1:
Clone the repository:  
git clone https://github.com/stolniceanuiustin/NESEmulatorCPP.git  
cd NESEmulatorCPP    

Step 2:  
meson setup build   

Step 3:    
cd build/ 
meson compile  

Step 4:  
Run the emulator. You will need a valid ROM file(not provided with the repository for copyright reasons)  
./build/nes_emulator path/to/your_game.nes
