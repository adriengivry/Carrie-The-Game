#pragma once

#include "AudioManager.h"

typedef int16_t ChannelID;

enum class AudioType
{
	A_SOUND,
	A_MUSIC
};

struct Sound
{
	explicit Sound(const std::string p_audioID, const AudioType p_audioType) : m_audioID(p_audioID), m_audioType(p_audioType) {}
	std::string m_audioID;
	sf::Sound m_data;
	AudioType m_audioType;
};


class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	void SetAudioManager(AudioManager* p_audioManager);

	ChannelID PlayMusic(const std::string p_soundName, const bool p_continueIfExist = true, const bool p_stack = false, const ChannelID p_channelID = 0);
	ChannelID PlaySound(const std::string p_soundName, const bool p_stack = false, const ChannelID p_channelID = 1);

	void PauseChannel(const ChannelID p_channelToStop);
	void PlayChannel(const ChannelID p_channelToPlay);

	void Pause(const std::string p_soundName);
	void Stop(const std::string p_soundName);

	void PauseMusics();
	void PauseSounds();

	void StopMusics();
	void StopSounds();

	void ClearMusics();
	void ClearSounds();

	void Clear(const ChannelID p_channelID);
	void Clear(const std::string p_soundName);
	void ClearAll();

	ChannelID Register(const ChannelID p_channelID, Sound* p_sound, const bool p_stack);
	void AddSound(const ChannelID p_channelID, Sound* p_sound);

private:
	AudioManager* m_audioManager;
	std::map<ChannelID, Sound*> m_sounds;
};
