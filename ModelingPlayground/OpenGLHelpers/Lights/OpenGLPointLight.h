#pragma once

#include "OpenGLLight.h"

class PointLightComponent;

class OpenGLPointLight : public OpenGLLight
{
public:
	OpenGLPointLight(std::shared_ptr<OpenGLShader> defaultShader, std::shared_ptr<SceneNode> lightSceneNode,
	                 unsigned int lightIndex);

protected:
	void UpdateShadowMap(OpenGLRenderer* openGLRenderer) override;

	void SetAllUniforms() override;
	void SetLightTypeUniform() const override;
	void SetLightShadowMapHandleUniform() const override;

	std::shared_ptr<PointLightComponent> m_pointLightComponent;
};
