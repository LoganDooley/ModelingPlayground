#pragma once

#include "SceneObject.h"
#include <memory>

class SceneObjectNode
{
public:
    SceneObjectNode(SceneObject sceneObject, std::vector<std::shared_ptr<SceneObjectNode>> childSceneObjectNodes = std::vector<std::shared_ptr<SceneObjectNode>>());

    void AddChild(std::shared_ptr<SceneObjectNode> childSceneObjectNode);
    bool RemoveChild(std::shared_ptr<SceneObjectNode> targetChildSceneObjectNode);

    const SceneObject& GetSceneObject();
    std::vector<std::shared_ptr<SceneObjectNode>> GetChildren();
    bool HasChildren() const;

private:
    SceneObject m_sceneObject;
    std::vector<std::shared_ptr<SceneObjectNode>> m_childSceneObjectNodes;
};
