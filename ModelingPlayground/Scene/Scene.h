#pragma once
#include "SceneNode/SceneNode.h"

class Scene {
public:
    Scene();

    const std::shared_ptr<SceneNode>& GetRootSceneNode() const;
    void SetSceneNodeSelected(const std::shared_ptr<SceneNode>& sceneNode);
    const std::shared_ptr<SceneNode>& GetSelectedSceneNode() const;
    bool IsSceneNodeSelected(const std::shared_ptr<SceneNode>& sceneNode) const;

private:
    std::shared_ptr<SceneNode> m_selectedSceneNode;
    std::shared_ptr<SceneNode> m_rootSceneNode;
};
