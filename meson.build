project('6502Emu', 'cpp')

sdl2_dep = dependency('sdl2')

src_files = [
    'src/main_debug.cpp',
    'src/address_logic.cpp',
    'src/cpu_core.cpp',
    'src/inst_group_1.cpp',
    'src/inst_group_2.cpp',
    'src/inst_group_3.cpp',
    'src/single_byte_instr.cpp',
    'src/irregular_functions.cpp',
    'src/unittest.cpp',
]


executable('cpu', src_files, dependencies: sdl2_dep)