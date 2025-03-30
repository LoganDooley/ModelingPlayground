#pragma once

#include "OpenGLLight.h"

class DirectionalLightComponent;

class OpenGLDirectionalLight : public OpenGLLight
{
public:
	OpenGLDirectionalLight(std::shared_ptr<OpenGLShader> defaultShader, std::shared_ptr<SceneNode> lightSceneNode,
	                       unsigned int lightIndex);

protected:
	void SetAllUniforms() override;
	void SetLightTypeUniform() const override;

	std::shared_ptr<DirectionalLightComponent> m_directionalLightComponent;
};
