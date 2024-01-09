#include "Spawner.h"

namespace Tmpl8
{
    Spawner::Spawner(int maxEnemies, float spawnRate)
        : maxEnemies(maxEnemies), spawnRate(spawnRate)
    {
    }

    void Spawner::SpawnEnemy()
    {
        if (enemies.size() < maxEnemies)
        {
            // Create a new enemy character and add it to the list of enemies
            std::shared_ptr<Sprite> enemySprite = std::make_shared<Sprite>(new Surface("assets/enemy.png"), 8);
            
            const int x = rand() % 2 == 0 ? rand() % 128 : rand() % 128 + 600;
            const int y = rand() % 2 == 0 ? rand() % 128 : rand() % 128 + 600;
            
            enemies.emplace_back(x, y, "Enemy" + enemies.size(), 100, 100, enemySprite);
        }
    }

    void Spawner::Update(float deltaTime, int playerHearts)
    {
        if (enemies.empty() && playerHearts <= 4)
        {
            SpawnEnemy();
        }

        spawnRate += deltaTime * playerHearts;
        spawnCooldown -= deltaTime;

        if (spawnRate >= 15.0f && spawnCooldown <= 0.0f)
        {
            SpawnEnemy();
            spawnCooldown = 1;
            spawnRate = 0.0f;
        }
    }

    std::vector<character>& Spawner::GetEnemies()
    {
        return enemies;
    }
}