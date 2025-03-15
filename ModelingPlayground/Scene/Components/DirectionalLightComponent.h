#pragma once

#include <glm/vec3.hpp>

#include "Component.h"

class DirectionalLightComponent : public Component
{
public:
    DirectionalLightComponent();

    void RenderInspector() override;

    glm::vec3 GetLightColor() const;
    
private:
    glm::vec3 m_lightColor;
};
