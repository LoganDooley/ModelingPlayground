#pragma once

#include <glm/vec4.hpp>

#include "Component.h"
#include "../../nlohmann/json_fwd.hpp"
#include "glad/glad.h"

class OpenGLTextureCache;

class MaterialComponent : public Component
{
public:
	MaterialComponent();
	MaterialComponent(std::shared_ptr<OpenGLTextureCache> openGLTextureCache);

	void SetTextureCache(std::shared_ptr<OpenGLTextureCache> openGLTextureCache);

	void RenderInspector() override;

	bool GetUseColorTexture() const;
	glm::vec4 GetMaterialColor() const;
	GLuint64 GetMaterialTexture() const;

	bool GetUseMetallicMap() const;
	float GetMetallic() const;
	GLuint64 GetMetallicMap() const;

	bool GetUseRoughnessMap() const;
	float GetRoughness() const;
	GLuint64 GetRoughnessMap() const;

	friend void to_json(nlohmann::json& json, const MaterialComponent& materialComponent);
	friend void from_json(const nlohmann::json& json, MaterialComponent& materialComponent);

private:
	std::shared_ptr<OpenGLTextureCache> m_openGLTextureCache;

	bool m_useColorTexture;
	glm::vec4 m_materialColor;
	std::string m_materialColorTexturePath;
	bool m_useMetallicMap;
	float m_metallic;
	std::string m_metallicMapPath;
	bool m_useRoughnessMap;
	float m_roughness;
	std::string m_roughnessMapPath;
};
