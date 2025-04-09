#include "PrimitiveComponent.h"

#include "imgui.h"
#include "../../../../ModelingEngine/ModelingEngine/ModelingEngine/Libraries/tinyfiledialogs/tinyfiledialogs.h"
#include "../../Utils/PropertyDrawer.h"

PrimitiveComponent::PrimitiveComponent():
	m_primitiveName("Triangle"),
	m_currentItem(0)
{
}

PrimitiveComponent::PrimitiveComponent(std::shared_ptr<OpenGLPrimitiveManager> openGLPrimitiveManager):
	m_openGLPrimitiveManager(openGLPrimitiveManager),
	m_primitiveName("Triangle"),
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
		PropertyDrawer::DrawCombo("Primitive Type", m_openGLPrimitiveManager->GetPrimitiveNames(), m_primitiveName);

		if (ImGui::Button("Use Custom Obj"))
		{
			const char* lFilterPatterns[1] = {"*.obj"};
			const char* filePath = tinyfd_openFileDialog(
				"Select an obj file to use",
				"",
				1,
				lFilterPatterns,
				".obj",
				0
			);
			// If user actually selected a file
			if (filePath != nullptr)
			{
				m_primitiveName = m_openGLPrimitiveManager->LoadPrimitive(filePath);
			}
		}
	}
}

void PrimitiveComponent::SetPrimitiveName(const std::string& primitiveName)
{
	m_primitiveName = primitiveName;
}

std::string PrimitiveComponent::GetPrimitiveName() const
{
	return m_primitiveName;
}
