#include <unistd.h>

#include <iostream>

#include "../include/SDL_backend.h"
#include "../include/bus.h"
#include "../include/cartridge.h"
#include "../include/cpu_header.h"
#include "../include/emulator_config.h"
#include "../include/mapper.h"
#include "../include/ppu.h"
#include "../include/unittest.h"
#include "SDL.h"
using std::cout;

const double CLK_TIME = 1.0 / 1789773.0;
uint64_t frame_duration = 1e9/60.0;
int main(int argc, char *argv[]) {
	// TODO : move everything to bus, use this just for parsing!
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << "rom_name\n";
		return -1;
	}
	Screen screen;

	Config config = Config(argv[1]);
	CARTRIDGE cartridge(config);
	if (!cartridge.read_file()) exit(-1);

	std::cout << "READ FILE SUCCESFULLY\n";
	CPU cpu;
	PPU ppu(cpu, screen);
	BUS bus(cpu, ppu, cartridge);
	cpu.connect_bus(&bus);
	ppu.connect_bus(&bus);
	ppu.connect_cartridge(&cartridge);
	cpu.reset();
	cpu.init();

	SDL sdl(screen, bus);
	init_sdl(sdl, false);
	sdl.state = PAUSED;
	config.code_segment = cpu.read_abs_address(0xFFFC);
	//cpu.PC = 0xC000;
	// TODO run for one frmae then sleep
	// TODO check with sanitiser
	int frames = 0;
	std::thread input_thread(input_thread_function, std::ref(sdl));
	while (sdl.running) {
		//handle_input(sdl);
		sdl.bus.controller[0] = sdl.controller_input_buffer;
		if (sdl.state == RUNNING) {
			uint32_t start_time = SDL_GetTicks();
			uint32_t end_time = 0;
			bus.clock(false);
			if (screen.RENDER_ENABLED == true) {
				// cout << "=======RENDERING FRAME=======" << frames << '\n';
				sdl.render_frame();
				frames++;
				uint32_t end_time = SDL_GetTicks(); 
				screen.RENDER_ENABLED = false;
				double time_elapsed = (end_time - start_time) * 1e6;
				if (time_elapsed < frame_duration) {
					//SDL_Delay(static_cast<uint32_t>(frame_duration - time_elapsed));
					timespec requested_time;
					timespec remaining_time;
					requested_time.tv_nsec = frame_duration - time_elapsed;
					//cout << requested_time.tv_nsec << '\n';
					nanosleep(&requested_time, &remaining_time);
					//cout << end_time << " " << start_time << std::endl;
				}
			}
			if (frames == 30) {
				std::cout << "ONE SECOND PASSED" << std::endl;
				frames = 0;
			}
		} else if (sdl.state == PAUSED) {
			if (sdl.tick == true) {
				std::string debug_output_log = bus.clock(true);
				// for (int i = 0; i < 3; i++) 
				// 	ppu.execute();
				// render_frmae
				if (screen.RENDER_ENABLED == true) {
					sdl.render_frame();
					screen.RENDER_ENABLED = false;
				}
				render_text(sdl, debug_output_log);
				sdl.tick = false;
			}
			continue;
		} else if (sdl.state == QUIT) {
			sdl.running = false;
			bus.hexdump();
			ppu.hexdump();
			cpu.hexdump();
			goto ENDLOOP;
		}
	}
ENDLOOP:
	destroy_sdl(sdl);
	if(input_thread.joinable())
		input_thread.join();
	// ram.hexdump();

	// SDL_DestroyRenderer(renderer);
	// SDL_DestroyWindow(window);
	// SDL_Quit();
	// Assert assert;
	// assert.unit_test();
}