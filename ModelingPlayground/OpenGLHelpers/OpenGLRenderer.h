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

	void ResetAllLightTransforms() const;

	void RenderSceneHierarchy() const;
	void RenderSceneHierarchyForShadowMap() const;
	std::shared_ptr<OpenGLPrimitiveManager> GetOpenGLPrimitiveManager() const;

private:
	void ProcessObject(const Object& object) const;
	void DrawMesh(const PrimitiveComponent& primitiveComponent, const TransformComponent& transformComponent,
	              const MaterialComponent& materialComponent) const;

	void CacheSceneHierarchyLights() const;
	void OnSceneNodeAdded(const std::shared_ptr<SceneNode>& newSceneNode) const;

	std::shared_ptr<OpenGLShader> m_defaultShader;
	std::shared_ptr<SceneViewCamera> m_camera;
	std::shared_ptr<SceneHierarchy> m_sceneHierarchy;
	std::shared_ptr<OpenGLPrimitiveManager> m_openGLPrimitiveManager;
	std::unique_ptr<OpenGLLightContainer> m_openGLLightContainer;
};
