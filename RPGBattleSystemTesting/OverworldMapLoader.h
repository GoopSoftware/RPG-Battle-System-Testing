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
		std::string tsxPath = json["tilesets"][0]["source"];

		std::string pngFileName = convertTsxToPng(tsxPath); // "TestTileset.png"
		std::string pngPath = "Assets/tilesets/" + pngFileName;

		std::cout << "Loading tileset: " << pngPath << std::endl;

		TextureManager::Get().Load("overworld_tileset", pngPath);
		outMap.tilesetTexture = TextureManager::Get().Get("overworld_tileset");



		for (auto& layer : json["layers"]) {
			std::string name = layer["name"];
			auto data = layer["data"].get<std::vector<int>>();

			if (name == "Collision") {
				outMap.collision = data;
			}
			else if (name == "Regions") {
				outMap.regions = data;
			}
			else {
				OverworldMap::TileLayer tl;
				tl.name = name;
				tl.tiles = data;
				outMap.visualLayers.push_back(tl);

			}
		}

		// DEBUG ---------------------------------------------------------------------------
		std::cout << "[MapLoader] Loaded map: " << path << "\n";
		std::cout << "  Size: " << outMap.width << "x" << outMap.height << "\n";
		std::cout << "  TileSize: " << outMap.tileWidth << "x" << outMap.tileHeight << "\n";

		std::cout << "  Visual Layers: " << outMap.visualLayers.size() << "\n";
		for (auto& layer : outMap.visualLayers) {
			std::cout << "     - " << layer.name << " (" << layer.tiles.size() << " tiles)\n";
		}

		std::cout << "  Collision tiles: " << outMap.collision.size() << "\n";
		std::cout << "  Regions tiles:   " << outMap.regions.size() << "\n";

		std::cout << "  tilesetTexture: "
			<< outMap.tilesetTexture.width << "x"
			<< outMap.tilesetTexture.height << "\n";
		// END DEBUG ---------------------------------------------------------------------------
		return true;
	}

private:

	static std::string convertTsxToPng(const std::string& tsxPath) {
		// Example: "../TestTileset.tsx" -> "../TestTileset.png"
		size_t dot = tsxPath.rfind('.');
		return tsxPath.substr(0, dot) + ".png";
	}
};

