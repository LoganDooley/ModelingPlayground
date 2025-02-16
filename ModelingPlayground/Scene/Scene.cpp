#include "Scene.h"

#include "Object.h"
#include "Components/AmbientLightComponent.h"
#include "Components/ClearColorComponent.h"

Scene::Scene()
{
    m_rootSceneNode = std::make_shared<SceneNode>("World");
    m_rootSceneNode->GetObject().AddComponent<AmbientLightComponent>();
    m_rootSceneNode->GetObject().AddComponent<ClearColorComponent>();
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
