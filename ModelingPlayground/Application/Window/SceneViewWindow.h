#pragma once

#include <memory>
#include <glm/fwd.hpp>

#include "SceneViewCamera.h"
#include "Window.h"
#include "glad/glad.h"

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
	void DrawMesh(const PrimitiveComponent& primitiveComponent, const TransformComponent& transformComponent, glm::mat4& cumulativeModelMatrix) const;

	GLuint m_triangleVBO;
	GLuint m_triangleVAO;

	std::unique_ptr<SceneViewCamera> m_camera;
	
	GLuint m_defaultShader;
	GLint m_modelMatrixLocation;
	GLint m_cameraMatrixLocation;

	std::shared_ptr<Scene> m_scene;
};

