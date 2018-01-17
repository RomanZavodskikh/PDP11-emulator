#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "qstring.h"

const int KEY_AMOUNT = 8 * 16;
const int KEY_WIDTH = 12;
const int KEY_HEIGHT = 18;

class Keyboard
{
public:
    Keyboard(std::string path);

    bool keyToVRAM(uchar* vram, int x, int y, char key);
    bool stringToVRAM(uchar* vram, int x, int y, std::string str);

private:
    uchar keys_[KEY_AMOUNT * KEY_WIDTH * KEY_HEIGHT];
};

#endif // KEYBOARD_H
