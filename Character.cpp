#include "Character.h"
#include <cmath>
#include <SDL_stdinc.h>

namespace Tmpl8
{
    void character::Attack(int x, int y, const std::vector<character>& enemies, Audio::Sound& attackSound)
    {
        if (this->attackCooldownRemaining > 0)
            return;
        
        if (this->energy <= this->attackCost)
            return;

        isAttacking = true;
        currentAttackFrame = 0;
        currentAttackLocation = std::make_pair(x, y);

        attackSound.replay();

        this->energy -= this->attackCost;
        this->attackCooldownRemaining = this->attackCooldown;

        //get direction from origin to target
        const int vectorX = x - this->x;
        const int vectorY = y - this->y;

        //normalize direction
        const float vectorLength = sqrtf(vectorX * vectorX + vectorY * vectorY);
        const float directionX = vectorX / vectorLength;
        const float directionY = vectorY / vectorLength;

        for (auto& enemy : enemies)
        {
            if (IsInCone(enemy, directionX, directionY, this->attackRange))
            {
                enemy.addHealth(-this->attackDamage);
            }
        }
    }

    void character::Eat(Audio::Sound& eatSound)
    {
        if (this->hearts <= 0)
        {
            return;
        }

        if (this->eatingCooldownRemaining > 0)
        {
            return;
        }

        eatSound.replay();

        this->eatingCooldownRemaining = this->eatingCooldown;
        this->hearts--;
        this->addHealth(30);
    }

    bool character::IsInCone(const character& other, float directionX, float directionY, float maxDistance)
    {
        // Calculate the center of the other character
        float otherCenterX = other.x + other.sprite->GetWidth() / 2;
        float otherCenterY = other.y + other.sprite->GetHeight() / 2;

        float vectorX = otherCenterX - this->x + this->sprite->GetWidth() / 2;
        float vectorY = otherCenterY - this->y + this->sprite->GetHeight() / 2;

        float directionLength = sqrtf(directionX * directionX + directionY * directionY);
        float vectorLength = sqrtf(vectorX * vectorX + vectorY * vectorY);

        if (vectorLength > maxDistance) {
            return false;
        }

        directionX /= directionLength;
        directionY /= directionLength;
        vectorX /= vectorLength;
        vectorY /= vectorLength;

        // Calculate the dot product of the vectors
        float dotProduct = directionX * vectorX + directionY * vectorY;

        float cos45 = std::cos(45.0f * M_PI / 180.0f);
        // If the dot product is greater than or equal to cos(45), the other character is in the cone
        return dotProduct >= cos45;
    }

    void character::MoveTowards(const character& target, float speed) const
    {
        float vectorX = target.x - this->x;
        float vectorY = target.y - this->y;

        float vectorLength = sqrtf(vectorX * vectorX + vectorY * vectorY);

        vectorX /= vectorLength;
        vectorY /= vectorLength;

        this->x += vectorX * speed > 0 ? std::ceil(vectorX * speed) : std::floor(vectorX * speed);
        this->y += vectorY * speed > 0 ? std::ceil(vectorY * speed) : std::floor(vectorY * speed);
    }

    bool character::isColliding(const character& other) const
    {
        // Calculate the center of this character
        float thisCenterX = this->x + this->sprite->GetWidth() / 2;
        float thisCenterY = this->y + this->sprite->GetHeight() / 2;

        // Calculate the center of the other character
        float otherCenterX = other.x + other.sprite->GetWidth() / 2;
        float otherCenterY = other.y + other.sprite->GetHeight() / 2;

        // Calculate the distance between the centers
        float distanceX = thisCenterX - otherCenterX;
        float distanceY = thisCenterY - otherCenterY;
        float distance = sqrtf(distanceX * distanceX + distanceY * distanceY);

        // Calculate the sum of the radii
        float radiusSum = this->sprite->GetWidth() / 2 + other.sprite->GetWidth() / 2;

        // If the distance between the centers is less than the sum of the radii, the characters are colliding
        return distance < radiusSum;
    }

}
