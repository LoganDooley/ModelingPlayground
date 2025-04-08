#include "OpenGLTextureCache.h"

#include <iostream>

#include "OpenGLTexture.h"
#include "../Libraries/stb/stb_image.h"

OpenGLTextureCache::OpenGLTextureCache():
	m_textureCache(std::unordered_map<std::string, std::pair<std::shared_ptr<OpenGLTexture>, int>>())
{
}

OpenGLTextureCache::~OpenGLTextureCache()
{
}

void OpenGLTextureCache::LoadTexture(const std::string& filePath)
{
	if (filePath.empty())
	{
		return;
	}

	if (m_textureCache.contains(filePath))
	{
		std::cout << "OpenGLTextureCache::LoadTexture: File path " << filePath << " is already loaded as a texture.\n";
		return;
	}
	m_textureCache[filePath] = {std::make_shared<OpenGLTexture>(filePath), 0};
}

std::shared_ptr<OpenGLTexture> OpenGLTextureCache::GetTexture(const std::string& filePath)
{
	if (!m_textureCache.contains(filePath))
	{
		std::cerr << "OpenGLTextureCache|GetTexture: File not loaded: " << filePath << "\n";
		return nullptr;
	}
	return m_textureCache[filePath].first;
}

void OpenGLTextureCache::IncrementTextureUsage(const std::string& filePath)
{
	if (!m_textureCache.contains(filePath))
	{
		std::cout << "OpenGLTextureCache|IncrementTextureUsage: File not loaded: " << filePath << "\n";
		return;
	}
	m_textureCache[filePath].second++;
	if (m_textureCache[filePath].second == 1)
	{
		m_textureCache[filePath].first->MakeTextureResident();
	}
}

void OpenGLTextureCache::DecrementTextureUsage(const std::string& filePath)
{
	if (!m_textureCache.contains(filePath))
	{
		std::cout << "OpenGLTextureCache|DecrementTextureUsage: File not loaded: " << filePath << "\n";
		return;
	}
	if (m_textureCache[filePath].second <= 0)
	{
		std::cout << "OpenGLTextureCache|DecrementTextureUsage: Double decrement detected for file at: " << filePath <<
			"\n";
		m_textureCache[filePath].second = 0;
		return;
	}
	m_textureCache[filePath].second--;
	if (m_textureCache[filePath].second == 0)
	{
		m_textureCache[filePath].first->MakeTextureNonResident();
	}
}

const std::unordered_map<std::string, std::pair<std::shared_ptr<OpenGLTexture>, int>>&
OpenGLTextureCache::GetAllTextures() const
{
	return m_textureCache;
}
