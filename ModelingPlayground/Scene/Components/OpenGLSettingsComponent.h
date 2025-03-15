#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Component.h"

class OpenGLSettingsComponent : public Component
{
public:
    OpenGLSettingsComponent();

    void RenderInspector() override;

    glm::vec4 GetClearColor() const;
    glm::vec3 GetAmbientLight() const;
    
private:
    glm::vec4 m_clearColor;
    glm::vec3 m_ambientLight;
};
