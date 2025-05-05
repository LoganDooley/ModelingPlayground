#pragma once
#include <memory>
#include <string>
#include <vector>

class RenderingManager;
class SceneHierarchy;
class SceneNode;
class Object;

enum class SceneNodeType
{
    Primitive, DirectionalLight, PointLight, SpotLight, Transform
};

class SceneNodeGenerator
{
public:
    static std::shared_ptr<SceneNode> CreateSceneNodeAndAddAsChild(SceneNodeType sceneNodeType,
                                                                   const std::shared_ptr<SceneNode>& parent,
                                                                   const std::shared_ptr<RenderingManager>&
                                                                   renderingManager,
                                                                   const std::shared_ptr<SceneHierarchy>&
                                                                   sceneHierarchy,
                                                                   std::string sceneNodeName = "");
    static std::vector<SceneNodeType> GetSceneNodeTypes();
    static std::string GetSceneNodeTypeName(SceneNodeType sceneNodeType);

private:
    static void InitializePrimitiveObject(Object& object, const std::shared_ptr<RenderingManager>& renderingManager,
                                          const std::shared_ptr<SceneNode>& parent);
    static void InitializeDirectionalLightObject(Object& object, const std::shared_ptr<SceneNode>& parent);
    static void InitializePointLightObject(Object& object, const std::shared_ptr<SceneNode>& parent);
    static void InitializeSpotLightObject(Object& object, const std::shared_ptr<SceneNode>& parent);
    static void InitializeTransformObject(Object& object, const std::shared_ptr<SceneNode>& parent);

    static std::string GetSceneNodeName(SceneNodeType sceneNodeType, const std::shared_ptr<SceneNode>& parent,
                                        std::string desiredSceneNodeName = "");
};
