#pragma once
#include <string>

// Ability.h
class Ability
{
public:
    std::string name;
    int cost;
    int damage;
    int speed;
    float cooldown;
    int range;
    float duration;

    void Use();

    // Default constructor
    Ability() : name(""),cost(0), damage(0), speed(0), cooldown(0), range(0), duration(0.0f) {}

    // Parameterized constructor
    Ability(std::string name,const int cost, const int damage, const int speed, const float cooldown, const int range, const float duration)
        : name(std::move(name)), cost(cost), damage(damage), speed(speed), cooldown(cooldown), range(range), duration(duration) {}

private:
    
};
