#pragma once
#include <memory>

#include "RenderPipelineStep.h"

class RenderPipelineDrawStep : public RenderPipelineStep
{
public:
    RenderPipelineDrawStep(const std::string& name, std::shared_ptr<DrawCommand> drawCommand);
    ~RenderPipelineDrawStep() override;

    void Execute() const override;

protected:
    virtual void SetupAndBindShader() const = 0;

    std::shared_ptr<DrawCommand> m_drawCommand;
};
