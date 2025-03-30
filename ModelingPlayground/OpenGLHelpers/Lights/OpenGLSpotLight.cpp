#include "OpenGLSpotLight.h"

#include "../OpenGLShader.h"
#include "../../Scene/Object.h"
#include "../../Scene/Components/ComponentIncludes.h"
#include "../../Scene/SceneNode/SceneNode.h"

OpenGLSpotLight::OpenGLSpotLight(std::shared_ptr<OpenGLShader> defaultShader, std::shared_ptr<SceneNode> lightSceneNode,
                                 unsigned int lightIndex):
	OpenGLLight(defaultShader, lightSceneNode, lightIndex),
	m_spotLightComponent(lightSceneNode->GetObject().GetFirstComponentOfType<SpotLightComponent>())
{
	m_spotLightComponent->GetLightColorDataBinding().Subscribe([this](const glm::vec3& lightColor, glm::vec3)
	{
		SetLightColorUniform(lightColor);
	});

	m_transformComponent->GetPositionDataBinding().Subscribe([this](const glm::vec3&, glm::vec3)
	{
		SetLightPositionUniform(m_transformComponent->GetWorldSpacePosition());
		SetShadowMapDirty();
	});

	m_transformComponent->GetLocalXUnitVectorDataBinding().Subscribe(
		[this](const glm::vec3&, glm::vec3)
		{
			SetLightDirectionUniform(m_transformComponent->GetWorldSpaceXUnitVector());
			SetShadowMapDirty();
		});

	m_transformComponent->GetParentCumulativeModelMatrixDataBinding().Subscribe(
		[this](const glm::mat4&, glm::mat4)
		{
			SetLightPositionUniform(m_transformComponent->GetWorldSpacePosition());
			SetLightDirectionUniform(m_transformComponent->GetWorldSpaceXUnitVector());
			SetShadowMapDirty();
		});

	m_spotLightComponent->GetLightFalloffAnglesDataBinding().Subscribe(
		[this](const glm::vec2& lightFalloffAngles, glm::vec2)
		{
			SetLightFalloffUniform(glm::vec3(lightFalloffAngles, 0));
			SetShadowMapDirty();
		});

	OpenGLSpotLight::SetAllUniforms();
}

void OpenGLSpotLight::UpdateShadowMap(OpenGLRenderer* openGLRenderer)
{
}

void OpenGLSpotLight::SetAllUniforms()
{
	SetLightTypeUniform();
	SetLightColorUniform(m_spotLightComponent->GetLightColor());
	SetLightPositionUniform(m_transformComponent->GetWorldSpacePosition());
	SetLightDirectionUniform(m_transformComponent->GetWorldSpaceXUnitVector());
	SetLightFalloffUniform(glm::vec3(m_spotLightComponent->GetLightFalloffAngles(), 0));
}

void OpenGLSpotLight::SetLightTypeUniform() const
{
	m_defaultShader->SetUniformBufferObjectSubData(m_lightsBlockName, m_lightIndex * m_lightStructSize, Spot);
}

void OpenGLSpotLight::SetLightShadowMapHandleUniform() const
{
}
