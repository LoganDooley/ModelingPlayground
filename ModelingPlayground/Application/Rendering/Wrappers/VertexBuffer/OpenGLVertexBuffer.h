#pragma once
#include <vector>

#include "../../../../OpenGLHelpers/OpenGLTypeMap.h"
#include "glad/glad.h"

class OpenGLVertexBuffer
{
public:
    template <typename T>
    OpenGLVertexBuffer(const std::vector<T>& vertices, GLenum usageMode):
        m_vbo(0),
        m_usageMode(usageMode),
        m_type(OpenGLTypeMap::ToGLEnum<T>())
    {
        glGenBuffers(1, &m_vbo);
        Bind();
        glBufferData(GL_ARRAY_BUFFER, sizeof(T) * vertices.size(), vertices.data(), m_usageMode);
        Unbind();
    }

    ~OpenGLVertexBuffer();

    void Bind() const;
    static void Unbind();

private:
    GLuint m_vbo;
    GLenum m_usageMode;
    GLenum m_type;
};
