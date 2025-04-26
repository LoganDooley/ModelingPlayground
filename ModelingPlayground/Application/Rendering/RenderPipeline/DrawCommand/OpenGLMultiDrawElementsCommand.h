#pragma once
#include <memory>
#include <vector>

#include "DrawCommand.h"

class OpenGLIndexBuffer;
class OpenGLVertexArray;

using DrawElementsIndirectCommand = struct
{
    unsigned int count; // Number of indices to draw
    unsigned int instanceCount; // Number of instances
    unsigned int firstIndex; // Base index in the index buffer
    unsigned int baseVertex; // Base index in the vertex buffer
    unsigned int baseInstance; // Instance number to start at
};

class OpenGLMultiDrawElementsCommand : public DrawCommand
{
public:
    OpenGLMultiDrawElementsCommand(std::shared_ptr<OpenGLVertexArray> vao, std::shared_ptr<OpenGLIndexBuffer> ebo);
    ~OpenGLMultiDrawElementsCommand() override;

    void Execute() override;

private:
    std::shared_ptr<OpenGLVertexArray> m_vao;
    std::shared_ptr<OpenGLIndexBuffer> m_ebo;

    std::vector<DrawElementsIndirectCommand> m_drawElementsIndirectCommands;
};
