#pragma once

#include "RasterRenderer.h"
#include "../RenderingManager.h"
#include "../../../Scene/SceneHierarchy.h"
#include "../../OpenGLHelpers/OpenGLLightContainer.h"

#include "../../OpenGLHelpers/OpenGLPrimitiveManager.h"
#include "../../OpenGLHelpers/OpenGLTextureCache.h"

class MaterialComponent;
class PrimitiveComponent;
class TransformComponent;
class SceneViewCamera;

class OpenGLRenderer : public RasterRenderer
{
public:
    OpenGLRenderer();
    ~OpenGLRenderer() override;

    /* Renderer Public Methods */

    void Initialize() override;
    void SetCamera(std::shared_ptr<SceneViewCamera> camera) override;
    void SetSceneHierarchy(std::shared_ptr<SceneHierarchy> sceneHierarchy) override;
    void Render() const override;
    void AddPrimitive(const std::string& filePath) const override;
    void AddTexture(const std::string& filePath) const override;
    void IncrementTextureUsage(const std::string& filePath, void* user) const override;
    void DecrementTextureUsage(const std::string& filePath, void* user) const override;
    const std::unique_ptr<OpenGLTextureCache>& GetTextureCache() const override;
    std::vector<std::string> GetPrimitiveNames() const override;
    void SelectObjectAtPixel(int x, int y) const override;

    /* Raster Renderer Public Methods */

    void RenderUnidirectionalShadow(const glm::mat4& lightMatrix) const override;
    void RenderOmnidirectionalShadow(const glm::vec3& lightPosition) const override;

    /* OpenGLRenderer Public Methods */

    void ResetOpenGLPrimitiveManager(OpenGLPrimitiveManager* openGLPrimitiveManager);
    void ResetOpenGLTextureCache(OpenGLTextureCache* openGLTextureCache);

protected:
    /* Renderer Protected Methods */

    void OnSceneNodeAdded(const std::shared_ptr<SceneNode>& newSceneNode) const override;

    /* Raster Renderer Protected Methods */

    void ClearCameraFramebuffer() const override;
    void DepthPrepass() const override;
    void SetAmbientLightColor() const override;

    /* OpenGLRenderer Protected Methods */

    void RenderScene() const;
    void RenderSceneHierarchy(const std::shared_ptr<OpenGLShader>& activeShader) const;

    void ProcessObject(const Object& object, std::shared_ptr<OpenGLShader> activeShader) const;
    void DrawMesh(const PrimitiveComponent& primitiveComponent, const TransformComponent& transformComponent,
                  const MaterialComponent& materialComponent, std::shared_ptr<OpenGLShader> activeShader) const;

    std::shared_ptr<OpenGLShader> m_defaultShader;
    std::shared_ptr<OpenGLShader> m_depthShader;
    std::shared_ptr<OpenGLShader> m_omnidirectionalDepthShader;

    std::unique_ptr<OpenGLPrimitiveManager> m_openGLPrimitiveManager;
    std::unique_ptr<OpenGLLightContainer> m_openGLLightContainer;
    std::unique_ptr<OpenGLTextureCache> m_openGLTextureCache;
};
