#include "SceneViewWindow.h"

#include <ostream>
#include <stack>

#include "imgui.h"
#include "../../Scene/Object.h"
#include "../../Scene/SceneHierarchy.h"
#include "../../Scene/Components/OpenGLSettingsComponent.h"
#include "../../Scene/Components/MaterialComponent.h"
#include "../../Scene/Components/PrimitiveComponent.h"
#include "../../Scene/Components/TransformComponent.h"
#include "glm/glm.hpp"

SceneViewWindow::SceneViewWindow(std::shared_ptr<OpenGLRenderer> openGLRenderer, std::shared_ptr<InputManager> inputManager):
	m_camera(std::make_shared<SceneViewCamera>(inputManager, glm::uvec2(1, 1))),
	m_openGLRenderer(openGLRenderer)
{
	m_openGLRenderer->SetCamera(m_camera);
}

void SceneViewWindow::Render()
{
	m_openGLRenderer->RenderSceneHierarchy();
	
	ImGui::Begin(Name.c_str(), nullptr, ImGuiWindowFlags_NoMove);
	ImVec2 windowSize = ImGui::GetContentRegionAvail();
	m_camera->SetScreenSize(glm::uvec2(windowSize.x, windowSize.x / m_camera->GetAspectRatio()));
	ImGui::Text("I'm the scene view!");
	{
		ImGui::BeginChild("GameRender");
		ImGui::Image(m_camera->GetFramebuffer(), ImVec2(static_cast<float>(m_camera->GetScreenSize().x), static_cast<float>(m_camera->GetScreenSize().y)),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();
	}
	ImGui::End();
}

void SceneViewWindow::Update(double seconds)
{
	m_camera->Update(seconds);
}