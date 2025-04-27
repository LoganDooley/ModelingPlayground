#include "OpenGLMultiDrawElementsCommand.h"

#include "../../Wrappers/VertexArray/OpenGLVertexArray.h"
#include "../../Wrappers/OpenGLBuffer.h"

OpenGLMultiDrawElementsCommand::OpenGLMultiDrawElementsCommand(std::shared_ptr<OpenGLVertexArray> vao,
                                                               const std::vector<DrawElementsIndirectCommand>&
                                                               drawElementsIndirectCommands):
    m_vao(vao),
    m_drawIndirectBuffer(
        std::make_unique<OpenGLBuffer>(drawElementsIndirectCommands, GL_DRAW_INDIRECT_BUFFER, GL_STATIC_DRAW)),
    m_drawCount(drawElementsIndirectCommands.size())
{
}

OpenGLMultiDrawElementsCommand::~OpenGLMultiDrawElementsCommand()
{
}

void OpenGLMultiDrawElementsCommand::Execute()
{
    m_vao->Bind();
    m_drawIndirectBuffer->Bind();
    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, m_drawCount, 0);
    m_drawIndirectBuffer->Unbind();
    OpenGLVertexArray::Unbind();
}
