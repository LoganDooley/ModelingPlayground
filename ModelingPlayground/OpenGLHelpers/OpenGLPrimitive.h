#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "glad/glad.h"

class OpenGLVertex
{
public:
    OpenGLVertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords, bool hasTexCoords);
    
    std::vector<float> GetVertexData() const;
    bool HasTexCoords() const;
    
private:
    std::vector<float> m_vertexData;
    bool m_hasTexCoords;
};

// Wrapper class used to store object geometry. Including Position, Normals, and UVs
class OpenGLPrimitive
{
public:
    OpenGLPrimitive(const std::vector<OpenGLVertex>& vertices, const std::vector<int>& indices = {});
    OpenGLPrimitive(const std::vector<float>& vertices, GLsizei vertexCount, bool hasTexCoords, const std::vector<int>& indices = {});

    void Draw() const;
    
private:
    GLuint m_vbo;
    GLuint m_vao;
    GLuint m_ebo;
    GLsizei m_vertexCount;
    GLsizei m_indexCount;
};
