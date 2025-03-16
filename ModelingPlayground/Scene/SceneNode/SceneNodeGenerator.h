#pragma once
#include <memory>
#include <string>
#include <vector>

class SceneNode;
class Object;

enum class SceneNodeType
{
    Primitive, DirectionalLight, PointLight, SpotLight,
};

class SceneNodeGenerator
{
public:
    static std::shared_ptr<SceneNode> CreateSceneNodeAndAddAsChild(SceneNodeType sceneNodeType, const std::shared_ptr<SceneNode>& parent);
    static std::vector<SceneNodeType> GetSceneNodeTypes();
    static std::string GetSceneNodeTypeName(SceneNodeType sceneNodeType);

private:
    static void InitializePrimitiveObject(Object& object);
    static void InitializeDirectionalLightObject(Object& object);
    static void InitializePointLightObject(Object& object);
    static void InitializeSpotLightObject(Object& object);
    
    static std::string GetDefaultSceneNodeName(SceneNodeType sceneNodeType, const std::shared_ptr<SceneNode>& parent);
};
