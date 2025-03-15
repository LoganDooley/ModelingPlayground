#include "SceneNodeGenerator.h"

#include <unordered_set>

#include "../Object.h"
#include "../Scene.h"
#include "../Components/MaterialComponent.h"
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
    }
    parent.AddChild(sceneNode);
}

std::vector<SceneNodeType> SceneNodeGenerator::GetSceneNodeTypes()
{
    return {
        SceneNodeType::Primitive
        };
}

std::string SceneNodeGenerator::GetSceneNodeTypeName(SceneNodeType sceneNodeType)
{
    switch (sceneNodeType)
    {
    case SceneNodeType::Primitive:
        return "Primitive";
    }
    return "UnknownType";
}

void SceneNodeGenerator::InitializePrimitiveObject(Object& object)
{
    object.AddComponent<TransformComponent>();
    object.AddComponent<PrimitiveComponent>();
    object.AddComponent<MaterialComponent>();
}

std::string SceneNodeGenerator::GetDefaultSceneNodeName(SceneNodeType sceneNodeType, SceneNode& parent)
{
    std::string baseName;
    switch (sceneNodeType)
    {
    case SceneNodeType::Primitive:
        baseName = "Primitive";
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
