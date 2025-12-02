#pragma once
#include <fstream>
#include <nlohmann/json.hpp>
#include "OverworldMap.h"
#include "TextureManager.h"


class OverworldMapLoader
{
public:

	static bool Load(const std::string& path, OverworldMap& outMap) {
		std::ifstream file(path);
		if (!file.is_open()) {
			std::cout << "[DEBUG] Failed to open overworld map:\n";
			return false;
		}

		nlohmann::json json;
		file >> json;

		outMap.width = json["width"];
		outMap.height = json["height"];
		outMap.tileWidth = json["tilewidth"];
		outMap.tileHeight = json["tileheight"];
		outMap.firstGid = json["tilesets"][0]["firstgid"];

		int totalTiles = outMap.width * outMap.height;

		outMap.collision.resize(totalTiles, 0);
		outMap.regions.resize(totalTiles, 0);

		std::string tsxPath = json["tilesets"][0]["source"];
		std::string pngFileName = convertTsxToPng(tsxPath);
		std::string pngPath = "Assets/tilesets/" + pngFileName;

		std::cout << "Loading tileset: " << pngPath << std::endl;

		TextureManager::Get().Load("overworld_tileset", pngPath);
		outMap.tilesetTexture = TextureManager::Get().Get("overworld_tileset");

		for (auto& layer : json["layers"]) {
			std::string name = layer["name"];
			auto data = layer["data"].get<std::vector<int>>();

			if (data.size() != totalTiles) {
				std::cout << "[ERROR] Layer '" << name << "' has incorrect tile count!\n";
				continue;
			}

			if (name == "Collision") {
				for (int i = 0; i < totalTiles; i++) {
					outMap.collision[i] = (data[i] != 0);
				}
			}

			else if (name == "Terrain") { outMap.terrain = { name, data }; }
			else if (name == "Grass") { outMap.grass = { name, data }; }
			else if (name == "Mountain") { outMap.mountain = { name, data }; }
			else if (name == "Water") { outMap.water = { name, data }; }
			else { outMap.visualLayers.push_back({ name, data }); }
		}

		std::cout << "[MapLoader] Loaded map: " << path << "\n"
			<< "  Size: " << outMap.width << "x" << outMap.height << "\n"
			<< "  tileset: " << outMap.tilesetTexture.width
			<< "x" << outMap.tilesetTexture.height << "\n\n";

		return true;
	}

private:

	static std::string convertTsxToPng(const std::string& tsxPath) {
		// Example: "../TestTileset.tsx" -> "../TestTileset.png"
		size_t dot = tsxPath.rfind('.');
		return tsxPath.substr(0, dot) + ".png";
	}
};

