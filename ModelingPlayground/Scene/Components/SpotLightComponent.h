#pragma once

#include <glm/vec3.hpp>

#include "Component.h"

class SpotLightComponent : public Component
{
public:
    SpotLightComponent();

    void RenderInspector() override;

    glm::vec3 GetLightColor() const;
    float GetLightFalloffAngle() const;
    
private:
    glm::vec3 m_lightColor;
    float m_falloffAngle;
};