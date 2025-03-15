#include "PrimitiveComponent.h"

#include "imgui.h"

PrimitiveComponent::PrimitiveComponent():
    m_meshType(PrimitiveType::Triangle)
{
}

void PrimitiveComponent::RenderInspector()
{
    ImGuiTreeNodeFlags meshHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
    if (ImGui::CollapsingHeader("Mesh", meshHeaderFlags))
    {
        int currentItem;
        const char* meshTypes[1] = { "Triangle" };
        if (ImGui::ListBox("Mesh Type", &currentItem, meshTypes, 1))
        {
            if (currentItem == 0)
            {
                m_meshType = PrimitiveType::Triangle;
            }
        }
    }
}
