#include "OpenGLPrimitive.h"

#include <iostream>

OpenGLVertex::OpenGLVertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords, bool hasTexCoords):
    m_hasTexCoords(hasTexCoords)
{
    m_vertexData.push_back(position.x);
    m_vertexData.push_back(position.y);
    m_vertexData.push_back(position.z);

    m_vertexData.push_back(normal.x);
    m_vertexData.push_back(normal.y);
    m_vertexData.push_back(normal.z);
    
    if (m_hasTexCoords)
    {
        m_vertexData.push_back(texCoords.x);
        m_vertexData.push_back(texCoords.y);
    }
}

std::vector<float> OpenGLVertex::GetVertexData() const
{
    return m_vertexData;
}

bool OpenGLVertex::HasTexCoords() const
{
    return m_hasTexCoords;
}

OpenGLPrimitive::OpenGLPrimitive(const std::vector<OpenGLVertex>& vertices, const std::vector<int>& indices):
    m_vbo(-1),
    m_vao(-1),
    m_ebo(-1),
    m_vertexCount(vertices.size()),
    m_indexCount(indices.size())
{
    if (vertices.empty())
    {
        std::cout<<"OpenGLPrimitive: Created with no vertices!\n";
        return;
    }

    std::vector<float> vertexData = vertices[0].GetVertexData();
    size_t vertexMemorySize = vertexData.size() * sizeof(float);
    for (int i = 1; i < m_vertexCount; i++)
    {
        std::vector<float> individualData = vertices[i].GetVertexData();
        vertexData.insert(vertexData.end(), individualData.begin(), individualData.end());
    }

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
	
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertexCount * vertexMemorySize, vertexData.data(), GL_STATIC_DRAW);

    GLsizei vertexStride = vertices[0].HasTexCoords() ? 8 * sizeof(float) : 6 * sizeof(float);
	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexStride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    if (vertices[0].HasTexCoords())
    {
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexStride, (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    if (m_indexCount > 0)
    {
        glGenBuffers(1, &m_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(int), indices.data(), GL_STATIC_DRAW);
    }
}

OpenGLPrimitive::OpenGLPrimitive(const std::vector<float>& vertices, GLsizei vertexCount, bool hasTexCoords, const std::vector<int>& indices):
    m_vbo(0),
    m_vao(0),
    m_ebo(0),
    m_vertexCount(vertexCount),
    m_indexCount(indices.size())
{
    if (vertices.empty())
    {
        std::cout<<"OpenGLPrimitive: Created with no vertices!\n";
        return;
    }
    
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
	
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    GLsizei vertexStride = hasTexCoords ? 8 * sizeof(float) : 6 * sizeof(float);
	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexStride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexStride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    if (hasTexCoords)
    {
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexStride, (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    if (m_indexCount > 0)
    {
        glGenBuffers(1, &m_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(int), indices.data(), GL_STATIC_DRAW);
    }
}

void OpenGLPrimitive::Draw() const
{
    if (m_indexCount > 0)
    {
        // Uses index buffer
        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    }
    else
    {
        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
    }
}
