#pragma once
#include "ResourceManager.h"
#include <SFML/Audio.hpp>

enum class AudioType
{
	SOUND,
	MUSIC
};

typedef int16_t ChannelID;
typedef std::string AudioID;
typedef sf::Sound AudioData;

struct Audio
{
	explicit Audio(const std::string p_audioID, const AudioType p_audioType) : m_audioID(p_audioID), m_audioType(p_audioType) {}
	AudioID m_audioID;
	AudioData m_audioData;
	AudioType m_audioType;
	bool m_singleton = true;
};

typedef std::vector<Audio*> AudioContainer;

class AudioManager : public ResourceManager<AudioManager, sf::SoundBuffer>
{
public:
	AudioManager() : ResourceManager(Utils::loadConfig("audio.cfg"))
	{
		m_mute = false;
	}

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

	void PlayMusic(const AudioID p_musicID)
	{
		PlayAudio(m_musics, p_musicID);
	}

	void PlaySound(const AudioID p_soundID)
	{
		PlayAudio(m_sounds, p_soundID);
	}

	void PauseMusics()
	{
		PauseAudio(m_musics);
	}

	void PauseSounds()
	{
		PauseAudio(m_sounds);
	}

	void Pause(const AudioID p_audioId)
	{
		for (auto music : m_musics)
		{
			if (p_audioId == music->m_audioID && music->m_audioData.getStatus() == sf::Sound::Playing)
				music->m_audioData.pause();
		}

		for (auto sound : m_sounds)
		{
			if (p_audioId == sound->m_audioID && sound->m_audioData.getStatus() == sf::Sound::Playing)
				sound->m_audioData.pause();
		}
	}

	void PlayAudio(AudioContainer& p_audioContainer, const AudioID p_audioID)
	{
		for (auto audioNode : p_audioContainer)
		{
			if (audioNode->m_audioID == p_audioID && (audioNode->m_singleton || audioNode->m_audioData.getStatus() != sf::Sound::Playing))
			{
				if (audioNode->m_audioData.getStatus() != sf::Sound::Playing)
					audioNode->m_audioData.play();

				return;
			}
		}

		
		if (RequireResource(p_audioID))
		{
			AudioType audioType = AudioType::MUSIC;
			if (&p_audioContainer == &m_musics)			audioType = AudioType::MUSIC;
			else if (&p_audioContainer == &m_sounds)	audioType = AudioType::SOUND;

			Audio* newAudio = new Audio(p_audioID, audioType);
			newAudio->m_audioData.setBuffer(*GetResource(p_audioID));
			newAudio->m_audioData.play();

			switch (newAudio->m_audioType)
			{
			case AudioType::MUSIC:
				newAudio->m_audioData.setLoop(true);
				newAudio->m_singleton = true;
				break;

			case AudioType::SOUND:
				newAudio->m_audioData.setLoop(false);
				newAudio->m_singleton = false;
				break;
			}

			if (m_mute)
				newAudio->m_audioData.setVolume(0);

			Register(p_audioContainer, newAudio);
		}
	}

	static void PauseAudio(AudioContainer& p_audioContainer)
	{
		for (auto audioNode : p_audioContainer)
		{
			if (audioNode->m_audioData.getStatus() == sf::Sound::Playing)
				audioNode->m_audioData.pause();
		}
	}

	void Register(AudioContainer& p_audioContainer, Audio* p_audio)
	{
		p_audioContainer.push_back(p_audio);
		if (&p_audioContainer == &m_musics)			std::cout << "MUSIC [";
		else if (&p_audioContainer == &m_sounds)	std::cout << "SOUND [";
		std::cout << p_audio->m_audioID << "] REGISTERED ON CHANNEL : " << p_audioContainer.size() << std::endl;
	}

	void ClearAll()
	{
		for (auto music : m_musics)
		{
			delete music;
		}

		for (auto sound : m_sounds)
		{
			delete sound;
		}

		m_musics.clear();
		m_sounds.clear();
	}

	void ToggleMute()
	{
		m_mute = !m_mute;

		if (m_mute)
		{
			SetSoundVolume(0);
			SetMusicVolume(0);
		}

		if (!m_mute)
		{
			SetSoundVolume(100);
			SetMusicVolume(100);
		}
	}

	void SetSoundVolume(const float p_volume)
	{
		for (auto sound : m_sounds)
		{
			sound->m_audioData.setVolume(p_volume);
		}
	}

	void SetMusicVolume(const float p_volume)
	{
		for (auto music : m_musics)
		{
			music->m_audioData.setVolume(p_volume);
		}
	}

private:
	AudioContainer m_musics;
	AudioContainer m_sounds;

	bool m_mute;
};