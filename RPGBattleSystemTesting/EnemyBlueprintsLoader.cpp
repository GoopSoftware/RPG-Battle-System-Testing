#include "EnemyBlueprintsLoader.h"


#include <fstream>
#include <nlohmann/json.hpp>

using nlohmann::json;


EnemyBlueprintsDb loadEnemyBlueprintsFromFile(const std::string& path) {
	std::ifstream in(path);
	json root;
	in >> root;

	EnemyBlueprintsDb db;
	db.schemaVersion = root.value("schemaVersion", 1);

	// ej = enemyJson
	for (const auto& ej : root.at("enemies")) {
		EnemyBlueprint enemy;
		enemy.id = ej.at("id").get<std::string>();
		enemy.displayName = ej.value("displayName", enemy.id);
		enemy.baseName = ej.value("baseName", enemy.displayName);

		if (ej.contains("tags")) {
			enemy.tags = ej.at("tags").get<std::vector<std::string>>();
		}

		// stats
		enemy.health.maxHp = ej.at("health").at("maxHp").get<int>();
		enemy.stats.attack = ej.at("stats").at("attack").get<int>();
		enemy.stats.defense = ej.at("stats").at("defense").get<int>();
		enemy.stats.speed = ej.at("stats").at("speed").get<int>();

		// Sprite info spj = spriteJson
		const auto& spj = ej.at("sprite");
		enemy.sprite.textureKey = spj.at("textureKey").get<std::string>();
		enemy.sprite.columns = spj.at("columns").get<int>();
		enemy.sprite.rows = spj.at("rows").get<int>();
		// Get value from scale, if there is none default to 1.0f
		enemy.sprite.scale = spj.value("scale", 1.0f);

		if (spj.contains("animations")) {
			// aj = animationJson
			for (const auto& aj : spj.at("animations")) {
				AnimationClipData clip;
				clip.state = aj.at("state").get<std::string>();
				clip.startFrame = aj.at("startFrame").get<int>();
				clip.endFrame = aj.at("endFrame").get<int>();
				// get value from frameTime defualt to 0.15f if none exists
				clip.frameTime = aj.value("frameTime", 0.15f);
				enemy.sprite.animations.push_back(std::move(clip));
			}
		}

		// insert into db
		db.byId.emplace(enemy.id, std::move(enemy));

	}

	return db;
}