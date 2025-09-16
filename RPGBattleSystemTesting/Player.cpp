#include "Player.h"
#include <raylib.h>

Player::Player() {

}

Player::~Player() {


}

int Player::calculateDamage() {
	return GetRandomValue(1, 25) + attack;
}