#pragma once
#include <string>
#include "Entity.h"

struct Encounter {
	std::vector<Entity> enemies; // generated list of enemies created by generateEncounter
	int difficulty; // difficulty value returned by players position in the world and how far in the game for scalability
	std::string zone; // Forest, Ocean, etc 
	std::string encounterName; // For later tracking if needed or special battles "forest_5_goblin_ambush"
};