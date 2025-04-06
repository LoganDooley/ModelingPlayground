#include "OpenGLSpotLight.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../OpenGLFramebuffer.h"
#include "../OpenGLShader.h"
#include "../../Scene/Object.h"
#include "../../Scene/Components/ComponentIncludes.h"
#include "../../Scene/SceneNode/SceneNode.h"
#include "../ShadowMaps/UnidirectionalLightShadowMap.h"

OpenGLSpotLight::OpenGLSpotLight(std::shared_ptr<OpenGLShader> defaultShader, std::shared_ptr<SceneNode> lightSceneNode,
                                 unsigned int lightIndex):
	OpenGLLight(defaultShader, lightSceneNode, lightIndex),
	m_spotLightComponent(lightSceneNode->GetObject().GetFirstComponentOfType<SpotLightComponent>())
{
	UpdateLightMatrix();

	m_spotLightComponent->GetLightColorDataBinding().Subscribe([this](const glm::vec3& lightColor, glm::vec3)
	{
		SetLightColorUniform(lightColor);
	});

	m_transformComponent->GetPositionDataBinding().Subscribe([this](const glm::vec3&, glm::vec3)
	{
		SetLightPositionUniform(m_transformComponent->GetWorldSpacePosition());
		UpdateLightMatrix();
		SetShadowMapDirty();
	});

	m_transformComponent->GetLocalXUnitVectorDataBinding().Subscribe(
		[this](const glm::vec3&, glm::vec3)
		{
			SetLightDirectionUniform(m_transformComponent->GetWorldSpaceXUnitVector());
			UpdateLightMatrix();
			SetShadowMapDirty();
		});

	m_transformComponent->GetParentCumulativeModelMatrixDataBinding().Subscribe(
		[this](const glm::mat4&, glm::mat4)
		{
			SetLightPositionUniform(m_transformComponent->GetWorldSpacePosition());
			SetLightDirectionUniform(m_transformComponent->GetWorldSpaceXUnitVector());
			UpdateLightMatrix();
			SetShadowMapDirty();
		});

	m_spotLightComponent->GetLightFalloffAnglesDataBinding().Subscribe(
		[this](const glm::vec2& lightFalloffAngles, glm::vec2)
		{
			SetLightFalloffUniform(glm::vec3(lightFalloffAngles, 0));
			UpdateLightMatrix();
			SetShadowMapDirty();
		});

	m_lightMatrix.Subscribe([this](const glm::mat4& lightMatrix, glm::mat4)
	{
		SetLightMatrixUniform(lightMatrix);
		SetShadowMapDirty();
	});

	m_spotLightComponent->SetOnDebugCaptureShadowMap([this](GLuint* targetTexture, int& width, int& height)
	{
		m_shadowMap->DebugCaptureShadowMap(targetTexture, width, height);
	});

	m_shadowMap = std::make_shared<UnidirectionalLightShadowMap>();
	m_shadowMap->GetFramebuffer()->GetTexture(GL_DEPTH_ATTACHMENT)->MakeTextureResident();

	OpenGLSpotLight::SetAllUniforms();
}

void OpenGLSpotLight::UpdateShadowMap(const OpenGLRenderer* openGLRenderer)
{
	m_shadowMap->CaptureShadowMap(m_lightMatrix.GetData(), openGLRenderer);
}

void OpenGLSpotLight::SetAllUniforms()
{
	SetLightTypeUniform();
	SetLightShadowMapHandleUniform();
	SetLightColorUniform(m_spotLightComponent->GetLightColor());
	SetLightPositionUniform(m_transformComponent->GetWorldSpacePosition());
	SetLightDirectionUniform(m_transformComponent->GetWorldSpaceXUnitVector());
	SetLightFalloffUniform(glm::vec3(m_spotLightComponent->GetLightFalloffAngles(), 0));
	SetHasShadowMapUniform(true);
	SetLightMatrixUniform(m_lightMatrix.GetData());
}

void OpenGLSpotLight::SetLightTypeUniform() const
{
	m_defaultShader->SetUniformBufferObjectSubData(m_lightsBlockName, m_lightIndex * m_lightStructSize + 8, Spot);
}

void OpenGLSpotLight::SetLightShadowMapHandleUniform() const
{
	m_defaultShader->SetUniformBufferObjectSubData(m_lightsBlockName, m_lightIndex * m_lightStructSize,
	                                               m_shadowMap->GetFramebuffer()->GetTexture(GL_DEPTH_ATTACHMENT)->
	                                                            GetTextureHandle());
}

void OpenGLSpotLight::UpdateLightMatrix()
{
	glm::mat4 lightProjection = glm::perspective(2 * glm::radians(m_spotLightComponent->GetLightFalloffAngles().y), 1.f,
	                                             0.01f, 10.f);
	glm::vec3 lightPosition = m_transformComponent->GetWorldSpacePosition();
	glm::mat4 lightView = lookAt(lightPosition,
	                             lightPosition + m_transformComponent->
	                             GetWorldSpaceXUnitVector(), glm::vec3(0, 1, 0));

	m_lightMatrix.SetAndNotify(lightProjection * lightView);
}
