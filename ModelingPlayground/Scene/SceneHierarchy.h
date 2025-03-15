#pragma once
#include "../OpenGLHelpers/LightsContainer.h"
#include "SceneNode/SceneNode.h"

class SceneHierarchy {
public:
    SceneHierarchy();

    const std::shared_ptr<SceneNode>& GetRootSceneNode() const;
    void SetSceneNodeSelected(const std::shared_ptr<SceneNode>& sceneNode);
    const std::shared_ptr<SceneNode>& GetSelectedSceneNode() const;
    bool IsSceneNodeSelected(const std::shared_ptr<SceneNode>& sceneNode) const;

    void CreateLightsContainer(const std::shared_ptr<OpenGLShader>& shader);
    void RegisterLightWithLightsContainer(const std::shared_ptr<SceneNode>& sceneNode, LightType lightType) const;

private:
    std::shared_ptr<SceneNode> m_selectedSceneNode;
    std::shared_ptr<SceneNode> m_rootSceneNode;
    std::shared_ptr<LightsContainer> m_lightsContainer;
};
