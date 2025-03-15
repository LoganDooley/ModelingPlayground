#include "MaterialComponent.h"

#include "imgui.h"

MaterialComponent::MaterialComponent():
    m_materialColor(glm::vec3(1))
{
}

void MaterialComponent::RenderInspector()
{
    ImGuiTreeNodeFlags materialHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
    if (ImGui::CollapsingHeader("Material", materialHeaderFlags))
    {
        // Material Color
        float materialColor[3] = { m_materialColor.r, m_materialColor.g, m_materialColor.b };
				
        if (ImGui::ColorPicker3("Color", materialColor))
        {
            m_materialColor = glm::vec3(materialColor[0], materialColor[1], materialColor[2]);
        }
    }
}

glm::vec3 MaterialComponent::GetMaterialColor() const
{
    return m_materialColor;
}
