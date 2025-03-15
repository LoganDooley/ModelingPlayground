#include "DirectionalLightComponent.h"

#include <glm/geometric.hpp>

#include "imgui.h"

DirectionalLightComponent::DirectionalLightComponent():
    m_lightColor(glm::vec3(1.0f, 1.0f, 1.0f)),
    m_lightDirection(glm::vec3(0.0f, -1.0f, 0.0f))
{
}

void DirectionalLightComponent::RenderInspector()
{
    ImGuiTreeNodeFlags directionalLightHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
    if (ImGui::CollapsingHeader("Directional Light", directionalLightHeaderFlags))
    {
        // Direction
        float direction[3] = { m_lightDirection.x, m_lightDirection.y, m_lightDirection.z };
				
        if (ImGui::DragFloat3("Direction", direction, 0.01f))
        {
            m_lightDirection = glm::normalize(glm::vec3(direction[0], direction[1], direction[2]));
        }
        
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

glm::vec3 DirectionalLightComponent::GetLightDirection() const
{
    return m_lightDirection;
}
