#include "RenderSystem.h"
#include "GameStateManager.h"
#include "SpriteComponent.h"
#include "PositionComponent.h"



RenderSystem::RenderSystem(int windowWidth, int windowHeight, int targetWidth, int targetHeight) :
	windowWidth(windowWidth), windowHeight(windowHeight), targetWidth(targetWidth), targetHeight(targetHeight)
{

}

RenderSystem::~RenderSystem() 
{
	shutdown();
}


void RenderSystem::init() {

	InitWindow(windowWidth, windowHeight, "C++ RPG");
	SetTargetFPS(60);

	target = LoadRenderTexture(targetWidth, targetHeight);
	SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
}

void RenderSystem::shutdown() {
	if (IsWindowReady()) {
		UnloadRenderTexture(target);
		CloseWindow();
	}
}

void RenderSystem::begin() {
	BeginTextureMode(target);
	ClearBackground(RAYWHITE);
}

void RenderSystem::renderer(GameStateManager& game) {

	switch (game.getCurrentState()) {
		case GameState::OVERWORLD:
			renderOverworld(game);
			break;
		case GameState::BATTLE:
			renderBattle(game);
			break;
		case GameState::MENU:
			// TODO: ADD MENUSYSTEMRENDER
		case GameState::GAME_OVER:
			// TODO: ADD GAMEOVERRENDER
		default:
			break;
	}
	renderUI(game);

}

void RenderSystem::end() {
	EndTextureMode();

	BeginDrawing();
	ClearBackground(RAYWHITE);

	float screenWidth = static_cast<float>(GetScreenWidth());
	float screenHeight = static_cast<float>(GetScreenHeight());

	Rectangle src = { 0, 0, (float)target.texture.width, -(float)target.texture.height };
	Rectangle dst = { 0, 0, screenWidth, screenHeight };
	DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0, WHITE);

	EndDrawing();
} 


void RenderSystem::renderOverworld(GameStateManager& game) {
	game.getOverworld().draw(*this);

}

void RenderSystem::renderBattle(GameStateManager& game) {
	game.getBattleSystem()->draw(*this);

}

void RenderSystem::renderUI(GameStateManager& game) {


}

void RenderSystem::drawSprite(Entity entity, const SpriteComponent& sprite, const PositionComponent& pos) {

	// Source rect based on animation frame
	Rectangle src = {
		(float)(sprite.frameWidth * sprite.currentFrame),
		0.0f,
		(float)sprite.frameWidth,
		(float)sprite.frameHeight
	};

	// Destination rect where we draw on screen
	Rectangle dest = {
		pos.x + sprite.offset.x,
		pos.y + sprite.offset.y,
		sprite.frameWidth * sprite.scale,
		sprite.frameHeight * sprite.scale
	};

	// Draw
	DrawTexturePro(sprite.texture, src, dest,
		{ 0, 0 }, 0.0f, sprite.tint);

}

// TODO: AnimationSystem refactor this into it
void RenderSystem::updateAnimation(SpriteComponent& sprite, float dt)
{
	if (sprite.maxFrames <= 1) return;  // not animated

	sprite.frameTimer += dt;

	if (sprite.frameTimer >= sprite.frameTime)
	{
		sprite.currentFrame = (sprite.currentFrame + 1) % sprite.maxFrames;
		sprite.frameTimer = 0.0f;
	}
}