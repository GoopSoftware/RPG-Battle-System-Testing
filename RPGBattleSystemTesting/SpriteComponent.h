#pragma once
#include "raylib.h"
#include "unordered_map"

enum class AnimationState {
	Idle,
	Walk,
	Attack,
	Attack2,
	Attack3,
	Hurt,
	Dead
};

struct AnimationData {
	int startFrame;
	int endFrame;
	float frameTime;
};


struct SpriteComponent {

	Texture2D texture;
	int frameWidth = 0;
	int frameHeight= 0;
	int columns;
	int rows;

	float scale = 1.0f;

	//int maxFrames;
	int currentFrame = 0;

	//float frameTime = 0.15f;
	float frameTimer = 0.0f;
	
	AnimationState state = AnimationState::Idle;
	std::unordered_map<AnimationState, AnimationData> animations;


	Vector2 offset{ 0, 0 };
	Color tint = WHITE;

};