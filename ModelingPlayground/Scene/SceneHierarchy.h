#pragma once

#include <functional>
#include "../../nlohmann/json_fwd.hpp"

class SceneNode;

class SceneHierarchy
{
public:
    SceneHierarchy();
    ~SceneHierarchy() = default;

    SceneHierarchy(const SceneHierarchy& other) noexcept;
    SceneHierarchy(SceneHierarchy&& other) noexcept;

    SceneHierarchy& operator=(const SceneHierarchy& other) noexcept;
    SceneHierarchy& operator=(SceneHierarchy&& other) noexcept;

    void SetFilePath(std::string filePath);
    std::string GetFilePath() const;

    void SetRootSceneNode(std::shared_ptr<SceneNode> rootSceneNode);
    const std::shared_ptr<SceneNode>& GetRootSceneNode() const;
    void SetSceneNodeSelected(std::shared_ptr<SceneNode> sceneNode);
    const std::shared_ptr<SceneNode>& GetSelectedSceneNode() const;
    bool IsSceneNodeSelected(const std::shared_ptr<SceneNode>& sceneNode) const;

    void SubscribeToSceneNodeAdded(std::function<void(const std::shared_ptr<SceneNode>&)> callback);
    void OnSceneNodeAdded(const std::shared_ptr<SceneNode>& sceneNode) const;

    void SubscribeToSceneNodeRemoved(std::function<void(const std::shared_ptr<SceneNode>&)> callback);
    void OnSceneNodeRemoved(const std::shared_ptr<SceneNode>& sceneNode) const;

    void BreadthFirstProcessAllSceneNodes(
        const std::function<void(std::shared_ptr<SceneNode>)>& sceneNodeProcessingFunction) const;

    friend void to_json(nlohmann::json& json, const SceneHierarchy& sceneHierarchy);
    friend void from_json(const nlohmann::json& json, SceneHierarchy& sceneHierarchy);

private:
    std::string m_filePath;
    std::shared_ptr<SceneNode> m_selectedSceneNode;
    std::shared_ptr<SceneNode> m_rootSceneNode;
    std::vector<std::function<void(const std::shared_ptr<SceneNode>&)>> m_sceneNodeAddedSubscribers;
    std::vector<std::function<void(const std::shared_ptr<SceneNode>&)>> m_sceneNodeRemovedSubscribers;
};
