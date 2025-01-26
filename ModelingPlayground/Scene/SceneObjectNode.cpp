#include "SceneObjectNode.h"

SceneObjectNode::SceneObjectNode(SceneObject sceneObject, std::vector<std::shared_ptr<SceneObjectNode>> childSceneObjectNodes):
    m_sceneObject(sceneObject),
    m_childSceneObjectNodes(childSceneObjectNodes)
{
}

void SceneObjectNode::AddChild(std::shared_ptr<SceneObjectNode> childSceneObjectNode)
{
    m_childSceneObjectNodes.push_back(childSceneObjectNode);
}

bool SceneObjectNode::RemoveChild(std::shared_ptr<SceneObjectNode> targetChildSceneObjectNode)
{
    return std::erase(m_childSceneObjectNodes, targetChildSceneObjectNode) > 0;
}

const SceneObject& SceneObjectNode::GetSceneObject()
{
    return m_sceneObject;
}

std::vector<std::shared_ptr<SceneObjectNode>> SceneObjectNode::GetChildren()
{
    return m_childSceneObjectNodes;
}

bool SceneObjectNode::HasChildren() const
{
    return !m_childSceneObjectNodes.empty();
}