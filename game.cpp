#include "game.h"

#include <SDL.h>
#include <Audio/Sound.hpp>

#include <cstdio>
#include <string>
#include <cmath>
#include <iostream>
#include <vector>

#include "Button.h"
#include "surface.h"
#include "Character.h"
#include "Spawner.h"

namespace Tmpl8
{
    enum game_state
    {
        MENU,
        PLAYING,
        PAUSED,
        GAME_OVER
    };
    
    Audio::Sound lowBgMusic{"assets/low.wav", Audio::Sound::Type::Stream};
    Audio::Sound midBgMusic{"assets/mid.wav", Audio::Sound::Type::Stream};
    Audio::Sound highBgMusic{"assets/high.wav", Audio::Sound::Type::Stream};

    // -----------------------------------------------------------
    // Initialize the application
    // -----------------------------------------------------------
    void Game::Init()
    {
        lowBgMusic.setVolume(0.5f);
        midBgMusic.setVolume(0.5f);
        highBgMusic.setVolume(0.5f);
    }

    // -----------------------------------------------------------
    // Close down application
    // -----------------------------------------------------------
    void Game::Shutdown()
    {
    }

    static float speed_x = 0.0f;
    static float speed_y = 0.0f;
    std::shared_ptr<Sprite> player_sprite = std::make_shared<Sprite>(new Surface("assets/sniper.tga"), 32);
    character player(400, 300, "Survivor", 100, 100, player_sprite);

    static int mouse_x = 0;
    static int mouse_y = 0;

    Spawner enemySpawner(12, 0.0f);

    static game_state state = MENU;

    // Sounds
    Audio::Sound arcade{"assets/arcade.wav"};
    Audio::Sound sword{"assets/sword_whoosh.wav"};
    Audio::Sound eat{"assets/eating.wav"};
    Audio::Sound death{"assets/zombie_death.wav"};
    Audio::Sound game_over{"assets/game_over.wav"};


    // UI
    Sprite title = Sprite(new Surface("assets/title.png"), 1);
    Sprite paused = Sprite(new Surface("assets/paused.png"), 1);
    Sprite gameover = Sprite(new Surface("assets/gameover.png"), 1);
    Button startButton(350, 400, 100, 50, 0x3fa142, "Start");
    Button resumeButton(350, 400, 100, 50, 0x3fa142, "Resume");
    Button quitButton(350, 500, 100, 50, 0x3fa142, "Quit");

    // -----------------------------------------------------------
    // Main application tick function
    // -----------------------------------------------------------
    void Game::Tick(float deltaTime)
    {
        screen->Clear(0x2b2b2b);

        deltaTime *= 0.001f; //convert to seconds

        switch (state)
        {
        case MENU:
            MainMenu();
            break;
        case PLAYING:
            Playing(deltaTime);
            break;
        case PAUSED:
            Paused();
            break;
        case GAME_OVER:
            GameOver();
            break;
        }
    }

    // -----------------------------------------------------------
    // Handle key down events
    // -----------------------------------------------------------
    void Game::KeyDown(int key)
    {
        constexpr float speedIncrement = 200.0f;

        //W - 26
        //A - 4
        //S - 22
        //D - 7

        switch (key)
        {
        case 26: speed_y = -speedIncrement;
            break; //W
        case 22: speed_y = speedIncrement;
            break; //S
        case 4: speed_x = -speedIncrement;
            break; //A
        case 7: speed_x = speedIncrement;
            break; //D
        case 41:
            state = PAUSED;
            break; //ESC
        }
    }

    // -----------------------------------------------------------
    // Handle key up events
    // -----------------------------------------------------------
    void Game::KeyUp(int key)
    {
        switch (key)
        {
        case 26:
        case 22: speed_y = 0.0f;
            break; //W, S
        case 4:
        case 7: speed_x = 0.0f;
            break; //A, D
        }
    }

    void Game::MouseDown(int button)
    {
        switch (button)
        {
        case 1:
            if (state != PLAYING)
            {
                HandleButtons();
            }
            else
            {
                player.Attack(mouse_x, mouse_y, enemySpawner.GetEnemies(), sword);
            }
            break;
        case 3:
            player.Eat(eat);
            break;
        }
    }

    void Game::MouseMove(int x, int y)
    {
        mouse_x = x;
        mouse_y = y;
    }

    int Game::CalculateSpriteFrame(character origin, int target_x, int target_y, int offset, int frameCount)
    {
        const int vectorX = target_x - origin.x;
        const int vectorY = target_y - origin.y;

        // Calculate the angle in radians
        float angleRadians = std::atan2(vectorY, vectorX);

        // Convert the angle to degrees
        float angleDegrees = angleRadians * (180.0 / M_PI);

        // Normalize angle to be between 0 and 360
        if (angleDegrees < 0)
        {
            angleDegrees += 360;
        }

        //angleDegrees = std::abs(360 - angleDegrees);

        // offset by 90 degrees due to the sprite being rotated
        angleDegrees += offset;

        // Map angle to a frame
        int frame = static_cast<int>(angleDegrees / (360.0f / frameCount)) % frameCount;

        // Clamp the frame to be between 0 and the max frame count (bad things happen otherwise)
        if (frame < 0) frame = 0;
        if (frame > frameCount) frame = frameCount - 1;

        return frame;
    }

    std::pair<int, int> Game::CalculateSpritePosition(float deltaTime)
    {
        int newPositionX = static_cast<int>(speed_x * deltaTime > 0
                                                ? std::ceil(player.x + speed_x * deltaTime)
                                                : std::floor(player.x + speed_x * deltaTime));

        int newPositionY = static_cast<int>(speed_y * deltaTime > 0
                                                ? std::ceil(player.y + speed_y * deltaTime)
                                                : std::floor(player.y + speed_y * deltaTime));

        // Make sure new positions cannot go out of bounds
        if (newPositionX < 0) newPositionX = 0;
        if (newPositionX > screen->GetWidth() - player.sprite->GetWidth()) newPositionX = screen->GetWidth() - player.
            sprite->GetWidth() - 1;
        if (newPositionY < 0) newPositionY = 0;
        if (newPositionY > screen->GetHeight() - player.sprite->GetHeight()) newPositionY = screen->GetHeight() - player
            .sprite->GetHeight() - 1;

        return {newPositionX, newPositionY};
    }

    void Game::DecreaseCooldowns(float deltaTime)
    {
        if (player.attackCooldownRemaining > 0)
        {
            player.attackCooldownRemaining -= deltaTime;
        }

        if (player.eatingCooldownRemaining > 0)
        {
            player.eatingCooldownRemaining -= deltaTime;
        }
    }

    void Game::HandleEnemies(float deltaTime)
    {
        enemySpawner.Update(deltaTime, player.getHearts());

        for (int i = 0; i < enemySpawner.GetEnemies().size(); ++i)
        {
            auto& enemy = enemySpawner.GetEnemies()[i];

            if (enemy.moveTimer > 0)
            {
                enemy.moveTimer -= 1;
                enemy.sprite->Draw(screen, enemy.x, enemy.y);
                continue;
            }

            if (enemy.stunDuration > 0)
            {
                enemy.stunDuration -= deltaTime;
                enemy.sprite->Draw(screen, enemy.x, enemy.y);
                continue;
            }
            //change the sprite based on the player's position in relation to the enemy (up, down, left, right)
            int spriteFrame = CalculateSpriteFrame(enemy, player.x, player.y, 180, 4);

            if (enemy.getHealth() <= 50)
            {
                spriteFrame += 4;
            }

            enemy.sprite->SetFrame(spriteFrame);
            enemy.MoveTowards(player, 1.0f * deltaTime);
            enemy.sprite->Draw(screen, enemy.x, enemy.y);

            if (enemy.isColliding(player))
            {
                player.addHealth(-10);
                //push the enemy away from the player, direction dependent
                enemy.x += (enemy.x > player.x ? 1 : -1) * 20;
                enemy.y += (enemy.y > player.y ? 1 : -1) * 20;
                enemy.stunDuration = 0.75f;
            }

            if (enemy.getHealth() <= 0)
            {
                death.replay();
                player.collectHeart();
                enemySpawner.GetEnemies().erase(enemySpawner.GetEnemies().begin() + i);
                --i; // decrement the index to account for the removed enemy
            }


            if (player.getHearts() >= 35)
            {
                enemy.moveTimer = 2;
            }
            else if (player.getHearts() >= 20)
            {
                enemy.moveTimer = 3;
            }
            else
            {
                enemy.moveTimer = 4;
            }
        }
    }

    void Game::Playing(float deltaTime)
    {
        HandleMusic();
        
        if (player.getHealth() <= 0)
        {
            lowBgMusic.stop();
            midBgMusic.stop();
            highBgMusic.stop();
            
            game_over.replay();
            quitButton.SetColor(0xc20f08);
            state = GAME_OVER;
        }

        const std::pair<int, int> newPosition = CalculateSpritePosition(deltaTime);
        DecreaseCooldowns(deltaTime);
        player.addEnergy(player.energyRegenRate * deltaTime);

        player.x = newPosition.first;
        player.y = newPosition.second;

        //print the position
        screen->Print(const_cast<char*>(("PlayerX: " + std::to_string(player.x)).c_str()), 2, 2, 0xC0C0C0);
        screen->Print(const_cast<char*>(("PlayerY: " + std::to_string(player.y)).c_str()), 2, 12, 0xC0C0C0);

        //print character health and energy
        screen->Print(
            const_cast<char*>(("Health: " + std::to_string(player.getHealth()) + "/" +
                std::to_string(player.getMaxHealth())).c_str()), 100, 2, 0xC0C0C0);
        screen->Print(
            const_cast<char*>(("Energy: " + std::to_string(player.getEnergy()) + "/" +
                std::to_string(player.getMaxEnergy())).c_str()), 100, 12, 0xC0C0C0);
        screen->Print(const_cast<char*>(("Hearts: " + std::to_string(player.getHearts())).c_str()), 100, 22, 0xC0C0C0);

        //print ability cooldowns
        if (player.attackCooldownRemaining > 0)
        {
            screen->Print(const_cast<char*>(("Attack: " + std::to_string(player.attackCooldownRemaining)).c_str()), 200,
                2, 0xC0C0C0);
        } else
        {
            screen->Print("Attack: ready", 200, 2, 0xC0C0C0);
        }

        if (player.eatingCooldownRemaining > 0)
        {
            screen->Print(const_cast<char*>(("Eat: " + std::to_string(player.eatingCooldownRemaining)).c_str()), 200, 12,
                0xC0C0C0);
        } else
        {
            screen->Print("Eat: ready", 200, 12, 0xC0C0C0);
        }
        

        //draw a small circle around the player using the plot function
        for (int i = 0; i < 360; ++i)
        {
            const float angle = i * (M_PI / 180.0f);
            const int x = static_cast<int>(player.x + player.sprite->GetWidth() / 2 + 10 * cos(angle));
            const int y = static_cast<int>(player.y + player.sprite->GetHeight() / 2 + 10 * sin(angle));
            screen->Plot(x, y, 0xffffff);
        }

        HandleEnemies(deltaTime);

        //Circle representing attack range (debug)
        /*
        for (int i = 0; i < 360; ++i)
        {
            const float angle = i * (M_PI / 180.0f);
            const int x = static_cast<int>(player.x + player.sprite->GetWidth() / 2 + player.attackRange * cos(angle));
            const int y = static_cast<int>(player.y + player.sprite->GetHeight() / 2 + player.attackRange * sin(angle));
            screen->Plot(x, y, 0xffffff);
        }
        */

        if (player.isAttacking)
        {
            // Calculate the angle of the original line in radians
            float angle = atan2(player.currentAttackLocation.second - (player.y + player.sprite->GetHeight() / 2),
                                player.currentAttackLocation.first - (player.x + player.sprite->GetWidth() / 2));

            // Calculate the end points of the two new lines
            int new_x1 = player.x + player.sprite->GetWidth() / 2 + player.attackRange * cos(angle + M_PI / 4);
            int new_y1 = player.y + player.sprite->GetHeight() / 2 + player.attackRange * sin(angle + M_PI / 4);
            int new_x2 = player.x + player.sprite->GetWidth() / 2 + player.attackRange * cos(angle - M_PI / 4);
            int new_y2 = player.y + player.sprite->GetHeight() / 2 + player.attackRange * sin(angle - M_PI / 4);

            // Draw the two new lines
            screen->Line(player.x + player.sprite->GetWidth() / 2, player.y + player.sprite->GetHeight() / 2, new_x1, new_y1, 0xcccccc);
            screen->Line(player.x + player.sprite->GetWidth() / 2, player.y + player.sprite->GetHeight() / 2, new_x2, new_y2, 0xcccccc);
            
            player.currentAttackFrame++;
            if (player.currentAttackFrame >= player.attackFrameCount)
            {
                player.isAttacking = false;
            }
        }

        player.sprite->SetFrame(CalculateSpriteFrame(player, mouse_x, mouse_y, 90, 32));
        player.sprite->Draw(screen, player.x, player.y);
    }

    void Game::HandleMusic()
    {
        if (player.getHearts() >= 50)
        {
            if (!highBgMusic.isPlaying())
            {
                lowBgMusic.stop();
                midBgMusic.stop();
                highBgMusic.replay();
            }
        }
        else if (player.getHearts() >= 20)
        {
            if (!midBgMusic.isPlaying())
            {
                lowBgMusic.stop();
                midBgMusic.replay();
                highBgMusic.stop();
            }
        }
        else
        {
            if (!lowBgMusic.isPlaying())
            {
                lowBgMusic.replay();
                midBgMusic.stop();
                highBgMusic.stop();
            }
        }
    }
    
    // UI
    void Game::MainMenu() const
    {
        title.Draw(screen, 175, 100);

        startButton.Draw(screen);
        quitButton.Draw(screen);
    }

    void Game::Paused() const
    {
        paused.Draw(screen, 250, 130);

        resumeButton.Draw(screen);
        quitButton.Draw(screen);
    }

    void Game::GameOver() const
    {
        gameover.Draw(screen, 175, 130);

        screen->Print(const_cast<char*>(("Hearts Collected: " + std::to_string(player.getHearts())).c_str()), 350, 300, 0xC0C0C0);

        quitButton.Draw(screen);
    }

    void Game::HandleButtons()
    {
        if (state == PAUSED)
        {
            if (resumeButton.IsInside(mouse_x, mouse_y))
            {
                state = PLAYING;
            }
        } else
        {
            if (startButton.IsInside(mouse_x, mouse_y))
            {
                arcade.play();
                state = PLAYING;
            }
        }

        if (quitButton.IsInside(mouse_x, mouse_y))
        {
            exit(0);
        }
    }
};
