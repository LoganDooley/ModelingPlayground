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
	static void InitializePrimitiveObject(Object& object, const std::shared_ptr<RenderingManager>& renderingManager);
	static void InitializeDirectionalLightObject(Object& object);
	static void InitializePointLightObject(Object& object);
	static void InitializeSpotLightObject(Object& object);
	static void InitializeTransformObject(Object& object);

	static std::string GetSceneNodeName(SceneNodeType sceneNodeType, const std::shared_ptr<SceneNode>& parent,
	                                    std::string desiredSceneNodeName = "");
};
