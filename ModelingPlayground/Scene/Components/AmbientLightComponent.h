#pragma once

#include <glm/vec3.hpp>

#include "Component.h"

class AmbientLightComponent : public Component
{
public:
    AmbientLightComponent();

    void RenderInspector() override;
    
private:
    glm::vec3 m_ambientColor;
};
