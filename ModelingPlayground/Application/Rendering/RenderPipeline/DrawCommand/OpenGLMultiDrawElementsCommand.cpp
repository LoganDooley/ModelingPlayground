#include "OpenGLMultiDrawElementsCommand.h"

#include "../../Wrappers/VertexArray/OpenGLVertexArray.h"
#include "../../Wrappers/OpenGLBuffer.h"

OpenGLMultiDrawElementsCommand::OpenGLMultiDrawElementsCommand(std::shared_ptr<OpenGLVertexArray> vao,
                                                               std::shared_ptr<OpenGLBuffer> ebo,
                                                               const std::vector<DrawElementsIndirectCommand>&
                                                               drawElementsIndirectCommands):
    m_vao(vao),
    m_ebo(ebo),
    m_drawIndirectBuffer(
        std::make_unique<OpenGLBuffer>(drawElementsIndirectCommands, GL_DRAW_INDIRECT_BUFFER, GL_STATIC_DRAW)),
    m_drawCount(drawElementsIndirectCommands.size())
{
}

OpenGLMultiDrawElementsCommand::~OpenGLMultiDrawElementsCommand()
{
}

void OpenGLMultiDrawElementsCommand::ResetCommands(
    const std::vector<DrawElementsIndirectCommand>& drawElementsIndirectCommands)
{
    m_drawIndirectBuffer = std::make_unique<OpenGLBuffer>(drawElementsIndirectCommands, GL_DRAW_INDIRECT_BUFFER,
                                                          GL_DYNAMIC_STORAGE_BIT, true);
    m_drawCount = drawElementsIndirectCommands.size();
}

void OpenGLMultiDrawElementsCommand::Execute() const
{
    if (!m_vao || !m_ebo)
    {
        return;
    }
    m_vao->Bind();
    m_ebo->Bind();
    std::vector<unsigned int> indices = m_ebo->GetData<unsigned int>();
    std::vector<DrawElementsIndirectCommand> drawElementsIndirectCommands = m_drawIndirectBuffer->GetData<DrawElementsIndirectCommand>();
    m_drawIndirectBuffer->Bind();
    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, m_drawCount, 0);
    m_drawIndirectBuffer->Unbind();
    m_ebo->Unbind();
    OpenGLVertexArray::Unbind();
}
