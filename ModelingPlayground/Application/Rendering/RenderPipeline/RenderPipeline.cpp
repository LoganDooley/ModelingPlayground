#include "RenderPipeline.h"

#include "RenderPipelineSteps/RenderPipelineStep.h"

RenderPipeline::RenderPipeline()
{
}

RenderPipeline::~RenderPipeline()
{
}

void RenderPipeline::RemoveRenderPipelineStep(const std::string& name)
{
    int index = -1;
    for (int i = 0; i < m_renderPipelineSteps.size(); i++)
    {
        if (m_renderPipelineSteps[i]->GetName() == name)
        {
            index = i;
            break;
        }
    }

    if (index != -1)
    {
        m_renderPipelineSteps.erase(m_renderPipelineSteps.begin() + index);
    }
}

void RenderPipeline::Render() const
{
    for (const auto& step : m_renderPipelineSteps)
    {
        step->Execute();
    }
}
