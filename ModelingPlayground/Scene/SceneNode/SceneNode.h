#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "../../nlohmann/json_fwd.hpp"

class Object;

class SceneNode
{
public:
    SceneNode() = default;
    SceneNode(std::string name, const std::vector<std::shared_ptr<SceneNode>>& childSceneNodes = std::vector<std::shared_ptr<SceneNode>>());
    ~SceneNode();
    
    const std::vector<std::shared_ptr<SceneNode>>& GetChildren() const;
    void AddChild(const std::shared_ptr<SceneNode>& childSceneNode);
    bool RemoveChild(const std::shared_ptr<SceneNode>& targetChildSceneNode);
    bool HasChildren() const;
    std::shared_ptr<SceneNode> GetParent() const;
    void SetParent(const std::shared_ptr<SceneNode>& parentSceneNode);
    bool HasParent() const;
    void SetName(std::string name);
    std::string GetName() const;
    Object& GetObject() const;
    void RenderInspector() const;
    void SubscribeToOnDestroyed(const std::function<void()>& callback);

    friend void to_json(nlohmann::json& json, const std::shared_ptr<SceneNode>& sceneNode);
    friend void from_json(const nlohmann::json& json, std::shared_ptr<SceneNode>& sceneNode);

private:
    std::string m_name;
    std::shared_ptr<Object> m_object;
    std::weak_ptr<SceneNode> m_parentSceneNode;
    std::vector<std::shared_ptr<SceneNode>> m_childSceneNodes;
    std::vector<std::function<void()>> m_onDestroyedSubscribers;
};
