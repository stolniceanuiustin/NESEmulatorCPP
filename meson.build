project('6502Emu', 'cpp', default_options: ['buildtype=debug'])

sdl2_dep = dependency('sdl2')
sdl2_ttf = dependency('sdl2_ttf', required: false)

#add_project_arguments(['-fsanitize=address', '-fno-omit-frame-pointer'], language: 'cpp')
#add_project_arguments('-fno-omit-frame-pointer', language: 'cpp')
add_project_arguments('-Wall', '-Wpedantic', language: 'cpp')
if not sdl2_ttf.found()
    sdl2_ttf = declare_dependency(
        include_directories: include_directories('/usr/include/SDL2'),
        link_args: ['-lSDL2_ttf']
    )
endif

src_files = [
    'src/main_debug.cpp',
    'src/CPU/address_logic.cpp',
    'src/CPU/cpu_core.cpp',
    'src/CPU/instructions.cpp',
    'src/CPU/tracer.cpp',
    'src/PPU/PPU.cpp',
    'src/Mappers/mapper0.cpp',
    'src/Mappers/mapper1.cpp',
    'src/SDL/SDL_backend.cpp',
    'src/Cartridge.cpp',
    'src/bus.cpp',
    'src/Log.cpp',
    'src/CPU/cycle_counting.cpp'
]


executable('emulator', src_files, dependencies: [sdl2_dep, sdl2_ttf])