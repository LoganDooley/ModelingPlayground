#include "PrimitiveComponent.h"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

PrimitiveComponent::PrimitiveComponent():
    m_primitiveType(PrimitiveType::Triangle),
    m_currentItem(0)
{
}

void PrimitiveComponent::RenderInspector()
{
    ImGuiTreeNodeFlags meshHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
    if (ImGui::CollapsingHeader("Mesh", meshHeaderFlags))
    {
        if (ImGui::Combo("Primitive Type", &m_currentItem, PrimitiveTypeEnumLabel.GetCStrings().data(), PrimitiveTypeEnumLabel.Count()))
        {
            m_primitiveType = PrimitiveTypeEnumLabel.GetEnums()[m_currentItem];
        }
    }
}

PrimitiveType PrimitiveComponent::GetPrimitiveType() const
{
    return m_primitiveType;
}
