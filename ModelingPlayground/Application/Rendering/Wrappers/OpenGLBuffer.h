#pragma once
#include <vector>

#include "../../../OpenGLHelpers/OpenGLTypeMap.h"
#include "glad/glad.h"
#include "glm/mat4x4.hpp"

class OpenGLBuffer
{
public:
    template <typename T>
    OpenGLBuffer(const std::vector<T>& initialData, GLenum target, unsigned int usageMode, bool immutable = false):
        m_handle(0),
        m_target(target),
        m_usageMode(usageMode),
        m_immutable(immutable),
        m_dataType(OpenGLTypeMap::ToGLEnum<T>())
    {
        glGenBuffers(1, &m_handle);

        if (immutable)
        {
            SetStorage<T>(initialData);
        }
        else
        {
            SetData<T>(initialData);
        }
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

    template <typename T>
    std::vector<T> GetData() const
    {
        std::vector<T> data(m_dataSize / sizeof(T));
        Bind();
        glGetBufferSubData(m_target, 0, m_dataSize, data.data());
        Unbind();
        return data;
    }

private:
    template <typename T>
    void SetData(const std::vector<T>& data)
    {
        Bind();
        glBufferData(m_target, sizeof(T) * data.size(), data.data(), m_usageMode);
        Unbind();
        m_dataSize = sizeof(T) * data.size();
    }

    template <typename T>
    void SetStorage(const std::vector<T>& data)
    {
        Bind();
        glBufferStorage(m_target, sizeof(T) * data.size(), data.data(), m_usageMode);
        Unbind();
        m_dataSize = sizeof(T) * data.size();
    }

    GLuint m_handle;
    GLenum m_target;
    unsigned int m_usageMode;
    bool m_immutable;
    GLenum m_dataType;
    GLsizeiptr m_dataSize;
    bool m_shouldDeleteOpenGLObjectsWhenDestroyed = true;
};
