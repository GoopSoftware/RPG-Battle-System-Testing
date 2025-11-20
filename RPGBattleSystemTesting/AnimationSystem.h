#pragma once
#include <unordered_map>
#include "Entity.h"
#include "SpriteComponent.h"


class AnimationSystem
{

public:
	AnimationSystem(std::unordered_map<Entity, SpriteComponent>& sprites);
	void update(float deltaTime);
	void updateSprite(SpriteComponent& sprite, float deltaTime);
private:
	std::unordered_map<Entity, SpriteComponent>& sprites;

};

