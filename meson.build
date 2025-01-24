project('6502Emu', 'cpp')

sdl2_dep = dependency('sdl2')

src_files = [
    'src/main_debug.cpp',
    'src/CPU/address_logic.cpp',
    'src/CPU/cpu_core.cpp',
    'src/CPU/instructions_addressed.cpp',
    'src/CPU/instructions_intrerrupts.cpp',
    'src/CPU/instructions_single_byte.cpp',
    'src/CPU/unittest.cpp',
    'src/CPU/tracer.cpp',
    'src/Mappers/mapper0.cpp'
]


executable('cpu', src_files, dependencies: sdl2_dep)