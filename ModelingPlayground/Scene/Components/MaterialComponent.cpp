#include "MaterialComponent.h"

#include "imgui.h"
#include "../../Utils/PropertyDrawer.h"

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
        PropertyDrawer::DrawVec3fColor("Color", m_materialColor);
    }
}

glm::vec3 MaterialComponent::GetMaterialColor() const
{
    return m_materialColor;
}
