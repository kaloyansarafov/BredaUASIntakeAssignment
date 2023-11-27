#pragma once
#include <string>

#include "Ability.h"
#include "Character.h"

struct Player
{
    std::string name;
    int health;
    int maxHealth = 100;
    int energy;
    int maxEnergy = 100;

    Character character;

    Ability abilities[3];

    //ctor
    Player(std::string name, int health, int energy, Character character, Ability abilities[3])
        : name(std::move(name)), health(health), maxHealth(health), energy(energy), maxEnergy(energy), character(
              std::move(character))
    {
        for (int i = 0; i < 3; i++)
        {
            this->abilities[i] = abilities[i];
        }
    }
};
