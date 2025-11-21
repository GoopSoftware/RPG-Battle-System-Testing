#include "TextureManager.h"
#include "raylib.h"
#include <iostream>

void TextureManager::Load(const std::string& name, const std::string& path)
{
    std::cout << "[TextureManager] Loading texture: " << path << "\n";

    Texture2D tex = LoadTexture(path.c_str());

    if (tex.width == 0 || tex.height == 0) {
        std::cerr << "ERROR: Failed to load texture: " << path << "\n";
        abort();
    }

    textures[name] = tex;
}

Texture2D& TextureManager::Get(const std::string& name)
{
    return textures.at(name);
}

void TextureManager::UnloadAll()
{
    for (auto& pair : textures)
        UnloadTexture(pair.second);

    textures.clear();
}
