#include "RenderPipelineDrawStep.h"

#include "../DrawCommand/DrawCommand.h"

RenderPipelineDrawStep::RenderPipelineDrawStep(const std::string& name, std::shared_ptr<DrawCommand> drawCommand):
    RenderPipelineStep(name),
    m_drawCommand(drawCommand)
{
}

RenderPipelineDrawStep::~RenderPipelineDrawStep()
{
}

void RenderPipelineDrawStep::Execute() const
{
    SetupAndBindShader();
    m_drawCommand->Execute();
}
