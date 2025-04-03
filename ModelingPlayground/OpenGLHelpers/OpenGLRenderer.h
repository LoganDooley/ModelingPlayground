#pragma once

#include "OpenGLLightContainer.h"
#include "OpenGLPrimitiveManager.h"
#include "../Scene/SceneHierarchy.h"

class MaterialComponent;
class PrimitiveComponent;
class TransformComponent;
class SceneViewCamera;

class OpenGLRenderer
{
public:
	OpenGLRenderer();
	~OpenGLRenderer() = default;

	void Initialize();

	void SetCamera(std::shared_ptr<SceneViewCamera> camera);
	void SetSceneHierarchy(std::shared_ptr<SceneHierarchy> sceneHierarchy);

	void TryUpdateShadowMaps();
	void RenderScene() const;
	void RenderUnidirectionalShadow(const glm::mat4& lightMatrix) const;
	std::shared_ptr<OpenGLPrimitiveManager> GetOpenGLPrimitiveManager() const;

private:
	void RenderSceneHierarchy(std::shared_ptr<OpenGLShader> activeShader) const;

	void ProcessObject(const Object& object, std::shared_ptr<OpenGLShader> activeShader) const;
	void DrawMesh(const PrimitiveComponent& primitiveComponent, const TransformComponent& transformComponent,
	              const MaterialComponent& materialComponent, std::shared_ptr<OpenGLShader> activeShader) const;

	void OnSceneNodeAdded(const std::shared_ptr<SceneNode>& newSceneNode) const;

	std::shared_ptr<OpenGLShader> m_defaultShader;
	std::shared_ptr<OpenGLShader> m_unidirectionalShadowsShader;
	std::shared_ptr<SceneViewCamera> m_camera;
	std::shared_ptr<SceneHierarchy> m_sceneHierarchy;
	std::shared_ptr<OpenGLPrimitiveManager> m_openGLPrimitiveManager;
	std::unique_ptr<OpenGLLightContainer> m_openGLLightContainer;
};
