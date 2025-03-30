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

	m_transformComponent->GetPositionDataBinding().Subscribe([this](const glm::vec3& localPosition, glm::vec3)
	{
		auto worldPosition = glm::vec3(
			m_transformComponent->GetParentCumulativeModelMatrix() * glm::vec4(localPosition, 1));
		SetLightPositionUniform(worldPosition);
	});

	m_transformComponent->GetParentCumulativeModelMatrixDataBinding().Subscribe(
		[this](const glm::mat4& parentCumulativeModelMatrix, glm::mat4)
		{
			auto worldPosition = glm::vec3(
				parentCumulativeModelMatrix * glm::vec4(m_transformComponent->GetPosition(), 1));
			SetLightPositionUniform(worldPosition);
		});

	m_pointLightComponent->GetFalloffDataBinding().Subscribe([this](const glm::vec3& falloff, glm::vec3)
	{
		SetLightFalloffUniform(falloff);
	});

	OpenGLPointLight::SetAllUniforms();
}

void OpenGLPointLight::SetAllUniforms()
{
	SetLightTypeUniform();
	SetLightColorUniform(m_pointLightComponent->GetLightColor());
	auto localPosition = glm::vec4(m_transformComponent->GetPosition(), 1.0f);
	auto worldPosition = glm::vec3(m_transformComponent->GetParentCumulativeModelMatrix() * localPosition);
	SetLightPositionUniform(worldPosition);
	SetLightFalloffUniform(m_pointLightComponent->GetFalloff());
}

void OpenGLPointLight::SetLightTypeUniform() const
{
	m_defaultShader->SetUniformBufferObjectSubData(m_lightsBlockName, m_lightIndex * m_lightStructSize, Point);
}
