#include "OpenGLDirectionalLight.h"

#include "../OpenGLShader.h"
#include "../../Scene/Object.h"
#include "../../Scene/Components/ComponentIncludes.h"
#include "../../Scene/SceneNode/SceneNode.h"

OpenGLDirectionalLight::OpenGLDirectionalLight(std::shared_ptr<OpenGLShader> defaultShader,
                                               std::shared_ptr<SceneNode> lightSceneNode, unsigned int lightIndex):
	OpenGLLight(defaultShader, lightSceneNode, lightIndex),
	m_directionalLightComponent(lightSceneNode->GetObject().GetFirstComponentOfType<DirectionalLightComponent>())
{
	m_directionalLightComponent->GetLightColorDataBinding().Subscribe([this](const glm::vec3& lightColor, glm::vec3)
	{
		SetLightColorUniform(lightColor);
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
			auto worldXUnitVector = glm::vec3(
				parentCumulativeModelMatrix * glm::vec4(m_transformComponent->GetLocalXUnitVector(), 0));
			SetLightDirectionUniform(worldXUnitVector);
		});

	OpenGLDirectionalLight::SetAllUniforms();
}

void OpenGLDirectionalLight::SetAllUniforms()
{
	SetLightTypeUniform();
	SetLightColorUniform(m_directionalLightComponent->GetLightColor());
	auto localXUnitVector = glm::vec4(m_transformComponent->GetLocalXUnitVector(), 0);
	auto worldXUnitVector = glm::vec3(m_transformComponent->GetParentCumulativeModelMatrix() * localXUnitVector);
	SetLightDirectionUniform(worldXUnitVector);
}

void OpenGLDirectionalLight::SetLightTypeUniform() const
{
	m_defaultShader->SetUniformBufferObjectSubData(m_lightsBlockName, m_lightIndex * m_lightStructSize, Directional);
}
