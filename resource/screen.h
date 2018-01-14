#ifndef SCREEN_H
#define SCREEN_H

const int SCREEN_WIDTH = 531;
const int SCREEN_HEIGHT = 321;

struct Pixel
{
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
};

class screen
{
public:
    screen();
};

#endif // SCREEN_H
