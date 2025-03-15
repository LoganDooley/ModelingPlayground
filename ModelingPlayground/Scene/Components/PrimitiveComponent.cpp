#include "PrimitiveComponent.h"

#include "imgui.h"
#include "../../Utils/PropertyDrawer.h"
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
        PropertyDrawer::DrawEnumLabelCombo<PrimitiveType>("Primitive Type", PrimitiveTypeEnumLabel, m_primitiveType);
    }
}

PrimitiveType PrimitiveComponent::GetPrimitiveType() const
{
    return m_primitiveType;
}
