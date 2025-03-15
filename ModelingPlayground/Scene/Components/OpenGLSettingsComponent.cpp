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
    }
}

glm::vec4 OpenGLSettingsComponent::GetClearColor() const
{
    return m_clearColor;
}
