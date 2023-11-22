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
	void MouseUp( int button ) { /* implement if you want to detect mouse button presses */ }
	void MouseDown( int button ) { /* implement if you want to detect mouse button presses */ }
	void MouseMove( int x, int y ) { /* implement if you want to detect mouse movement */ }
	void KeyUp( int key );
	void KeyDown( int key );
	void DrawCharacter(char ch, int startX, int startY, int size, Pixel color);
private:
	Surface* screen;
};

}; // namespace Tmpl8