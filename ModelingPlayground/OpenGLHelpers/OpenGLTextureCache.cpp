#include "OpenGLTextureCache.h"

#include <iostream>

#include "OpenGLTexture.h"
#include "../Libraries/stb/stb_image.h"

OpenGLTextureCache::OpenGLTextureCache():
	m_textureCache(std::unordered_map<std::string, std::shared_ptr<OpenGLTexture>>())
{
}

OpenGLTextureCache::~OpenGLTextureCache()
{
}

void OpenGLTextureCache::LoadTexture(const std::string& filePath)
{
	if (m_textureCache.contains(filePath))
	{
		std::cout << "OpenGLTextureCache::LoadTexture: File path " << filePath << " is already loaded as a texture.\n";
		return;
	}
	m_textureCache[filePath] = std::make_shared<OpenGLTexture>(filePath);
}

std::shared_ptr<OpenGLTexture> OpenGLTextureCache::GetTexture(const std::string& filePath)
{
	if (!m_textureCache.contains(filePath))
	{
		std::cerr << "OpenGLTextureCache|GetTexture: File not loaded: " << filePath << "\n";
		return nullptr;
	}
	return m_textureCache[filePath];
}

const std::unordered_map<std::string, std::shared_ptr<OpenGLTexture>>& OpenGLTextureCache::GetAllTextures() const
{
	return m_textureCache;
}
