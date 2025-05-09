#pragma once
#include <memory>
#include <vector>

#include "DrawCommand.h"
#include "glad/glad.h"

class OpenGLBuffer;
class OpenGLVertexArray;

using DrawElementsIndirectCommand = struct
{
    unsigned int count; // Number of indices to draw
    unsigned int instanceCount; // Number of instances
    unsigned int firstIndex; // Base index in the index buffer
    int baseVertex; // Base index in the vertex buffer
    unsigned int baseInstance; // Instance number to start at
};

class OpenGLMultiDrawElementsCommand : public DrawCommand
{
public:
    OpenGLMultiDrawElementsCommand(std::shared_ptr<OpenGLVertexArray> vao, std::shared_ptr<OpenGLBuffer> ebo,
                                   const std::vector<DrawElementsIndirectCommand>& drawElementsIndirectCommands);
    ~OpenGLMultiDrawElementsCommand() override;

    void ResetCommands(const std::vector<DrawElementsIndirectCommand>& drawElementsIndirectCommands);

    void Execute() const override;

private:
    std::shared_ptr<OpenGLVertexArray> m_vao;
    std::shared_ptr<OpenGLBuffer> m_ebo;
    std::unique_ptr<OpenGLBuffer> m_drawIndirectBuffer;
    GLsizei m_drawCount;
};
