#pragma once

#include <memory>
#include <string>
#include <vector>

class Object;

class SceneNode
{
public:
    SceneNode(std::string name, const std::vector<std::shared_ptr<SceneNode>>& childSceneNodes = std::vector<std::shared_ptr<SceneNode>>());
    
    const std::vector<std::shared_ptr<SceneNode>>& GetChildren() const;
    void AddChild(const std::shared_ptr<SceneNode>& childSceneNode);
    bool RemoveChild(const std::shared_ptr<SceneNode>& targetChildSceneNode);
    bool HasChildren() const;
    std::string GetName() const;
    const std::shared_ptr<Object>& GetObject() const;
    void RenderInspector() const;

private:
    std::string m_name;
    std::shared_ptr<Object> m_object;
    std::vector<std::shared_ptr<SceneNode>> m_childSceneNodes;
};
