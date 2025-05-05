#pragma once
#include <vector>

#include "../../../OpenGLHelpers/OpenGLTypeMap.h"
#include "glad/glad.h"
#include "glm/mat4x4.hpp"

class OpenGLBuffer
{
public:
    template <typename T>
    OpenGLBuffer(const std::vector<T>& initialData, GLenum target, GLenum usageMode):
        m_handle(0),
        m_target(target),
        m_usageMode(usageMode),
        m_dataType(OpenGLTypeMap::ToGLEnum<T>())
    {
        glGenBuffers(1, &m_handle);

        SetData<T>(initialData);
    }

    OpenGLBuffer(unsigned int sizeInBytes, GLenum target, GLenum usageMode);

    ~OpenGLBuffer();

    OpenGLBuffer(OpenGLBuffer&& other) noexcept;
    OpenGLBuffer& operator=(OpenGLBuffer&& other) noexcept;

    void BindBase(GLuint index) const;

    void Bind() const;
    void Unbind() const;

    template <typename T>
    void SetSubData(unsigned int offset, const T& value) const
    {
        Bind();
        glBufferSubData(m_target, offset, sizeof(T),
                        &value);
        Unbind();
    }

    void SetSubData(unsigned int offset, const glm::mat4& value) const;

private:
    template <typename T>
    void SetData(const std::vector<T>& data) const
    {
        Bind();
        glBufferData(m_target, sizeof(T) * data.size(), data.data(), m_usageMode);
        Unbind();
    }

    GLuint m_handle;
    GLenum m_target;
    GLenum m_usageMode;
    GLenum m_dataType;
    bool m_shouldDeleteOpenGLObjectsWhenDestroyed = true;
};
