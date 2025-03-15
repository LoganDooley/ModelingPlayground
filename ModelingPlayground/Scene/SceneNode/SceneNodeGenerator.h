﻿#pragma once
#include <memory>
#include <string>
#include <vector>

class SceneNode;
class Object;

enum class SceneNodeType
{
    Primitive, DirectionalLight, PointLight,
};

class SceneNodeGenerator
{
public:
    static void CreateSceneNodeAndAddAsChild(SceneNodeType sceneNodeType, SceneNode& parent);
    static std::vector<SceneNodeType> GetSceneNodeTypes();
    static std::string GetSceneNodeTypeName(SceneNodeType sceneNodeType);

private:
    static void InitializePrimitiveObject(Object& object);
    static void InitializeDirectionalLightObject(Object& object);
    static void InitializePointLightObject(Object& object);
    
    static std::string GetDefaultSceneNodeName(SceneNodeType sceneNodeType, SceneNode& parent);
};
