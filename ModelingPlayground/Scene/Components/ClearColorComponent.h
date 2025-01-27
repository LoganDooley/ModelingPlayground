#pragma once
#include <glm/vec4.hpp>

#include "Component.h"

class ClearColorComponent : public Component
{
public:
    ClearColorComponent();

    void RenderInspector() override;

    const glm::vec4& GetClearColor() const;
    
private:
    glm::vec4 m_clearColor;
};
