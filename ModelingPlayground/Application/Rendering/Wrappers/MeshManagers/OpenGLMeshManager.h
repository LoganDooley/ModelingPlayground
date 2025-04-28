#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "../../RenderPipeline/DrawCommand/OpenGLMultiDrawElementsCommand.h"

class PrimitiveManager;
class SceneHierarchy;
class OpenGLVertexArray;
class OpenGLBuffer;

struct OpenGLMeshData
{
    unsigned int m_vertexOffset;
    unsigned int m_indexOffset;
    unsigned int m_indexCount;
};

class OpenGLMeshManager
{
public:
    OpenGLMeshManager(std::shared_ptr<SceneHierarchy> sceneHierarchy,
                      std::shared_ptr<PrimitiveManager> primitiveManager);
    ~OpenGLMeshManager();

    DrawElementsIndirectCommand CreateDrawElementsIndirectCommand(const std::string& primitiveName) const;
    const std::shared_ptr<OpenGLVertexArray>& GetVertexArray() const;

private:
    void RegeneratePrimitiveBatch();

    std::shared_ptr<OpenGLBuffer> m_vbo;
    std::shared_ptr<OpenGLBuffer> m_ebo;
    std::shared_ptr<OpenGLVertexArray> m_vao;
    std::unordered_map<std::string, OpenGLMeshData> m_meshData;

    std::shared_ptr<SceneHierarchy> m_sceneHierarchy;
    std::shared_ptr<PrimitiveManager> m_primitiveManager;
};
