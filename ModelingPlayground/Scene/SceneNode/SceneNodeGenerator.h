#pragma once
#include <memory>
#include <string>
#include <vector>

#include "../../OpenGLHelpers/OpenGLPrimitiveManager.h"

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

    void SetOpenGLPrimitiveManager(std::shared_ptr<OpenGLPrimitiveManager> openGLPrimitiveManager);
    
    std::shared_ptr<SceneNode> CreateSceneNodeAndAddAsChild(SceneNodeType sceneNodeType, const std::shared_ptr<SceneNode>& parent);
    static std::vector<SceneNodeType> GetSceneNodeTypes();
    static std::string GetSceneNodeTypeName(SceneNodeType sceneNodeType);

private:
    void InitializePrimitiveObject(Object& object);
    static void InitializeDirectionalLightObject(Object& object);
    static void InitializePointLightObject(Object& object);
    static void InitializeSpotLightObject(Object& object);
    
    static std::string GetDefaultSceneNodeName(SceneNodeType sceneNodeType, const std::shared_ptr<SceneNode>& parent);

    std::shared_ptr<OpenGLPrimitiveManager> m_openGLPrimitiveManager;
};
