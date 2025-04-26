#pragma once
#include <vector>

#include "glad/glad.h"

class OpenGLIndexBuffer
{
public:
    OpenGLIndexBuffer(const std::vector<unsigned int>& indices, GLenum usageMode);
    ~OpenGLIndexBuffer();

    void Bind() const;
    static void Unbind();

private:
    GLuint m_ebo;
    GLenum m_usageMode;
};
