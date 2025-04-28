#include "OpenGLBuffer.h"

#include <glm/gtc/type_ptr.hpp>

OpenGLBuffer::OpenGLBuffer(unsigned int sizeInBytes, GLenum target, GLenum usageMode):
    m_handle(0),
    m_target(target),
    m_usageMode(usageMode),
    m_dataType(GL_NONE)
{
    glGenBuffers(1, &m_handle);
    Bind();
    glBufferData(target, sizeInBytes, nullptr, usageMode);
    Unbind();
}

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

void OpenGLBuffer::SetSubData(unsigned int offset, const glm::mat4& value) const
{
    Bind();
    glBufferSubData(m_target, offset, sizeof(glm::mat4),
                    glm::value_ptr(value));
    Unbind();
}
