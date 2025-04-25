#pragma once
#include <memory>

#include "Window.h"

class RenderingManager;

class RenderingSettingsWindow : public Window
{
public:
    RenderingSettingsWindow(std::shared_ptr<RenderingManager> renderingManager);

    void Render() override;
    void Update(double seconds) override;

    inline static const std::string Name = "Rendering Settings";

private:
    std::shared_ptr<RenderingManager> m_renderingManager;
};
