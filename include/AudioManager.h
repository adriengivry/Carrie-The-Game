#pragma once
#include "ResourceManager.h"
#include <SFML/Audio.hpp>

class AudioManager : public ResourceManager<AudioManager, sf::SoundBuffer>
{
public:
	AudioManager() : ResourceManager(Utils::loadConfig("audio.cfg")) {}

	sf::SoundBuffer* Load(const std::string& l_path) override
	{
		sf::SoundBuffer* sound = new sf::SoundBuffer();
		if (!sound->loadFromFile(
			Utils::loadAsset(l_path)))
		{
			delete sound;
			sound = nullptr;
			std::cerr << "! Failed to load sound: " << l_path << std::endl;
		}
		return sound;
	}
};