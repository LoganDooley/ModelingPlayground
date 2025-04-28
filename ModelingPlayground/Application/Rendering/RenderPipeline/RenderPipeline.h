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

    template <class T, typename... Args>
    void AddRenderPipelineStep(Args... args)
    {
        m_renderPipelineSteps.push_back(std::make_shared<T>(args...));
    }

    void RemoveRenderPipelineStep(const std::string& name);

    void Render() const;

private:
    std::vector<std::shared_ptr<RenderPipelineStep>> m_renderPipelineSteps;
};
