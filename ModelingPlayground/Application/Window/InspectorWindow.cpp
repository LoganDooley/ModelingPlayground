#include "InspectorWindow.h"

#include "imgui.h"
#include "../../Scene/Scene.h"

InspectorWindow::InspectorWindow(const std::shared_ptr<Scene>& scene):
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
