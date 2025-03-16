#include "LightsContainer.h"

#include <iostream>
#include <ostream>

#include "../Scene/Object.h"

LightsContainer::LightsContainer(const std::shared_ptr<OpenGLShader>& shader, uint32_t maxLights):
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

bool LightsContainer::AddLight(const std::shared_ptr<SceneNode>& light, LightType type)
{
    if (m_lights.size() == m_maxLights)
    {
        std::cout << "LightsContainer|AddLight: Could not add light due to exceeding maxLights!\n";
        return false;
    }

    m_lights.push_back({light, type});
    SetLightUniforms(m_lights.size() - 1);
    m_shader->SetUniform1i("lightCount", m_lights.size());
    return true;
}

void LightsContainer::ClearLights()
{
    m_lights.clear();
    m_shader->SetUniform1i("lightCount", 0);
}

std::string LightsContainer::GetLightTypeUniformName(uint32_t lightIndex)
{
    return "lights[" + std::to_string(lightIndex) + "].type";
}

std::string LightsContainer::GetLightPositionUniformName(uint32_t lightIndex)
{
    return "lights[" + std::to_string(lightIndex) + "].position";
}

std::string LightsContainer::GetLightDirectionUniformName(uint32_t lightIndex)
{
    return "lights[" + std::to_string(lightIndex) + "].direction";
}

std::string LightsContainer::GetLightColorUniformName(uint32_t lightIndex)
{
    return "lights[" + std::to_string(lightIndex) + "].color";
}

std::string LightsContainer::GetLightFalloffUniformName(uint32_t lightIndex)
{
    return "lights[" + std::to_string(lightIndex) + "].falloff";
}

void LightsContainer::SetLightUniforms(uint32_t lightIndex) const
{
    std::pair<std::shared_ptr<SceneNode>, LightType> light = m_lights[lightIndex];
    SetLightTypeUniform(lightIndex, light.second);
    std::shared_ptr<TransformComponent> transformComponent = light.first->GetObject().GetComponents<TransformComponent>()[0];
    switch (light.second)
    {
        case LightType::Directional:
            {
                std::shared_ptr<DirectionalLightComponent> directionalLightComponent = light.first->GetObject().GetComponents<DirectionalLightComponent>()[0];
                SetDirectionalLightUniforms(lightIndex, transformComponent, directionalLightComponent);
            }
            break;
        case LightType::Point:
            {
                std::shared_ptr<PointLightComponent> pointLightComponent = light.first->GetObject().GetComponents<PointLightComponent>()[0];
                SetPointLightUniforms(lightIndex, transformComponent, pointLightComponent);
            }
            break;
        case LightType::Spot:
            {
                std::shared_ptr<SpotLightComponent> spotLightComponent = light.first->GetObject().GetComponents<SpotLightComponent>()[0];
                SetSpotLightUniforms(lightIndex, transformComponent, spotLightComponent);
            }
            break;
    }    
}

void LightsContainer::SetDirectionalLightUniforms(uint32_t lightIndex,
    const std::shared_ptr<TransformComponent>& transformComponent,
    const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent) const
{
    SetLightTypeUniform(lightIndex, LightType::Directional);

    SetLightColorUniform(lightIndex, directionalLightComponent->GetLightColor());
    SetLightDirectionUniform(lightIndex, transformComponent->GetLocalXUnitVector());
}

void LightsContainer::SetPointLightUniforms(uint32_t lightIndex,
    const std::shared_ptr<TransformComponent>& transformComponent,
    const std::shared_ptr<PointLightComponent>& pointLightComponent) const
{
    SetLightTypeUniform(lightIndex, LightType::Point);

    SetLightColorUniform(lightIndex, pointLightComponent->GetLightColor());
    SetLightPositionUniform(lightIndex, transformComponent->GetPosition());
    SetLightFalloffUniform(lightIndex, pointLightComponent->GetFalloff());
}

void LightsContainer::SetSpotLightUniforms(uint32_t lightIndex,
    const std::shared_ptr<TransformComponent>& transformComponent,
    const std::shared_ptr<SpotLightComponent>& spotLightComponent) const
{
    SetLightTypeUniform(lightIndex, LightType::Spot);

    SetLightColorUniform(lightIndex, spotLightComponent->GetLightColor());
    SetLightPositionUniform(lightIndex, transformComponent->GetPosition());
    SetLightDirectionUniform(lightIndex, transformComponent->GetLocalXUnitVector());
    SetLightFalloffUniform(lightIndex, glm::vec3(spotLightComponent->GetLightFalloffAngle()));
}

void LightsContainer::SetLightTypeUniform(uint32_t lightIndex, LightType type) const
{
    m_shader->SetUniform1i(GetLightTypeUniformName(lightIndex), type);
}

void LightsContainer::SetLightPositionUniform(uint32_t lightIndex, const glm::vec3& position) const
{
    m_shader->SetUniform3f(GetLightPositionUniformName(lightIndex), position);
}

void LightsContainer::SetLightDirectionUniform(uint32_t lightIndex, const glm::vec3& direction) const
{
    m_shader->SetUniform3f(GetLightDirectionUniformName(lightIndex), direction);
}

void LightsContainer::SetLightColorUniform(uint32_t lightIndex, const glm::vec3& color) const
{
    m_shader->SetUniform3f(GetLightColorUniformName(lightIndex), color);
}

void LightsContainer::SetLightFalloffUniform(uint32_t lightIndex, const glm::vec3& falloff) const
{
    m_shader->SetUniform3f(GetLightFalloffUniformName(lightIndex), falloff);
}
