#include "HierarchyWindow.h"
#include "imgui.h"
#include "SceneViewWindow.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneNode/SceneNodeGenerator.h"

HierarchyWindow::HierarchyWindow(const std::shared_ptr<Scene>& scene):
	m_scene(scene)
{
}

void HierarchyWindow::Render()
{
	ImGui::Begin(Name.c_str(), nullptr, ImGuiWindowFlags_NoMove);
	ImGui::Text("I'm the hierarchy window!");
	DrawNode(m_scene->GetRootSceneNode());
	ImGui::End();
}

void HierarchyWindow::DrawNode(const std::shared_ptr<SceneNode>& node)
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_None;
	if (!node->HasChildren())
	{
		nodeFlags |= ImGuiTreeNodeFlags_Leaf;
	}
	else
	{
		nodeFlags |= ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;
	}
	if (m_scene->IsSceneNodeSelected(node))
	{
		nodeFlags |= ImGuiTreeNodeFlags_Selected;
	}
	if (ImGui::TreeNodeEx(node->GetName().c_str(), nodeFlags))
	{
		CheckNodeSelection(node);
		CheckNodePopupMenu(node);
		for (const std::shared_ptr<SceneNode>& child : node->GetChildren())
		{
			DrawNode(child);
		}
		ImGui::TreePop();
	}
}

void HierarchyWindow::CheckNodeSelection(const std::shared_ptr<SceneNode>& node)
{
	if (ImGui::IsItemClicked())
	{
		m_scene->SetSceneNodeSelected(node);
	}
}

void HierarchyWindow::CheckNodePopupMenu(const std::shared_ptr<SceneNode>& node)
{
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::BeginMenu("Add"))
		{
			const std::vector<SceneNodeType> sceneNodeTypes = SceneNodeGenerator::GetSceneNodeTypes();
			for (const SceneNodeType& sceneNodeType : sceneNodeTypes)
			{
				if (ImGui::MenuItem(SceneNodeGenerator::GetSceneNodeTypeName(sceneNodeType).c_str()))
				{
					SceneNodeGenerator::CreateSceneNodeAndAddAsChild(sceneNodeType, node);
				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Delete"))
		{
			
		}
		ImGui::EndPopup();
	}
}
