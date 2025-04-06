#pragma once
#include <memory>

#include "glad/glad.h"

class OpenGLFramebuffer;
class OpenGLRenderer;
class SceneViewCamera;

class ShadowMap
{
public:
	virtual ~ShadowMap() = default;

	virtual void CaptureShadowMap(const glm::mat4& lightMatrix, OpenGLRenderer* openGLRenderer) = 0;
	std::shared_ptr<OpenGLFramebuffer> GetFramebuffer() { return m_shadowMapFramebuffer; }

	virtual void DebugCaptureShadowMap(GLuint* targetTexture, int& width, int& height) = 0;

protected:
	std::shared_ptr<OpenGLFramebuffer> m_shadowMapFramebuffer;
};
