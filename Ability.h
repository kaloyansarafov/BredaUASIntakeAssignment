#pragma once
#include <string>

// Ability.h
struct Ability
{
    std::string name;
    int damage;
    int speed;
    float cooldown;
    int range;
    float duration;

    // Default constructor
    Ability() : name(""), damage(0), speed(0), cooldown(0), range(0), duration(0.0f) {}

    // Parameterized constructor
    Ability(std::string name, int damage, int speed, float cooldown, int range, float duration)
        : name(name), damage(damage), speed(speed), cooldown(cooldown), range(range), duration(duration) {}
};
