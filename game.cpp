#include "game.h"
#include "surface.h"
#include <iostream>
#include <cstdio> //printf
#include <SDL.h>
#include <string>
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

	static Ability abilities[3] = { Ability("Sword", 50, 1, 1, 10, 0.4f), Ability("Shield", 0, 1, 1, 10, 0.5f), Ability("Dash", 0, 4, 5, 0, 0.2f) };

	//must be a shared_ptr because character may hold a reference after destruction otherwise
	std::shared_ptr<Tmpl8::Sprite> rotatingGun = std::make_shared<Tmpl8::Sprite>(new Surface("assets/aagun.tga"), 36);
	Character character(100, 100, rotatingGun);

	static Player player("Gladiator", 100, 100, character, abilities);
	
	static int frame = 0;

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{
		float deltaTimeS = deltaTime / 1000.0f;
		
		float newPositionX =  player.character.x + speedX * deltaTimeS;
		float newPositionY =  player.character.y + speedY * deltaTimeS;


		// Make sure new positions cannot go out of bounds
		if (newPositionX < 0)
		{
			//log position before it is changed
			printf("X is smaller than 0: %f\n", newPositionX);
			newPositionX = 0;
		}
		if (newPositionX > 800 - player.character.sprite->GetWidth())
		{
			printf("X is bigger than 800 - sprite width: %f\n", newPositionX);
			
			newPositionX = 800 - player.character.sprite->GetWidth() - 1;
		}
		if (newPositionY < 0)
		{
			printf("Y is smaller than 0: %f\n", newPositionY);
			newPositionY = 0;
		}
		if (newPositionY > 512 - player.character.sprite->GetHeight())
		{
			printf("Y is bigger than 512 - sprite Height: %f\n", newPositionY);
			newPositionY = 512 - player.character.sprite->GetHeight() - 1;
		}

		player.character.x = newPositionX;
		player.character.y = newPositionY;
		
		screen->Clear(0);

		//draw a bounding box around the gun for debugging
		screen->Box(player.character.x,  player.character.y,  player.character.x + player.character.sprite->GetWidth(),  player.character.y + player.character.sprite->GetHeight(), 0xffffff);

		player.character.sprite->SetFrame(frame);
		player.character.sprite->Draw(screen, player.character.x, player.character.y);
		
		if (++frame == 36) frame = 0;
	}

	// -----------------------------------------------------------
	// Handle key down events
	// -----------------------------------------------------------
	void Game::KeyDown( int key )
	{
		printf("Key %d was pressed\n", key);
		
		const float speedIncrement = 100.0f;
	
		//W - 26
		//A - 4
		//S - 22
		//D - 7
		
		switch (key)
		{
		case 26:    speedY -= speedIncrement; break; //W
		case 22:  speedY += speedIncrement; break; //S
		case 4:  speedX -= speedIncrement; break; //A
		case 7: speedX += speedIncrement; break; //D
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
};