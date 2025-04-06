#include "SpotLightComponent.h"

#include "imgui.h"
#include "tinyfiledialogs.h"
#include "../../Utils/PropertyDrawer.h"

SpotLightComponent::SpotLightComponent():
	m_lightColor(glm::vec3(1.0f)),
	m_falloffAngles(glm::vec2(25.f, 50.f))
{
}

void SpotLightComponent::RenderInspector()
{
	ImGuiTreeNodeFlags spotLightHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Spot Light", spotLightHeaderFlags))
	{
		// Color
		PropertyDrawer::DrawVec3fColor("Color", m_lightColor);

		// Falloff angle
		PropertyDrawer::DrawVec2fDrag("Falloff Angle", m_falloffAngles, 1.f, 0.f, 180.f);
	}
}

const glm::vec3& SpotLightComponent::GetLightColor() const
{
	return m_lightColor.GetData();
}

DataBinding<glm::vec3>& SpotLightComponent::GetLightColorDataBinding()
{
	return m_lightColor;
}

const glm::vec2& SpotLightComponent::GetLightFalloffAngles() const
{
	return m_falloffAngles.GetData();
}

DataBinding<glm::vec2>& SpotLightComponent::GetLightFalloffAnglesDataBinding()
{
	return m_falloffAngles;
}
