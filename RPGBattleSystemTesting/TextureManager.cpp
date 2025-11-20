#include "TextureManager.h"
#include "raylib.h"
#include <iostream>

void TextureManager::Load(const std::string& name, const std::string& path)
{
    Texture2D tex = LoadTexture(path.c_str());
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
