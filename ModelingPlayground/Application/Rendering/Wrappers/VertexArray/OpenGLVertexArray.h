#pragma once
#include "../../Primitives/Primitive.h"
#include "glad/glad.h"

class OpenGLVertexBuffer;

class OpenGLVertexArray
{
public:
    OpenGLVertexArray(const std::shared_ptr<OpenGLVertexBuffer>& vbo,
                      const std::vector<VertexAttribute>& vertexAttributes);
    ~OpenGLVertexArray();

    void Bind() const;
    static void Unbind();

private:
    GLuint m_vao;
};
