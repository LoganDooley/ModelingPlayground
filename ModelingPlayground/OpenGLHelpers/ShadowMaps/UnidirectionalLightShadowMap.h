#pragma once

#include <glm/mat4x4.hpp>

#include "ShadowMap.h"

class UnidirectionalLightShadowMap : public ShadowMap
{
public:
	UnidirectionalLightShadowMap(unsigned int resolution = 1024);
	~UnidirectionalLightShadowMap() override;

	void CaptureShadowMap(const glm::mat4& lightMatrix, const OpenGLRenderer* openGLRenderer);

private:
	unsigned int m_resolution;
};
