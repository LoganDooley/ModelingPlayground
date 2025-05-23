﻿#include "Renderer.h"

#include "../../Scene/Components/ComponentIncludes.h"
#include "../../Scene/SceneNode/SceneNode.h"
#include "../../Scene/Object.h"
#include "../../Scene/SceneHierarchy.h"
#include "../../Window/SceneViewCamera.h"
#include "../Primitives/PrimitiveManager.h"
#include "../Primitives/Primitive.h"

Renderer::Renderer(std::shared_ptr<PrimitiveManager> primitiveManager, std::shared_ptr<SceneHierarchy> sceneHierarchy):
    m_sceneHierarchy(sceneHierarchy),
    m_primitiveManager(primitiveManager)
{
}

void Renderer::SelectObjectAtPixel(int x, int y) const
{
    float minT = std::numeric_limits<float>::max();
    std::shared_ptr<SceneNode> closestSceneNode = nullptr;
    m_sceneHierarchy->BreadthFirstProcessAllSceneNodes(
        [this, x, y, &minT, &closestSceneNode](std::shared_ptr<SceneNode> sceneNode)
        {
            std::shared_ptr<TransformComponent> transformComponent = sceneNode->GetObject().GetFirstComponentOfType<
                TransformComponent>();
            std::shared_ptr<PrimitiveComponent> primitiveComponent = sceneNode->GetObject().GetFirstComponentOfType<
                PrimitiveComponent>();

            if (primitiveComponent == nullptr || transformComponent == nullptr)
            {
                return;
            }

            std::pair<glm::vec3, glm::vec3> ray = m_camera->GetWorldSpaceRayThroughPixel(x, y);
            glm::mat4 modelMatrix = transformComponent->GetCumulativeModelMatrixDataBinding().GetData();
            glm::vec3 p = modelMatrix * glm::vec4(ray.first, 1.0);
            glm::vec3 d = modelMatrix * glm::vec4(ray.second, 0.0);

            float t = m_primitiveManager->GetPrimitive(primitiveComponent->GetPrimitiveNameDataBinding().GetData())->
                                          Raycast(p, d);
            if (t > 0 && t < minT)
            {
                minT = t;
                closestSceneNode = sceneNode;
            }
        });

    m_sceneHierarchy->SetSceneNodeSelected(closestSceneNode);
}
