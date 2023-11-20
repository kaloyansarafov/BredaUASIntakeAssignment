#include "game.h"
#include "surface.h"
#include <iostream>
#include <cstdio> //printf
#include <SDL.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <string>

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

	static Sprite rotatingGun(new Surface("assets/aagun.tga"), 36);
	static int frame = 0;

	static float positionX = 100.0f;
	static float positionY = 100.0f;
	static float speedX = 0.0f;
	static float speedY = 0.0f;

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{
		positionX += speedX * deltaTime;
		positionY += speedY * deltaTime;
		
		// clear the graphics window
		screen->Clear(0);
		// print something in the graphics window
		screen->Print("hello world", 2, 2, 0xffffff);
		// draw a sprite
		rotatingGun.SetFrame(frame);
		rotatingGun.Draw(screen, static_cast<int>(positionX), static_cast<int>(positionY));
		if (++frame == 36) frame = 0;
	}

	// -----------------------------------------------------------
	// Handle key down events
	// -----------------------------------------------------------
	void Game::KeyDown( int key )
	{
		//print the key that was pressed
		printf("Key %d was pressed\n", key);
		//print the name of the key that was pressed
		printf("Key %s was pressed\n", SDL_GetKeyName(static_cast<SDL_Keycode>(key)));
		
		// Adjust sprite speed based on arrow keys
		const float speedIncrement = 50.0f;  // Adjust as needed
		switch (key)
		{
		case SDLK_UP:    speedY -= speedIncrement; break;
		case SDLK_DOWN:  speedY += speedIncrement; break;
		case SDLK_LEFT:  speedX -= speedIncrement; break;
		case SDLK_RIGHT: speedX += speedIncrement; break;
		}
	}

	// -----------------------------------------------------------
	// Handle key up events
	// -----------------------------------------------------------
	void Game::KeyUp( int key )
	{
		// Stop sprite movement when arrow keys are released
		switch (key)
		{
		case SDLK_UP:
		case SDLK_DOWN:  speedY = 0.0f; break;
		case SDLK_LEFT:
		case SDLK_RIGHT: speedX = 0.0f; break;
		}
	}
};