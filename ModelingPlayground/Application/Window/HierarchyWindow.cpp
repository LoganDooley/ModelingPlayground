#include "HierarchyWindow.h"
#include "imgui.h"

HierarchyWindow::HierarchyWindow()
{
}

HierarchyWindow::~HierarchyWindow()
{
}

void HierarchyWindow::Render()
{
	ImGui::Begin(Name.c_str(), nullptr, ImGuiWindowFlags_NoMove);
	ImGui::Text("I'm the hierarchy window!");
	ImGui::End();
}
