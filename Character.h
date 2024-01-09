#pragma once

#include "surface.h"
#include <memory>
#include <string>
#include <vector>
#include <Audio/Sound.hpp>

namespace Tmpl8
{
    class character
    {
    public:
        std::string name;
        std::shared_ptr<Sprite> sprite;
        mutable int x, y;
        int energyRegenRate = 15;
        
        float attackCooldown = 0.5f;
        float attackCooldownRemaining = 0;
        int attackDamage = 50;
        int attackRange = 125;
        int attackCost = 30;

        int moveTimer = 0;

        int eatingCooldown = 3;
        float eatingCooldownRemaining = 0;

        mutable float stunDuration = 0.0f; // s

        bool isAttacking = false;
        int currentAttackFrame = 0;
        int attackFrameCount = 30;
        std::pair<int, int> currentAttackLocation;

        //ctor
        character(int x, int y, std::string name, int health, int energy, std::shared_ptr<Sprite> sprite)
            : name(std::move(name)), sprite(std::move(sprite)), x(x), y(y), hearts(0), health(health), energy(energy)
        {
        }

        void MoveTowards(const character& other, float speed) const;

        void addHealth(int amount) const
        {
            this->health += amount;
            if (this->health > this->maxHealth)
            {
                this->health = this->maxHealth;
            }
        }

        int getHealth() const
        {
            return this->health;
        }

        int getMaxHealth()
        {
            return this->maxHealth;
        }

        void addEnergy(float amount)
        {
            this->energy += amount;
            if (this->energy > this->maxEnergy)
            {
                this->energy = this->maxEnergy;
            }
        }

        int getEnergy()
        {
            return this->energy;
        }

        int getMaxEnergy()
        {
            return this->maxEnergy;
        }

        int getHearts()
        {
            return this->hearts;
        }

        void collectHeart()
        {
            this->hearts++;
        }

        void Attack(int x, int y, const std::vector<character>& enemies, Audio::Sound& attackSound);

        void Eat(Audio::Sound& eatSound);

        bool IsInCone(const character& other, float directionX, float directionY, float maxDistance);

        bool isColliding(const character& other) const;
        

    private:
        int hearts;
        mutable int health;
        float energy;
        int maxHealth = 100, maxEnergy = 100;
        
    };
}
