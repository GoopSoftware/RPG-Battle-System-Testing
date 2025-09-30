#include "OverworldSystem.h"
#include <iostream>
#include <raylib.h>

OverworldSystem::OverworldSystem()
	
{

}

OverworldSystem::~OverworldSystem() {

}



void OverworldSystem::update() {
	if (IsKeyPressed(KEY_W)) {
		std::cout << "Moved Up\n";
		encounterCheck();
	}
	if (IsKeyPressed(KEY_S)) {
		std::cout << "Moved Down\n";
		encounterCheck();

	}
	if (IsKeyPressed(KEY_A)) {
		std::cout << "Moved Left\n";
		encounterCheck();

	}
	if (IsKeyPressed(KEY_D)) {
		std::cout << "Moved Right\n";
		encounterCheck();

	}
}

void OverworldSystem::encounterCheck() {
	int encounterRoll = GetRandomValue(1, 10);
	if (encounterRoll == 1) { 
		encounter = true;
		std::cout << "Encountered a Battle!";
		generateEncounter();
	}
}


void OverworldSystem::generateEncounter() {
	std::cout << "Generating Encounter()\n";
	// input = overworld information
	// algorithm to generate a battle based on overworld information
	// 

}