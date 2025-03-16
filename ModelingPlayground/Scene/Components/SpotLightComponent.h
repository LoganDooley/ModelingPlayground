#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Component.h"
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
    
private:
    DataBinding<glm::vec3> m_lightColor;
    DataBinding<glm::vec2> m_falloffAngles;
};