#include "OpenGLIndexBuffer.h"

OpenGLIndexBuffer::OpenGLIndexBuffer(const std::vector<unsigned int>& indices, GLenum usageMode):
    m_ebo(0),
    m_usageMode(usageMode)
{
    glGenBuffers(1, &m_ebo);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), m_usageMode);
    Unbind();
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
    if (m_ebo != 0)
    {
        glDeleteBuffers(1, &m_ebo);
    }
}

void OpenGLIndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
}

void OpenGLIndexBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
