#pragma once
#include <memory>
#include <string>
#include <vector>

class SceneHierarchy;
class OpenGLRenderer;
class SceneNode;
class Object;

enum class SceneNodeType
{
	Primitive, DirectionalLight, PointLight, SpotLight, Transform
};

class SceneNodeGenerator
{
public:
	SceneNodeGenerator();

	void SetOpenGLRenderer(std::shared_ptr<OpenGLRenderer> openGLRenderer);
	void SetSceneHierarchy(std::shared_ptr<SceneHierarchy> sceneHierarchy);

	std::shared_ptr<SceneNode> CreateSceneNodeAndAddAsChild(SceneNodeType sceneNodeType,
	                                                        const std::shared_ptr<SceneNode>& parent);
	static std::vector<SceneNodeType> GetSceneNodeTypes();
	static std::string GetSceneNodeTypeName(SceneNodeType sceneNodeType);

private:
	void InitializePrimitiveObject(Object& object);
	void InitializeDirectionalLightObject(Object& object);
	void InitializePointLightObject(Object& object);
	void InitializeSpotLightObject(Object& object);
	void InitializeTransformObject(Object& object);

	static std::string GetDefaultSceneNodeName(SceneNodeType sceneNodeType, const std::shared_ptr<SceneNode>& parent);

	std::shared_ptr<OpenGLRenderer> m_openGLRenderer;
	std::shared_ptr<SceneHierarchy> m_sceneHierarchy;
};
