#pragma once
#include "raylib.h"
#include <vector>
#include "iostream"
#include "unordered_map"

class TextureManager
{

public:
	static TextureManager& Get() {
		static TextureManager instance;
		return instance;
	}

	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

	void Load(const std::string& name, const std::string& path);
	Texture2D& Get(const std::string& name);
	void UnloadAll();

private:


	TextureManager() {};
	~TextureManager() {};

	std::unordered_map<std::string, Texture2D> textures;


};

