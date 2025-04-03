#include "DirectionalLightComponent.h"

#include <glm/geometric.hpp>

#include "imgui.h"
#include "tinyfiledialogs.h"
#include "../../Utils/PropertyDrawer.h"

DirectionalLightComponent::DirectionalLightComponent():
	m_lightColor(glm::vec3(1.0f, 1.0f, 1.0f))
{
}

void DirectionalLightComponent::RenderInspector()
{
	ImGuiTreeNodeFlags directionalLightHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Directional Light", directionalLightHeaderFlags))
	{
		// Color
		PropertyDrawer::DrawVec3fColor("Color", m_lightColor);

		// Debug capture shadow map
		if (ImGui::Button("Capture Shadow Map"))
		{
			const char* lFilterPatterns[1] = {"*.png"};
			const char* filePath = tinyfd_saveFileDialog(
				"Save shadow map as",
				"",
				1,
				lFilterPatterns,
				"png files"
			);
			m_onCaptureShadowMap(filePath);
		}
	}
}

const glm::vec3& DirectionalLightComponent::GetLightColor() const
{
	return m_lightColor.GetData();
}

DataBinding<glm::vec3>& DirectionalLightComponent::GetLightColorDataBinding()
{
	return m_lightColor;
}

void DirectionalLightComponent::SetOnCaptureShadowMap(std::function<void(const std::string&)> onCaptureShadowMap)
{
	m_onCaptureShadowMap = onCaptureShadowMap;
}
