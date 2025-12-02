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
	int firstGid = 1;

	struct TileLayer {
		std::string name;
		std::vector<int> tiles;
	};

	TileLayer terrain;
	TileLayer grass;
	TileLayer mountain;
	TileLayer water;

	std::vector<TileLayer> visualLayers;

	std::vector<int> collision;
	std::vector<int> regions;


	Texture2D tilesetTexture;

	int getTile(const TileLayer& layer, int x, int y) const {
		return layer.tiles[y * width + x];
	}

	inline bool isBlocked(int x, int y) const {
		return collision[y * width + x];
	}

private:


};

