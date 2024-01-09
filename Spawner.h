#pragma once

#include "Character.h"
#include <vector>

namespace Tmpl8
{
    class Spawner
    {
    public:
        Spawner(int maxEnemies, float spawnRate);

        void SpawnEnemy();
        void Update(float deltaTime, int playerHearts);
        std::vector<character>& GetEnemies();

    private:
        int maxEnemies;
        float spawnRate;
        std::vector<character> enemies;
        float spawnCooldown = 0;
    };
}