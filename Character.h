#pragma once

#include "surface.h"
#include <memory> // for std::shared_ptr

struct Character
{
    int x, y; //top left corner
    std::shared_ptr<Tmpl8::Sprite> sprite;

    Character(int x, int y, std::shared_ptr<Tmpl8::Sprite> sprite)
        : x(x), y(y), sprite(sprite)
    {
    }
};
