#pragma once

#include <memory>
#include <vector>

#include "OpenGLShader.h"
#include "../Scene/Components/TransformComponent.h"
#include "../Scene/Components/DirectionalLightComponent.h"
#include "../Scene/Components/PointLightComponent.h"
#include "../Scene/Components/SpotLightComponent.h"
#include "../Scene/SceneNode/SceneNode.h"

enum LightType : int
{
    Directional = 0, Point = 1, Spot = 2
};

class LightsContainer
{
public:
    LightsContainer(const std::shared_ptr<OpenGLShader>& shader, uint32_t maxLights = 8);
    ~LightsContainer() = default;

    bool AddLight(const std::shared_ptr<SceneNode>& light, LightType lightType);
    void ClearLights();

private:
    static std::string GetLightTypeUniformName(uint32_t lightIndex);
    static std::string GetLightPositionUniformName(uint32_t lightIndex);
    static std::string GetLightDirectionUniformName(uint32_t lightIndex);
    static std::string GetLightColorUniformName(uint32_t lightIndex);
    static std::string GetLightFalloffUniformName(uint32_t lightIndex);
    
    
    void SetLightUniforms(uint32_t lightIndex) const;

    void SetDirectionalLightUniforms(uint32_t lightIndex, const std::shared_ptr<TransformComponent>& transformComponent, const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent) const;
    void SetPointLightUniforms(uint32_t lightIndex, const std::shared_ptr<TransformComponent>& transformComponent, const std::shared_ptr<PointLightComponent>& pointLightComponent) const;
    void SetSpotLightUniforms(uint32_t lightIndex, const std::shared_ptr<TransformComponent>& transformComponent, const std::shared_ptr<SpotLightComponent>& spotLightComponent) const;
    
    void SetLightTypeUniform(uint32_t lightIndex, LightType type) const;
    void SetLightPositionUniform(uint32_t lightIndex, const glm::vec3& position) const;
    void SetLightDirectionUniform(uint32_t lightIndex, const glm::vec3& direction) const;
    void SetLightColorUniform(uint32_t lightIndex, const glm::vec3& color) const;
    void SetLightFalloffUniform(uint32_t lightIndex, const glm::vec3& falloff) const;

    std::shared_ptr<OpenGLShader> m_shader;
    uint32_t m_maxLights;
    std::vector<std::pair<std::shared_ptr<SceneNode>, LightType>> m_lights;
};
