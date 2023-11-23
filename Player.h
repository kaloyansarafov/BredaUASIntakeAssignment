#pragma once
#include <string>

#include "Ability.h"
#include "Character.h"
#include "surface.h"

struct Player
{
    std::string name;
    int health;
    int maxHealth;
    int energy;
    int maxEnergy;

    Character character;

    Ability abilities[3];

    //ctor
    Player(std::string name, int health, int energy, Character character, Ability abilities[3])
        : name(name), health(health), maxHealth(health), energy(energy), maxEnergy(energy), character(character)
    {
        for (int i = 0; i < 3; i++)
        {
            this->abilities[i] = abilities[i];
        }
    }
};
