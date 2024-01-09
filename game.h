#pragma once
#include <utility>

#include "surface.h"

namespace Tmpl8 {
	class character;

	class Surface;
class Game
{
public:
	void SetTarget( Surface* surface ) { screen = surface; }
	void Init();
	void Shutdown();
	void Tick( float deltaTime );
	void MouseUp( int button ) {};
	void MouseDown( int button );
	void MouseMove( int x, int y );
	void KeyUp( int key );
	void KeyDown( int key );
	void DrawCharacter(char ch, int startX, int startY, int size, Pixel color);
	
private:
	Surface* screen;
	int CalculateSpriteFrame(character origin, int x, int y, int offset, int frameCount);
	std::pair<int, int> CalculateSpritePosition(float deltaTime);
	void DecreaseCooldowns(float deltaTime);
	void HandleEnemies(float deltaTime);
	void MainMenu() const;
	void GameOver() const;
	void Playing(float deltaTime);
	void Paused() const;
	void HandleButtons();
	void HandleMusic();
};

}; // namespace Tmpl8