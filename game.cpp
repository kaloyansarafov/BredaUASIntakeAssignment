#include "game.h"
#include "surface.h"
#include <iostream>
#include <cstdio> //printf
#include <SDL.h>
#include <string>
#include <cmath>
#include "Player.h"

namespace Tmpl8
{
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	void Game::Init()
	{
	}
	
	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}
	
	static float speedX = 0.0f;
	static float speedY = 0.0f;

	static Ability abilities[3] = { Ability("Sword",20, 50, 1, 1, 10, 0.4f), Ability("Shield",30, 0, 1, 1, 10, 0.5f), Ability("Dash",60, 30, 4, 5, 0, 0.2f) };

	//must be a shared_ptr because character may hold a reference after destruction otherwise
	std::shared_ptr<Sprite> rotatingGun = std::make_shared<Sprite>(new Surface("assets/aagun.tga"), 36);
	Character character(100, 100, rotatingGun);

	static Player player("Gladiator", 100, 100, character, abilities);

	static int mouseX = 0;
	static int mouseY = 0;

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{
		float deltaTimeS = deltaTime / 1000.0f;
		
		int newPositionX =  player.character.x + speedX * deltaTime;
		int newPositionY =  player.character.y + speedY * deltaTime;
		
		// Make sure new positions cannot go out of bounds
		if (newPositionX < 0)	newPositionX = 0;
		if (newPositionX > 800 - player.character.sprite->GetWidth())	newPositionX = 800 - player.character.sprite->GetWidth() - 1;
		if (newPositionY < 0)	newPositionY = 0;
		if (newPositionY > 512 - player.character.sprite->GetHeight())	newPositionY = 512 - player.character.sprite->GetHeight() - 1;

		player.character.x = newPositionX;
		player.character.y = newPositionY;
		
		screen->Clear(0);

		//draw a bounding box around the gun for debugging
		screen->Box(player.character.x,  player.character.y,  player.character.x + player.character.sprite->GetWidth(),  player.character.y + player.character.sprite->GetHeight(), 0xffffff);

		player.character.sprite->SetFrame(CalculateSpriteFrame());
		player.character.sprite->Draw(screen, player.character.x, player.character.y);
	}

	// -----------------------------------------------------------
	// Handle key down events
	// -----------------------------------------------------------
	void Game::KeyDown( int key )
	{
		printf("Key %d was pressed\n", key);

		constexpr float speedIncrement = 0.2f;
	
		//W - 26
		//A - 4
		//S - 22
		//D - 7
		
		switch (key)
		{
		case 26:  speedY = -speedIncrement; break; //W
		case 22:  speedY = speedIncrement; break; //S
		case 4: speedX = -speedIncrement; break; //A
		case 7: speedX = speedIncrement; break; //D
		}
	}

	// -----------------------------------------------------------
	// Handle key up events
	// -----------------------------------------------------------
	void Game::KeyUp( int key )
	{
		switch (key)
		{
		case 26:
		case 22:  speedY = 0.0f; break; //W, S
		case 4:
		case 7: speedX = 0.0f; break; //A, D
		}
	}

	void Game::MouseDown(int button)
	{
		
	}

	void Game::MouseUp(int button)
	{
		
	}


	void Game::MouseMove(int x, int y)
	{
		mouseX = x;
		mouseY = y;
	}

	int Game::CalculateSpriteFrame()
	{
		int vectorX = mouseX - player.character.x;
		int vectorY = mouseY - player.character.y;

		// Calculate the angle in radians
		float angleRadians = std::atan2(vectorY, vectorX);

		// Convert the angle to degrees
		float angleDegrees = angleRadians * (180.0 / M_PI);

		// Normalize angle to be between 0 and 360
		if (angleDegrees < 0) {
			angleDegrees += 360;
		}

		angleDegrees = std::abs(360 - angleDegrees);

		angleDegrees += 90;
		
		// Map angle to a frame
		int frame = static_cast<int>(angleDegrees / 10) % 36;

		// Clamp the frame to be between 0 and 36 (bad things happen otherwise)
		if (frame < 0) frame = 0;
		if (frame > 36) frame = 36;

		return frame;
	}


};