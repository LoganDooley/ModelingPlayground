#pragma once
#include <memory>
#include <string>
#include <unordered_map>

class OpenGLTexture;

class OpenGLTextureCache
{
public:
	OpenGLTextureCache();
	~OpenGLTextureCache();

	void LoadTexture(const std::string& filePath);
	std::shared_ptr<OpenGLTexture> GetTexture(const std::string& filePath);

	const std::unordered_map<std::string, std::shared_ptr<OpenGLTexture>>& GetAllTextures() const;

private:
	std::unordered_map<std::string, std::shared_ptr<OpenGLTexture>> m_textureCache;
};
