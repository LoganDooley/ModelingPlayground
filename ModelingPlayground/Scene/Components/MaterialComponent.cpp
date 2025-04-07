#include "MaterialComponent.h"

#include "imgui.h"
#include "../../OpenGLHelpers/OpenGLTextureCache.h"
#include "../../Utils/PropertyDrawer.h"

MaterialComponent::MaterialComponent():
	m_useColorTexture(false),
	m_materialColor(glm::vec4(1)),
	m_useMetallicMap(false),
	m_metallic(0.f),
	m_useRoughnessMap(false),
	m_roughness(0.f)
{
}

MaterialComponent::MaterialComponent(std::shared_ptr<OpenGLTextureCache> openGLTextureCache):
	m_openGLTextureCache(openGLTextureCache),
	m_useColorTexture(false),
	m_materialColor(glm::vec4(1)),
	m_useMetallicMap(false),
	m_metallic(0.f),
	m_useRoughnessMap(false),
	m_roughness(0.f)
{
}

void MaterialComponent::RenderInspector()
{
	ImGuiTreeNodeFlags materialHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Material", materialHeaderFlags))
	{
		const std::unordered_map<std::string, std::shared_ptr<OpenGLTexture>>& textures = m_openGLTextureCache->
			GetAllTextures();
		// Material Color
		ImGui::Checkbox("Use Color Texture", &m_useColorTexture);
		if (m_useColorTexture)
		{
			if (ImGui::BeginCombo("Color Texture", m_materialColorTexturePath.c_str()))
			{
				for (const auto& texture : textures)
				{
					bool selected = texture.first == m_materialColorTexturePath;
					if (ImGui::Selectable(texture.first.c_str(), selected))
					{
						m_materialColorTexturePath = texture.first;
					}
					if (selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		}
		else
		{
			PropertyDrawer::DrawVec4fColor("Color", m_materialColor);
		}

		// Material Metallic
		ImGui::Checkbox("Use Metallic Map", &m_useMetallicMap);
		if (m_useMetallicMap)
		{
		}
		else
		{
			PropertyDrawer::DrawFloatDrag("Metallic", m_metallic, 0.01f, 0.f, 1.0f);
		}

		// Material Roughness
		ImGui::Checkbox("Use Roughness Map", &m_useRoughnessMap);
		if (m_useRoughnessMap)
		{
		}
		else
		{
			PropertyDrawer::DrawFloatDrag("Roughness", m_roughness, 0.01f, 0.f, 1.0f);
		}
	}
}

glm::vec4 MaterialComponent::GetMaterialColor() const
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
