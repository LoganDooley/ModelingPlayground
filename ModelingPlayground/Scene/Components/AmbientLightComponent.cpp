#include "AmbientLightComponent.h"

#include "imgui.h"

AmbientLightComponent::AmbientLightComponent():
    m_ambientColor(glm::vec3(0))
{
}

void AmbientLightComponent::RenderInspector()
{
    ImGuiTreeNodeFlags ambientLightHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
    if (ImGui::CollapsingHeader("Global Lighting", ambientLightHeaderFlags))
    {
        // Ambient color
        float ambientColor[3] = { m_ambientColor.r, m_ambientColor.g, m_ambientColor.b };
				
        if (ImGui::ColorPicker3("Ambient Color", ambientColor))
        {
            m_ambientColor = glm::vec3(ambientColor[0], ambientColor[1], ambientColor[2]);
        }
    }
}
