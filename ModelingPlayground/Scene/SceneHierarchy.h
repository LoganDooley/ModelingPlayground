#pragma once
#include "../OpenGLHelpers/OpenGLLightContainer.h"
#include "SceneNode/SceneNode.h"
#include "../../nlohmann/json_fwd.hpp"

class SceneHierarchy {
public:
    SceneHierarchy();

    const std::shared_ptr<SceneNode>& GetRootSceneNode() const;
    void SetSceneNodeSelected(const std::shared_ptr<SceneNode>& sceneNode);
    const std::shared_ptr<SceneNode>& GetSelectedSceneNode() const;
    bool IsSceneNodeSelected(const std::shared_ptr<SceneNode>& sceneNode) const;

    void SubscribeToSceneNodeAdded(std::function<void(const std::shared_ptr<SceneNode>&)> callback);
    void OnSceneNodeAdded(const std::shared_ptr<SceneNode>& sceneNode) const;

    friend void to_json(nlohmann::json& json, const std::shared_ptr<SceneHierarchy>& sceneHierarchy);
    friend void from_json(const nlohmann::json& json, std::shared_ptr<SceneHierarchy>& sceneHierarchy);
    
private:
    std::shared_ptr<SceneNode> m_selectedSceneNode;
    std::shared_ptr<SceneNode> m_rootSceneNode;
    std::vector<std::function<void(const std::shared_ptr<SceneNode>&)>> m_sceneNodeAddedSubscribers;
};
