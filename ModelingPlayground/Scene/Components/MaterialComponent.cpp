#include "MaterialComponent.h"

#include "imgui.h"
#include "../../Utils/PropertyDrawer.h"

MaterialComponent::MaterialComponent():
	m_materialColor(glm::vec3(1)),
	m_metallic(0.f),
	m_roughness(0.f)
{
}

void MaterialComponent::RenderInspector()
{
	ImGuiTreeNodeFlags materialHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Material", materialHeaderFlags))
	{
		// Material Color
		PropertyDrawer::DrawVec3fColor("Color", m_materialColor);

		// Material Metallic
		PropertyDrawer::DrawFloatDrag("Metallic", m_metallic, 0.01f, 0.f, 1.0f);

		// Material Roughness
		PropertyDrawer::DrawFloatDrag("Roughness", m_roughness, 0.01f, 0.f, 1.0f);
	}
}

glm::vec3 MaterialComponent::GetMaterialColor() const
{
	return m_materialColor;
}

float MaterialComponent::GetMetallic() const
{
	return m_metallic;
}

float MaterialComponent::GetRoughness() const
{
	return m_roughness;
}
