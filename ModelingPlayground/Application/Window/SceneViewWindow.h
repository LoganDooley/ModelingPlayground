#pragma once

#include <memory>
#include <glm/fwd.hpp>

#include "SceneViewCamera.h"
#include "Window.h"
#include "../../OpenGLHelpers/OpenGLRenderer.h"

class MaterialComponent;
class TransformComponent;
class PrimitiveComponent;
class Object;
class SceneNode;
class ObjectData;
class SceneHierarchy;

class SceneViewWindow : public Window
{
public:
	SceneViewWindow(const std::shared_ptr<OpenGLRenderer>& openGLRenderer, const std::shared_ptr<InputManager>& inputManager);

	void Render() override;
	void Update(double seconds) override;

	inline static const std::string Name = "Scene View";

private:
	std::shared_ptr<SceneViewCamera> m_camera;
	std::shared_ptr<OpenGLRenderer> m_openGLRenderer;
};

