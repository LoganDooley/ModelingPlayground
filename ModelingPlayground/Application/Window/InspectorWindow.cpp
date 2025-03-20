#include "InspectorWindow.h"

#include "imgui.h"
#include "../../Scene/SceneHierarchy.h"
#include "../../Scene/SceneNode/SceneNode.h"

InspectorWindow::InspectorWindow(const std::shared_ptr<SceneHierarchy>& scene):
	m_scene(scene)
{
}

void InspectorWindow::Render()
{
	ImGui::Begin(Name.c_str(), nullptr, ImGuiWindowFlags_NoMove);
	ImGui::Text("I'm the inspector!");
	if (std::shared_ptr<SceneNode> selectedSceneNode = m_scene->GetSelectedSceneNode())
	{
		selectedSceneNode->RenderInspector();
	}
	ImGui::End();
}

void InspectorWindow::Update(double seconds)
{
}
