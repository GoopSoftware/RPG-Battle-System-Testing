#include "TextureManager.h"


TextureManager::TextureManager() :
	textures(textures)
{

}


TextureManager::~TextureManager() {

	for (int i = 0; i < textures.size(); i++) {
		UnloadTexture(textures[i]);
	}
}
