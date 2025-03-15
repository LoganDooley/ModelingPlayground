#pragma once

#include <glm/vec3.hpp>

#include "Component.h"

class PointLightComponent : public Component
{
public:
    PointLightComponent();

    void RenderInspector() override;

    glm::vec3 GetLightColor() const;
    
private:
    glm::vec3 m_lightColor;
};