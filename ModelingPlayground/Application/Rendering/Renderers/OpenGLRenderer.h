#pragma once

#include "Renderer.h"
#include "../RenderingManager.h"
#include "../../../Scene/SceneHierarchy.h"
#include "../../OpenGLHelpers/OpenGLLightContainer.h"

#include "../../OpenGLHelpers/OpenGLPrimitiveManager.h"
#include "../../OpenGLHelpers/OpenGLTextureCache.h"

class MaterialComponent;
class PrimitiveComponent;
class TransformComponent;
class SceneViewCamera;

class OpenGLRenderer : public Renderer
{
public:
	OpenGLRenderer();
	~OpenGLRenderer() override = default;

	void Initialize() override;

	void DrawSettings() override;

	void SetCamera(std::shared_ptr<SceneViewCamera> camera) override;

	void SetSceneHierarchy(std::shared_ptr<SceneHierarchy> sceneHierarchy) override;
	void ResetOpenGLPrimitiveManager(OpenGLPrimitiveManager* openGLPrimitiveManager);
	void ResetOpenGLTextureCache(OpenGLTextureCache* openGLTextureCache);

	void Render() const override;

	void RenderUnidirectionalShadow(const glm::mat4& lightMatrix) const;
	void RenderOmnidirectionalShadow(const glm::vec3& lightPosition) const;

	void AddPrimitive(const std::string& filePath) const override;
	void AddTexture(const std::string& filePath) const override;

	void IncrementTextureUsage(const std::string& filePath, void* user) const override;
	void DecrementTextureUsage(const std::string& filePath, void* user) const override;

	const std::unique_ptr<OpenGLTextureCache>& GetTextureCache() const override;
	std::vector<std::string> GetPrimitiveNames() const override;

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

	RasterPipeline m_rasterPipeline;
	GlobalIllumination m_globalIllumination;
	AmbientOcclusion m_ambientOcclusion;
	AntiAliasing m_antiAliasing;
};
