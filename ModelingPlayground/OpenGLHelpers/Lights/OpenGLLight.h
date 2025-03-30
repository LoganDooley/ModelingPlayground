#pragma once
#include <memory>
#include <string>
#include <glm/vec3.hpp>

class OpenGLRenderer;
class TransformComponent;
class OpenGLShader;
class SceneNode;
class ShadowMap;

enum LightType : int
{
	Directional = 0, Point = 1, Spot = 2, None = 3
};

class OpenGLLight
{
public:
	OpenGLLight(std::shared_ptr<OpenGLShader> defaultShader, std::shared_ptr<SceneNode> lightSceneNode,
	            unsigned int lightIndex);
	virtual ~OpenGLLight() = default;

	void SetLightIndex(unsigned int lightIndex);

	void SetShadowMapDirty();
	void TryUpdateShadowMap(OpenGLRenderer* openGLRenderer);

protected:
	virtual void UpdateShadowMap(OpenGLRenderer* openGLRenderer) = 0;

	// A batch function to set all uniforms. Prefer data bindings if possible
	virtual void SetAllUniforms() = 0;
	virtual void SetLightTypeUniform() const = 0;
	virtual void SetLightShadowMapHandleUniform() const = 0;
	void SetLightPositionUniform(const glm::vec3& position) const;
	void SetLightDirectionUniform(const glm::vec3& direction) const;
	void SetLightColorUniform(const glm::vec3& color) const;
	void SetLightFalloffUniform(const glm::vec3& falloff) const;

	std::shared_ptr<OpenGLShader> m_defaultShader;
	std::shared_ptr<TransformComponent> m_transformComponent;
	std::shared_ptr<ShadowMap> m_shadowMap;
	bool m_shadowMapIsDirty;
	unsigned int m_lightIndex;

	const int m_maxLights = 199;
	const int m_lightStructSize = 80;
	const int m_lightCountOffset = m_maxLights * m_lightStructSize;
	const std::string m_lightsBlockName = "LightsBlock";
};
