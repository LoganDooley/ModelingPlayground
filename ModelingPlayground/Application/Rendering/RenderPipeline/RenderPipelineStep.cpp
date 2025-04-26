#include "RenderPipelineStep.h"

RenderPipelineStep::RenderPipelineStep(const std::string& name):
    m_name(name)
{
}

RenderPipelineStep::~RenderPipelineStep()
{
}

const std::string& RenderPipelineStep::GetName() const
{
    return m_name;
}
