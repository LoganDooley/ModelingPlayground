#pragma once

#include "OpenGLLight.h"

class SpotLightComponent;

class OpenGLSpotLight : public OpenGLLight
{
public:
	OpenGLSpotLight(std::shared_ptr<OpenGLShader> defaultShader, std::shared_ptr<SceneNode> lightSceneNode,
	                unsigned int lightIndex);

protected:
	void SetAllUniforms() override;
	void SetLightTypeUniform() const override;

	std::shared_ptr<SpotLightComponent> m_spotLightComponent;
};
