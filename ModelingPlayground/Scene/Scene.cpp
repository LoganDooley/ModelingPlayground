#include "Scene.h"

#include "Object.h"
#include "Components/OpenGLSettingsComponent.h"

Scene::Scene()
{
    m_rootSceneNode = std::make_shared<SceneNode>("World");
    m_rootSceneNode->GetObject().AddComponent<OpenGLSettingsComponent>();
}

const std::shared_ptr<SceneNode>& Scene::GetRootSceneNode() const
{
    return m_rootSceneNode;
}

void Scene::SetSceneNodeSelected(const std::shared_ptr<SceneNode>& sceneNode)
{
    m_selectedSceneNode = sceneNode;
}

const std::shared_ptr<SceneNode>& Scene::GetSelectedSceneNode() const
{
    return m_selectedSceneNode;
}

bool Scene::IsSceneNodeSelected(const std::shared_ptr<SceneNode>& sceneNode) const
{
    return sceneNode == m_selectedSceneNode;
}

void Scene::CreateLightsContainer(const std::shared_ptr<OpenGLShader>& shader)
{
    m_lightsContainer = std::make_shared<LightsContainer>(shader);
}

void Scene::RegisterLightWithLightsContainer(const std::shared_ptr<SceneNode>& sceneNode, LightType lightType) const
{
    m_lightsContainer->AddLight(sceneNode, lightType);
}
