#pragma once
class Enemy
{
public:
	Enemy();
	~Enemy();


	int health{ 100 };
	int attack{ 10 };
	int defense{ 10 };

	int decideAction();
	int calculateDamage();

private:

};

