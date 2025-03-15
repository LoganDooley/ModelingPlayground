#include "PointLightComponent.h"

#include <glm/geometric.hpp>

#include "imgui.h"
#include "../../Utils/PropertyDrawer.h"

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
        PropertyDrawer::DrawVec3fColor("Color", m_lightColor);
    }
}

glm::vec3 PointLightComponent::GetLightColor() const
{
    return m_lightColor;
}