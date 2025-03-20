#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Component.h"
#include "../../Utils/DataBinding.h"
#include "../../nlohmann/json_fwd.hpp"

class PointLightComponent : public Component
{
public:
    PointLightComponent();

    void RenderInspector() override;

    const glm::vec3& GetLightColor() const;
    DataBinding<glm::vec3>& GetLightColorDataBinding();
    
    const glm::vec3& GetFalloff() const;
    DataBinding<glm::vec3>& GetFalloffDataBinding();

    friend void to_json(nlohmann::json& json, const PointLightComponent& pointLightComponent);
    friend void from_json(const nlohmann::json& json, PointLightComponent& pointLightComponent);
    
private:
    DataBinding<glm::vec3> m_lightColor;
    DataBinding<glm::vec3> m_falloff;
};