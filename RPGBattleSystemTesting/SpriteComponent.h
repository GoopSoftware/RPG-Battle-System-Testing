#pragma once
#include "raylib.h"


enum class AnimationState {
	Idle,
	Walk,
	Attack,
	Hurt,
	Dead
};

struct SpriteComponent {

	Texture2D texture;
	int frameWidth = 0;
	int frameHeight= 0;
	float scale = 1.0f;

	int maxFrames;
	int currentFrame = 0;

	float frameTime = 0.15f;
	float frameTimer = 0.0f;;
	
	AnimationState state = AnimationState::Idle;

	Vector2 offset{ frameWidth * .5f ,frameHeight * .5f };
	Color tint = WHITE;

};