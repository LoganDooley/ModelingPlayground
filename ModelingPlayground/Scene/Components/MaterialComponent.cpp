#include "MaterialComponent.h"

#include "imgui.h"
#include "../../Application/Rendering/RenderingManager.h"
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

MaterialComponent::MaterialComponent(std::shared_ptr<RenderingManager> renderingManager):
	m_renderingManager(renderingManager),
	m_useColorTexture(false),
	m_materialColor(glm::vec4(1)),
	m_useMetallicMap(false),
	m_metallic(0.f),
	m_useRoughnessMap(false),
	m_roughness(0.f)
{
}

void MaterialComponent::SetOpenGLRenderer(std::shared_ptr<RenderingManager> renderingManager)
{
	m_renderingManager = renderingManager;

	// Load textures into memory
	m_renderingManager->AddTexture(m_materialColorTexturePath);
	if (m_useColorTexture)
	{
		m_renderingManager->IncrementTextureUsage(m_materialColorTexturePath, this);
	}

	m_renderingManager->AddTexture(m_metallicMapPath);
	if (m_useMetallicMap)
	{
		m_renderingManager->IncrementTextureUsage(m_metallicMapPath, this);
	}

	m_renderingManager->AddTexture(m_roughnessMapPath);
	if (m_useRoughnessMap)
	{
		m_renderingManager->IncrementTextureUsage(m_roughnessMapPath, this);
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
					m_renderingManager->IncrementTextureUsage(m_materialColorTexturePath, this);
				}
				else
				{
					m_renderingManager->DecrementTextureUsage(m_materialColorTexturePath, this);
				}
			}
		}
		if (m_useColorTexture)
		{
			std::string previousPath = m_materialColorTexturePath;
			PropertyDrawer::DrawTextureCacheCombo("Color Texture", m_renderingManager->GetTextureCache(),
			                                      m_materialColorTexturePath);
			if (previousPath != m_materialColorTexturePath)
			{
				m_renderingManager->DecrementTextureUsage(previousPath, this);
				m_renderingManager->IncrementTextureUsage(m_materialColorTexturePath, this);
			}
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
					m_renderingManager->IncrementTextureUsage(m_metallicMapPath, this);
				}
				else
				{
					m_renderingManager->DecrementTextureUsage(m_metallicMapPath, this);
				}
			}
		}
		if (m_useMetallicMap)
		{
			std::string previousPath = m_metallicMapPath;
			PropertyDrawer::DrawTextureCacheCombo("Metallic Map", m_renderingManager->GetTextureCache(),
			                                      m_metallicMapPath);
			if (previousPath != m_metallicMapPath)
			{
				m_renderingManager->DecrementTextureUsage(previousPath, this);
				m_renderingManager->IncrementTextureUsage(m_metallicMapPath, this);
			}
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
					m_renderingManager->IncrementTextureUsage(m_roughnessMapPath, this);
				}
				else
				{
					m_renderingManager->DecrementTextureUsage(m_roughnessMapPath, this);
				}
			}
		}
		if (m_useRoughnessMap)
		{
			std::string previousPath = m_roughnessMapPath;
			PropertyDrawer::DrawTextureCacheCombo("Roughness Map", m_renderingManager->GetTextureCache(),
			                                      m_roughnessMapPath);
			if (previousPath != m_roughnessMapPath)
			{
				m_renderingManager->DecrementTextureUsage(previousPath, this);
				m_renderingManager->IncrementTextureUsage(m_roughnessMapPath, this);
			}
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

const std::string& MaterialComponent::GetMaterialTexture() const
{
	return m_materialColorTexturePath;
}

void MaterialComponent::SetMaterialTexture(const std::string& materialTexture, bool setUseColorTexture)
{
	m_materialColorTexturePath = materialTexture;

	if (setUseColorTexture)
	{
		m_useColorTexture = true;
		m_renderingManager->IncrementTextureUsage(m_materialColorTexturePath, this);
	}
}

bool MaterialComponent::GetUseMetallicMap() const
{
	return m_useMetallicMap && !m_metallicMapPath.empty();
}

float MaterialComponent::GetMetallic() const
{
	return m_metallic;
}

const std::string& MaterialComponent::GetMetallicMap() const
{
	return m_metallicMapPath;
}

void MaterialComponent::SetMetallicMap(const std::string& metallicMap, bool setUseMetallicMap)
{
	m_metallicMapPath = metallicMap;

	if (setUseMetallicMap)
	{
		m_useMetallicMap = true;
		m_renderingManager->IncrementTextureUsage(m_metallicMapPath, this);
	}
}

bool MaterialComponent::GetUseRoughnessMap() const
{
	return m_useRoughnessMap && !m_roughnessMapPath.empty();
}

float MaterialComponent::GetRoughness() const
{
	return m_roughness;
}

const std::string& MaterialComponent::GetRoughnessMap() const
{
	return m_roughnessMapPath;
}

void MaterialComponent::SetRoughnessMap(const std::string& roughnessMap, bool setUseRoughnessMap)
{
	m_roughnessMapPath = roughnessMap;

	if (setUseRoughnessMap)
	{
		m_useRoughnessMap = true;
		m_renderingManager->IncrementTextureUsage(m_roughnessMapPath, this);
	}
}
