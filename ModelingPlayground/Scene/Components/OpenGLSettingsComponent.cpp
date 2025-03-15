#include "OpenGLSettingsComponent.h"

#include <glm/vec3.hpp>

#include "imgui.h"

OpenGLSettingsComponent::OpenGLSettingsComponent():
    m_clearColor(glm::vec4(glm::vec3(0.0f), 1.0f))
{
}

void OpenGLSettingsComponent::RenderInspector()
{
    ImGuiTreeNodeFlags ambientLightHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
    if (ImGui::CollapsingHeader("OpenGL Settings", ambientLightHeaderFlags))
    {
        // Clear color
        float clearColor[4] = { m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a };
				
        if (ImGui::ColorPicker4("Clear Color", clearColor))
        {
            m_clearColor = glm::vec4(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
        }

        // Ambient light
        float ambientLight[3] = { m_ambientLight.r, m_ambientLight.g, m_ambientLight.b };
				
        if (ImGui::ColorPicker3("AmbientLight", ambientLight))
        {
            m_ambientLight = glm::vec3(ambientLight[0], ambientLight[1], ambientLight[2]);
        }
    }
}

glm::vec4 OpenGLSettingsComponent::GetClearColor() const
{
    return m_clearColor;
}

glm::vec3 OpenGLSettingsComponent::GetAmbientLight() const
{
    return m_ambientLight;
}
