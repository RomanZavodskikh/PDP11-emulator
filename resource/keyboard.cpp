#include "keyboard.h"
#include "screen.h"
#include <fstream>
#include <assert.h>
#include <iostream>

Keyboard::Keyboard(std::string path)
{
    std::ifstream fd;
    fd.open(path, std::ios::binary | std::ios::ate );


    if (!fd.is_open()) {
        std::cout << "not opened" << std::endl;
        std::fflush(0);
    }

    //Offset of bitmap can be found
    fd.seekg(0x0A, std::ios::beg);

    int offset;
    fd.read((char*) &offset, sizeof(int));

    //Start bitmap
    fd.seekg(offset, std::ios::beg);

    for (int i = 0; i < KEY_AMOUNT * KEY_WIDTH * KEY_HEIGHT; i++)
    {
        char temp_rgba[3] = {};
        fd.read((char*) temp_rgba, 3);
        keys_[i] = (temp_rgba[0] + temp_rgba[1] + temp_rgba[2]) / 3;
    }

    fd.close();
}

bool Keyboard::keyToVRAM(uchar* vram, int x, int y, char key)
{
    assert(0 <= x && x < SCREEN_WIDTH);
    assert(0 <= y && y < SCREEN_HEIGHT);

    int key_x = (key / 16) * KEY_WIDTH;
    int key_y = (key % 16) * KEY_HEIGHT;

    for (int i = 0; i < KEY_HEIGHT; i++)
    {
        for (int j = 0; j < KEY_WIDTH; j++)
        {
            vram[(x + j) + (y + i) * SCREEN_WIDTH] =  keys_[(key_x + j) + (key_y + i) * 8 * KEY_WIDTH];
        }
    }

    return true;
}

bool Keyboard::stringToVRAM(uchar* vram, int x, int y, std::string str)
{
       int strlen = str.length();

    for (int i = 0; i < strlen; i++)
    {
        keyToVRAM(vram, x + i * KEY_WIDTH, y, str[i]);
    }

    return true;
}
