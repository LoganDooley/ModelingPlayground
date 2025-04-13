#pragma once

#include "OpenGLLightContainer.h"
#include "../Scene/SceneHierarchy.h"

#include "../OpenGLHelpers/OpenGLTextureCache.h"
#include "../OpenGLHelpers/OpenGLPrimitiveManager.h"

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
	void ResetOpenGLPrimitiveManager(OpenGLPrimitiveManager* openGLPrimitiveManager);
	void ResetOpenGLTextureCache(OpenGLTextureCache* openGLTextureCache);

	void Render() const;

	void RenderUnidirectionalShadow(const glm::mat4& lightMatrix) const;
	void RenderOmnidirectionalShadow(const glm::vec3& lightPosition) const;
	const std::unique_ptr<OpenGLPrimitiveManager>& GetOpenGLPrimitiveManager() const;
	const std::unique_ptr<OpenGLTextureCache>& GetOpenGLTextureCache() const;

private:
	void ClearCameraFramebuffer() const;
	void DepthPrepass() const;
	void SetAmbientLightColor() const;
	void RenderScene() const;
	void RenderSceneHierarchy(const std::shared_ptr<OpenGLShader>& activeShader) const;

	void ProcessObject(const Object& object, std::shared_ptr<OpenGLShader> activeShader) const;
	void DrawMesh(const PrimitiveComponent& primitiveComponent, const TransformComponent& transformComponent,
	              const MaterialComponent& materialComponent, std::shared_ptr<OpenGLShader> activeShader) const;

	void OnSceneNodeAdded(const std::shared_ptr<SceneNode>& newSceneNode) const;

	std::shared_ptr<OpenGLShader> m_defaultShader;
	std::shared_ptr<OpenGLShader> m_depthShader;
	std::shared_ptr<OpenGLShader> m_omnidirectionalDepthShader;
	std::shared_ptr<SceneViewCamera> m_camera;
	std::shared_ptr<SceneHierarchy> m_sceneHierarchy;

	std::unique_ptr<OpenGLPrimitiveManager> m_openGLPrimitiveManager;
	std::unique_ptr<OpenGLLightContainer> m_openGLLightContainer;
	std::unique_ptr<OpenGLTextureCache> m_openGLTextureCache;
};
