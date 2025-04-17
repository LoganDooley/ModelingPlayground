#pragma once

#include <memory>

#include "Window.h"

class RenderingManager;
class MaterialComponent;
class TransformComponent;
class PrimitiveComponent;
class Object;
class SceneNode;
class ObjectData;
class SceneHierarchy;
class SceneViewCamera;
class InputManager;

class SceneViewWindow : public Window
{
public:
	SceneViewWindow(std::shared_ptr<RenderingManager> renderingManager, std::shared_ptr<InputManager> inputManager);

	void Render() override;
	void Update(double seconds) override;

	inline static const std::string Name = "Scene View";

private:
	std::shared_ptr<SceneViewCamera> m_camera;
	std::shared_ptr<RenderingManager> m_renderingManager;
	std::shared_ptr<InputManager> m_inputManager;
};
