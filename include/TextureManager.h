#pragma once
#include "ResourceManager.h"
#include <SFML/Graphics/Texture.hpp>

class TextureManager: public ResourceManager<TextureManager, sf::Texture>
{
public:
	TextureManager() : ResourceManager(Utils::loadConfig("textures.cfg")) {}

	sf::Texture* Load(const std::string& l_path) override
	{
		sf::Texture* texture = new sf::Texture();
		if(!texture->loadFromFile(
			Utils::loadAsset(l_path)))
		{
			delete texture;
			texture = nullptr;
			std::cerr << "! Failed to load texture: " << l_path << std::endl;
		}

		return texture;
	}
};