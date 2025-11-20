#include "AnimationSystem.h"

AnimationSystem::AnimationSystem(std::unordered_map<Entity, SpriteComponent>& sprites) :
	sprites(sprites)

{

}

void AnimationSystem::setState(SpriteComponent& sprite, AnimationState newState) {

	if (sprite.state != newState) {
		sprite.state = newState;

		// Reset to first frame of the new animation
		const AnimationData& anim = sprite.animations[newState];
		sprite.currentFrame = anim.startFrame;
		sprite.frameTimer = 0.0f;
	}
}



void AnimationSystem::updateSprite(SpriteComponent& sprite, float deltaTime) {

	const AnimationData& anim = sprite.animations[sprite.state];

	sprite.frameTimer += deltaTime;

	if (sprite.frameTimer >= anim.frameTime) {
		sprite.frameTimer = 0.0f;

		sprite.currentFrame++;

		if (sprite.currentFrame > anim.endFrame) {
			sprite.currentFrame = anim.startFrame;
		}
	}
}