#include "RenderSystem.h"
#include "GameStateManager.h"
#include "SpriteComponent.h"
#include "PositionComponent.h"



#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif
//#define CLAMP(x, lower, upper) ( ((x)<(lower))?(lower):(((x)>(upper))?(upper):(x)) )


/*
Checks game state from GameStateManager
Runs renderState() and renderStateUI() based on state

Renders only the current frame of the sprite component

drawSprite(): 
	- DrawTexturePro(sprite.texture, src, dest, { 0, 0 }, 0.0f, sprite.tint)
	- src animation frame changes from:
		- static_cast<float>(sprite.frameWidth * sprite.currentFrame)
			- sprite.currentFrame changes based on updateAnimation()



*/

RenderSystem::RenderSystem(int windowWidth, int windowHeight, int targetWidth, int targetHeight) :
	windowWidth(windowWidth), 
	windowHeight(windowHeight), 
	targetWidth(targetWidth), 
	targetHeight(targetHeight)
{

}

RenderSystem::~RenderSystem() 
{
	shutdown();
}

void RenderSystem::updateOverworldCamera(GameStateManager& game) {
	Entity player = game.getOverworld().getPlayer();
	const PositionComponent& pos = game.getPosition(player);
	const OverworldMap& map = game.getOverworldMap();

	float worldWidth = map.width * map.tileWidth;
	float worldHeight = map.height * map.tileHeight;

	// Smooth camera follow (lerp)
	float speed = 10.0f * GetFrameTime();

	Vector2 target = { pos.x, pos.y };

	overworldCamera.target.x += (target.x - overworldCamera.target.x) * speed;
	overworldCamera.target.y += (target.y - overworldCamera.target.y) * speed;

	// Clamp camera to map bounds
	float halfW = overworldCamera.offset.x / overworldCamera.zoom;
	float halfH = overworldCamera.offset.y / overworldCamera.zoom;

	overworldCamera.target.x = std::clamp(overworldCamera.target.x, halfW, worldWidth - halfW);
	overworldCamera.target.y = std::clamp(overworldCamera.target.y, halfH, worldHeight - halfH);

}

void RenderSystem::init() {

	InitWindow(windowWidth, windowHeight, "C++ RPG");
	SetTargetFPS(60);

	overworldCamera.target = { 0, 0 };
	overworldCamera.offset = { targetWidth / 2.0f, targetHeight / 2.0f };
	overworldCamera.rotation = 0.0f;
	overworldCamera.zoom = 1.0f;

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
			updateOverworldCamera(game);

			BeginMode2D(overworldCamera);
			renderOverworld(game);
			renderOverworldPlayer(game);
			EndMode2D();

			renderOverworldUI(game);


			break;
		case GameState::BATTLE:
			renderBattleBG(game);
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

	int totalCols = sprite.columns;
	int col = sprite.currentFrame % totalCols;
	int row = sprite.currentFrame / totalCols;

	Rectangle src = {
		static_cast<float>(col * sprite.frameWidth),
		static_cast<float>(row * sprite.frameHeight),
		(sprite.flipX ? -sprite.frameWidth : sprite.frameWidth),
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



void RenderSystem::renderUI(GameStateManager& game) {


}

void RenderSystem::renderOverworldUI(GameStateManager& game) {


}

void RenderSystem::renderOverworldPlayer(GameStateManager& game) {

	Entity player = game.getOverworld().getPlayer();

	SpriteComponent& sprite = game.getSprite(player);
	PositionComponent& pos = game.getPosition(player);

	switch (game.getOverworld().getMoveDirection())
	{

		case MoveDirection::Left:
			sprite.flipX = true;
			game.animationSystem.setState(sprite, AnimationState::Walk);
			break;
		case MoveDirection::Right:
			sprite.flipX = false;
			game.animationSystem.setState(sprite, AnimationState::Walk);
			break;
		case MoveDirection::Up:
		case MoveDirection::Down:
			game.animationSystem.setState(sprite, AnimationState::Walk);
			break;
		case MoveDirection::None:
			game.animationSystem.setState(sprite, AnimationState::Idle);
			break;
	}
	// Draw sprite
	game.animationSystem.updateSprite(sprite, GetFrameTime());
	drawSprite(player, sprite, pos);


}



void RenderSystem::renderOverworld(GameStateManager& game)
{
	const OverworldMap& map = game.getOverworldMap();

	drawTileLayer(map, map.terrain);
	
	/*drawTileLayer(map, map.grass);
	drawTileLayer(map, map.mountain);
	drawTileLayer(map, map.water);*/

	// Optional future extra visual layers:
	for (const auto& layer : map.visualLayers)
		drawTileLayer(map, layer);
}

void RenderSystem::drawTileLayer(const OverworldMap& map, const OverworldMap::TileLayer& layer)
{
	if (layer.tiles.empty()) return;

	for (int y = 0; y < map.height; ++y)
	{
		for (int x = 0; x < map.width; ++x)
		{
			int tileId = map.getTile(layer, x, y);
			if (tileId == 0) continue;

			int index = tileId - map.firstGid;
			int tilesPerRow = map.tilesetTexture.width / map.tileWidth;

			int srcX = (index % tilesPerRow) * map.tileWidth;
			int srcY = (index / tilesPerRow) * map.tileHeight;

			Rectangle src = {
				(float)srcX,
				(float)srcY,
				(float)map.tileWidth,
				(float)map.tileHeight
			};

			Vector2 dest = {
				(float)x * map.tileWidth,
				(float)y * map.tileHeight
			};

			DrawTextureRec(map.tilesetTexture, src, dest, WHITE);
		}
	}
}



void RenderSystem::renderBattleUI(GameStateManager& game) {


}

void RenderSystem::renderBattleBG(GameStateManager& game) {

	Texture2D bg = TextureManager::Get().Get("BattleBG");

	Rectangle src = { 0, 0, (float)bg.width, (float)bg.height };
	Rectangle dst = { 0, 0, (float)targetWidth, (float)targetHeight };

	DrawTexturePro(bg, src, dst, { 0.f, 0.f }, 0.f, WHITE);

}

void RenderSystem::renderBattle(GameStateManager& game) {


	const BattleSystem* battle = game.getBattleSystem();

	// DEBUG
	DebugSystem::handleAnimationDebug(game, battle);
	// END DEBUG

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

		game.animationSystem.updateSprite(sprite, GetFrameTime());


		if (battle->isTargeting() && i == battle->getTargetIndex()) {
			drawSpriteOutlined(e, sprite, pos);
		}
		else {
			drawSprite(e, sprite, pos);
		}

	}

	battle->draw(*this);

}