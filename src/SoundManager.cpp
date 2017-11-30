#include "SoundManager.h"

SoundManager::SoundManager() : m_audioManager(nullptr)
{
}

SoundManager::~SoundManager()
{
	for (auto it : m_sounds)
	{
		delete it.second;
	}
}

void SoundManager::SetAudioManager(AudioManager* p_audioManager)
{
	m_audioManager = p_audioManager;
}

ChannelID SoundManager::PlayMusic(const std::string p_soundName, const bool p_continueIfExist, const bool p_stack, const ChannelID p_channelID)
{
	
	for (const auto it : m_sounds)
		if (it.second->m_audioID == p_soundName)
		{
			if (p_continueIfExist)
			{
				if (it.second->m_data.getStatus() == sf::SoundSource::Paused || it.second->m_data.getStatus() == sf::SoundSource::Stopped)
					it.second->m_data.play();
				return it.first;
			}
			
			it.second->m_data.setPlayingOffset(sf::Time::Zero);
			it.second->m_data.play();
			return it.first;
		}
	
	Sound* newSound = new Sound(p_soundName, AudioType::A_MUSIC);
	if (m_audioManager->RequireResource(p_soundName))
	{
		newSound->m_data.setBuffer(*m_audioManager->GetResource(p_soundName));
		newSound->m_data.play();
		newSound->m_data.setLoop(true);
		return Register(p_channelID, newSound, p_stack);
	}

	return 0;
}

ChannelID SoundManager::PlaySound(const std::string p_soundName, const bool p_stack, const ChannelID p_channelID)
{
	Sound* newSound = new Sound(p_soundName, AudioType::A_SOUND);
	if (m_audioManager->RequireResource(p_soundName))
	{
		newSound->m_data.setBuffer(*m_audioManager->GetResource(p_soundName));
		newSound->m_data.play();
		newSound->m_data.setLoop(false);
		return Register(p_channelID, newSound, p_stack);
	}

	return 0;
}

void SoundManager::PauseChannel(const ChannelID p_channelToStop)
{
	m_sounds[p_channelToStop]->m_data.stop();
}

void SoundManager::PlayChannel(const ChannelID p_channelToPlay)
{
	m_sounds[p_channelToPlay]->m_data.play();
}

void SoundManager::Pause(const std::string p_soundName)
{
	for (auto it : m_sounds)
	{
		if (it.second->m_audioID == p_soundName)
			it.second->m_data.pause();
	}
}

void SoundManager::Stop(const std::string p_soundName)
{
	for (auto it : m_sounds)
	{
		if (it.second->m_audioID == p_soundName)
			it.second->m_data.stop();
	}
}

void SoundManager::PauseMusics()
{
	for (auto it : m_sounds)
		if (it.second->m_audioType == AudioType::A_MUSIC)
			it.second->m_data.pause();
}

void SoundManager::PauseSounds()
{
	for (auto it : m_sounds)
		if (it.second->m_audioType == AudioType::A_SOUND)
			it.second->m_data.pause();
}

void SoundManager::StopMusics()
{
	for (auto it : m_sounds)
	{
		if (it.second->m_audioType == AudioType::A_MUSIC)
		{
			it.second->m_data.stop();
		}
	}
}

void SoundManager::StopSounds()
{
	for (auto it : m_sounds)
	{
		if (it.second->m_audioType == AudioType::A_SOUND)
		{
			it.second->m_data.stop();
		}
	}
}

void SoundManager::ClearMusics()
{
	for (auto it : m_sounds)
	{
		if (it.second->m_audioType == AudioType::A_MUSIC)
		{
			it.second->m_data.stop();
			m_sounds.erase(m_sounds.find(it.first));
			delete it.second;
		}
	}
}

void SoundManager::ClearSounds()
{
	for (auto it : m_sounds)
	{
		if (it.second->m_audioType == AudioType::A_SOUND)
		{
			it.second->m_data.stop();
			m_sounds.erase(m_sounds.find(it.first));
			delete it.second;
		}
	}
}

void SoundManager::Clear(const ChannelID p_channelID)
{
	if (m_sounds.find(p_channelID) != m_sounds.end())
	{
		delete m_sounds[p_channelID];
		m_sounds.erase(m_sounds.find(p_channelID));
	}
}

void SoundManager::Clear(const std::string p_soundName)
{
	std::vector<ChannelID> toDelete;

	for (const auto it : m_sounds)
	{
		if (it.second->m_audioID == p_soundName)
		{
			toDelete.push_back(it.first);
		}
	}
	
	for(auto it : toDelete)
		Clear(it);
}

void SoundManager::ClearAll()
{
	for (auto it : m_sounds)
		delete it.second;

	m_sounds.clear();
}

ChannelID SoundManager::Register(const ChannelID p_channelID, Sound* p_sound, const bool p_stack)
{
	ChannelID channelUsed = 0;

	if (p_stack)
	{
		while (m_sounds.find(channelUsed) != m_sounds.end())
			++channelUsed;
		AddSound(channelUsed, p_sound);
	}
	else
	{
		channelUsed = p_channelID;
		AddSound(channelUsed, p_sound);
	}

	return channelUsed;
}

void SoundManager::AddSound(const ChannelID p_channelID, Sound* p_sound)
{
	if (m_sounds.find(p_channelID) != m_sounds.end())
	{
		delete m_sounds[p_channelID];
		m_sounds[p_channelID] = p_sound;
	}
	else
	{
		m_sounds.insert(std::pair<ChannelID, Sound*>(p_channelID, p_sound));
	}
}
