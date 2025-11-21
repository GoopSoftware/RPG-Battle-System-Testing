#pragma once
#include <iostream>
#include <vector>

#include "raylib.h"

class OverworldMap
{

public:
	int width = 0;
	int height = 0;
	int tileWidth = 0;
	int tileHeight = 0;

	struct TileLayer {
		std::string name;
		std::vector<int> tiles;
	};

	std::vector<TileLayer> visualLayers;
	std::vector<int> collision;
	std::vector<int> regions;

	Texture2D tilesetTexture;
	int firstGid = 1;

	int getTile(const TileLayer& layer, int x, int y) const {
		return layer.tiles[y * width + x];
	}

private:


};

