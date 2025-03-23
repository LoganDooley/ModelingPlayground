#include "SceneHierarchy.h"

#include <queue>
#include <stack>

#include "SceneNode/SceneNode.h"

SceneHierarchy::SceneHierarchy()
{
}

SceneHierarchy::SceneHierarchy(const SceneHierarchy& other) noexcept
{
	m_filePath = other.m_filePath;
	m_selectedSceneNode = other.m_selectedSceneNode;
	m_rootSceneNode = other.m_rootSceneNode;
	m_sceneNodeAddedSubscribers = other.m_sceneNodeAddedSubscribers;
}

SceneHierarchy::SceneHierarchy(SceneHierarchy&& other) noexcept
{
	m_filePath = std::move(other.m_filePath);
	m_selectedSceneNode = std::move(other.m_selectedSceneNode);
	m_rootSceneNode = std::move(other.m_rootSceneNode);
	m_sceneNodeAddedSubscribers = std::move(other.m_sceneNodeAddedSubscribers);
}

SceneHierarchy& SceneHierarchy::operator=(const SceneHierarchy& other) noexcept
{
	m_filePath = other.m_filePath;
	m_selectedSceneNode = other.m_selectedSceneNode;
	m_rootSceneNode = other.m_rootSceneNode;
	m_sceneNodeAddedSubscribers = other.m_sceneNodeAddedSubscribers;
	return *this;
}

SceneHierarchy& SceneHierarchy::operator=(SceneHierarchy&& other) noexcept
{
	m_filePath = std::move(other.m_filePath);
	m_selectedSceneNode = std::move(other.m_selectedSceneNode);
	m_rootSceneNode = std::move(other.m_rootSceneNode);
	m_sceneNodeAddedSubscribers = std::move(other.m_sceneNodeAddedSubscribers);
	return *this;
}

void SceneHierarchy::SetFilePath(std::string filePath)
{
	m_filePath = filePath;
}

std::string SceneHierarchy::GetFilePath() const
{
	return m_filePath;
}

void SceneHierarchy::SetRootSceneNode(std::shared_ptr<SceneNode> rootSceneNode)
{
	m_rootSceneNode = rootSceneNode;
}

const std::shared_ptr<SceneNode>& SceneHierarchy::GetRootSceneNode() const
{
	return m_rootSceneNode;
}

void SceneHierarchy::SetSceneNodeSelected(std::shared_ptr<SceneNode> sceneNode)
{
	m_selectedSceneNode = sceneNode;
}

const std::shared_ptr<SceneNode>& SceneHierarchy::GetSelectedSceneNode() const
{
	return m_selectedSceneNode;
}

bool SceneHierarchy::IsSceneNodeSelected(const std::shared_ptr<SceneNode>& sceneNode) const
{
	return sceneNode == m_selectedSceneNode;
}

void SceneHierarchy::SubscribeToSceneNodeAdded(std::function<void(const std::shared_ptr<SceneNode>&)> callback)
{
	m_sceneNodeAddedSubscribers.push_back(callback);
}

void SceneHierarchy::OnSceneNodeAdded(const std::shared_ptr<SceneNode>& sceneNode) const
{
	for (const auto& subscriber : m_sceneNodeAddedSubscribers)
	{
		subscriber(sceneNode);
	}
}

void SceneHierarchy::BreadthFirstProcessAllSceneNodes(
	const std::function<void(std::shared_ptr<SceneNode>)>& sceneNodeProcessingFunction) const
{
	std::queue<std::shared_ptr<SceneNode>> bfs;
	if (m_rootSceneNode != nullptr)
	{
		bfs.push(m_rootSceneNode);
	}

	while (!bfs.empty())
	{
		std::shared_ptr<SceneNode> currentSceneNode = bfs.front();
		bfs.pop();

		sceneNodeProcessingFunction(currentSceneNode);
		for (const auto& childSceneNode : currentSceneNode->GetChildren())
		{
			bfs.push(childSceneNode);
		}
	}
}

void SceneHierarchy::DepthFirstProcessAllSceneNodes(
	const std::function<void(std::shared_ptr<SceneNode>)>& sceneNodeProcessingFunction) const
{
	std::stack<std::shared_ptr<SceneNode>> dfs;
	if (m_rootSceneNode != nullptr)
	{
		dfs.push(m_rootSceneNode);
	}

	while (!dfs.empty())
	{
		std::shared_ptr<SceneNode> currentSceneNode = dfs.top();
		dfs.pop();

		sceneNodeProcessingFunction(currentSceneNode);
		for (const auto& childSceneNode : currentSceneNode->GetChildren())
		{
			dfs.push(childSceneNode);
		}
	}
}
