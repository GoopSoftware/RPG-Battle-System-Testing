#include "CombatSystem.h"

std::unordered_map<Entity, HealthComponent> healthStore;
std::unordered_map<Entity, CombatStatsComponent> statsStore;

int calculateDamage(const CombatStatsComponent& attacker,
					const CombatStatsComponent& defender) 
{
	int dmg = attacker.attack - defender.defense;
	return (dmg > 0) ? dmg : 1;
}

void attack(Entity attacker, Entity defender) {
	auto& attackerStats = statsStore[attacker];
	auto& defenderStats = statsStore[defender];
	auto& defenderHealth = healthStore[defender];

	int damage = calculateDamage(attackerStats, defenderStats);
	defenderHealth.hp -= damage;

	std::cout << "Entity " << attacker
		<< " hit Entity " << defender
		<< " for " << damage
		<< " damage! Remaining HP: " << defenderHealth.hp
		<< std::endl;
}
