#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "Component.h"
#include "../../nlohmann/json_fwd.hpp"

class OpenGLSettingsComponent : public Component
{
public:
    OpenGLSettingsComponent();

    void RenderInspector() override;

    glm::vec4 GetClearColor() const;
    glm::vec3 GetAmbientLight() const;

    friend void to_json(nlohmann::json& json, const OpenGLSettingsComponent& openGLSettingsComponent);
    friend void from_json(const nlohmann::json& json, OpenGLSettingsComponent& openGLSettingsComponent);
    
private:
    glm::vec4 m_clearColor;
    glm::vec3 m_ambientLight;
};
