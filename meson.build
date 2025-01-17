project('6502Emu', 'cpp')

sdl2_dep = dependency('sdl2')

src_files = [
    'src/main_debug.cpp',
    'src/address_logic.cpp',
    'src/cpu_core.cpp',
    'src/instructions_addressed.cpp',
    'src/instructions_intrerrupts.cpp',
    'src/instructions_single_byte.cpp',
    'src/unittest.cpp',
    'src/tracer.cpp'
]


executable('cpu', src_files, dependencies: sdl2_dep)