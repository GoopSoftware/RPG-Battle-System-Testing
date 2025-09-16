#include <iostream>
#include <raylib.h>


#include "Player.h"
#include "Enemy.h"
#include "BattleSystem.h"


/*
This is a text based testing of an RPG battle system

The initiation to the battle is bool battleActive.

The structure of 

*/



int main() {


	Player player;
	Enemy enemy;


	BattleSystem battleSystem(&player, &enemy);

	SetRandomSeed(time(NULL));

	bool battleActive = true;
	int userInput;

	while (battleActive) {

		battleSystem.battleState();

	}
	
	return 0;
}


