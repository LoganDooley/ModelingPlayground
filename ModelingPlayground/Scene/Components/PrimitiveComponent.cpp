#include "PrimitiveComponent.h"

#include "imgui.h"
#include "../../../../ModelingEngine/ModelingEngine/ModelingEngine/Libraries/tinyfiledialogs/tinyfiledialogs.h"
#include "../../Utils/PropertyDrawer.h"
#include "misc/cpp/imgui_stdlib.h"

PrimitiveComponent::PrimitiveComponent(std::shared_ptr<OpenGLPrimitiveManager> openGLPrimitiveManager):
    m_openGLPrimitiveManager(openGLPrimitiveManager),
    m_primitiveType(PrimitiveType::Triangle),
    m_currentItem(0)
{
}

void PrimitiveComponent::SetPrimitiveManager(std::shared_ptr<OpenGLPrimitiveManager> openGLPrimitiveManager)
{
    m_openGLPrimitiveManager = openGLPrimitiveManager;
}

void PrimitiveComponent::RenderInspector()
{
    ImGuiTreeNodeFlags meshHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
    if (ImGui::CollapsingHeader("Mesh", meshHeaderFlags))
    {
        PropertyDrawer::DrawEnumLabelCombo<PrimitiveType>("Primitive Type", PrimitiveTypeEnumLabel, m_primitiveType);
    }
    if (ImGui::Button("Use Custom Obj"))
    {
        char const * lFilterPatterns[1] = { "*.obj" };
        const char* filePath = tinyfd_openFileDialog(
            "Select an obj file to use",
            "",
            1,
            lFilterPatterns,
            ".obj",
            0
            );
        // If user actually selected a file
        if (filePath != NULL)
        {
            m_customPrimitiveFilePath = m_openGLPrimitiveManager->LoadPrimitive(filePath);
        }
    }
}

std::string PrimitiveComponent::GetCustomPrimitiveFilePath() const
{
    return m_customPrimitiveFilePath;
}

PrimitiveType PrimitiveComponent::GetPrimitiveType() const
{
    return m_primitiveType;
}
