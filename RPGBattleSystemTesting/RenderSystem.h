#pragma once
#include <string>
#include "raylib.h"
#include "SpriteComponent.h"
#include "PositionComponent.h"
#include "Entity.h"


class GameStateManager;

class RenderSystem
{
public:
	RenderSystem(int windowWidth, int windowHeight, int targetWidth, int targetHeight);
	~RenderSystem();
	
	Texture2D testTexture;

	void init();
	void shutdown();

	void begin();
	void render(GameStateManager& game);
	void end();

	void drawSprite(Entity entity, const SpriteComponent& sprite, const PositionComponent& pos);
	void updateAnimation(SpriteComponent& sprite, float dt);

private:

	RenderTexture2D target;

	int windowWidth;
	int windowHeight;
	int targetWidth;
	int targetHeight;
	
	void renderUI(GameStateManager& game);
	void renderOverworld(GameStateManager& game);
	void renderBattle(GameStateManager& game);
	void renderBattleUI(GameStateManager& game);
	void renderOverworldUI(GameStateManager& game);

};

