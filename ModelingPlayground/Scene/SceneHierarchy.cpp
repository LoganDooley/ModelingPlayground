#include "SceneHierarchy.h"

#include "Object.h"
#include "Components/OpenGLSettingsComponent.h"

SceneHierarchy::SceneHierarchy(bool fromSerialization)
{
    if (!fromSerialization)
    {
        m_rootSceneNode = std::make_shared<SceneNode>("World");
        m_rootSceneNode->GetObject().AddComponent<OpenGLSettingsComponent>();
    }
}

const std::shared_ptr<SceneNode>& SceneHierarchy::GetRootSceneNode() const
{
    return m_rootSceneNode;
}

void SceneHierarchy::SetSceneNodeSelected(const std::shared_ptr<SceneNode>& sceneNode)
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
