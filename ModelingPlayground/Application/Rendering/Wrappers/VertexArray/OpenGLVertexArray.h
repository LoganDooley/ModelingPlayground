#pragma once
#include "../../Primitives/Primitive.h"
#include "glad/glad.h"

class OpenGLBuffer;

class OpenGLVertexArray
{
public:
    OpenGLVertexArray(const std::shared_ptr<OpenGLBuffer>& vbo,
                      const std::vector<VertexAttribute>& vertexAttributes);
    ~OpenGLVertexArray();

    OpenGLVertexArray(OpenGLVertexArray&& other) noexcept;
    OpenGLVertexArray& operator=(OpenGLVertexArray&& other) noexcept;

    void Bind() const;
    static void Unbind();

private:
    GLuint m_vao;
    bool m_shouldDeleteOpenGLObjectsWhenDestroyed = true;
};
