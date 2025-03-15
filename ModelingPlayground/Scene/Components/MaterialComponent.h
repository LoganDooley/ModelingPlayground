#pragma once

#include <glm/vec3.hpp>

#include "Component.h"

class MaterialComponent : public Component
{
public:
    MaterialComponent();
    
    void RenderInspector() override;

    glm::vec3 GetMaterialColor() const;

private:
    glm::vec3 m_materialColor;
};
