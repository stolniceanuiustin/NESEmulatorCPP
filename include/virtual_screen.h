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

//     // Pixel(): nes_color_index(0)FF, rgba(0x000000) {};
//     // void set_pixel(uint8_t nes_color_indexFF, uint32_t nes_pallete[64])
//     // {
//     //     nes_color_index = index;
//     //     rgba = nes_pallete[index];
//     // }
//     uint32_t rgba;
//     Pixel() : rgba(0x000000) {};
//     Pixel(uint32_t rgba) : rgba(rgba) {};
// };
const uint32_t nes_pallete[64] = {
    // First 16 colors (universal background + first 3 palettes)
    0x7C7C7CFF, 0x0000FCFF, 0x0000BCFF, 0x4428BCFF,
    0x940084FF, 0xA80020FF, 0xA81000FF, 0x881400FF,
    0x503000FF, 0x007800FF, 0x006800FF, 0x005800FF,
    0x004058FF, 0x000000FF, 0x000000FF, 0x000000FF,
    
    // Next 16 colors
    0xBCBCBCFF, 0x0078F8FF, 0x0058F8FF, 0x6844FCFF,
    0xD800CCFF, 0xE40058FF, 0xF83800FF, 0xE45C10FF,
    0xAC7C00FF, 0x00B800FF, 0x00A800FF, 0x00A844FF,
    0x008888FF, 0x000000FF, 0x000000FF, 0x000000FF,
    
    // Next 16 colors
    0xF8F8F8FF, 0x3CBCFCFF, 0x6888FCFF, 0x9878F8FF,
    0xF878F8FF, 0xF85898FF, 0xF87858FF, 0xFCA044FF,
    0xF8B800FF, 0xB8F818FF, 0x58D854FF, 0x58F898FF,
    0x00E8D8FF, 0x787878FF, 0x000000FF, 0x000000FF,
    
    // Last 16 colors
    0xFCFCFCFF, 0xA4E4FCFF, 0xB8B8F8FF, 0xD8B8F8FF,
    0xF8B8F8FF, 0xF8A4C0FF, 0xF0D0B0FF, 0xFCE0A8FF,
    0xF8D878FF, 0xD8F878FF, 0xB8F8B8FF, 0xB8F8D8FF,
    0x00FCFCFF, 0xF8D8F8FF, 0x000000FF, 0x000000FF
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
            pixels[i] = 0x0000; // Test color;
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