#pragma once
#include "Entity.h"
#include <vector>

/*
struct Encounter {

	std::vector<Entity> enemies;
	std::string encounterName;

};
*/
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
	void generateEncounter();
	bool getEncounter() { return encounter; }

private:

	Entity player;

	float deltaTime;

	int encounterRate;
	void encounterCheck();
	bool encounter = false;

	std::string currentBiome = "forest";

};

