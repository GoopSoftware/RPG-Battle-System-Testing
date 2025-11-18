#pragma once
#include "raylib.h"
#include <vector>
#include "iostream"

class TextureManager
{

public:
	TextureManager();
	~TextureManager();

	Texture2D goblinTexture;
	std::vector<Texture2D> textures;

	void debugPrint() const {
		std::cout << "[TextureManager] GoblinTexture ID: " << goblinTexture.id << "\n";
	}
private:



};

