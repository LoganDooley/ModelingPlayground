#include "DirectionalLightComponent.h"

#include <glm/geometric.hpp>

#include "imgui.h"

DirectionalLightComponent::DirectionalLightComponent():
    m_lightColor(glm::vec3(1.0f, 1.0f, 1.0f))
{
}

void DirectionalLightComponent::RenderInspector()
{
    ImGuiTreeNodeFlags directionalLightHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
    if (ImGui::CollapsingHeader("Directional Light", directionalLightHeaderFlags))
    {
        // Color
        float lightColor[3] = { m_lightColor.r, m_lightColor.g, m_lightColor.b };
				
        if (ImGui::ColorPicker3("Color", lightColor))
        {
            m_lightColor = glm::vec3(lightColor[0], lightColor[1], lightColor[2]);
        }
    }
}

glm::vec3 DirectionalLightComponent::GetLightColor() const
{
    return m_lightColor;
}
