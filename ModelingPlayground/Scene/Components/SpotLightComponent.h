#pragma once

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
    
    const float& GetLightFalloffAngle() const;
    DataBinding<float>& GetLightFalloffAngleDataBinding();
    
private:
    DataBinding<glm::vec3> m_lightColor;
    DataBinding<float> m_falloffAngle;
};