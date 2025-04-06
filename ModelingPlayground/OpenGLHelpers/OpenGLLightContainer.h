#pragma once

#include <memory>
#include <vector>

#include "OpenGLShader.h"
#include "../Scene/SceneHierarchy.h"
#include "../Scene/Components/DirectionalLightComponent.h"
#include "../Scene/Components/PointLightComponent.h"
#include "../Scene/Components/SpotLightComponent.h"
#include "../Scene/Components/TransformComponent.h"
#include "../Scene/SceneNode/SceneNode.h"
#include "Lights/OpenGLLight.h"

class OpenGLLight;

class OpenGLLightContainer
{
public:
	OpenGLLightContainer();
	~OpenGLLightContainer() = default;

	void Initialize(std::shared_ptr<OpenGLShader> defaultShader);
	void SetSceneHierarchy(const std::shared_ptr<SceneHierarchy>& sceneHierarchy);

	bool TryAddLight(const std::shared_ptr<SceneNode>& lightSceneNode);
	void ClearLights();

	void SetAllShadowMapsDirty() const;
	void UpdateDirtyShadowMaps(const OpenGLRenderer* openGLRenderer) const;

private:
	bool AddLightInternal(const std::shared_ptr<SceneNode>& lightSceneNode, LightType lightType);
	static LightType GetLightType(const std::shared_ptr<SceneNode>& lightSceneNode);

	std::shared_ptr<OpenGLShader> m_defaultShader;
	std::vector<std::shared_ptr<OpenGLLight>> m_lights;

	const int m_maxLights = 50;
	const int m_lightStructSize = 128;
	const int m_lightCountOffset = m_maxLights * m_lightStructSize;
	const std::string m_lightsBlockName = "LightsBlock";
};
