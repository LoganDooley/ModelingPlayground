#pragma once

#include <string>

class DrawCommand;

class RenderPipelineStep
{
public:
    RenderPipelineStep(const std::string& name);
    virtual ~RenderPipelineStep();

    const std::string& GetName() const;
    
    virtual void Execute() const = 0;

private:
    std::string m_name;
};
