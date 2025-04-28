#include "OpenGLMeshManager.h"

#include <iostream>

#include "../../Primitives/PrimitiveManager.h"
#include "../../Primitives/Primitive.h"
#include "../OpenGLBuffer.h"
#include "../VertexArray/OpenGLVertexArray.h"

#include <vector>

#include "glad/glad.h"

OpenGLMeshManager::OpenGLMeshManager(std::shared_ptr<SceneHierarchy> sceneHierarchy,
                                     std::shared_ptr<PrimitiveManager> primitiveManager):
    m_sceneHierarchy(sceneHierarchy),
    m_primitiveManager(primitiveManager)
{
    m_primitiveManager->SubscribeToOnPrimitiveAdded([this]
    {
        RegeneratePrimitiveBatch();
    });
}

OpenGLMeshManager::~OpenGLMeshManager()
{
}

DrawElementsIndirectCommand OpenGLMeshManager::CreateDrawElementsIndirectCommand(const std::string& primitiveName) const
{
    if (!m_meshData.contains(primitiveName))
    {
        std::cerr << "OpenGLMeshManager|CreateDrawElementsIndirectCommand: Failed to create indirect draw command for "
            << primitiveName << "!\n";
        return {};
    }

    OpenGLMeshData meshData = m_meshData.at(primitiveName);

    return DrawElementsIndirectCommand{
        .count = meshData.m_indexCount,
        .instanceCount = 1,
        .firstIndex = meshData.m_indexOffset,
        .baseVertex = meshData.m_vertexOffset,
        .baseInstance = 0,
    };
}

const std::shared_ptr<OpenGLVertexArray>& OpenGLMeshManager::GetVertexArray() const
{
    return m_vao;
}

void OpenGLMeshManager::RegeneratePrimitiveBatch()
{
    m_meshData.clear();
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int vertexOffset = 0;
    unsigned int indexOffset = 0;
    for (const auto& primitive : m_primitiveManager->GetAllPrimitives())
    {
        vertices.insert(vertices.end(), primitive.second->GetVertices().begin(), primitive.second->GetVertices().end());
        indices.insert(indices.end(), primitive.second->GetIndices().begin(), primitive.second->GetIndices().end());
        unsigned int vertexCount = primitive.second->GetVertices().size() / 8;
        unsigned int indexCount = primitive.second->GetIndices().size();
        m_meshData[primitive.first] = {
            .m_vertexOffset = vertexOffset,
            .m_indexOffset = indexOffset,
            .m_indexCount = indexCount
        };
        vertexOffset += vertexCount;
        indexOffset += indexCount;
    }

    m_vbo = std::make_shared<OpenGLBuffer>(vertices, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    m_vao = std::make_shared<OpenGLVertexArray>(m_vbo, std::vector({
                                                    VertexAttribute::PositionF3, VertexAttribute::NormalF3,
                                                    VertexAttribute::UVF2
                                                }));
    m_vao->Bind();
    m_ebo = std::make_shared<OpenGLBuffer>(indices, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    OpenGLVertexArray::Unbind();
}
