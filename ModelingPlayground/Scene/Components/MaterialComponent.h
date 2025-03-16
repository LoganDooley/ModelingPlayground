#pragma once

#include <memory>
#include <glm/vec3.hpp>

#include "Component.h"
#include "../../nlohmann/json_fwd.hpp"

class MaterialComponent : public Component
{
public:
    MaterialComponent();
    
    void RenderInspector() override;

    glm::vec3 GetMaterialColor() const;
    float GetMetallic() const;
    float GetRoughness() const;

    friend void to_json(nlohmann::json& json, const std::shared_ptr<MaterialComponent>& materialComponent);
    friend void from_json(const nlohmann::json& json, std::shared_ptr<MaterialComponent>& materialComponent);

private:
    glm::vec3 m_materialColor;
    float m_metallic;
    float m_roughness;
};
