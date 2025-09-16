#include "Enemy.h"
#include <raylib.h>


Enemy::Enemy() {

}

Enemy::~Enemy() {

}

int Enemy::decideAction() {
	int decision = GetRandomValue(1, 3);
	if (decision == 1) {
		return 1;
	}
	else if (decision == 2) {
		return 2;
	}
	else if (decision == 3) {
		return 3;
	}
}
/*
int Enemy::calculateDamage() {

}
*/