#include "AnimationSystem.h"

AnimationSystem::AnimationSystem(std::unordered_map<Entity, SpriteComponent>& sprites) :
	sprites(sprites)

{

}

void AnimationSystem::update(float deltaTime) {
	for (auto& [entity, sprite] : sprites) {
		if (sprite.maxFrames > 1) {
			updateSprite(sprite, deltaTime);
		}
	}
}

void AnimationSystem::updateSprite(SpriteComponent& sprite, float deltaTime) {
	sprite.frameTimer += deltaTime;

	if (sprite.frameTimer >= sprite.frameTime) {
		sprite.frameTimer = 0.0f;

		// Single animation state system. Will change into multiple branch later based on spritecomponent state
		sprite.currentFrame = (sprite.currentFrame + 1) % sprite.maxFrames;
		//switch(sprite.animationState)
	}

}