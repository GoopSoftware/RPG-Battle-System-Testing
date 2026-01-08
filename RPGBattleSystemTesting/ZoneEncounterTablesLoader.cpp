#include "ZoneEncounterTablesLoader.h"


#include <fstream>
#include <stdexcept>
#include "nlohmann/json.hpp"

using nlohmann::json;


ZoneEncounterTablesDb loadZoneEncounterTablesFromFile(const std::string& path) {
	std::ifstream in(path);
	if (!in.is_open()) {
		throw std::runtime_error("Failed to open file: " + path);
	}

	json root;
	in >> root;

	ZoneEncounterTablesDb db;
	db.schemaVersion = root.value("schemaVersion", 1);

	// zj = zoneJson
	for (const auto& zj : root.at("zones")) {
		ZoneEncounterTable zone;
		zone.id = zj.at("zoneId").get<std::string>();


		for (const auto& ej : zj.at("encounters")) {
			WeightedTemplateChoice choice;
			choice.templateId = ej.at("templateId").get<std::string>();
			choice.weight = ej.value("weight", 1);
			zone.encounters.push_back(std::move(choice));
		}


		db.byZoneId.emplace(zone.id, std::move(zone));
	}

	return db;

}