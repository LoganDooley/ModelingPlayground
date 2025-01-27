#include "MeshComponent.h"

#include "imgui.h"

MeshComponent::MeshComponent():
    m_meshType(MeshType::Triangle)
{
}

void MeshComponent::RenderInspector()
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
                m_meshType = MeshType::Triangle;
            }
        }
    }
}
