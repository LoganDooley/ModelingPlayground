#pragma once
#include <memory>
#include <string>
#include <vector>

class RenderPipelineStep;

class RenderPipeline
{
public:
    RenderPipeline();
    ~RenderPipeline();

    template <typename T>
    void AddRenderPipelineStep(const std::string& name)
    {
        m_renderPipelineSteps.push_back(std::make_shared<T>(name));
    }

    void RemoveRenderPipelineStep(const std::string& name);

private:
    std::vector<std::shared_ptr<RenderPipelineStep>> m_renderPipelineSteps;
};
