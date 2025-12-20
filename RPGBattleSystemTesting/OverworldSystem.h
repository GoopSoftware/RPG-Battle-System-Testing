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


enum class MoveDirection {
	None,
	Up,
	Down,
	Left,
	Right
};

class OverworldSystem
{
public:
	OverworldSystem(std::unordered_map<Entity, HealthComponent>& healthStore,
					std::unordered_map<Entity, CombatStatsComponent>& statsStore,
					std::unordered_map<Entity, NameComponent>& nameStore,
					std::unordered_map<Entity, SpriteComponent>& spriteStore,
					std::unordered_map<Entity, PositionComponent>& positionStore);
	~OverworldSystem();
	
	void update(const OverworldMap& map);
	void encounterCheck();

	void setPlayerEntity(Entity player) { overworldPlayer = player; }

	//void update(float deltaTime, OverworldMap)

	Encounter generateEncounter();

	bool getEncounter() { return encounter; }
	MoveDirection getMoveDirection() const { return moveDirection; }
	void clearEncounter() { encounter = false; }
	void draw(RenderSystem& renderer) const;

	void initializePlayer();

	Entity getPlayer() const { return overworldPlayer; }



private:

	std::unordered_map<Entity, HealthComponent>& healthStore;
	std::unordered_map<Entity, CombatStatsComponent>& statsStore;
	std::unordered_map<Entity, NameComponent>& nameStore;
	std::unordered_map<Entity, SpriteComponent>& spriteStore;
	std::unordered_map<Entity, PositionComponent>& positionStore;

	MoveDirection moveDirection = MoveDirection::None;

	SpriteComponent overworldSprite;
	Entity overworldPlayer;
	std::vector<Vector2> calculateEnemyEncounterPosition(int total, float screenWidth, float screenHeight);
	//DebugSystem debug;
	int encounterRate; // Unused for now but allows for scaling encounter chance
	bool isBlocked(const OverworldMap& map, float x, float y) const;

	bool encounter = false;
};

