#include "OpenGLLightContainer.h"

#include <iostream>
#include <ostream>

#include "../Scene/Object.h"

OpenGLLightContainer::OpenGLLightContainer(const std::shared_ptr<OpenGLShader>& shader, uint32_t maxLights):
    m_shader(shader),
    m_maxLights(maxLights)
{
    // Register uniforms
    for (uint32_t i = 0; i<maxLights; i++)
    {
        m_shader->RegisterUniformVariable(GetLightTypeUniformName(i));
        m_shader->RegisterUniformVariable(GetLightPositionUniformName(i));
        m_shader->RegisterUniformVariable(GetLightDirectionUniformName(i));
        m_shader->RegisterUniformVariable(GetLightColorUniformName(i));
        m_shader->RegisterUniformVariable(GetLightFalloffUniformName(i));
    }
    m_shader->RegisterUniformVariable("lightCount");
}

bool OpenGLLightContainer::AddLight(const std::shared_ptr<SceneNode>& light, LightType type)
{
    if (m_lights.size() == m_maxLights)
    {
        std::cout << "LightsContainer|AddLight: Could not add light due to exceeding maxLights!\n";
        return false;
    }

    m_lights.push_back({light, type});
    InitializeLight(m_lights.size() - 1);
    m_shader->SetUniform1i("lightCount", m_lights.size());
    return true;
}

void OpenGLLightContainer::RemoveLight(uint32_t lightIndex)
{
    m_lights.erase(m_lights.begin() + lightIndex);
    for (int i = lightIndex; i < m_lights.size(); i++)
    {
        // Reset this light's uniforms
        SetLightUniforms(lightIndex);
    }
    m_shader->SetUniform1i("lightCount", m_lights.size());
}

void OpenGLLightContainer::ClearLights()
{
    m_lights.clear();
    m_shader->SetUniform1i("lightCount", 0);
}

uint32_t OpenGLLightContainer::GetLightIndex(const std::shared_ptr<SceneNode>& lightSceneNode) const
{
    for (int i = 0; i<m_lights.size(); i++)
    {
        if (m_lights[i].first.lock() == lightSceneNode)
        {
            return i;
        }
    }
    return -1;
}

std::string OpenGLLightContainer::GetLightTypeUniformName(uint32_t lightIndex)
{
    return "lights[" + std::to_string(lightIndex) + "].type";
}

std::string OpenGLLightContainer::GetLightPositionUniformName(uint32_t lightIndex)
{
    return "lights[" + std::to_string(lightIndex) + "].position";
}

std::string OpenGLLightContainer::GetLightDirectionUniformName(uint32_t lightIndex)
{
    return "lights[" + std::to_string(lightIndex) + "].direction";
}

std::string OpenGLLightContainer::GetLightColorUniformName(uint32_t lightIndex)
{
    return "lights[" + std::to_string(lightIndex) + "].color";
}

std::string OpenGLLightContainer::GetLightFalloffUniformName(uint32_t lightIndex)
{
    return "lights[" + std::to_string(lightIndex) + "].falloff";
}

void OpenGLLightContainer::InitializeLight(uint32_t lightIndex)
{
    SetLightUniforms(lightIndex);
    SubscribeToLight(lightIndex);
}

void OpenGLLightContainer::InitializeDirectionalLight(uint32_t lightIndex,
    const std::shared_ptr<TransformComponent>& transformComponent,
    const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent) const
{
    SetDirectionalLightUniforms(lightIndex, transformComponent, directionalLightComponent);
    SubscribeToDirectionalLight(lightIndex, transformComponent, directionalLightComponent);
}

void OpenGLLightContainer::InitializePointLight(uint32_t lightIndex,
    const std::shared_ptr<TransformComponent>& transformComponent,
    const std::shared_ptr<PointLightComponent>& pointLightComponent) const
{
    SetPointLightUniforms(lightIndex, transformComponent, pointLightComponent);
    SubscribeToPointLight(lightIndex, transformComponent, pointLightComponent);
}

void OpenGLLightContainer::InitializeSpotLight(uint32_t lightIndex,
    const std::shared_ptr<TransformComponent>& transformComponent,
    const std::shared_ptr<SpotLightComponent>& spotLightComponent) const
{
    SetSpotLightUniforms(lightIndex, transformComponent, spotLightComponent);
    SubscribeToSpotLight(lightIndex, transformComponent, spotLightComponent);
}

void OpenGLLightContainer::SetLightUniforms(uint32_t lightIndex)
{
    std::shared_ptr<SceneNode> lightSceneNode = m_lights[lightIndex].first.lock();
    if (!lightSceneNode)
    {
        m_lights[lightIndex].second = LightType::None;
        std::cerr<<"OpenGLLightContainer|SetLightUniforms: lightIndex: "<<lightIndex<<" has been deleted!\n";
        return;
    }
    Object& lightObject = lightSceneNode->GetObject();
    switch (m_lights[lightIndex].second)
    {
        case LightType::Directional:
            SetDirectionalLightUniforms(lightIndex,
                lightObject.GetFirstComponentOfType<TransformComponent>(),
                lightObject.GetFirstComponentOfType<DirectionalLightComponent>());
        break;
        case LightType::Point:
            SetPointLightUniforms(lightIndex,
                lightObject.GetFirstComponentOfType<TransformComponent>(),
                lightObject.GetFirstComponentOfType<PointLightComponent>());
        break;
        case LightType::Spot:
            SetSpotLightUniforms(lightIndex,
                lightObject.GetFirstComponentOfType<TransformComponent>(),
                lightObject.GetFirstComponentOfType<SpotLightComponent>());
        break;
    }
}

void OpenGLLightContainer::SetDirectionalLightUniforms(uint32_t lightIndex,
    const std::shared_ptr<TransformComponent>& transformComponent,
    const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent) const
{
    SetLightTypeUniform(lightIndex, LightType::Directional);
    SetLightColorUniform(lightIndex, directionalLightComponent->GetLightColor());
    SetLightDirectionUniform(lightIndex, transformComponent->GetLocalXUnitVector());
}

void OpenGLLightContainer::SetPointLightUniforms(uint32_t lightIndex,
    const std::shared_ptr<TransformComponent>& transformComponent,
    const std::shared_ptr<PointLightComponent>& pointLightComponent) const
{
    SetLightTypeUniform(lightIndex, LightType::Point);
    SetLightColorUniform(lightIndex, pointLightComponent->GetLightColor());
    SetLightPositionUniform(lightIndex, transformComponent->GetPosition());
    SetLightFalloffUniform(lightIndex, pointLightComponent->GetFalloff());
}

void OpenGLLightContainer::SetSpotLightUniforms(uint32_t lightIndex,
    const std::shared_ptr<TransformComponent>& transformComponent,
    const std::shared_ptr<SpotLightComponent>& spotLightComponent) const
{
    SetLightTypeUniform(lightIndex, LightType::Spot);
    SetLightColorUniform(lightIndex, spotLightComponent->GetLightColor());
    SetLightPositionUniform(lightIndex, transformComponent->GetPosition());
    SetLightDirectionUniform(lightIndex, transformComponent->GetLocalXUnitVector());
    SetLightFalloffUniform(lightIndex, glm::vec3(spotLightComponent->GetLightFalloffAngles(), 0.f));
}

void OpenGLLightContainer::SubscribeToLight(uint32_t lightIndex)
{
    std::shared_ptr<SceneNode> lightSceneNode = m_lights[lightIndex].first.lock();
    if (!lightSceneNode)
    {
        m_lights[lightIndex].second = LightType::None;
        std::cerr<<"OpenGLLightContainer|SubscribeToLight: lightIndex: "<<lightIndex<<" has been deleted!\n";
        return;
    }
    Object& lightObject = lightSceneNode->GetObject();
    switch (m_lights[lightIndex].second)
    {
    case LightType::Directional:
        SubscribeToDirectionalLight(lightIndex,
            lightObject.GetFirstComponentOfType<TransformComponent>(),
            lightObject.GetFirstComponentOfType<DirectionalLightComponent>());
        break;
    case LightType::Point:
        SubscribeToPointLight(lightIndex,
            lightObject.GetFirstComponentOfType<TransformComponent>(),
            lightObject.GetFirstComponentOfType<PointLightComponent>());
        break;
    case LightType::Spot:
        SubscribeToSpotLight(lightIndex,
            lightObject.GetFirstComponentOfType<TransformComponent>(),
            lightObject.GetFirstComponentOfType<SpotLightComponent>());
        break;
    }

    lightSceneNode->SubscribeToOnDestroyed([this, lightSceneNode]()
    {
        RemoveLight(GetLightIndex(lightSceneNode));
    });
}

void OpenGLLightContainer::SubscribeToDirectionalLight(uint32_t lightIndex,
                                                       const std::shared_ptr<TransformComponent>& transformComponent,
                                                       const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent) const
{
    const std::shared_ptr<SceneNode>& lightSceneNode = m_lights[lightIndex].first.lock();
    directionalLightComponent->GetLightColorDataBinding().Subscribe([this, lightSceneNode](const glm::vec3& lightColor, glm::vec3)
    {
        uint32_t lightIndex = GetLightIndex(lightSceneNode);
        SetLightColorUniform(lightIndex, lightColor);
    });

    transformComponent->GetLocalXUnitVectorDataBinding().Subscribe([this, lightSceneNode](const glm::vec3& localXUnitVector, glm::vec3)
    {
        uint32_t lightIndex = GetLightIndex(lightSceneNode);
        SetLightDirectionUniform(lightIndex, localXUnitVector);
    });
}

void OpenGLLightContainer::SubscribeToPointLight(uint32_t lightIndex,
    const std::shared_ptr<TransformComponent>& transformComponent,
    const std::shared_ptr<PointLightComponent>& pointLightComponent) const
{
    const std::shared_ptr<SceneNode>& lightSceneNode = m_lights[lightIndex].first.lock();
    pointLightComponent->GetLightColorDataBinding().Subscribe([this, lightSceneNode](const glm::vec3& lightColor, glm::vec3)
    {
        uint32_t lightIndex = GetLightIndex(lightSceneNode);
        SetLightColorUniform(lightIndex, lightColor);
    });

    transformComponent->GetPositionDataBinding().Subscribe([this, lightSceneNode](const glm::vec3& position, glm::vec3)
    {
        uint32_t lightIndex = GetLightIndex(lightSceneNode);
        SetLightPositionUniform(lightIndex, position);
    });

    pointLightComponent->GetFalloffDataBinding().Subscribe([this, lightSceneNode](const glm::vec3& falloff, glm::vec3)
    {
        uint32_t lightIndex = GetLightIndex(lightSceneNode);
        SetLightFalloffUniform(lightIndex, falloff);
    });
}

void OpenGLLightContainer::SubscribeToSpotLight(uint32_t lightIndex,
    const std::shared_ptr<TransformComponent>& transformComponent,
    const std::shared_ptr<SpotLightComponent>& spotLightComponent) const
{
    const std::shared_ptr<SceneNode>& lightSceneNode = m_lights[lightIndex].first.lock();
    spotLightComponent->GetLightColorDataBinding().Subscribe([this, lightSceneNode](const glm::vec3& lightColor, glm::vec3)
    {
        uint32_t lightIndex = GetLightIndex(lightSceneNode);
        SetLightColorUniform(lightIndex, lightColor);
    });

    transformComponent->GetPositionDataBinding().Subscribe([this, lightSceneNode](const glm::vec3& position, glm::vec3)
    {
        uint32_t lightIndex = GetLightIndex(lightSceneNode);
        SetLightPositionUniform(lightIndex, position);
    });

    transformComponent->GetLocalXUnitVectorDataBinding().Subscribe([this, lightSceneNode](const glm::vec3& localXUnitVector, glm::vec3)
    {
        uint32_t lightIndex = GetLightIndex(lightSceneNode);
        SetLightDirectionUniform(lightIndex, localXUnitVector);
    });

    spotLightComponent->GetLightFalloffAnglesDataBinding().Subscribe([this, lightSceneNode](const glm::vec2& falloffAngle, glm::vec2)
    {
        uint32_t lightIndex = GetLightIndex(lightSceneNode);
        SetLightFalloffUniform(lightIndex, glm::vec3(falloffAngle, 0.f));
    });
}

void OpenGLLightContainer::SetLightTypeUniform(uint32_t lightIndex, LightType type) const
{
    m_shader->SetUniform1i(GetLightTypeUniformName(lightIndex), type);
}

void OpenGLLightContainer::SetLightPositionUniform(uint32_t lightIndex, const glm::vec3& position) const
{
    m_shader->SetUniform3f(GetLightPositionUniformName(lightIndex), position);
}

void OpenGLLightContainer::SetLightDirectionUniform(uint32_t lightIndex, const glm::vec3& direction) const
{
    m_shader->SetUniform3f(GetLightDirectionUniformName(lightIndex), direction);
}

void OpenGLLightContainer::SetLightColorUniform(uint32_t lightIndex, const glm::vec3& color) const
{
    m_shader->SetUniform3f(GetLightColorUniformName(lightIndex), color);
}

void OpenGLLightContainer::SetLightFalloffUniform(uint32_t lightIndex, const glm::vec3& falloff) const
{
    m_shader->SetUniform3f(GetLightFalloffUniformName(lightIndex), falloff);
}
