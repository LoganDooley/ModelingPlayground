#pragma once
#include "ShadowMap.h"

class OpenGLFramebuffer;

class DirectionalLightShadowMap : public ShadowMap
{
public:
	DirectionalLightShadowMap(unsigned int resolution = 1024);
	~DirectionalLightShadowMap() override;

	void CaptureShadowMap(std::shared_ptr<SceneViewCamera> camera,
	                      std::shared_ptr<OpenGLRenderer> openGLRenderer) override;

private:
	std::shared_ptr<OpenGLFramebuffer> m_shadowMapFramebuffer;
};
