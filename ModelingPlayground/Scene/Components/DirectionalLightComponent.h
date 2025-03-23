#pragma once

#include <glm/vec3.hpp>

#include "Component.h"
#include "../../nlohmann/json_fwd.hpp"
#include "../../Utils/DataBinding.h"

class DirectionalLightComponent : public Component
{
public:
	DirectionalLightComponent();

	void RenderInspector() override;

	const glm::vec3& GetLightColor() const;
	DataBinding<glm::vec3>& GetLightColorDataBinding();

	friend void to_json(nlohmann::json& json, const DirectionalLightComponent& directionalLightComponent);
	friend void from_json(const nlohmann::json& json, DirectionalLightComponent& directionalLightComponent);

private:
	DataBinding<glm::vec3> m_lightColor;
};
