#include "SceneNode.h"

#include "../Object.h"

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
