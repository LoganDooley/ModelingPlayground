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

	std::shared_ptr<OpenGLFramebuffer> GetFramebuffer() { return m_shadowMapFramebuffer; }

protected:
	std::shared_ptr<OpenGLFramebuffer> m_shadowMapFramebuffer;
};
