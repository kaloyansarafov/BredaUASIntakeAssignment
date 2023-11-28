#pragma once
#include "surface.h"

namespace Tmpl8 {

class Surface;
class Game
{
public:
	void SetTarget( Surface* surface ) { screen = surface; }
	void Init();
	void Shutdown();
	void Tick( float deltaTime );
	void MouseUp( int button );
	void MouseDown( int button );
	void MouseMove( int x, int y );
	void KeyUp( int key );
	void KeyDown( int key );
	void DrawCharacter(char ch, int startX, int startY, int size, Pixel color);
	
private:
	Surface* screen;
	int CalculateSpriteFrame();
};

}; // namespace Tmpl8