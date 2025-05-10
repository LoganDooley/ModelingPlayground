#include "OpenGLBuffer.h"

#include <glm/gtc/type_ptr.hpp>

OpenGLBuffer::OpenGLBuffer(unsigned int sizeInBytes, GLenum target, GLenum usageMode):
    m_handle(0),
    m_target(target),
    m_usageMode(usageMode),
    m_dataType(GL_NONE),
    m_dataSize(sizeInBytes)
{
    glGenBuffers(1, &m_handle);
    Bind();
    glBufferData(target, sizeInBytes, nullptr, usageMode);
    Unbind();
}

OpenGLBuffer::~OpenGLBuffer()
{
    if (m_shouldDeleteOpenGLObjectsWhenDestroyed && m_handle != 0)
    {
        glDeleteBuffers(1, &m_handle);
    }
}

OpenGLBuffer::OpenGLBuffer(OpenGLBuffer&& other)
    noexcept
{
    m_handle = std::move(other.m_handle);
    m_target = std::move(other.m_target);
    m_usageMode = std::move(other.m_usageMode);
    m_dataType = std::move(other.m_dataType);
    m_dataSize = std::move(other.m_dataSize);
    other.m_shouldDeleteOpenGLObjectsWhenDestroyed = false;
}

OpenGLBuffer& OpenGLBuffer::operator=(OpenGLBuffer&& other)
    noexcept
{
    if (m_handle != 0 && m_handle != other.m_handle)
    {
        glDeleteBuffers(1, &m_handle);
    }

    m_handle = std::move(other.m_handle);
    m_target = std::move(other.m_target);
    m_usageMode = std::move(other.m_usageMode);
    m_dataType = std::move(other.m_dataType);
    m_dataSize = std::move(other.m_dataSize);
    other.m_shouldDeleteOpenGLObjectsWhenDestroyed = false;
    return *this;
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
