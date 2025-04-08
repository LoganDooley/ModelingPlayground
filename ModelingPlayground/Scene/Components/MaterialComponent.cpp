#include "MaterialComponent.h"

#include "imgui.h"
#include "../../OpenGLHelpers/OpenGLTexture.h"
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

void MaterialComponent::SetTextureCache(std::shared_ptr<OpenGLTextureCache> openGLTextureCache)
{
	m_openGLTextureCache = openGLTextureCache;

	// Load textures into memory
	m_openGLTextureCache->LoadTexture(m_materialColorTexturePath);
	if (m_useColorTexture)
	{
		m_openGLTextureCache->IncrementTextureUsage(m_materialColorTexturePath);
	}

	m_openGLTextureCache->LoadTexture(m_metallicMapPath);
	if (m_useMetallicMap)
	{
		m_openGLTextureCache->IncrementTextureUsage(m_metallicMapPath);
	}

	m_openGLTextureCache->LoadTexture(m_roughnessMapPath);
	if (m_useRoughnessMap)
	{
		m_openGLTextureCache->IncrementTextureUsage(m_roughnessMapPath);
	}
}

void MaterialComponent::RenderInspector()
{
	ImGuiTreeNodeFlags materialHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Material", materialHeaderFlags))
	{
		// Material Color
		if (ImGui::Checkbox("Use Color Texture", &m_useColorTexture))
		{
			if (!m_materialColorTexturePath.empty())
			{
				if (m_useColorTexture)
				{
					m_openGLTextureCache->IncrementTextureUsage(m_materialColorTexturePath);
				}
				else
				{
					m_openGLTextureCache->DecrementTextureUsage(m_materialColorTexturePath);
				}
			}
		}
		if (m_useColorTexture)
		{
			PropertyDrawer::DrawTextureCacheCombo("Color Texture", m_openGLTextureCache, m_materialColorTexturePath);
		}
		else
		{
			PropertyDrawer::DrawVec4fColor("Color", m_materialColor);
		}

		// Material Metallic
		if (ImGui::Checkbox("Use Metallic Map", &m_useMetallicMap))
		{
			if (!m_metallicMapPath.empty())
			{
				if (m_useMetallicMap)
				{
					m_openGLTextureCache->IncrementTextureUsage(m_metallicMapPath);
				}
				else
				{
					m_openGLTextureCache->DecrementTextureUsage(m_metallicMapPath);
				}
			}
		}
		if (m_useMetallicMap)
		{
			PropertyDrawer::DrawTextureCacheCombo("Metallic Map", m_openGLTextureCache, m_metallicMapPath);
		}
		else
		{
			PropertyDrawer::DrawFloatDrag("Metallic", m_metallic, 0.01f, 0.f, 1.0f);
		}

		// Material Roughness
		if (ImGui::Checkbox("Use Roughness Map", &m_useRoughnessMap))
		{
			if (!m_roughnessMapPath.empty())
			{
				if (m_useRoughnessMap)
				{
					m_openGLTextureCache->IncrementTextureUsage(m_roughnessMapPath);
				}
				else
				{
					m_openGLTextureCache->DecrementTextureUsage(m_roughnessMapPath);
				}
			}
		}
		if (m_useRoughnessMap)
		{
			PropertyDrawer::DrawTextureCacheCombo("Roughness Map", m_openGLTextureCache, m_roughnessMapPath);
		}
		else
		{
			PropertyDrawer::DrawFloatDrag("Roughness", m_roughness, 0.01f, 0.f, 1.0f);
		}
	}
}

bool MaterialComponent::GetUseColorTexture() const
{
	return m_useColorTexture && !m_materialColorTexturePath.empty();
}

glm::vec4 MaterialComponent::GetMaterialColor() const
{
	return m_materialColor;
}

GLuint64 MaterialComponent::GetMaterialTexture() const
{
	return m_openGLTextureCache->GetTexture(m_materialColorTexturePath)->GetTextureHandle();
}

bool MaterialComponent::GetUseMetallicMap() const
{
	return m_useMetallicMap && !m_metallicMapPath.empty();
}

float MaterialComponent::GetMetallic() const
{
	return m_metallic;
}

GLuint64 MaterialComponent::GetMetallicMap() const
{
	return m_openGLTextureCache->GetTexture(m_metallicMapPath)->GetTextureHandle();
}

bool MaterialComponent::GetUseRoughnessMap() const
{
	return m_useRoughnessMap && !m_roughnessMapPath.empty();
}

float MaterialComponent::GetRoughness() const
{
	return m_roughness;
}

GLuint64 MaterialComponent::GetRoughnessMap() const
{
	return m_openGLTextureCache->GetTexture(m_roughnessMapPath)->GetTextureHandle();
}
