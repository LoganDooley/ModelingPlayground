#include "PointLightComponent.h"

#include <glm/geometric.hpp>

#include "imgui.h"
#include "../../Utils/PropertyDrawer.h"

PointLightComponent::PointLightComponent():
	m_lightColor(glm::vec3(1.0f, 1.0f, 1.0f)),
	m_falloff(glm::vec3(0))
{
}

void PointLightComponent::RenderInspector()
{
	ImGuiTreeNodeFlags directionalLightHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Point Light", directionalLightHeaderFlags))
	{
		// Color
		PropertyDrawer::DrawVec3fColor("Color", m_lightColor);

		// Falloff
		PropertyDrawer::DrawVec3fDrag("Falloff", m_falloff);
	}
}

const glm::vec3& PointLightComponent::GetLightColor() const
{
	return m_lightColor.GetData();
}

DataBinding<glm::vec3>& PointLightComponent::GetLightColorDataBinding()
{
	return m_lightColor;
}

const glm::vec3& PointLightComponent::GetFalloff() const
{
	return m_falloff.GetData();
}

DataBinding<glm::vec3>& PointLightComponent::GetFalloffDataBinding()
{
	return m_falloff;
}
