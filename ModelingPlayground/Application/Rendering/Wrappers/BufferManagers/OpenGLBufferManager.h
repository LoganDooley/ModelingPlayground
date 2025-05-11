#pragma once
#include <memory>

class OpenGLTextureCache;
class OpenGLShaderBlock;
class OpenGLBuffer;
class SceneHierarchy;

class OpenGLBufferManager
{
public:
    OpenGLBufferManager(std::shared_ptr<SceneHierarchy> sceneHierarchy,
                        std::shared_ptr<OpenGLTextureCache> textureCache,
                        std::shared_ptr<OpenGLShaderBlock> modelMatrixBlock,
                        std::shared_ptr<OpenGLShaderBlock> inverseTransposeModelMatrixBlock,
                        std::shared_ptr<OpenGLShaderBlock> materialBlock,
                        std::shared_ptr<OpenGLShaderBlock> lightBlock);
    ~OpenGLBufferManager() = default;

    const std::shared_ptr<OpenGLBuffer>& GetModelMatrixBuffer() const;
    const std::shared_ptr<OpenGLBuffer>& GetInverseTransposeModelMatrixBuffer() const;
    const std::shared_ptr<OpenGLBuffer>& GetMaterialBuffer() const;
    const std::shared_ptr<OpenGLBuffer>& GetLightBuffer() const;

private:
    void RegenerateBuffers();

    std::shared_ptr<SceneHierarchy> m_sceneHierarchy;
    std::shared_ptr<OpenGLTextureCache> m_textureCache;
    std::shared_ptr<OpenGLShaderBlock> m_modelMatrixBlock;
    std::shared_ptr<OpenGLShaderBlock> m_inverseTransposeModelMatrixBlock;
    std::shared_ptr<OpenGLShaderBlock> m_materialBlock;
    std::shared_ptr<OpenGLShaderBlock> m_lightBlock;

    std::shared_ptr<OpenGLBuffer> m_modelMatrixBuffer;
    std::shared_ptr<OpenGLBuffer> m_inverseTransposeModelMatrixBuffer;
    std::shared_ptr<OpenGLBuffer> m_materialBuffer;
    std::shared_ptr<OpenGLBuffer> m_lightBuffer;
};
