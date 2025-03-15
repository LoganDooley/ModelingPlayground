#include "HierarchyWindow.h"
#include "imgui.h"
#include "SceneViewWindow.h"
#include "../../Scene/SceneHierarchy.h"

HierarchyWindow::HierarchyWindow(const std::shared_ptr<SceneHierarchy>& scene):
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

void HierarchyWindow::Update(double seconds)
{
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
		CheckNodePopupMenu(*node);
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

void HierarchyWindow::CheckNodePopupMenu(SceneNode& node)
{
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::BeginMenu("Add"))
		{
			const std::vector<SceneNodeType> sceneNodeTypes = SceneNodeGenerator::GetSceneNodeTypes();
			// TODO: Consider using ImGui::SeparatorText to add sections for nodes of different types
			for (const SceneNodeType& sceneNodeType : sceneNodeTypes)
			{
				if (ImGui::MenuItem(SceneNodeGenerator::GetSceneNodeTypeName(sceneNodeType).c_str()))
				{
					std::shared_ptr<SceneNode> newChildSceneNode = SceneNodeGenerator::CreateSceneNodeAndAddAsChild(sceneNodeType, node);
					TryRegisterSceneNodeAsLight(newChildSceneNode, sceneNodeType);
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

void HierarchyWindow::TryRegisterSceneNodeAsLight(const std::shared_ptr<SceneNode>& node, SceneNodeType sceneNodeType)
{
	switch (sceneNodeType)
	{
		case SceneNodeType::DirectionalLight:
			m_scene->RegisterLightWithLightsContainer(node, LightType::Directional);
			break;
		case SceneNodeType::PointLight:
			m_scene->RegisterLightWithLightsContainer(node, LightType::Point);
			break;
		case SceneNodeType::SpotLight:
			m_scene->RegisterLightWithLightsContainer(node, LightType::Spot);
			break;
	}
}
