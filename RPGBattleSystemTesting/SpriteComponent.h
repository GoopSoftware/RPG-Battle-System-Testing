#pragma once
#include "raylib.h"

struct SpriteComponent {

	Texture2D texture;
	int frameWidth = 0;
	int frameHeight= 0;
	int maxFrames;
	int currentFrame;

	float frameTime;
	float frameTimer;
	
	Vector2 offset;
	float scale = 1.0f;
	Color tint = WHITE;
	
	/*Rectangle sourceRectangle;
	Rectangle destRectangle;
	Vector2 origin;*/

};