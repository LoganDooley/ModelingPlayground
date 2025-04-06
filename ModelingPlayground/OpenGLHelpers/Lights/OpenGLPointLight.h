#pragma once

#include "OpenGLLight.h"

class OmnidirectionalLightShadowMap;
class PointLightComponent;

class OpenGLPointLight : public OpenGLLight
{
public:
	OpenGLPointLight(std::shared_ptr<OpenGLShader> defaultShader, std::shared_ptr<SceneNode> lightSceneNode,
	                 unsigned int lightIndex);

protected:
	void UpdateShadowMap(const OpenGLRenderer* openGLRenderer) override;

	void SetAllUniforms() override;
	void SetLightTypeUniform() const override;
	void SetLightShadowMapHandleUniform() const override;

	std::shared_ptr<PointLightComponent> m_pointLightComponent;
	std::shared_ptr<OmnidirectionalLightShadowMap> m_shadowMap;
};
