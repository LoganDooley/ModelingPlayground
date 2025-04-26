#include "OpenGLVertexBuffer.h"

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    if (m_vbo != 0)
    {
        glDeleteBuffers(1, &m_vbo);
    }
}

void OpenGLVertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
}

void OpenGLVertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
