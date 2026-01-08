#pragma once
#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "raylib.h"
#include "ContentDatabase.h"
#include "EncounterTypes.h"
#include "Encounter.h"

#include "HealthComponent.h"
#include "CombatStatsComponent.h"
#include "NameComponent.h"
#include "SpriteComponent.h"
#include "PositionComponent.h"



class EncounterSpawner
{

public:
	EncounterSpawner(
		const ContentDatabase& contentDb,
		std::unordered_map<Entity, HealthComponent>& healthStore,
		std::unordered_map<Entity, CombatStatsComponent>& statsStore,
		std::unordered_map<Entity, NameComponent>& nameStore,
		std::unordered_map<Entity, SpriteComponent>& spriteStore,
		std::unordered_map<Entity, PositionComponent>& positionStore
	);

	Encounter spawn(const EncounterResult& result);

private:
	const ContentDatabase& contentDb;

	std::unordered_map<Entity, HealthComponent>& healthStore;
	std::unordered_map<Entity, CombatStatsComponent>& statsStore;
	std::unordered_map<Entity, NameComponent>& nameStore;
	std::unordered_map<Entity, SpriteComponent>& spriteStore;
	std::unordered_map<Entity, PositionComponent>& positionStore;

	static AnimationState toAnimationState(const std::string& stateStr);
	static std::vector<Vector2> calculateEnemyEncounterPosition(int totalEnemies, float screenWidth, float screenHeight);

};

