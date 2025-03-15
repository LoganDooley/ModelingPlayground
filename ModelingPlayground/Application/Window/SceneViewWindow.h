#pragma once

#include <memory>
#include <glm/fwd.hpp>

#include "SceneViewCamera.h"
#include "Window.h"
#include "../../OpenGLWrappers/OpenGLShader.h"
#include "../../Utils/OpenGLPrimitiveDrawer.h"

class MaterialComponent;
class TransformComponent;
class PrimitiveComponent;
class Object;
class SceneNode;
class ObjectData;
class Scene;

class SceneViewWindow : public Window
{
public:
	SceneViewWindow(const std::shared_ptr<Scene>& scene, const std::shared_ptr<InputManager>& inputManager);

	void Render() override;
	void Update(double seconds) override;

	inline static const std::string Name = "Scene View";

private:
	void InitializeOpenGLObjects();

	void DrawScene() const;

	void ProcessObject(const Object& object, glm::mat4& cumulativeModelMatrix) const;
	void DrawMesh(const PrimitiveComponent& primitiveComponent, const TransformComponent& transformComponent, const MaterialComponent& materialComponent, glm::mat4& cumulativeModelMatrix) const;

	std::unique_ptr<OpenGLPrimitiveDrawer> m_openGLPrimitiveDrawer;

	std::unique_ptr<SceneViewCamera> m_camera;

	std::unique_ptr<OpenGLShader> m_defaultShader;

	std::shared_ptr<Scene> m_scene;
};

