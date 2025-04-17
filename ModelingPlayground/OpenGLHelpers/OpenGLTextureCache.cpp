#include "OpenGLTextureCache.h"

#include <iostream>

#include "OpenGLTexture.h"
#include "../Libraries/stb/stb_image.h"

OpenGLTextureCache::OpenGLTextureCache():
	m_textureCache(
		std::unordered_map<std::string, std::pair<std::shared_ptr<OpenGLTexture>, std::unordered_set<void*>>>())
{
}

OpenGLTextureCache::~OpenGLTextureCache()
{
}

void OpenGLTextureCache::AddTexture(const std::string& filePath)
{
	if (filePath.empty())
	{
		std::cout << "OpenGLTextureCache|AddTexture: No file path specified.\n";
		return;
	}

	if (m_textureCache.contains(filePath))
	{
		std::cout << "OpenGLTextureCache::AddTexture: File path " << filePath << " is already loaded as a texture.\n";
		return;
	}
	m_textureCache[filePath] = {std::make_shared<OpenGLTexture>(filePath), {}};
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

void OpenGLTextureCache::IncrementTextureUsage(const std::string& filePath, void* user)
{
	if (!m_textureCache.contains(filePath))
	{
		std::cout << "OpenGLTextureCache|IncrementTextureUsage: File not loaded: " << filePath << "\n";
		return;
	}
	m_textureCache[filePath].second.insert(user);
	if (m_textureCache[filePath].second.size() == 1)
	{
		m_textureCache[filePath].first->MakeTextureResident();
	}
}

void OpenGLTextureCache::DecrementTextureUsage(const std::string& filePath, void* user)
{
	if (!m_textureCache.contains(filePath))
	{
		std::cout << "OpenGLTextureCache|DecrementTextureUsage: File not loaded: " << filePath << "\n";
		return;
	}
	if (!m_textureCache[filePath].second.contains(user))
	{
		std::cout << "OpenGLTextureCache|DecrementTextureUsage: Double decrement detected for file at: " << filePath <<
			"\n";
		return;
	}
	m_textureCache[filePath].second.erase(user);
	if (m_textureCache[filePath].second.empty())
	{
		m_textureCache[filePath].first->MakeTextureNonResident();
	}
}

const std::unordered_map<std::string, std::pair<std::shared_ptr<OpenGLTexture>, std::unordered_set<void*>>>&
OpenGLTextureCache::GetAllTextures() const
{
	return m_textureCache;
}
