#include "PointLightComponent.h"

#include <glm/geometric.hpp>

#include "imgui.h"

PointLightComponent::PointLightComponent():
    m_lightColor(glm::vec3(1.0f, 1.0f, 1.0f)),
    m_lightPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
    m_lightDirection(glm::vec3(0.0f, -1.0f, 0.0f))
{
}

void PointLightComponent::RenderInspector()
{
    ImGuiTreeNodeFlags directionalLightHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
    if (ImGui::CollapsingHeader("Point Light", directionalLightHeaderFlags))
    {
        // Position
        float position[3] = { m_lightPosition.x, m_lightPosition.y, m_lightPosition.z };
				
        if (ImGui::DragFloat3("Position", position, 0.01f))
        {
            m_lightPosition = glm::normalize(glm::vec3(position[0], position[1], position[2]));
        }
        
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

glm::vec3 PointLightComponent::GetLightColor() const
{
    return m_lightColor;
}

glm::vec3 PointLightComponent::GetLightPosition() const
{
    return m_lightPosition;
}

glm::vec3 PointLightComponent::GetLightDirection() const
{
    return m_lightDirection;
}
