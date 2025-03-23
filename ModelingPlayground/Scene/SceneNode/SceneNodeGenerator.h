#pragma once
#include <memory>
#include <string>
#include <vector>

class OpenGLRenderer;
class SceneNode;
class Object;

enum class SceneNodeType
{
    Primitive, DirectionalLight, PointLight, SpotLight,
};

class SceneNodeGenerator
{
public:
    SceneNodeGenerator();

    void SetOpenGLRenderer(std::shared_ptr<OpenGLRenderer> openGLRenderer);
    
    std::shared_ptr<SceneNode> CreateSceneNodeAndAddAsChild(SceneNodeType sceneNodeType, const std::shared_ptr<SceneNode>& parent);
    static std::vector<SceneNodeType> GetSceneNodeTypes();
    static std::string GetSceneNodeTypeName(SceneNodeType sceneNodeType);

private:
    void InitializePrimitiveObject(Object& object);
    void InitializeDirectionalLightObject(Object& object);
    void InitializePointLightObject(Object& object);
    void InitializeSpotLightObject(Object& object);
    
    static std::string GetDefaultSceneNodeName(SceneNodeType sceneNodeType, const std::shared_ptr<SceneNode>& parent);

    std::shared_ptr<OpenGLRenderer> m_openGLRenderer;
};
