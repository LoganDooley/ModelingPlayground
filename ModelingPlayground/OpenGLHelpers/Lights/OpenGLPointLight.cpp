#include "OpenGLPointLight.h"

#include "../OpenGLShader.h"
#include "../../Scene/Object.h"
#include "../../Scene/Components/ComponentIncludes.h"
#include "../../Scene/SceneNode/SceneNode.h"

OpenGLPointLight::OpenGLPointLight(std::shared_ptr<OpenGLShader> defaultShader,
                                   std::shared_ptr<SceneNode> lightSceneNode, unsigned int lightIndex):
	OpenGLLight(defaultShader, lightSceneNode, lightIndex),
	m_pointLightComponent(lightSceneNode->GetObject().GetFirstComponentOfType<PointLightComponent>())
{
	m_pointLightComponent->GetLightColorDataBinding().Subscribe([this](const glm::vec3& lightColor, glm::vec3)
	{
		SetLightColorUniform(lightColor);
	});

	m_transformComponent->GetPositionDataBinding().Subscribe([this](const glm::vec3&, glm::vec3)
	{
		SetLightPositionUniform(m_transformComponent->GetWorldSpacePosition());
		SetShadowMapDirty();
	});

	m_transformComponent->GetParentCumulativeModelMatrixDataBinding().Subscribe(
		[this](const glm::mat4&, glm::mat4)
		{
			SetLightPositionUniform(m_transformComponent->GetWorldSpacePosition());
			SetShadowMapDirty();
		});

	m_pointLightComponent->GetFalloffDataBinding().Subscribe([this](const glm::vec3& falloff, glm::vec3)
	{
		SetLightFalloffUniform(falloff);
	});

	OpenGLPointLight::SetAllUniforms();
}

void OpenGLPointLight::UpdateShadowMap(OpenGLRenderer* openGLRenderer)
{
}

void OpenGLPointLight::SetAllUniforms()
{
	SetLightTypeUniform();
	SetLightColorUniform(m_pointLightComponent->GetLightColor());
	SetLightPositionUniform(m_transformComponent->GetWorldSpacePosition());
	SetLightFalloffUniform(m_pointLightComponent->GetFalloff());
}

void OpenGLPointLight::SetLightTypeUniform() const
{
	m_defaultShader->SetUniformBufferObjectSubData(m_lightsBlockName, m_lightIndex * m_lightStructSize, Point);
}

void OpenGLPointLight::SetLightShadowMapHandleUniform() const
{
}
