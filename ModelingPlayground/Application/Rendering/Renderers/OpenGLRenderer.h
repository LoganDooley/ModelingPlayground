#pragma once

#include "RasterRenderer.h"
#include "../RenderingManager.h"
#include "../../../Scene/SceneHierarchy.h"
#include "../../OpenGLHelpers/OpenGLLightContainer.h"

#include "../../OpenGLHelpers/OpenGLTextureCache.h"

class OpenGLBufferManager;
class OpenGLMeshManager;
class OpenGLMultiDrawElementsCommand;
class OpenGLVertexArray;
class OpenGLBuffer;
class MaterialComponent;
class PrimitiveComponent;
class TransformComponent;
class SceneViewCamera;

class OpenGLRenderer : public RasterRenderer
{
public:
    OpenGLRenderer(std::shared_ptr<PrimitiveManager> primitiveManager, std::shared_ptr<SceneHierarchy> sceneHierarchy);
    ~OpenGLRenderer() override = default;

    /* Renderer Public Methods */

    void SetCamera(std::shared_ptr<SceneViewCamera> camera) override;
    void AddTexture(const std::string& filePath) const override;
    void IncrementTextureUsage(const std::string& filePath, void* user) const override;
    void DecrementTextureUsage(const std::string& filePath, void* user) const override;
    const std::shared_ptr<OpenGLTextureCache>& GetTextureCache() const override;

    /* Raster Renderer Public Methods */

    void RenderUnidirectionalShadow(const glm::mat4& lightMatrix) const override;
    void RenderOmnidirectionalShadow(const glm::vec3& lightPosition) const override;

    /* OpenGLRenderer Public Methods */

    void ResetOpenGLTextureCache(OpenGLTextureCache* openGLTextureCache);

private:
    /* Renderer Protected Methods */

    /* Raster Renderer Protected Methods */

    void ClearCameraFramebuffer() const override;
    void DepthPrepass() const override;
    void SetAmbientLightColor() const override;

    /* OpenGLRenderer Protected Methods */
    void RebuildMultiDrawCommand();

    std::shared_ptr<OpenGLShader> m_defaultShader;
    std::shared_ptr<OpenGLShader> m_depthShader;
    std::shared_ptr<OpenGLShader> m_omnidirectionalDepthShader;

    std::unique_ptr<OpenGLLightContainer> m_openGLLightContainer;
    std::shared_ptr<OpenGLTextureCache> m_openGLTextureCache;

    std::shared_ptr<OpenGLMeshManager> m_meshManager;
    std::shared_ptr<OpenGLBufferManager> m_bufferManager;
    std::shared_ptr<OpenGLMultiDrawElementsCommand> m_sceneMultiDrawElementsCommand;
};
