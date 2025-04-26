#include "RenderPipeline.h"

#include "RenderPipelineStep.h"

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
