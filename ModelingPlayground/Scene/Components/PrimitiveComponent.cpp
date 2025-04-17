#include "PrimitiveComponent.h"

#include "imgui.h"
#include "../../Application/Rendering/RenderingManager.h"
#include "../../Utils/PropertyDrawer.h"

PrimitiveComponent::PrimitiveComponent():
	m_primitiveName("Triangle"),
	m_currentItem(0)
{
}

PrimitiveComponent::PrimitiveComponent(std::shared_ptr<RenderingManager> renderingManager):
	m_renderingManager(renderingManager),
	m_primitiveName("Triangle"),
	m_currentItem(0)
{
}

void PrimitiveComponent::SetRenderingManager(std::shared_ptr<RenderingManager> renderingManager)
{
	m_renderingManager = renderingManager;
}

void PrimitiveComponent::RenderInspector()
{
	ImGuiTreeNodeFlags meshHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Mesh", meshHeaderFlags))
	{
		PropertyDrawer::DrawCombo("Primitive Type", m_renderingManager->GetPrimitiveNames(),
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
