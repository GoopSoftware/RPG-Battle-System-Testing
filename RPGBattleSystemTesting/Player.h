#pragma once
class Player
{

public:
	Player();
	~Player();


	int health{ 100 };
	int attack{ 10 };
	int defense{ 10 };


	int calculateDamage();


private:

	int damage;

};

