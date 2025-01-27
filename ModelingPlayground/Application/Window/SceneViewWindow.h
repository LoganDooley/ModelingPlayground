#pragma once

#include <memory>
#include <glm/fwd.hpp>

#include "Window.h"
#include "glad/glad.h"

class TransformComponent;
class MeshComponent;
class Object;
class SceneNode;
class ObjectData;
class Scene;

class SceneViewWindow : public Window
{
public:
	SceneViewWindow(const std::shared_ptr<Scene>& scene);

	void Render() override;

	inline static const std::string Name = "Scene View";

private:
	void InitializeOpenGLObjects();

	void DrawScene() const;

	void ProcessObject(const std::shared_ptr<Object>& object, glm::mat4& cumulativeModelMatrix) const;
	void DrawMesh(const std::shared_ptr<MeshComponent>& meshComponent, const std::shared_ptr<TransformComponent>& transformComponent, glm::mat4& cumulativeModelMatrix) const;

	GLuint m_triangleVBO;
	GLuint m_triangleVAO;

	GLuint m_framebuffer;
	GLuint m_framebufferTexture;
	GLuint m_framebufferRenderbuffer;
	
	GLuint m_defaultShader;
	GLint m_modelMatrixLocation;

	std::shared_ptr<Scene> m_scene;
};

