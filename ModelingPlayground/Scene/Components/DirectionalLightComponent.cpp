#include "DirectionalLightComponent.h"

#include <glm/geometric.hpp>

#include "imgui.h"
#include "../../Utils/PropertyDrawer.h"

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
        PropertyDrawer::DrawVec3fColor("Color", m_lightColor);
    }
}

const glm::vec3& DirectionalLightComponent::GetLightColor() const
{
    return m_lightColor.GetData();
}

DataBinding<glm::vec3>& DirectionalLightComponent::GetLightColorDataBinding()
{
    return m_lightColor;
}
