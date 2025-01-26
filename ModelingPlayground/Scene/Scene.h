#pragma once
#include "SceneObjectNode.h"

class Scene {
public:
    Scene();

    void AddRootLevelSceneObjectNode(const std::shared_ptr<SceneObjectNode>& sceneObjectNode);
    std::vector<std::shared_ptr<SceneObjectNode>> GetRootLevelSceneObjectNodes();
    void SetSceneObjectNodeSelected(const std::shared_ptr<SceneObjectNode>& sceneObjectNode);
    bool IsSceneObjectNodeSelected(const std::shared_ptr<SceneObjectNode>& sceneObjectNode) const;

private:
    std::shared_ptr<SceneObjectNode> m_selectedSceneObjectNode;
    std::vector<std::shared_ptr<SceneObjectNode>> m_rootLevelSceneObjectNodes;
};
