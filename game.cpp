#include "game.h"

#include <SDL.h>

#include <cstdio>
#include <string>
#include <cmath>
#include <iostream>

#include "surface.h"
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
		screen->Clear(0);
		
		deltaTime *= 0.001f; //convert to seconds

		const std::pair<int, int> newPosition = CalculateSpritePosition(deltaTime);
		
		player.character.x = newPosition.first;
		player.character.y = newPosition.second;

		//print the position to the screen
		screen->Print(const_cast<char*>(("PlayerX: " + std::to_string(player.character.x)).c_str()), 2, 2, 0xC0C0C0);

		screen->Print(const_cast<char*>(("PlayerY: " + std::to_string(player.character.y)).c_str()), 2, 12, 0xC0C0C0);
		
		
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
		constexpr float speedIncrement = 2.0f;
	
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
		const int vectorX = mouseX - player.character.x;
		const int vectorY = mouseY - player.character.y;

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

	std::pair<int, int> Game::CalculateSpritePosition(float deltaTime)
	{
		int newPositionX =  static_cast<int>(speedX * deltaTime > 0
			? std::ceil(player.character.x + speedX * deltaTime)
			: std::floor(player.character.x + speedX * deltaTime));
		
		int newPositionY =  static_cast<int>(speedY * deltaTime > 0
			? std::ceil(player.character.y + speedY * deltaTime)
			: std::floor(player.character.y + speedY * deltaTime));
		
		// Make sure new positions cannot go out of bounds
		if (newPositionX < 0)	newPositionX = 0;
		if (newPositionX > 800 - player.character.sprite->GetWidth())	newPositionX = 800 - player.character.sprite->GetWidth() - 1;
		if (newPositionY < 0)	newPositionY = 0;
		if (newPositionY > 512 - player.character.sprite->GetHeight())	newPositionY = 512 - player.character.sprite->GetHeight() - 1;

		return {newPositionX, newPositionY};
	}

};