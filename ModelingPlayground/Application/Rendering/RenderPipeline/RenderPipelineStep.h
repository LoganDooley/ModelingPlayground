#pragma once
#include <memory>
#include <string>

class DrawCommand;

class RenderPipelineStep
{
public:
    RenderPipelineStep(const std::string& name);
    virtual ~RenderPipelineStep();

    const std::string& GetName() const;

private:
    std::string m_name;

    std::shared_ptr<DrawCommand> m_drawCommand;
};
