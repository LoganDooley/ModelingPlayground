#include "SceneHierarchy.h"

#include "Object.h"
#include "Components/OpenGLSettingsComponent.h"

SceneHierarchy::SceneHierarchy()
{
    m_rootSceneNode = std::make_shared<SceneNode>("World");
    m_rootSceneNode->GetObject().AddComponent<OpenGLSettingsComponent>();
}

const std::shared_ptr<SceneNode>& SceneHierarchy::GetRootSceneNode() const
{
    return m_rootSceneNode;
}

void SceneHierarchy::SetSceneNodeSelected(const std::shared_ptr<SceneNode>& sceneNode)
{
    m_selectedSceneNode = sceneNode;
}

const std::shared_ptr<SceneNode>& SceneHierarchy::GetSelectedSceneNode() const
{
    return m_selectedSceneNode;
}

bool SceneHierarchy::IsSceneNodeSelected(const std::shared_ptr<SceneNode>& sceneNode) const
{
    return sceneNode == m_selectedSceneNode;
}

void SceneHierarchy::CreateLightsContainer(const std::shared_ptr<OpenGLShader>& shader)
{
    m_lightsContainer = std::make_shared<LightsContainer>(shader);
}

void SceneHierarchy::RegisterLightWithLightsContainer(const std::shared_ptr<SceneNode>& sceneNode, LightType lightType) const
{
    m_lightsContainer->AddLight(sceneNode, lightType);
}
