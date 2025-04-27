#pragma once
#include <vector>

#include "../../../OpenGLHelpers/OpenGLTypeMap.h"
#include "glad/glad.h"

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
    }

    ~OpenGLBuffer();

    void BindBase(GLuint index) const;

    void Bind() const;
    void Unbind() const;

private:
    GLuint m_handle;
    GLenum m_target;
    GLenum m_usageMode;
    GLenum m_dataType;
};
