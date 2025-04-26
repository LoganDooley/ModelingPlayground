#include "OpenGLMultiDrawElementsCommand.h"
#include "glad/glad.h"

OpenGLMultiDrawElementsCommand::OpenGLMultiDrawElementsCommand(std::shared_ptr<OpenGLVertexArray> vao,
    std::shared_ptr<OpenGLIndexBuffer> ebo)
{

}

OpenGLMultiDrawElementsCommand::~OpenGLMultiDrawElementsCommand()
{
}

void OpenGLMultiDrawElementsCommand::Execute()
{
    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, m_drawElementsIndirectCommands.size(), 0);
}
