#pragma once

#include "OpenGLLight.h"
#include "../../Utils/DataBinding.h"
#include "glm/mat4x4.hpp"

class DirectionalLightComponent;

class OpenGLDirectionalLight : public OpenGLLight
{
public:
	OpenGLDirectionalLight(std::shared_ptr<OpenGLShader> defaultShader, std::shared_ptr<SceneNode> lightSceneNode,
	                       unsigned int lightIndex);

protected:
	void UpdateShadowMap(OpenGLRenderer* openGLRenderer) override;

	void SetAllUniforms() override;
	void SetLightTypeUniform() const override;
	void SetLightShadowMapHandleUniform() const override;
	void UpdateLightMatrix();

	std::shared_ptr<DirectionalLightComponent> m_directionalLightComponent;
	DataBinding<glm::mat4> m_lightMatrix;
};
