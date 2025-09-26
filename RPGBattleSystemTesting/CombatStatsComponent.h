#pragma once

// Stats component for entities
// in the future we may be able to make these const and when we
// create the level up logic we can re assign values rather than change.
// TODO: Make decision on ^ when the level up system is created

struct CombatStatsComponent {
	int attack;
	int defense;
	int speed;
};