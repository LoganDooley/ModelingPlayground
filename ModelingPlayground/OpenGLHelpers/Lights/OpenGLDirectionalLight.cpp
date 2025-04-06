#include "OpenGLDirectionalLight.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../OpenGLFramebuffer.h"
#include "../OpenGLShader.h"
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

	m_directionalLightComponent->GetLightColorDataBinding().Subscribe([this](const glm::vec3& lightColor, glm::vec3)
	{
		SetLightColorUniform(lightColor);
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
			SetLightDirectionUniform(m_transformComponent->GetWorldSpaceXUnitVector());
			UpdateLightMatrix();
			SetShadowMapDirty();
		});

	m_lightMatrix.Subscribe([this](const glm::mat4& lightMatrix, glm::mat4)
	{
		SetLightMatrixUniform(lightMatrix);
		SetShadowMapDirty();
	});

	m_directionalLightComponent->SetOnDebugCaptureShadowMap([this](GLuint* texture, int& width, int& height)
	{
		m_shadowMap->DebugCaptureShadowMap(texture, width, height);
	});

	m_shadowMap = std::make_shared<UnidirectionalLightShadowMap>();
	m_shadowMap->GetFramebuffer()->GetTexture(GL_DEPTH_ATTACHMENT)->MakeTextureResident();

	OpenGLDirectionalLight::SetAllUniforms();
}

void OpenGLDirectionalLight::UpdateShadowMap(OpenGLRenderer* openGLRenderer)
{
	m_shadowMap->CaptureShadowMap(m_lightMatrix.GetData(), openGLRenderer);
}

void OpenGLDirectionalLight::SetAllUniforms()
{
	SetLightTypeUniform();
	SetLightShadowMapHandleUniform();
	SetLightColorUniform(m_directionalLightComponent->GetLightColor());
	SetLightDirectionUniform(m_transformComponent->GetWorldSpaceXUnitVector());
	SetHasShadowMapUniform(true);
	SetLightMatrixUniform(m_lightMatrix.GetData());
}

void OpenGLDirectionalLight::SetLightTypeUniform() const
{
	m_defaultShader->
		SetUniformBufferObjectSubData(m_lightsBlockName, m_lightIndex * m_lightStructSize + 8, Directional);
}

void OpenGLDirectionalLight::SetLightShadowMapHandleUniform() const
{
	m_defaultShader->SetUniformBufferObjectSubData(m_lightsBlockName, m_lightIndex * m_lightStructSize,
	                                               m_shadowMap->GetFramebuffer()->GetTexture(GL_DEPTH_ATTACHMENT)->
	                                                            GetTextureHandle());
}

void OpenGLDirectionalLight::UpdateLightMatrix()
{
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.f);
	glm::vec3 lightPosition = m_transformComponent->GetWorldSpaceXUnitVector() * -50.f;
	glm::mat4 lightView = lookAt(lightPosition, glm::vec3(0), glm::vec3(0, 1, 0));
	m_lightMatrix.SetAndNotify(lightProjection * lightView);
}
