#pragma once
#include "raylib.h"

struct SpriteComponent {

	Texture2D texture;
	int frameWidth = 0;
	int frameHeight= 0;
	int maxFrames;
	int currentFrame = 0;
	float frameTime = 0.15f;
	float frameTimer = 0.0f;;
	
	Vector2 offset{ frameWidth * .5f ,frameHeight * .5f };
	float scale = 1.0f;
	Color tint = WHITE;

};