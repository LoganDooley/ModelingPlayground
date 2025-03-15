#pragma once

#include <glm/vec4.hpp>

#include "Component.h"

class OpenGLSettingsComponent : public Component
{
public:
    OpenGLSettingsComponent();

    void RenderInspector() override;

    glm::vec4 GetClearColor() const;
    
private:
    glm::vec4 m_clearColor;
};
