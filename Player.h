#pragma once
#include <string>

#include "Ability.h"
#include "surface.h"

struct Player
{
    Tmpl8::Sprite sprite;

    Ability abilities[3];
};
