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

    m_useRoughnessMap.Subscribe(this, [this](const bool& currentValue, bool)
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
    if (!m_materialColorTexturePath.GetData().empty())
    {
        m_renderingManager->AddTexture(m_materialColorTexturePath.GetData());
    }
    if (m_useColorTexture.GetData())
    {
        m_renderingManager->IncrementTextureUsage(m_materialColorTexturePath.GetData(), this);
    }

    if (!m_metallicMapPath.GetData().empty())
    {
        m_renderingManager->AddTexture(m_metallicMapPath.GetData());
    }
    if (m_useMetallicMap.GetData())
    {
        m_renderingManager->IncrementTextureUsage(m_metallicMapPath.GetData(), this);
    }

    if (!m_roughnessMapPath.GetData().empty())
    {
        m_renderingManager->AddTexture(m_roughnessMapPath.GetData());
    }
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

DataBinding<bool>& MaterialComponent::GetUseColorTextureDataBinding()
{
    return m_useColorTexture;
}

DataBinding<glm::vec4>& MaterialComponent::GetMaterialColorDataBinding()
{
    return m_materialColor;
}

DataBinding<std::string>& MaterialComponent::GetMaterialTextureDataBinding()
{
    return m_materialColorTexturePath;
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

DataBinding<bool>& MaterialComponent::GetUseMetallicMapDataBinding()
{
    return m_useMetallicMap;
}

DataBinding<float>& MaterialComponent::GetMetallicDataBinding()
{
    return m_metallic;
}

DataBinding<std::string>& MaterialComponent::GetMetallicMapDataBinding()
{
    return m_metallicMapPath;
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

DataBinding<bool>& MaterialComponent::GetUseRoughnessMapDataBinding()
{
    return m_useRoughnessMap;
}

DataBinding<float>& MaterialComponent::GetRoughnessDataBinding()
{
    return m_roughness;
}

DataBinding<std::string>& MaterialComponent::GetRoughnessMapDataBinding()
{
    return m_roughnessMapPath;
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
