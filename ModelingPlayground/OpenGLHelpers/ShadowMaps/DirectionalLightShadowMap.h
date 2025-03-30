#pragma once
#include <glm/mat4x4.hpp>

#include "ShadowMap.h"

class OpenGLFramebuffer;

class DirectionalLightShadowMap : public ShadowMap
{
public:
	DirectionalLightShadowMap(unsigned int resolution = 1024);
	~DirectionalLightShadowMap() override;

	void CaptureShadowMap(const glm::mat4& lightMatrix, OpenGLRenderer* openGLRenderer) override;

private:
	unsigned int m_resolution;
};
