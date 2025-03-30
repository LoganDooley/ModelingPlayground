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

	m_transformComponent->GetPositionDataBinding().Subscribe([this](const glm::vec3& localPosition, glm::vec3)
	{
		auto worldPosition = glm::vec3(
			m_transformComponent->GetParentCumulativeModelMatrix() * glm::vec4(localPosition, 1));
		SetLightPositionUniform(worldPosition);
	});

	m_transformComponent->GetLocalXUnitVectorDataBinding().Subscribe(
		[this](const glm::vec3& localXUnitVector, glm::vec3)
		{
			auto worldXUnitVector = glm::vec3(
				m_transformComponent->GetParentCumulativeModelMatrix() * glm::vec4(localXUnitVector, 0));
			SetLightDirectionUniform(worldXUnitVector);
		});

	m_transformComponent->GetParentCumulativeModelMatrixDataBinding().Subscribe(
		[this](const glm::mat4& parentCumulativeModelMatrix, glm::mat4)
		{
			auto worldPosition = glm::vec3(
				parentCumulativeModelMatrix * glm::vec4(m_transformComponent->GetPosition(), 1));
			SetLightPositionUniform(worldPosition);
			auto worldXUnitVector = glm::vec3(
				parentCumulativeModelMatrix * glm::vec4(m_transformComponent->GetLocalXUnitVector(), 0));
			SetLightDirectionUniform(worldXUnitVector);
		});

	m_spotLightComponent->GetLightFalloffAnglesDataBinding().Subscribe(
		[this](const glm::vec2& lightFalloffAngles, glm::vec2)
		{
			SetLightFalloffUniform(glm::vec3(lightFalloffAngles, 0));
		});

	OpenGLSpotLight::SetAllUniforms();
}

void OpenGLSpotLight::SetAllUniforms()
{
	SetLightTypeUniform();
	SetLightColorUniform(m_spotLightComponent->GetLightColor());
	glm::mat4 parentCumulativeModelMatrix = m_transformComponent->GetParentCumulativeModelMatrix();
	auto localPosition = glm::vec4(m_transformComponent->GetPosition(), 1);
	auto worldPosition = glm::vec3(parentCumulativeModelMatrix * localPosition);
	SetLightPositionUniform(worldPosition);
	auto localXUnitVector = glm::vec4(m_transformComponent->GetLocalXUnitVector(), 0);
	auto worldXUnitVector = glm::vec3(parentCumulativeModelMatrix * localXUnitVector);
	SetLightDirectionUniform(worldXUnitVector);
	SetLightFalloffUniform(glm::vec3(m_spotLightComponent->GetLightFalloffAngles(), 0));
}

void OpenGLSpotLight::SetLightTypeUniform() const
{
	m_defaultShader->SetUniformBufferObjectSubData(m_lightsBlockName, m_lightIndex * m_lightStructSize, Spot);
}
