#pragma once

#include <glm/mat4x4.hpp>

#include "ShadowMap.h"

class OmnidirectionalLightShadowMap : public ShadowMap
{
public:
	OmnidirectionalLightShadowMap(unsigned int resolution = 1024);
	~OmnidirectionalLightShadowMap() override;

	void CaptureShadowMap(const glm::vec3& lightPosition, const OpenGLRenderer* openGLRenderer);

private:
	unsigned int m_resolution;
};
