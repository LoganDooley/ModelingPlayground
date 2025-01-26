#include "Scene.h"

Scene::Scene():
    m_rootLevelSceneObjectNodes(std::vector<std::shared_ptr<SceneObjectNode>>())
{
}

void Scene::AddRootLevelSceneObjectNode(const std::shared_ptr<SceneObjectNode>& sceneObjectNode)
{
    m_rootLevelSceneObjectNodes.push_back(sceneObjectNode);
}

std::vector<std::shared_ptr<SceneObjectNode>> Scene::GetRootLevelSceneObjectNodes()
{
    return m_rootLevelSceneObjectNodes;
}

void Scene::SetSceneObjectNodeSelected(const std::shared_ptr<SceneObjectNode>& sceneObjectNode)
{
    m_selectedSceneObjectNode = sceneObjectNode;
}

bool Scene::IsSceneObjectNodeSelected(const std::shared_ptr<SceneObjectNode>& sceneObjectNode) const
{
    return sceneObjectNode == m_selectedSceneObjectNode;
}
