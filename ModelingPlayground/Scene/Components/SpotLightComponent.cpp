#include "SpotLightComponent.h"

#include "imgui.h"
#include "../../Utils/PropertyDrawer.h"

SpotLightComponent::SpotLightComponent():
    m_lightColor(glm::vec3(1.0f)),
    m_falloffAngle(45.f)
{
}

void SpotLightComponent::RenderInspector()
{
    ImGuiTreeNodeFlags spotLightHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
    if (ImGui::CollapsingHeader("Spot Light", spotLightHeaderFlags))
    {
        // Color
        PropertyDrawer::DrawVec3fColor("Color", m_lightColor);

        // Falloff angle
        PropertyDrawer::DrawFloatDrag("Falloff Angle", m_falloffAngle, 1.f, 0.f, 180.f);
    }
}

const glm::vec3& SpotLightComponent::GetLightColor() const
{
    return m_lightColor.GetData();
}

DataBinding<glm::vec3>& SpotLightComponent::GetLightColorDataBinding()
{
    return m_lightColor;
}

const float& SpotLightComponent::GetLightFalloffAngle() const
{
    return m_falloffAngle.GetData();
}

DataBinding<float>& SpotLightComponent::GetLightFalloffAngleDataBinding()
{
    return m_falloffAngle;
}
