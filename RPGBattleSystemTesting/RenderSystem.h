#pragma once
#include <string>
#include "raylib.h"
#include "SpriteComponent.h"
#include "PositionComponent.h"
#include "Entity.h"
#include "DebugSystem.h"
#include "TextureManager.h"


class GameStateManager;

class RenderSystem
{
public:
	RenderSystem(int windowWidth, int windowHeight, int targetWidth, int targetHeight);
	~RenderSystem();
	

	void init();
	void shutdown();

	void begin();
	void render(GameStateManager& game);
	void end();

	void drawSprite(Entity entity, const SpriteComponent& sprite, const PositionComponent& pos);
	void drawSpriteOutlined(Entity entity, const SpriteComponent& sprite, const PositionComponent& pos);

private:

	RenderTexture2D target;

	// Shader values for outlining enemies
	Shader outlineShader;
	int outlineSizeLoc;
	int outlineColorLoc;
	int textureSizeLoc;


	int windowWidth;
	int windowHeight;
	int targetWidth;
	int targetHeight;
	
	void renderUI(GameStateManager& game);
	void renderOverworld(GameStateManager& game);
	void renderBattle(GameStateManager& game);
	void renderBattleBG(GameStateManager& game);
	void renderBattleUI(GameStateManager& game);
	void renderOverworldUI(GameStateManager& game);

};

