#pragma once
#include <memory>
#include <string>
#include <vector>

class SceneNode;
class Object;

enum class SceneNodeType
{
    Primitive
};

class SceneNodeGenerator
{
public:
    static void CreateSceneNodeAndAddAsChild(SceneNodeType sceneNodeType, const std::shared_ptr<SceneNode>& parent);
    static std::vector<SceneNodeType> GetSceneNodeTypes();
    static std::string GetSceneNodeTypeName(SceneNodeType sceneNodeType);

private:
    static void InitializePrimitiveObject(const std::shared_ptr<Object>& object);
    static std::string GetDefaultSceneNodeName(SceneNodeType sceneNodeType, const std::shared_ptr<SceneNode>& parent);
};
