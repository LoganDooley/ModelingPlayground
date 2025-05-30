﻿#include "OpenGLVertexArray.h"

#include "../OpenGLBuffer.h"

OpenGLVertexArray::OpenGLVertexArray(const std::shared_ptr<OpenGLBuffer>& vbo,
                                     const std::vector<VertexAttribute>& vertexAttributes):
    m_vao(0)
{
    glGenVertexArrays(1, &m_vao);
    Bind();
    vbo->Bind();
    GLsizei stride = 0;
    for (const auto vertexAttribute : vertexAttributes)
    {
        stride += sizeof(float) * GetSize(vertexAttribute);
    }
    int offset = 0;
    for (int i = 0; i < vertexAttributes.size(); i++)
    {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, GetSize(vertexAttributes[i]), GL_FLOAT, GL_FALSE, stride, (void*)offset);
        offset += sizeof(float) * GetSize(vertexAttributes[i]);
    }
    vbo->Unbind();
    Unbind();
}

OpenGLVertexArray::~OpenGLVertexArray()
{
    if (m_shouldDeleteOpenGLObjectsWhenDestroyed && m_vao != 0)
    {
        glDeleteVertexArrays(1, &m_vao);
    }
}

OpenGLVertexArray::OpenGLVertexArray(OpenGLVertexArray&& other)
    noexcept
{
    m_vao = std::move(other.m_vao);
    other.m_shouldDeleteOpenGLObjectsWhenDestroyed = false;
}

OpenGLVertexArray& OpenGLVertexArray::operator=(OpenGLVertexArray&& other) noexcept
{
    m_vao = std::move(other.m_vao);
    other.m_shouldDeleteOpenGLObjectsWhenDestroyed = false;
    return *this;
}

void OpenGLVertexArray::Bind() const
{
    glBindVertexArray(m_vao);
}

void OpenGLVertexArray::Unbind()
{
    glBindVertexArray(0);
}
