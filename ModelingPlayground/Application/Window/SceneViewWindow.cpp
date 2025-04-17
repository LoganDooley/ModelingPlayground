#include "SceneViewWindow.h"

#include "imgui.h"
#include "SceneViewCamera.h"
#include "../InputManager.h"
#include "../../Application/Rendering/RenderingManager.h"
#include "../../Scene/Components/OpenGLSettingsComponent.h"
#include "glm/glm.hpp"

SceneViewWindow::SceneViewWindow(std::shared_ptr<RenderingManager> renderingManager,
                                 std::shared_ptr<InputManager> inputManager):
	m_camera(std::make_shared<SceneViewCamera>(inputManager, glm::uvec2(1, 1))),
	m_renderingManager(renderingManager),
	m_inputManager(inputManager)
{
	m_renderingManager->SetCamera(m_camera);
}

void SceneViewWindow::Render()
{
	m_renderingManager->Render();

	ImGui::Begin(Name.c_str(), nullptr, ImGuiWindowFlags_NoMove);
	ImVec2 windowSize = ImGui::GetContentRegionAvail();
	m_camera->SetScreenSize(glm::uvec2(windowSize.x, windowSize.x / m_camera->GetAspectRatio()));
	ImGui::BeginChild("GameRender");
	ImVec2 imagePos = ImGui::GetCursorScreenPos();
	m_inputManager->SetSceneViewPos(glm::vec2(imagePos.x, imagePos.y));
	ImGui::Image(m_camera->GetFramebuffer(),
	             ImVec2(static_cast<float>(m_camera->GetScreenSize().x),
	                    static_cast<float>(m_camera->GetScreenSize().y)),
	             ImVec2(0, 1), ImVec2(1, 0));
	ImGui::EndChild();
	ImGui::End();
}

void SceneViewWindow::Update(double seconds)
{
	m_camera->Update(seconds);
}
