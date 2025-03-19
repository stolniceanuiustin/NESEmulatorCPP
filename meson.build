project('6502Emu', 'cpp')

sdl2_dep = dependency('sdl2')

src_files = [
    'src/main_debug.cpp',
    'src/CPU/address_logic.cpp',
    'src/CPU/cpu_core.cpp',
    'src/CPU/instructions_addressed.cpp',
    'src/CPU/instructions_intrerrupts.cpp',
    'src/CPU/instructions_single_byte.cpp',
    'src/CPU/tracer.cpp',
    'src/PPU/PPU.cpp',
    'src/Mappers/mapper0.cpp',
    'src/SDL/SDL_backend.cpp',
    'src/Cartridge.cpp',
    'src/bus.cpp',
    'src/Log.cpp'
]


executable('emulator', src_files, dependencies: sdl2_dep)