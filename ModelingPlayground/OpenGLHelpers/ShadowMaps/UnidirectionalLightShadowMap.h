#pragma once
#include <string>
#include <glm/mat4x4.hpp>

#include "ShadowMap.h"

class UnidirectionalLightShadowMap : public ShadowMap
{
public:
	UnidirectionalLightShadowMap(unsigned int resolution = 1024);
	~UnidirectionalLightShadowMap() override;

	void CaptureShadowMap(const glm::mat4& lightMatrix, OpenGLRenderer* openGLRenderer) override;

	void DebugCaptureShadowMap(GLuint* targetTexture, int& width, int& height) override;

private:
	unsigned int m_resolution;
};
