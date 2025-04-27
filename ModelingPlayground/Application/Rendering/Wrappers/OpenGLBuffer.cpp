#include "OpenGLBuffer.h"

OpenGLBuffer::~OpenGLBuffer()
{
    if (m_handle != 0)
    {
        glDeleteBuffers(1, &m_handle);
    }
}

void OpenGLBuffer::BindBase(GLuint index) const
{
    glBindBufferBase(m_target, index, m_handle);
}

void OpenGLBuffer::Bind() const
{
    glBindBuffer(m_target, m_handle);
}

void OpenGLBuffer::Unbind() const
{
    glBindBuffer(m_target, 0);
}
