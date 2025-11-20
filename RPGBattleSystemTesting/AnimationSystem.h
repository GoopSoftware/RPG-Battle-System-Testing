#pragma once
#include <unordered_map>
#include "Entity.h"
#include "SpriteComponent.h"


/*
Scalable sprite animation system
We feed in a sprite into update() and it will apply the animation math to the sprites current frame 


updateAnimation():
	- updateAnimation(SpriteComponent& sprite, float dt)
	- increments time with
		- sprite.frameTimer += dt
	- changes animation frames with
		- sprite.currentFrame = (sprite.currentFrame + 1) % sprite.maxFrames

*/



class AnimationSystem
{

public:
	AnimationSystem(std::unordered_map<Entity, SpriteComponent>& sprites);
	void updateSprite(SpriteComponent& sprite, float deltaTime);
	void setState(SpriteComponent& sprite, AnimationState newState);
private:
	std::unordered_map<Entity, SpriteComponent>& sprites;
	
};

