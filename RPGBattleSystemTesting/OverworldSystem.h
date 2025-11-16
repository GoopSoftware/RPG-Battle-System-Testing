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

class RenderSystem;

enum class Biome {
	FOREST,
	OCEAN,
	MOUNTAINS
};


class OverworldSystem
{
public:
	OverworldSystem();
	~OverworldSystem();
	
	void update();
	void encounterCheck();

	Encounter generateEncounter(std::unordered_map<Entity, HealthComponent>& healthStore,
								std::unordered_map<Entity, CombatStatsComponent>& statsStore,
								std::unordered_map<Entity, NameComponent>& nameStore);

	bool getEncounter() { return encounter; }
	void clearEncounter() { encounter = false; }
	void draw(RenderSystem& renderer) const;


private:

	DebugSystem debug;
	Vector2 partyPosition = { 0, 0 };
	int encounterRate; // Unused for now but allows for scaling encounter chance
	
	// Flag used to trigger when a battle should happen, not when a battle is happening. 
	// State::BATTLE is for when a battle is happening
	bool encounter = false;
};

