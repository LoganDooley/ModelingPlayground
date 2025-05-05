#pragma once
#include <memory>

#include "RenderPipelineStep.h"

class RenderPipelineDrawStep : public RenderPipelineStep
{
public:
    RenderPipelineDrawStep(const std::string& name, std::shared_ptr<DrawCommand> drawCommand);
    ~RenderPipelineDrawStep() override;

    virtual void PreExecute() const = 0;
    void Execute() const override;
    virtual void PostExecute() const = 0;

protected:
    std::shared_ptr<DrawCommand> m_drawCommand;
};
