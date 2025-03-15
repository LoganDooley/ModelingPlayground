#pragma once

#include <glm/vec3.hpp>

#include "Component.h"

class PointLightComponent : public Component
{
public:
    PointLightComponent();

    void RenderInspector() override;

    glm::vec3 GetLightColor() const;
    glm::vec3 GetLightPosition() const;
    glm::vec3 GetLightDirection() const;
    
private:
    glm::vec3 m_lightColor;
    glm::vec3 m_lightPosition;
    glm::vec3 m_lightDirection;
};