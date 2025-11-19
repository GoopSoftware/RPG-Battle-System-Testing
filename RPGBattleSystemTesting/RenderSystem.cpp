#include "RenderSystem.h"
#include "GameStateManager.h"
#include "SpriteComponent.h"
#include "PositionComponent.h"



#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif


/*
Checks game state from GameStateManager
Runs render____() and render____UI() based on state

Renders sprites and animations using information from SpriteComponent, PositionComponent

drawSprite(): 
	- DrawTexturePro(sprite.texture, src, dest, { 0, 0 }, 0.0f, sprite.tint)
	- src animation frame changes from:
		- static_cast<float>(sprite.frameWidth * sprite.currentFrame)
			- sprite.currentFrame changes based on updateAnimation()

updateAnimation(): 
	- updateAnimation(SpriteComponent& sprite, float dt)
	- increments time with
		- sprite.frameTimer += dt
	- changes animation frames with
		- sprite.currentFrame = (sprite.currentFrame + 1) % sprite.maxFrames

*/

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

	outlineShader = LoadShader(0, TextFormat("Assets/Shaders/outline.fs", GLSL_VERSION));
	float outlineSize = 0.5f;
	float outlineColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

	outlineSizeLoc = GetShaderLocation(outlineShader, "outlineSize");
	outlineColorLoc = GetShaderLocation(outlineShader, "outlineColor");
	textureSizeLoc = GetShaderLocation(outlineShader, "textureSize");

	SetShaderValue(outlineShader, outlineSizeLoc, &outlineSize, SHADER_UNIFORM_FLOAT);
	SetShaderValue(outlineShader, outlineColorLoc, outlineColor, SHADER_UNIFORM_VEC4);

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

void RenderSystem::render(GameStateManager& game) {

	switch (game.getCurrentState()) {
		case GameState::OVERWORLD:
			renderOverworld(game);
			renderOverworldUI(game);
			break;
		case GameState::BATTLE:
			renderBattle(game);
			renderBattleUI(game);
			break;
		case GameState::MENU:
			// TODO: ADD menuSystemRender()
			// TODO: ADD menuSystemRenderUI()
		case GameState::GAME_OVER:
			// TODO: ADD gameOverRender()
			// TODO: ADD gameOverRenderUI()
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



void RenderSystem::drawSprite(Entity entity, const SpriteComponent& sprite, const PositionComponent& pos) {

	// Source rect based on animation frame
	Rectangle src = {
	static_cast<float>(sprite.frameWidth * sprite.currentFrame),
	static_cast<float>(0),
	static_cast<float>(sprite.frameWidth),
	static_cast<float>(sprite.frameHeight)
	};


	// Destination rect where we draw on screen
	Rectangle dest = {
		pos.x,
		pos.y,
		sprite.frameWidth * sprite.scale,
		sprite.frameHeight * sprite.scale
	};
	Vector2 origin = {
	dest.width / 2.0f,
	dest.height / 2.0f
	};
	DrawTexturePro(sprite.texture, src, dest, origin, 0.0f, sprite.tint);
}

void RenderSystem::drawSpriteOutlined(Entity entity, const SpriteComponent& sprite, const PositionComponent& pos) {


	float texSize[2] = { static_cast<float>(sprite.texture.width), static_cast<float>(sprite.texture.height) };
	SetShaderValue(outlineShader, textureSizeLoc, texSize, SHADER_UNIFORM_VEC2);

	BeginShaderMode(outlineShader);
	drawSprite(entity, sprite, pos);
	EndShaderMode();
}

// TODO: AnimationSystem refactor this into it
void RenderSystem::updateAnimation(SpriteComponent& sprite, float dt) {
	if (sprite.maxFrames <= 1) return;  // not animated

	sprite.frameTimer += dt;

	if (sprite.frameTimer >= sprite.frameTime)
	{
		sprite.currentFrame = (sprite.currentFrame + 1) % sprite.maxFrames;
		sprite.frameTimer = 0.0f;
	}
}


void RenderSystem::renderUI(GameStateManager& game) {


}

void RenderSystem::renderOverworldUI(GameStateManager& game) {


}

void RenderSystem::renderOverworld(GameStateManager& game) {
	game.getOverworld().draw(*this);

}

void RenderSystem::renderBattleUI(GameStateManager& game) {


}

void RenderSystem::renderBattle(GameStateManager& game) {

	const BattleSystem* battle = game.getBattleSystem();
	// Shouldnt run but just in case
	if (!battle) return;

	auto enemies = battle->getLivingEnemies();

	// For each living enemy in battle:
	for (int i = 0; i < enemies.size(); i++)
	{

		Entity e = enemies[i];

		// get sprite and info from GSM
		SpriteComponent& sprite = game.getSprite(e);
		PositionComponent& pos = game.getPosition(e);

		updateAnimation(sprite, GetFrameTime());


		if (battle->isTargeting() && i == battle->getTargetIndex()) {
			drawSpriteOutlined(e, sprite, pos);
		}
		else {
			drawSprite(e, sprite, pos);
		}

	}

	battle->draw(*this);

}