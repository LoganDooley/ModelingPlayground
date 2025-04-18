﻿#pragma once

#include <glm/vec4.hpp>

#include "Component.h"
#include "../../nlohmann/json_fwd.hpp"
#include "glad/glad.h"

class RenderingManager;

class MaterialComponent : public Component
{
public:
	MaterialComponent();
	MaterialComponent(std::shared_ptr<RenderingManager> renderingManager);

	void SetOpenGLRenderer(std::shared_ptr<RenderingManager> renderingManager);

	void RenderInspector() override;

	bool GetUseColorTexture() const;
	glm::vec4 GetMaterialColor() const;
	const std::string& GetMaterialTexture() const;
	void SetMaterialTexture(const std::string& materialTexture, bool setUseColorTexture = true);

	bool GetUseMetallicMap() const;
	float GetMetallic() const;
	const std::string& GetMetallicMap() const;
	void SetMetallicMap(const std::string& metallicMap, bool setUseMetallicMap = true);

	bool GetUseRoughnessMap() const;
	float GetRoughness() const;
	const std::string& GetRoughnessMap() const;
	void SetRoughnessMap(const std::string& roughnessMap, bool setUseRoughnessMap = true);

	friend void to_json(nlohmann::json& json, const MaterialComponent& materialComponent);
	friend void from_json(const nlohmann::json& json, MaterialComponent& materialComponent);

private:
	std::shared_ptr<RenderingManager> m_renderingManager;

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
