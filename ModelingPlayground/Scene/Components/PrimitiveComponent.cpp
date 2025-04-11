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
