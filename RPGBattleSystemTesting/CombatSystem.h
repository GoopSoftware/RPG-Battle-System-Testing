#pragma once

#include "Entity.h"
#include "HealthComponent.h"
#include "CombatStatsComponent.h"
#include <unordered_map>
#include <iostream>


extern std::unordered_map<Entity, HealthComponent> healthStore;
extern std::unordered_map<Entity, CombatStatsComponent> statsStore;


int calculateDamage(const CombatStatsComponent& attacker,
					const CombatStatsComponent& defender);

void attack(Entity attacker, Entity defender);