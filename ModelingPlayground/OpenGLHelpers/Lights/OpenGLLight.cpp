#include "OpenGLLight.h"

#include "../OpenGLShader.h"
#include "../../Scene/Object.h"
#include "../../Scene/Components/TransformComponent.h"
#include "../../Scene/SceneNode/SceneNode.h"
#include "../ShadowMaps/ShadowMap.h"

OpenGLLight::OpenGLLight(std::shared_ptr<OpenGLShader> defaultShader, std::shared_ptr<SceneNode> lightSceneNode,
                         unsigned int lightIndex):
	m_defaultShader(defaultShader),
	m_transformComponent(lightSceneNode->GetObject().GetFirstComponentOfType<TransformComponent>()),
	m_shadowMapIsDirty(true),
	m_lightIndex(lightIndex)
{
}

void OpenGLLight::SetLightIndex(unsigned int lightIndex)
{
	m_lightIndex = lightIndex;
	SetAllUniforms();
}

void OpenGLLight::SetShadowMapDirty()
{
	m_shadowMapIsDirty = true;
}

void OpenGLLight::TryUpdateShadowMap(const OpenGLRenderer* openGLRenderer)
{
	if (m_shadowMapIsDirty)
	{
		UpdateShadowMap(openGLRenderer);
		m_shadowMapIsDirty = false;
	}
}

void OpenGLLight::DebugCaptureShadowMap(GLuint* targetTexture, int& width, int& height)
{
	m_shadowMap->DebugCaptureShadowMap(targetTexture, width, height);
}

void OpenGLLight::SetLightPositionUniform(const glm::vec3& position) const
{
	m_defaultShader->SetUniformBufferObjectSubData(m_lightsBlockName, m_lightIndex * m_lightStructSize + 24, position);
}

void OpenGLLight::SetLightDirectionUniform(const glm::vec3& direction) const
{
	m_defaultShader->SetUniformBufferObjectSubData(m_lightsBlockName, m_lightIndex * m_lightStructSize + 36, direction);
}

void OpenGLLight::SetLightColorUniform(const glm::vec3& color) const
{
	m_defaultShader->SetUniformBufferObjectSubData(m_lightsBlockName, m_lightIndex * m_lightStructSize + 12, color);
}

void OpenGLLight::SetLightFalloffUniform(const glm::vec3& falloff) const
{
	m_defaultShader->SetUniformBufferObjectSubData(m_lightsBlockName, m_lightIndex * m_lightStructSize + 48, falloff);
}

void OpenGLLight::SetHasShadowMapUniform(bool hasShadowMap) const
{
	m_defaultShader->SetUniformBufferObjectSubData(m_lightsBlockName, m_lightIndex * m_lightStructSize + 60,
	                                               hasShadowMap);
}

void OpenGLLight::SetLightMatrixUniform(const glm::mat4& matrix) const
{
	m_defaultShader->SetUniformBufferObjectSubData(m_lightsBlockName, m_lightIndex * m_lightStructSize + 64, matrix);
}
