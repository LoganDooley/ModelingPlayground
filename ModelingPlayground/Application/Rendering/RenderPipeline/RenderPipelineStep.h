#pragma once
#include <string>

class RenderPipelineStep
{
public:
    RenderPipelineStep(const std::string& name);
    virtual ~RenderPipelineStep();

    const std::string& GetName() const;

private:
    std::string m_name;
};
