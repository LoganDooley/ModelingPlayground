#pragma once
#include <memory>

class OpenGLFramebuffer;
class OpenGLRenderer;
class SceneViewCamera;

class ShadowMap
{
public:
	virtual ~ShadowMap() = default;

	virtual void CaptureShadowMap(const glm::mat4& lightMatrix, OpenGLRenderer* openGLRenderer) = 0;
	std::shared_ptr<OpenGLFramebuffer> GetFramebuffer() { return m_shadowMapFramebuffer; }

protected:
	std::shared_ptr<OpenGLFramebuffer> m_shadowMapFramebuffer;
};
