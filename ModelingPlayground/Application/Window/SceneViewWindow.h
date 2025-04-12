#pragma once

#include <memory>

#include "Window.h"

class MaterialComponent;
class TransformComponent;
class PrimitiveComponent;
class Object;
class SceneNode;
class ObjectData;
class SceneHierarchy;
class SceneViewCamera;
class InputManager;
class OpenGLRenderer;

class SceneViewWindow : public Window
{
public:
	SceneViewWindow(std::shared_ptr<OpenGLRenderer> openGLRenderer, std::shared_ptr<InputManager> inputManager);

	void Render() override;
	void Update(double seconds) override;

	inline static const std::string Name = "Scene View";

private:
	std::shared_ptr<SceneViewCamera> m_camera;
	std::shared_ptr<OpenGLRenderer> m_openGLRenderer;
	std::shared_ptr<InputManager> m_inputManager;
};
