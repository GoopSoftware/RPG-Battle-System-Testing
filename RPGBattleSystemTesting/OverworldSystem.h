#pragma once
#include <vector>
#include <raylib.h>
#include <iostream>
#include <unordered_map>

#include "Entity.h"
#include "Encounter.h"
#include "HealthComponent.h"
#include "CombatStatsComponent.h"
#include "NameComponent.h"
#include "DebugSystem.h"
#include "SpriteComponent.h"
#include "PositionComponent.h"
#include "RenderSystem.h"
#include "TextureManager.h"



class RenderSystem;

enum class Biome {
	FOREST,
	OCEAN,
	MOUNTAINS
};


class OverworldSystem
{
public:
	OverworldSystem(TextureManager& textureManager);
	~OverworldSystem();
	
	void update();
	void encounterCheck();

	Encounter generateEncounter(std::unordered_map<Entity, HealthComponent>& healthStore,
								std::unordered_map<Entity, CombatStatsComponent>& statsStore,
								std::unordered_map<Entity, NameComponent>& nameStore,
								std::unordered_map<Entity, SpriteComponent>& spriteStore,
								std::unordered_map<Entity, PositionComponent>& positionStore);

	bool getEncounter() { return encounter; }
	void clearEncounter() { encounter = false; }
	void draw(RenderSystem& renderer) const;


private:

	TextureManager& textureManager;
	DebugSystem debug;
	Vector2 partyPosition = { 0, 0 };
	int encounterRate; // Unused for now but allows for scaling encounter chance
	
	bool encounter = false;
};

