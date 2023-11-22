#include "game.h"
#include "surface.h"
#include <iostream>
#include <cstdio> //printf
#include <SDL.h>
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
		// Calculate new positions
		float newPositionX = positionX + speedX * deltaTime;
		float newPositionY = positionY + speedY * deltaTime;

		// Make sure new positions cannot go out of bounds
		if (newPositionX < 0) newPositionX = 0;
		if (newPositionX > 800 - rotatingGun.GetWidth()) newPositionX = 800 - rotatingGun.GetWidth();
		if (newPositionY < 0) newPositionY = 0;
		if (newPositionY > 512 - rotatingGun.GetHeight()) newPositionY = 512 - rotatingGun.GetHeight();

		// Update positions
		positionX = newPositionX;
		positionY = newPositionY;
		
		// clear the graphics window
		screen->Clear(0);
		// print something in the graphics window
		screen->Print("Welcome to the arena!", 2, 2, 0xffffff);

		//rotate the gun
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
		
		// Adjust sprite speed based on WASD keys
		const float speedIncrement = 1.0f;  // Adjust as needed
	
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
		// Stop sprite movement when arrow keys are released
		switch (key)
		{
		case 26:
		case 22:  speedY = 0.0f; break; //W, S
		case 4:
		case 7: speedX = 0.0f; break; //A, D
		}
	}
};