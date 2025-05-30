﻿#include "OpenGLDirectionalLight.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../OpenGLFramebuffer.h"
#include "../Shader/OpenGLShader.h"
#include "../../Scene/Object.h"
#include "../../Scene/Components/ComponentIncludes.h"
#include "../../Scene/SceneNode/SceneNode.h"
#include "../ShadowMaps/UnidirectionalLightShadowMap.h"

OpenGLDirectionalLight::OpenGLDirectionalLight(std::shared_ptr<OpenGLShader> defaultShader,
                                               std::shared_ptr<SceneNode> lightSceneNode, unsigned int lightIndex):
    OpenGLLight(defaultShader, lightSceneNode, lightIndex),
    m_directionalLightComponent(lightSceneNode->GetObject().GetFirstComponentOfType<DirectionalLightComponent>())
{
    UpdateLightMatrix();

    m_directionalLightComponent->GetLightColorDataBinding().Subscribe(
        this, [this](const glm::vec3& lightColor, glm::vec3)
        {
            SetLightColorUniform(lightColor);
        });

    m_transformComponent->GetWorldSpaceXUnitVectorDataBinding().Subscribe(this,
                                                                          [this](
                                                                          const glm::vec3& newWorldSpaceXUnitVector,
                                                                          glm::vec3)
                                                                          {
                                                                              SetLightDirectionUniform(
                                                                                  newWorldSpaceXUnitVector);
                                                                              UpdateLightMatrix();
                                                                              SetShadowMapDirty();
                                                                          });


    m_lightMatrix.Subscribe(this, [this](const glm::mat4& lightMatrix, glm::mat4)
    {
        SetLightMatrixUniform(lightMatrix);
        SetShadowMapDirty();
    });

    m_shadowMap = std::make_shared<UnidirectionalLightShadowMap>();
    m_shadowMap->GetFramebuffer()->GetTexture(GL_DEPTH_ATTACHMENT)->MakeTextureResident();

    OpenGLDirectionalLight::SetAllUniforms();
}

void OpenGLDirectionalLight::UpdateShadowMap(const OpenGLRenderer* openGLRenderer)
{
    m_shadowMap->CaptureShadowMap(m_lightMatrix.GetData(), openGLRenderer);
}

void OpenGLDirectionalLight::SetAllUniforms()
{
    SetLightTypeUniform();
    SetLightShadowMapHandleUniform();
    SetLightColorUniform(m_directionalLightComponent->GetLightColor());
    SetLightDirectionUniform(m_transformComponent->GetWorldSpaceXUnitVectorDataBinding().GetData());
    SetHasShadowMapUniform(true);
    SetLightMatrixUniform(m_lightMatrix.GetData());
}

void OpenGLDirectionalLight::SetLightTypeUniform() const
{
    // tell opengl renderer to set uniform buffer
    //m_defaultShader->SetUniformBlockUniformValue(m_lightsBlockName, m_uniformNameBase + "type", Directional);
}

void OpenGLDirectionalLight::SetLightShadowMapHandleUniform() const
{
    /*m_defaultShader->SetUniformBlockUniformValue(m_lightsBlockName, m_uniformNameBase + "shadowMap",
                                                 m_shadowMap->GetFramebuffer()->GetTexture(GL_DEPTH_ATTACHMENT)->
                                                              GetTextureHandle());*/
}

void OpenGLDirectionalLight::UpdateLightMatrix()
{
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.f);
    glm::vec3 lightPosition = m_transformComponent->GetWorldSpaceXUnitVectorDataBinding().GetData() * -50.f;
    glm::mat4 lightView = lookAt(lightPosition, glm::vec3(0), glm::vec3(0, 1, 0));
    m_lightMatrix.SetAndNotify(lightProjection * lightView);
}
