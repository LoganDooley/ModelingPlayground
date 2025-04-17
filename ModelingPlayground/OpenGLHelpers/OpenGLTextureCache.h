#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../nlohmann/json_fwd.hpp"

class OpenGLTexture;

class OpenGLTextureCache
{
public:
	OpenGLTextureCache();
	~OpenGLTextureCache();

	void AddTexture(const std::string& filePath);
	std::shared_ptr<OpenGLTexture> GetTexture(const std::string& filePath);

	void IncrementTextureUsage(const std::string& filePath, void* user);
	void DecrementTextureUsage(const std::string& filePath, void* user);

	const std::unordered_map<std::string, std::pair<std::shared_ptr<OpenGLTexture>, std::unordered_set<void*>>>&
	GetAllTextures() const;

	friend void to_json(nlohmann::json& json, const OpenGLTextureCache& openGLTextureCache);
	friend void from_json(const nlohmann::json& json, OpenGLTextureCache& openGLTextureCache);

private:
	std::unordered_map<std::string, std::pair<std::shared_ptr<OpenGLTexture>, std::unordered_set<void*>>>
	m_textureCache;
};
