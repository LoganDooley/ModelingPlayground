#include "RenderingSettingsWindow.h"

#include "imgui.h"
#include "../Rendering/RenderingManager.h"

RenderingSettingsWindow::RenderingSettingsWindow(std::shared_ptr<RenderingManager> renderingManager):
    m_renderingManager(renderingManager)
{
}

void RenderingSettingsWindow::Render()
{
    ImGui::Begin(Name.c_str(), nullptr, ImGuiWindowFlags_NoMove);
    m_renderingManager->DrawSettings();
    ImGui::End();
}

void RenderingSettingsWindow::Update(double seconds)
{
}
