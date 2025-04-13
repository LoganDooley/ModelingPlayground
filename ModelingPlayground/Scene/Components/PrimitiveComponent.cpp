#include "PrimitiveComponent.h"

#include "imgui.h"
#include "../../../../ModelingEngine/ModelingEngine/ModelingEngine/Libraries/tinyfiledialogs/tinyfiledialogs.h"
#include "../../OpenGLHelpers/OpenGLRenderer.h"
#include "../../Utils/PropertyDrawer.h"

PrimitiveComponent::PrimitiveComponent():
	m_primitiveName("Triangle"),
	m_currentItem(0)
{
}

PrimitiveComponent::PrimitiveComponent(std::shared_ptr<OpenGLRenderer> openGLRenderer):
	m_openGLRenderer(openGLRenderer),
	m_primitiveName("Triangle"),
	m_currentItem(0)
{
}

void PrimitiveComponent::SetOpenGLRenderer(std::shared_ptr<OpenGLRenderer> openGLRenderer)
{
	m_openGLRenderer = openGLRenderer;
}

void PrimitiveComponent::RenderInspector()
{
	ImGuiTreeNodeFlags meshHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Mesh", meshHeaderFlags))
	{
		PropertyDrawer::DrawCombo("Primitive Type", m_openGLRenderer->GetOpenGLPrimitiveManager()->GetPrimitiveNames(),
		                          m_primitiveName);
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
