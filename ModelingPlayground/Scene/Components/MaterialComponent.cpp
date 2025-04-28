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
    m_useColorTexture.Subscribe(this, [this](const bool& currentValue, bool)
    {
        if (!m_renderingManager || m_materialColorTexturePath.GetData().empty())
        {
            return;
        }

        if (currentValue)
        {
            m_renderingManager->IncrementTextureUsage(m_materialColorTexturePath.GetData(), this);
        }
        else
        {
            m_renderingManager->DecrementTextureUsage(m_materialColorTexturePath.GetData(), this);
        }
    });

    m_materialColorTexturePath.Subscribe(this, [this](const std::string& currentValue, std::string previousValue)
    {
        if (m_renderingManager && m_useColorTexture.GetData())
        {
            m_renderingManager->IncrementTextureUsage(currentValue, this);
            m_renderingManager->DecrementTextureUsage(previousValue, this);
        }
    });

    m_useMetallicMap.Subscribe(this, [this](const bool& currentValue, bool)
    {
        if (!m_renderingManager || m_metallicMapPath.GetData().empty())
        {
            return;
        }

        if (currentValue)
        {
            m_renderingManager->IncrementTextureUsage(m_metallicMapPath.GetData(), this);
        }
        else
        {
            m_renderingManager->DecrementTextureUsage(m_metallicMapPath.GetData(), this);
        }
    });

    m_metallicMapPath.Subscribe(this, [this](const std::string& currentValue, std::string previousValue)
    {
        if (m_renderingManager && m_useMetallicMap.GetData())
        {
            m_renderingManager->IncrementTextureUsage(currentValue, this);
            m_renderingManager->DecrementTextureUsage(previousValue, this);
        }
    });

    m_useMetallicMap.Subscribe(this, [this](const bool& currentValue, bool)
    {
        if (!m_renderingManager || m_roughnessMapPath.GetData().empty())
        {
            return;
        }

        if (currentValue)
        {
            m_renderingManager->IncrementTextureUsage(m_roughnessMapPath.GetData(), this);
        }
        else
        {
            m_renderingManager->DecrementTextureUsage(m_roughnessMapPath.GetData(), this);
        }
    });

    m_roughnessMapPath.Subscribe(this, [this](const std::string& currentValue, std::string previousValue)
    {
        if (m_renderingManager && m_useRoughnessMap.GetData())
        {
            m_renderingManager->IncrementTextureUsage(currentValue, this);
            m_renderingManager->DecrementTextureUsage(previousValue, this);
        }
    });
}

void MaterialComponent::SetOpenGLRenderer(std::shared_ptr<RenderingManager> renderingManager)
{
    m_renderingManager = renderingManager;

    // Load textures into memory
    m_renderingManager->AddTexture(m_materialColorTexturePath.GetData());
    if (m_useColorTexture.GetData())
    {
        m_renderingManager->IncrementTextureUsage(m_materialColorTexturePath.GetData(), this);
    }

    m_renderingManager->AddTexture(m_metallicMapPath.GetData());
    if (m_useMetallicMap.GetData())
    {
        m_renderingManager->IncrementTextureUsage(m_metallicMapPath.GetData(), this);
    }

    m_renderingManager->AddTexture(m_roughnessMapPath.GetData());
    if (m_useRoughnessMap.GetData())
    {
        m_renderingManager->IncrementTextureUsage(m_roughnessMapPath.GetData(), this);
    }
}

void MaterialComponent::RenderInspector()
{
    ImGuiTreeNodeFlags materialHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
    if (ImGui::CollapsingHeader("Material", materialHeaderFlags))
    {
        // Material Color
        PropertyDrawer::DrawCheckbox("Use Color Texture", m_useColorTexture);
        if (m_useColorTexture.GetData())
        {
            PropertyDrawer::DrawTextureCacheCombo("Color Texture", m_renderingManager->GetTextureCache(),
                                                  m_materialColorTexturePath);
        }
        else
        {
            PropertyDrawer::DrawVec4fColor("Color", m_materialColor);
        }

        // Material Metallic
        PropertyDrawer::DrawCheckbox("Use Metallic Map", m_useMetallicMap);
        if (m_useMetallicMap.GetData())
        {
            PropertyDrawer::DrawTextureCacheCombo("Metallic Map", m_renderingManager->GetTextureCache(),
                                                  m_metallicMapPath);
        }
        else
        {
            PropertyDrawer::DrawFloatDrag("Metallic", m_metallic, 0.01f, 0.f, 1.0f);
        }

        // Material Roughness
        PropertyDrawer::DrawCheckbox("Use Roughness Map", m_useRoughnessMap);
        if (m_useRoughnessMap.GetData())
        {
            PropertyDrawer::DrawTextureCacheCombo("Roughness Map", m_renderingManager->GetTextureCache(),
                                                  m_roughnessMapPath);
        }
        else
        {
            PropertyDrawer::DrawFloatDrag("Roughness", m_roughness, 0.01f, 0.f, 1.0f);
        }
    }
}

bool MaterialComponent::GetUseColorTexture() const
{
    return m_useColorTexture.GetData() && !m_materialColorTexturePath.GetData().empty();
}

glm::vec4 MaterialComponent::GetMaterialColor() const
{
    return m_materialColor;
}

const std::string& MaterialComponent::GetMaterialTexture() const
{
    return m_materialColorTexturePath.GetData();
}

void MaterialComponent::SetMaterialTexture(const std::string& materialTexture, bool setUseColorTexture)
{
    m_materialColorTexturePath = materialTexture;

    if (setUseColorTexture)
    {
        m_useColorTexture = true;
        m_renderingManager->IncrementTextureUsage(m_materialColorTexturePath.GetData(), this);
    }
}

bool MaterialComponent::GetUseMetallicMap() const
{
    return m_useMetallicMap.GetData() && !m_metallicMapPath.GetData().empty();
}

float MaterialComponent::GetMetallic() const
{
    return m_metallic;
}

const std::string& MaterialComponent::GetMetallicMap() const
{
    return m_metallicMapPath.GetData();
}

void MaterialComponent::SetMetallicMap(const std::string& metallicMap, bool setUseMetallicMap)
{
    m_metallicMapPath = metallicMap;

    if (setUseMetallicMap)
    {
        m_useMetallicMap = true;
        m_renderingManager->IncrementTextureUsage(m_metallicMapPath.GetData(), this);
    }
}

bool MaterialComponent::GetUseRoughnessMap() const
{
    return m_useRoughnessMap.GetData() && !m_roughnessMapPath.GetData().empty();
}

float MaterialComponent::GetRoughness() const
{
    return m_roughness;
}

const std::string& MaterialComponent::GetRoughnessMap() const
{
    return m_roughnessMapPath.GetData();
}

void MaterialComponent::SetRoughnessMap(const std::string& roughnessMap, bool setUseRoughnessMap)
{
    m_roughnessMapPath = roughnessMap;

    if (setUseRoughnessMap)
    {
        m_useRoughnessMap = true;
        m_renderingManager->IncrementTextureUsage(m_roughnessMapPath.GetData(), this);
    }
}
