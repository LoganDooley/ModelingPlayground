#include "ClearColorComponent.h"

#include "imgui.h"

ClearColorComponent::ClearColorComponent():
    m_clearColor(glm::vec4(0, 0, 0, 1))
{
}

void ClearColorComponent::RenderInspector()
{
    ImGuiTreeNodeFlags meshHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
    if (ImGui::CollapsingHeader("OpenGL Constants", meshHeaderFlags))
    {
        // Clear color
        float clearColor[4] = { m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a };
				
        if (ImGui::ColorPicker4("Clear Color", clearColor))
        {
            m_clearColor = glm::vec4(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
        }
    }
}

const glm::vec4& ClearColorComponent::GetClearColor() const
{
    return m_clearColor;
}
