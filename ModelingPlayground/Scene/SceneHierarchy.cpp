#include "SceneHierarchy.h"

SceneHierarchy::SceneHierarchy()
{

}

SceneHierarchy::SceneHierarchy(const SceneHierarchy& other) noexcept
{
    m_selectedSceneNode = other.m_selectedSceneNode;
    m_rootSceneNode = other.m_rootSceneNode;
    m_sceneNodeAddedSubscribers = other.m_sceneNodeAddedSubscribers;
}

SceneHierarchy::SceneHierarchy(SceneHierarchy&& other) noexcept
{
    m_selectedSceneNode = std::move(other.m_selectedSceneNode);
    m_rootSceneNode = std::move(other.m_rootSceneNode);
    m_sceneNodeAddedSubscribers = std::move(other.m_sceneNodeAddedSubscribers);
}

SceneHierarchy& SceneHierarchy::operator=(const SceneHierarchy& other) noexcept
{
    m_selectedSceneNode = other.m_selectedSceneNode;
    m_rootSceneNode = other.m_rootSceneNode;
    m_sceneNodeAddedSubscribers = other.m_sceneNodeAddedSubscribers;
    return *this;
}

SceneHierarchy& SceneHierarchy::operator=(SceneHierarchy&& other) noexcept
{
    m_selectedSceneNode = std::move(other.m_selectedSceneNode);
    m_rootSceneNode = std::move(other.m_rootSceneNode);
    m_sceneNodeAddedSubscribers = std::move(other.m_sceneNodeAddedSubscribers);
    return *this;
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
