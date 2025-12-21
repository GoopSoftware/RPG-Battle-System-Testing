#include "EncounterTemplatesLoader.h"

#include <fstream>
#include <stdexcept>
#include "nlohmann/json.hpp"
#include "EncounterTemplates.h"

using nlohmann::json;

EncounterTemplatesDb loadEncounterTemplatesFromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    json root;
    in >> root;

    EncounterTemplatesDb db;
    db.schemaVersion = root.value("schemaVersion", 1);

    // tj = templateJson
    for (const auto& tj : root.at("templates")) {
        // t = tempalte
        EncounterTemplate t;
        t.id = tj.at("id").get<std::string>();
        // if there is no displayName default id
        t.displayName = tj.value("displayName", t.id);
        // get difficulty default to 1
        t.difficulty = tj.value("difficulty", 1);
        // get formation default to default
        t.formation = tj.value("formation", "default");

        // gj = group json
        for (const auto& gj : tj.at("groups")) {
            EncounterGroup g;
            g.minCount = gj.value("minCount", 1);
            g.maxCount = gj.value("maxCount", g.minCount);

    
            // cj = choiceJson weight values
            for(const auto & cj : gj.at("choices")) {
                WeightedEnemyChoice c;
                c.enemyId = cj.at("enemyId").get<std::string>();
                c.weight = cj.value("weight", 1);
                g.choices.push_back(std::move(c));
            }


            t.groups.push_back(std::move(g));

        }


        db.byId.emplace(t.id, std::move(t));

    }


    return db;

}