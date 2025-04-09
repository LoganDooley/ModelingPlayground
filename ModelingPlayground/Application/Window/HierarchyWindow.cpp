#include "HierarchyWindow.h"
#include "imgui.h"
#include "SceneViewWindow.h"
#include "tinyfiledialogs.h"
#include "../../OpenGLHelpers/OpenGLRenderer.h"
#include "../../Scene/SceneHierarchy.h"
#include "../../Scene/SceneNode/SceneNodeGenerator.h"
#include "../../Utils/ModelLoader.h"

HierarchyWindow::HierarchyWindow(std::shared_ptr<SceneHierarchy> scene, std::shared_ptr<OpenGLRenderer> openGLRenderer):
	m_sceneHierarchy(scene),
	m_sceneNodeGenerator(std::make_shared<SceneNodeGenerator>()),
	m_openGLRenderer(openGLRenderer)
{
	m_sceneNodeGenerator->SetOpenGLRenderer(openGLRenderer);
}

void HierarchyWindow::Render()
{
	ImGui::Begin(Name.c_str(), nullptr, ImGuiWindowFlags_NoMove);
	DrawNode(m_sceneHierarchy->GetRootSceneNode());
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
	if (m_sceneHierarchy->IsSceneNodeSelected(node))
	{
		nodeFlags |= ImGuiTreeNodeFlags_Selected;
	}
	if (ImGui::TreeNodeEx(node->GetName().c_str(), nodeFlags))
	{
		CheckNodeSelection(node);
		CheckNodePopupMenu(node);
		const std::vector<std::shared_ptr<SceneNode>>& children = node->GetChildren();
		// DrawNode may change the number of children if the selection menu deletes one
		for (int i = 0; i < children.size(); i++)
		{
			DrawNode(children[i]);
		}
		ImGui::TreePop();
	}
}

void HierarchyWindow::CheckNodeSelection(const std::shared_ptr<SceneNode>& node)
{
	if (ImGui::IsItemClicked())
	{
		m_sceneHierarchy->SetSceneNodeSelected(node);
	}
}

void HierarchyWindow::CheckNodePopupMenu(const std::shared_ptr<SceneNode>& node)
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
					std::shared_ptr<SceneNode> newChildSceneNode = m_sceneNodeGenerator->CreateSceneNodeAndAddAsChild(
						sceneNodeType, node);
					m_sceneHierarchy->OnSceneNodeAdded(newChildSceneNode);
				}
			}
			if (ImGui::MenuItem("Custom Scene File"))
			{
				const char* lFilterPatterns[8] = {
					"*.gltf", "*.fbx", "*.3ds", "*.ase", "*.ply", "*.obj", "*.dxf", "*.x"
				};
				const char* filePath = tinyfd_openFileDialog(
					"Select a scene file to open",
					"",
					8,
					lFilterPatterns,
					"files",
					0
				);
				if (filePath)
				{
					ModelLoader::LoadModel(node, filePath, m_openGLRenderer, m_sceneNodeGenerator);
				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Delete"))
		{
			if (node->HasParent())
			{
				node->GetParent()->RemoveChild(node);
			}
		}
		ImGui::EndPopup();
	}
}
