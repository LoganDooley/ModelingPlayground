#include "OpenGLLight.h"

#include "../Shader/OpenGLShader.h"
#include "../../Scene/Object.h"
#include "../../Scene/Components/TransformComponent.h"
#include "../../Scene/SceneNode/SceneNode.h"
#include "../ShadowMaps/ShadowMap.h"

OpenGLLight::OpenGLLight(std::shared_ptr<OpenGLShader> defaultShader, std::shared_ptr<SceneNode> lightSceneNode,
                         unsigned int lightIndex):
    m_defaultShader(defaultShader),
    m_transformComponent(lightSceneNode->GetObject().GetFirstComponentOfType<TransformComponent>()),
    m_shadowMapIsDirty(true),
    m_lightIndex(lightIndex),
    m_uniformNameBase("lights[" + std::to_string(lightIndex) + "].")
{
}

void OpenGLLight::SetLightIndex(unsigned int lightIndex)
{
    m_lightIndex = lightIndex;
    m_uniformNameBase = "lights[" + std::to_string(lightIndex) + "].";
    SetAllUniforms();
}

void OpenGLLight::SetShadowMapDirty()
{
    m_shadowMapIsDirty = true;
}

void OpenGLLight::TryUpdateShadowMap(const OpenGLRenderer* openGLRenderer)
{
    if (m_shadowMapIsDirty)
    {
        UpdateShadowMap(openGLRenderer);
        m_shadowMapIsDirty = false;
    }
}

void OpenGLLight::SetLightPositionUniform(const glm::vec3& position) const
{
    m_defaultShader->SetUniformBlockUniformValue(m_lightsBlockName, m_uniformNameBase + "positionX", position.x);
    m_defaultShader->SetUniformBlockUniformValue(m_lightsBlockName, m_uniformNameBase + "positionY", position.y);
    m_defaultShader->SetUniformBlockUniformValue(m_lightsBlockName, m_uniformNameBase + "positionZ", position.z);
}

void OpenGLLight::SetLightDirectionUniform(const glm::vec3& direction) const
{
    m_defaultShader->SetUniformBlockUniformValue(m_lightsBlockName, m_uniformNameBase + "directionX", direction.x);
    m_defaultShader->SetUniformBlockUniformValue(m_lightsBlockName, m_uniformNameBase + "directionY", direction.y);
    m_defaultShader->SetUniformBlockUniformValue(m_lightsBlockName, m_uniformNameBase + "directionZ", direction.z);
}

void OpenGLLight::SetLightColorUniform(const glm::vec3& color) const
{
    m_defaultShader->SetUniformBlockUniformValue(m_lightsBlockName, m_uniformNameBase + "colorR", color.r);
    m_defaultShader->SetUniformBlockUniformValue(m_lightsBlockName, m_uniformNameBase + "colorG", color.g);
    m_defaultShader->SetUniformBlockUniformValue(m_lightsBlockName, m_uniformNameBase + "colorB", color.b);
}

void OpenGLLight::SetLightFalloffUniform(const glm::vec3& falloff) const
{
    m_defaultShader->SetUniformBlockUniformValue(m_lightsBlockName, m_uniformNameBase + "falloffA", falloff.x);
    m_defaultShader->SetUniformBlockUniformValue(m_lightsBlockName, m_uniformNameBase + "falloffB", falloff.y);
    m_defaultShader->SetUniformBlockUniformValue(m_lightsBlockName, m_uniformNameBase + "falloffC", falloff.z);
}

void OpenGLLight::SetHasShadowMapUniform(bool hasShadowMap) const
{
    m_defaultShader->SetUniformBlockUniformValue(m_lightsBlockName, m_uniformNameBase + "hasShadowMap",
                                                 hasShadowMap);
}

void OpenGLLight::SetLightMatrixUniform(const glm::mat4& matrix) const
{
    m_defaultShader->SetUniformBlockUniformValue(m_lightsBlockName, m_uniformNameBase + "lightMatrix", matrix);
}
