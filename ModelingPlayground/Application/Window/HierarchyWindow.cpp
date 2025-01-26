#include "HierarchyWindow.h"
#include "imgui.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneObjectNode.h"

HierarchyWindow::HierarchyWindow(const std::shared_ptr<Scene>& scene):
	m_scene(scene)
{
}

HierarchyWindow::~HierarchyWindow()
{
}

void HierarchyWindow::Render()
{
	ImGui::Begin(Name.c_str(), nullptr, ImGuiWindowFlags_NoMove);
	ImGui::Text("I'm the hierarchy window!");
	for (const std::shared_ptr<SceneObjectNode>& rootLevelSceneObjectNode : m_scene->GetRootLevelSceneObjectNodes())
	{
		DrawNode(rootLevelSceneObjectNode);
	}
	ImGui::End();
}

void HierarchyWindow::DrawNode(const std::shared_ptr<SceneObjectNode>& node)
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_None;
	if (!node->HasChildren())
	{
		nodeFlags |= ImGuiTreeNodeFlags_Leaf;
	}
	if (m_scene->IsSceneObjectNodeSelected(node))
	{
		nodeFlags |= ImGuiTreeNodeFlags_Selected;
	}
	if (ImGui::TreeNodeEx(node->GetSceneObject().GetName().c_str(), nodeFlags))
	{
		CheckNodeSelection(node);
		CheckNodePopupMenu(node);
		for (const std::shared_ptr<SceneObjectNode>& child : node->GetChildren())
		{
			DrawNode(child);
		}
		ImGui::TreePop();
	}
}

void HierarchyWindow::CheckNodeSelection(const std::shared_ptr<SceneObjectNode>& node)
{
	if (ImGui::IsItemClicked())
	{
		m_scene->SetSceneObjectNodeSelected(node);
	}
}

void HierarchyWindow::CheckNodePopupMenu(const std::shared_ptr<SceneObjectNode>& node)
{
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Add"))
		{
			SceneObject sceneObject = SceneObject("object 2", PrimitiveType::Triangle);
			std::shared_ptr<SceneObjectNode> child = std::make_shared<SceneObjectNode>(sceneObject);
			node->AddChild(child);
		}
		if (ImGui::MenuItem("Delete"))
		{
			
		}
		ImGui::EndPopup();
	}
}
