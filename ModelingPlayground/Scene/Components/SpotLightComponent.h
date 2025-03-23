#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Component.h"
#include "../../nlohmann/json_fwd.hpp"
#include "../../Utils/DataBinding.h"

class SpotLightComponent : public Component
{
public:
	SpotLightComponent();

	void RenderInspector() override;

	const glm::vec3& GetLightColor() const;
	DataBinding<glm::vec3>& GetLightColorDataBinding();

	const glm::vec2& GetLightFalloffAngles() const;
	DataBinding<glm::vec2>& GetLightFalloffAnglesDataBinding();

	friend void to_json(nlohmann::json& json, const SpotLightComponent& spotLightComponent);
	friend void from_json(const nlohmann::json& json, SpotLightComponent& spotLightComponent);

private:
	DataBinding<glm::vec3> m_lightColor;
	DataBinding<glm::vec2> m_falloffAngles;
};
