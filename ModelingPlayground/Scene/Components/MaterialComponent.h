﻿#pragma once

#include <glm/vec4.hpp>

#include "Component.h"
#include "../../nlohmann/json_fwd.hpp"

class MaterialComponent : public Component
{
public:
	MaterialComponent();

	void RenderInspector() override;

	glm::vec4 GetMaterialColor() const;
	float GetMetallic() const;
	float GetRoughness() const;

	friend void to_json(nlohmann::json& json, const MaterialComponent& materialComponent);
	friend void from_json(const nlohmann::json& json, MaterialComponent& materialComponent);

private:
	glm::vec4 m_materialColor;
	float m_metallic;
	float m_roughness;
};
