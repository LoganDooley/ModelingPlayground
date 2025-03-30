#pragma once
#include <memory>

class OpenGLRenderer;
class SceneViewCamera;

class ShadowMap
{
public:
	virtual ~ShadowMap() = default;

	virtual void CaptureShadowMap(std::shared_ptr<SceneViewCamera> camera,
	                              std::shared_ptr<OpenGLRenderer> openGLRenderer) = 0;
};
