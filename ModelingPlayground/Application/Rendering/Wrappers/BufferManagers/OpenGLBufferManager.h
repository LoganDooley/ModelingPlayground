#pragma once
#include <memory>

class OpenGLTextureCache;
class OpenGLShaderStorageBlock;
class OpenGLBuffer;
class SceneHierarchy;

class OpenGLBufferManager
{
public:
    OpenGLBufferManager(std::shared_ptr<SceneHierarchy> sceneHierarchy,
                        std::shared_ptr<OpenGLTextureCache> textureCache,
                        std::shared_ptr<OpenGLShaderStorageBlock> modelMatrixBlock,
                        std::shared_ptr<OpenGLShaderStorageBlock> inverseTransposeModelMatrixBlock,
                        std::shared_ptr<OpenGLShaderStorageBlock> materialBlock);
    ~OpenGLBufferManager() = default;

    const std::shared_ptr<OpenGLBuffer>& GetModelMatrixBuffer() const;
    const std::shared_ptr<OpenGLBuffer>& GetInverseTransposeModelMatrixBuffer() const;
    const std::shared_ptr<OpenGLBuffer>& GetMaterialBuffer() const;

private:
    std::shared_ptr<SceneHierarchy> m_sceneHierarchy;
    std::shared_ptr<OpenGLTextureCache> m_textureCache;
    std::shared_ptr<OpenGLShaderStorageBlock> m_modelMatrixBlock;
    std::shared_ptr<OpenGLShaderStorageBlock> m_inverseTransposeModelMatrixBlock;
    std::shared_ptr<OpenGLShaderStorageBlock> m_materialBlock;

    std::shared_ptr<OpenGLBuffer> m_modelMatrixBuffer;
    std::shared_ptr<OpenGLBuffer> m_inverseTransposeModelMatrixBuffer;
    std::shared_ptr<OpenGLBuffer> m_materialBuffer;
};
