#pragma once
#include <string>
#include "raylib.h"

class GameStateManager;

class RenderSystem
{
public:
	RenderSystem(int windowWidth, int windowHeight, int targetWidth, int targetHeight);
	~RenderSystem();
	
	void init();
	void shutdown();

	void begin();
	void renderer(GameStateManager& game);
	void end();

	void clear(Color color = RAYWHITE);
	void drawTestPattern();

private:

	RenderTexture2D target;

	int windowWidth;
	int windowHeight;
	int targetWidth;
	int targetHeight;


	void renderOverworld(GameStateManager& game);
	void renderBattle(GameStateManager& game);
	void renderUI(GameStateManager& game);

};

