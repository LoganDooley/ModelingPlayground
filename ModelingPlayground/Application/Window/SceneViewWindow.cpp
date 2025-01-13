#include "SceneViewWindow.h"
#include "imgui.h"

SceneViewWindow::SceneViewWindow()
{
}

SceneViewWindow::~SceneViewWindow()
{
}

void SceneViewWindow::Render()
{
	ImGui::Begin(Name.c_str(), nullptr, ImGuiWindowFlags_NoMove);
	ImGui::Text("I'm the scene view!");
	ImGui::End();
}
