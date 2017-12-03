#pragma once
#define RUNNING_WINDOWS
#include <iostream>
#include <string>
#include <algorithm>
#include <SFML/Graphics.hpp>

namespace Utils
{
	#ifdef RUNNING_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <Shlwapi.h>
	inline std::string GetWorkingDirectory()
	{
		const HMODULE hModule = GetModuleHandle(nullptr);
		if(hModule)
		{
			char path[256];
			GetModuleFileName(hModule,path,sizeof(path));
			PathRemoveFileSpec(path);
			strcat_s(path,"\\");
			return std::string(path);
		}
		return "";
	}
	#elif defined RUNNING_LINUX
	#include <unistd.h>
	inline std::string GetWorkingDirectory()
	{
		char cwd[1024];
		if(getcwd(cwd, sizeof(cwd)) != nullptr)
		{
			return std::string(cwd) + std::string("/");
		}
		return "";
	}
	#endif

	inline std::string loadAsset(const std::string p_path)
	{
		return "../assets/" + p_path;
	}

	inline std::string loadConfig(const std::string p_path)
	{
		return "../config/" + p_path;
	}

	inline void centerOrigin(sf::Sprite& p_sprite)
	{
		const sf::FloatRect rect = p_sprite.getLocalBounds();
		p_sprite.setOrigin(rect.left + rect.width / 2.0f,
			rect.top + rect.height / 2.0f);
	}

	inline void centerOrigin(sf::Text& p_text)
	{
		const sf::FloatRect rect = p_text.getLocalBounds();
		p_text.setOrigin(rect.left + rect.width / 2.0f,
			rect.top + rect.height / 2.0f);
	}

	inline void centerOrigin(sf::RectangleShape& p_rect)
	{
		const sf::FloatRect rect = p_rect.getLocalBounds();
		p_rect.setOrigin(rect.left + rect.width / 2.0f,
			rect.top + rect.height / 2.0f);
	}

	inline void centerOrigin(sf::CircleShape& p_circle)
	{
		const sf::FloatRect rect = p_circle.getLocalBounds();
		p_circle.setOrigin(rect.left + rect.width / 2.0f,
			rect.top + rect.height / 2.0f);
	}

	inline int randomgen(const int min, const int max)
	{
		return rand() % (max + 1) + min;
	}
}