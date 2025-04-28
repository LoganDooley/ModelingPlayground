#include "SceneNode.h"

#include <queue>
#include <stack>
#include <glm/glm.hpp>
#include "../Object.h"

#include "../Components/TransformComponent.h"

SceneNode::SceneNode(std::string name, std::vector<std::shared_ptr<SceneNode>> childSceneNodes):
    m_name(name),
    m_object(std::make_shared<Object>(name)),
    m_childSceneNodes(childSceneNodes)
{
}

SceneNode::~SceneNode()
{
    for (const auto& subscriber : m_onDestroyedSubscribers)
    {
        subscriber();
    }
}

SceneNode::SceneNode(const SceneNode& other) noexcept
{
    m_name = other.m_name;
    m_object = other.m_object;
    m_parentSceneNode = other.m_parentSceneNode;
    m_childSceneNodes = other.m_childSceneNodes;
    m_onDestroyedSubscribers = other.m_onDestroyedSubscribers;
}

SceneNode::SceneNode(SceneNode&& other) noexcept
{
    m_name = std::move(other.m_name);
    m_object = std::move(other.m_object);
    m_parentSceneNode = std::move(other.m_parentSceneNode);
    m_childSceneNodes = std::move(other.m_childSceneNodes);
    m_onDestroyedSubscribers = std::move(other.m_onDestroyedSubscribers);
}

SceneNode& SceneNode::operator=(const SceneNode& other) noexcept
{
    m_name = other.m_name;
    m_object = other.m_object;
    m_parentSceneNode = other.m_parentSceneNode;
    m_childSceneNodes = other.m_childSceneNodes;
    m_onDestroyedSubscribers = other.m_onDestroyedSubscribers;
    return *this;
}

SceneNode& SceneNode::operator=(SceneNode&& other) noexcept
{
    m_name = std::move(other.m_name);
    m_object = std::move(other.m_object);
    m_parentSceneNode = std::move(other.m_parentSceneNode);
    m_childSceneNodes = std::move(other.m_childSceneNodes);
    m_onDestroyedSubscribers = std::move(other.m_onDestroyedSubscribers);
    return *this;
}

const std::vector<std::shared_ptr<SceneNode>>& SceneNode::GetChildren() const
{
    return m_childSceneNodes;
}

void SceneNode::AddChild(const std::shared_ptr<SceneNode>& childSceneNode)
{
    m_childSceneNodes.push_back(childSceneNode);
}

bool SceneNode::RemoveChild(const std::shared_ptr<SceneNode>& targetChildSceneNode)
{
    return std::erase(m_childSceneNodes, targetChildSceneNode) > 0;
}

bool SceneNode::HasChildren() const
{
    return !m_childSceneNodes.empty();
}

std::shared_ptr<SceneNode> SceneNode::GetParent() const
{
    return m_parentSceneNode.lock();
}

void SceneNode::SetParent(const std::shared_ptr<SceneNode>& parentSceneNode)
{
    m_parentSceneNode = parentSceneNode;
}

bool SceneNode::HasParent() const
{
    return GetParent() != nullptr;
}

void SceneNode::RegisterTransformModelMatrix() const
{
    std::shared_ptr<TransformComponent> transformComponent = m_object->GetFirstComponentOfType<TransformComponent>();
    if (transformComponent == nullptr)
    {
        return;
    }

    // Find first parent node with a transform and use it to set this nodes cumulative parent transform
    std::shared_ptr<SceneNode> parentSceneNode = GetParent();
    while (parentSceneNode != nullptr)
    {
        if (std::shared_ptr<TransformComponent> parentTransformComponent = parentSceneNode->GetObject().
            GetFirstComponentOfType<TransformComponent>())
        {
            transformComponent->SetParentCumulativeModelMatrix(parentTransformComponent->GetCumulativeModelMatrix());
            break;
        }
        parentSceneNode = parentSceneNode->GetParent();
    }

    // If this object's local model matrix changes
    transformComponent->GetLocalModelMatrixDataBinding().Subscribe(this,
                                                                   [this, transformComponent](
                                                                   const glm::mat4& modelMatrix, glm::mat4)
                                                                   {
                                                                       std::queue<std::pair<
                                                                           std::shared_ptr<SceneNode>, glm::mat4>> bfs;
                                                                       glm::mat4 parentModelMatrix = transformComponent
                                                                           ->GetParentCumulativeModelMatrix() *
                                                                           modelMatrix;
                                                                       // Start a bfs with this node's children
                                                                       for (const auto& childSceneNode :
                                                                            m_childSceneNodes)
                                                                       {
                                                                           bfs.emplace(
                                                                               childSceneNode, parentModelMatrix);
                                                                       }
                                                                       while (!bfs.empty())
                                                                       {
                                                                           std::pair<std::shared_ptr<SceneNode>,
                                                                               glm::mat4> sceneNodeAndMatrix = bfs.
                                                                               front();
                                                                           bfs.pop();

                                                                           // Does this node have a transform component?
                                                                           std::shared_ptr<TransformComponent>
                                                                               sceneNodeTransformComponent =
                                                                                   sceneNodeAndMatrix.first->GetObject()
                                                                                   .
                                                                                   GetFirstComponentOfType<
                                                                                       TransformComponent>();
                                                                           if (sceneNodeTransformComponent != nullptr)
                                                                           {
                                                                               // Set this node's parent cumulative matrix, and update the cumulative matrix to include this transform
                                                                               sceneNodeTransformComponent->
                                                                                   SetParentCumulativeModelMatrix(
                                                                                       sceneNodeAndMatrix.second);
                                                                               sceneNodeAndMatrix.second =
                                                                                   sceneNodeAndMatrix.second *
                                                                                   sceneNodeTransformComponent->
                                                                                   GetLocalModelMatrix();
                                                                           }
                                                                           for (const auto& childSceneNode :
                                                                               sceneNodeAndMatrix.first->GetChildren())
                                                                           {
                                                                               bfs.emplace(
                                                                                   childSceneNode,
                                                                                   sceneNodeAndMatrix.second);
                                                                           }
                                                                       }
                                                                   });
}

void SceneNode::SetName(std::string name)
{
    m_name = name;
}

std::string SceneNode::GetName() const
{
    return m_name;
}

Object& SceneNode::GetObject() const
{
    return *m_object;
}

void SceneNode::RenderInspector() const
{
    m_object->RenderInspector();
}

void SceneNode::SubscribeToOnDestroyed(const std::function<void()>& callback)
{
    m_onDestroyedSubscribers.push_back(callback);
}
