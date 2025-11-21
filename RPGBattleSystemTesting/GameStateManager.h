#pragma once
#include <memory>

#include "OverworldSystem.h"
#include "BattleSystem.h"
#include "Encounter.h"
#include "SpriteComponent.h"
#include "PositionComponent.h"
#include "TextureManager.h"
#include "AnimationSystem.h"
#include "OverworldMapLoader.h"
#include "OverworldMap.h"




enum class GameState {
	OVERWORLD,
	BATTLE,
	MENU,
	GAME_OVER
};

class GameStateManager
{
public:
	GameStateManager(float deltaTime,
					 std::vector<Entity> players,
					 std::unordered_map<Entity, HealthComponent>& healthStore,
					 std::unordered_map<Entity, CombatStatsComponent>& statsStore,
					 std::unordered_map<Entity, NameComponent>& nameStore,
					 std::unordered_map<Entity, SpriteComponent>& spriteStore,
					 std::unordered_map<Entity, PositionComponent>& positionStore);
	~GameStateManager();

	void init();
	void update();
	void triggerEncounter();
	GameState getCurrentState() const { return currentState; }
	
	//OverworldSystem& getOverworld() { return overworld; }
	const OverworldSystem& getOverworld() const { return overworld; }
	const OverworldMap& getOverworldMap() const { return overworldMap; }


	//BattleSystem* getBattleSystem() { return battleSystem.get(); }
	const BattleSystem* getBattleSystem() const { return battleSystem.get(); }
	SpriteComponent& getSprite(Entity e) const { return spriteStore.at(e); }
	PositionComponent& getPosition(Entity e) const { return positionStore.at(e); }

	AnimationSystem animationSystem;


private:
	float deltaTime;

	GameState currentState = GameState::OVERWORLD;
	OverworldSystem overworld;
	OverworldMap overworldMap;
	// MenuSystem menu;  ----- For when MenuSystem is done -----


	std::vector<Entity> players;
	std::unordered_map<Entity, HealthComponent>& healthStore;
	std::unordered_map<Entity, CombatStatsComponent>& statsStore;
	std::unordered_map<Entity, NameComponent>& nameStore;
	std::unordered_map<Entity, SpriteComponent>& spriteStore;
	std::unordered_map<Entity, PositionComponent>& positionStore;

	Encounter currentEncounter;
	std::unique_ptr<BattleSystem> battleSystem; // ptr created when battle triggered

};