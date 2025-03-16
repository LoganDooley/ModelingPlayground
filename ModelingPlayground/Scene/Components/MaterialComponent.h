#pragma once

#include <glm/vec3.hpp>

#include "Component.h"

class MaterialComponent : public Component
{
public:
    MaterialComponent();
    
    void RenderInspector() override;

    glm::vec3 GetMaterialColor() const;
    float GetMetallic() const;
    float GetRoughness() const;

private:
    glm::vec3 m_materialColor;
    float m_metallic;
    float m_roughness;
};
