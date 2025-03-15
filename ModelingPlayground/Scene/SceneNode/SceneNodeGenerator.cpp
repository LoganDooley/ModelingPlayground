#include "SceneNodeGenerator.h"

#include <unordered_set>

#include "../Object.h"
#include "../Scene.h"
#include "../Components/DirectionalLightComponent.h"
#include "../Components/MaterialComponent.h"
#include "../Components/PointLightComponent.h"
#include "../Components/PrimitiveComponent.h"
#include "../Components/TransformComponent.h"

void SceneNodeGenerator::CreateSceneNodeAndAddAsChild(SceneNodeType sceneNodeType, SceneNode& parent)
{
    std::shared_ptr<SceneNode> sceneNode = std::make_shared<SceneNode>(GetDefaultSceneNodeName(sceneNodeType, parent));
    switch (sceneNodeType)
    {
        case SceneNodeType::Primitive:
            InitializePrimitiveObject(sceneNode->GetObject());
            break;
        case SceneNodeType::DirectionalLight:
            InitializeDirectionalLightObject(sceneNode->GetObject());
            break;
        case SceneNodeType::PointLight:
            InitializePointLightObject(sceneNode->GetObject());
            break;
        default:
            std::cout<<"SceneNodeGenerator|CreateSceneNodeAndAddAsChild: Invalid Scene Node Type!\n";
    }
    parent.AddChild(sceneNode);
}

std::vector<SceneNodeType> SceneNodeGenerator::GetSceneNodeTypes()
{
    return {
        SceneNodeType::Primitive,
        SceneNodeType::DirectionalLight,
        SceneNodeType::PointLight,
        };
}

std::string SceneNodeGenerator::GetSceneNodeTypeName(SceneNodeType sceneNodeType)
{
    switch (sceneNodeType)
    {
        case SceneNodeType::Primitive:
            return "Primitive";
        case SceneNodeType::DirectionalLight:
            return "Directional Light";
        case SceneNodeType::PointLight:
            return "Point Light";
    }
    return "Unknown Type";
}

void SceneNodeGenerator::InitializePrimitiveObject(Object& object)
{
    object.AddComponent<TransformComponent>();
    object.AddComponent<PrimitiveComponent>();
    object.AddComponent<MaterialComponent>();
}

void SceneNodeGenerator::InitializeDirectionalLightObject(Object& object)
{
    object.AddComponent<TransformComponent>();
    object.AddComponent<DirectionalLightComponent>();
}

void SceneNodeGenerator::InitializePointLightObject(Object& object)
{
    object.AddComponent<TransformComponent>();
    object.AddComponent<PointLightComponent>();
}

std::string SceneNodeGenerator::GetDefaultSceneNodeName(SceneNodeType sceneNodeType, SceneNode& parent)
{
    std::string baseName;
    switch (sceneNodeType)
    {
        case SceneNodeType::Primitive:
            baseName = "Primitive";
            break;
        case SceneNodeType::DirectionalLight:
            baseName = "DirectionalLight";
            break;
        case SceneNodeType::PointLight:
            baseName = "PointLight";
            break;
    }
    std::unordered_set<std::string> takenNames;
    for (const std::shared_ptr<SceneNode>& child : parent.GetChildren())
    {
        takenNames.insert(child->GetName());
    }
    if (!takenNames.contains(baseName))
    {
        return baseName;
    }
    for (int i = 0; i<takenNames.size(); i++)
    {
        std::string enumeratedName = baseName + "_" + std::to_string(i+1);
        if (!takenNames.contains(enumeratedName))
        {
            return enumeratedName;
        }
    }

    // Should never get here
    return baseName;
}
