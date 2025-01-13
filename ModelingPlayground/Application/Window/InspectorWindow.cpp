#include "InspectorWindow.h"
#include "imgui.h"

InspectorWindow::InspectorWindow()
{
}

InspectorWindow::~InspectorWindow()
{
}

void InspectorWindow::Render()
{
	ImGui::Begin(Name.c_str(), nullptr, ImGuiWindowFlags_NoMove);
	ImGui::Text("I'm the inspector!");
	ImGui::End();
}
