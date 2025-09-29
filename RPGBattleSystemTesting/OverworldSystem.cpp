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
	}
	if (IsKeyPressed(KEY_S)) {
		std::cout << "Moved Down\n";
	}
	if (IsKeyPressed(KEY_A)) {
		std::cout << "Moved Left\n";
	}
	if (IsKeyPressed(KEY_D)) {
		std::cout << "Moved Right\n";
	}
}

void OverworldSystem::encounterTriggered() {
	std::cout << "encounter triggerd\n";
}

void OverworldSystem::generateEncounter() {
	std::cout << "Generating Encounter()\n";
}