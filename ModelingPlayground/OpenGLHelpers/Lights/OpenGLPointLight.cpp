#include "OpenGLPointLight.h"

#include "../OpenGLFramebuffer.h"
#include "../Shader/OpenGLShader.h"
#include "../../Scene/Object.h"
#include "../../Scene/Components/ComponentIncludes.h"
#include "../../Scene/SceneNode/SceneNode.h"
#include "../ShadowMaps/OmnidirectionalLightShadowMap.h"

OpenGLPointLight::OpenGLPointLight(std::shared_ptr<OpenGLShader> defaultShader,
                                   std::shared_ptr<SceneNode> lightSceneNode, unsigned int lightIndex):
    OpenGLLight(defaultShader, lightSceneNode, lightIndex),
    m_pointLightComponent(lightSceneNode->GetObject().GetFirstComponentOfType<PointLightComponent>())
{
    m_pointLightComponent->GetLightColorDataBinding().Subscribe(this, [this](const glm::vec3& lightColor, glm::vec3)
    {
        SetLightColorUniform(lightColor);
    });

    m_transformComponent->GetPositionDataBinding().Subscribe(this, [this](const glm::vec3&, glm::vec3)
    {
        SetLightPositionUniform(m_transformComponent->GetWorldSpacePosition());
        SetShadowMapDirty();
    });

    m_transformComponent->GetCumulativeModelMatrixDataBinding().Subscribe(this,
                                                                          [this](const glm::mat4&, glm::mat4)
                                                                          {
                                                                              SetLightPositionUniform(
                                                                                  m_transformComponent->
                                                                                  GetWorldSpacePosition());
                                                                              SetShadowMapDirty();
                                                                          });

    m_pointLightComponent->GetFalloffDataBinding().Subscribe(this, [this](const glm::vec3& falloff, glm::vec3)
    {
        SetLightFalloffUniform(falloff);
    });

    m_shadowMap = std::make_shared<OmnidirectionalLightShadowMap>();
    m_shadowMap->GetFramebuffer()->GetTexture(GL_DEPTH_ATTACHMENT)->MakeTextureResident();

    OpenGLPointLight::SetAllUniforms();
}

void OpenGLPointLight::UpdateShadowMap(const OpenGLRenderer* openGLRenderer)
{
    m_shadowMap->CaptureShadowMap(m_transformComponent->GetWorldSpacePosition(), openGLRenderer);
}

void OpenGLPointLight::SetAllUniforms()
{
    SetLightTypeUniform();
    SetLightShadowMapHandleUniform();
    SetLightColorUniform(m_pointLightComponent->GetLightColor());
    SetLightPositionUniform(m_transformComponent->GetWorldSpacePosition());
    SetLightFalloffUniform(m_pointLightComponent->GetFalloff());
    SetHasShadowMapUniform(true);
}

void OpenGLPointLight::SetLightTypeUniform() const
{
    //m_defaultShader->SetUniformBlockUniformValue(m_lightsBlockName, m_uniformNameBase + "type", Point);
}

void OpenGLPointLight::SetLightShadowMapHandleUniform() const
{
    /*m_defaultShader->SetUniformBlockUniformValue(m_lightsBlockName, m_uniformNameBase + "cubeShadowMap",
                                                 m_shadowMap->GetFramebuffer()->GetTexture(GL_DEPTH_ATTACHMENT)->
                                                              GetTextureHandle());*/
}
