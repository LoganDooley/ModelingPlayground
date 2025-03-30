#include "OpenGLLight.h"

#include "../OpenGLShader.h"
#include "../../Scene/Object.h"
#include "../../Scene/Components/TransformComponent.h"
#include "../../Scene/SceneNode/SceneNode.h"

OpenGLLight::OpenGLLight(std::shared_ptr<OpenGLShader> defaultShader, std::shared_ptr<SceneNode> lightSceneNode,
                         unsigned int lightIndex):
	m_defaultShader(defaultShader),
	m_transformComponent(lightSceneNode->GetObject().GetFirstComponentOfType<TransformComponent>()),
	m_lightIndex(lightIndex)
{
}

void OpenGLLight::SetLightIndex(unsigned int lightIndex)
{
	m_lightIndex = lightIndex;
	SetAllUniforms();
}

void OpenGLLight::SetLightPositionUniform(const glm::vec3& position) const
{
	m_defaultShader->SetUniformBufferObjectSubData(m_lightsBlockName, m_lightIndex * m_lightStructSize + 32, position);
}

void OpenGLLight::SetLightDirectionUniform(const glm::vec3& direction) const
{
	m_defaultShader->SetUniformBufferObjectSubData(m_lightsBlockName, m_lightIndex * m_lightStructSize + 48, direction);
}

void OpenGLLight::SetLightColorUniform(const glm::vec3& color) const
{
	m_defaultShader->SetUniformBufferObjectSubData(m_lightsBlockName, m_lightIndex * m_lightStructSize + 16, color);
}

void OpenGLLight::SetLightFalloffUniform(const glm::vec3& falloff) const
{
	m_defaultShader->SetUniformBufferObjectSubData(m_lightsBlockName, m_lightIndex * m_lightStructSize + 64, falloff);
}
