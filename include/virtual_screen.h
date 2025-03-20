#ifndef VIRTUALSCREEN_HEADERGUARD
#define VIRTUALSCREEN_HEADERGUARD
#include <stdint.h>

/*
This file contains a virtual screen from which
a SDL texture will be made
*/
// struct Pixel{
//     // uint8_t nes_color_index;
//     // uint32_t rgba;

//     // Pixel(): nes_color_index(0), rgba(0xFF000000) {};
//     // void set_pixel(uint8_t nes_color_index, uint32_t nes_pallete[64])
//     // {
//     //     nes_color_index = index;
//     //     rgba = nes_pallete[index];
//     // }
//     uint32_t rgba;
//     Pixel() : rgba(0xFF000000) {};
//     Pixel(uint32_t rgba) : rgba(rgba) {};
// };

class Screen
{
public:
    uint32_t pixels[256 * 240];
    bool RENDER_ENABLED;
    Screen()
    {
        for (int i = 0; i < 256 * 240; i++)
        {
            pixels[i] = 0x0000FFFF; // Test color;
        }
        RENDER_ENABLED = true;
    }
    void set_pixel(uint16_t scanline, uint16_t dot)
    {
        pixels[256 * scanline + dot] = 0x00FF00FF;
    }
};

#endif