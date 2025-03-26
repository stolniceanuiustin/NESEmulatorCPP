#ifndef VIRTUALSCREEN_HEADERGUARD
#define VIRTUALSCREEN_HEADERGUARD
#include <stdint.h>
#include <iostream>

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
const uint32_t nes_pallete[64] = {
    // First 16 colors (universal background + first 3 palettes)
    0xFF7C7C7C, 0xFF0000FC, 0xFF0000BC, 0xFF4428BC,
    0xFF940084, 0xFFA80020, 0xFFA81000, 0xFF881400,
    0xFF503000, 0xFF007800, 0xFF006800, 0xFF005800,
    0xFF004058, 0xFF000000, 0xFF000000, 0xFF000000,
    
    // Next 16 colors
    0xFFBCBCBC, 0xFF0078F8, 0xFF0058F8, 0xFF6844FC,
    0xFFD800CC, 0xFFE40058, 0xFFF83800, 0xFFE45C10,
    0xFFAC7C00, 0xFF00B800, 0xFF00A800, 0xFF00A844,
    0xFF008888, 0xFF000000, 0xFF000000, 0xFF000000,
    
    // Next 16 colors
    0xFFF8F8F8, 0xFF3CBCFC, 0xFF6888FC, 0xFF9878F8,
    0xFFF878F8, 0xFFF85898, 0xFFF87858, 0xFFFCA044,
    0xFFF8B800, 0xFFB8F818, 0xFF58D854, 0xFF58F898,
    0xFF00E8D8, 0xFF787878, 0xFF000000, 0xFF000000,
    
    // Last 16 colors
    0xFFFCFCFC, 0xFFA4E4FC, 0xFFB8B8F8, 0xFFD8B8F8,
    0xFFF8B8F8, 0xFFF8A4C0, 0xFFF0D0B0, 0xFFFCE0A8,
    0xFFF8D878, 0xFFD8F878, 0xFFB8F8B8, 0xFFB8F8D8,
    0xFF00FCFC, 0xFFF8D8F8, 0xFF000000, 0xFF000000
};
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
        RENDER_ENABLED = false;
    }
    void set_pixel(uint16_t scanline, uint16_t dot, uint8_t color_index)
    {
        //std::cout << (int)color_index << '\n';
        pixels[256 * scanline + dot] = nes_pallete[color_index];
    }
};

#endif