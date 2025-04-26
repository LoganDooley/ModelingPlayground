#pragma once
#include <vector>

#include "glad/glad.h"

template <typename T>
class OpenGLShaderStorageBuffer
{
public:
    OpenGLShaderStorageBuffer(const std::vector<T>& initialData, GLenum usageMode):
        m_ssbo(0),
        m_usageMode(usageMode)
    {
        glGenBuffers(1, &m_ssbo);
        Bind();
        glBufferData(GL_SHADER_STORAGE_BUFFER, initialData.size() * sizeof(T), initialData.data(), m_usageMode);
        Unbind();
    }

    ~OpenGLShaderStorageBuffer()
    {
        if (m_ssbo != 0)
        {
            glDeleteBuffers(1, &m_ssbo);
        }
    }

    void BindBase(GLuint index) const
    {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_ssbo);
    }

    void Bind() const
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
    }

    static void Unbind()
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

private:
    GLuint m_ssbo;
    GLenum m_usageMode;
};
