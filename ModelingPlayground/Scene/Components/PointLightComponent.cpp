#include "PointLightComponent.h"

#include <glm/geometric.hpp>

#include "imgui.h"

PointLightComponent::PointLightComponent():
    m_lightColor(glm::vec3(1.0f, 1.0f, 1.0f))
{
}

void PointLightComponent::RenderInspector()
{
    ImGuiTreeNodeFlags directionalLightHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
    if (ImGui::CollapsingHeader("Point Light", directionalLightHeaderFlags))
    {
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