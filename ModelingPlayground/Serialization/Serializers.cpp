#include "../nlohmann/json.hpp"
#include "../Scene/Object.h"
#include "../Scene/SceneHierarchy.h"
#include "../Scene/Components/ComponentIncludes.h"

#include "PolymorphicSerializer.h"

#include <memory>

// DirectionalLightComponent
void to_json(nlohmann::json& json, const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent)
{
    
}

void from_json(const nlohmann::json& json, std::shared_ptr<DirectionalLightComponent>& directionalLightComponent)
{
    
}

// MaterialComponent
void to_json(nlohmann::json& json, const std::shared_ptr<MaterialComponent>& materialComponent)
{
    
}

void from_json(const nlohmann::json& json, std::shared_ptr<MaterialComponent>& materialComponent)
{
    
}

// OpenGLSettingsComponent
void to_json(nlohmann::json& json, const std::shared_ptr<OpenGLSettingsComponent>& openGLSettingsComponent)
{
    
}

void from_json(const nlohmann::json& json, std::shared_ptr<OpenGLSettingsComponent>& openGLSettingsComponent)
{
    
}

// PointLightComponent
void to_json(nlohmann::json& json, const std::shared_ptr<PointLightComponent>& pointLightComponent)
{
    
}

void from_json(const nlohmann::json& json, std::shared_ptr<PointLightComponent>& pointLightComponent)
{
    
}

// PrimitiveComponent
void to_json(nlohmann::json& json, const std::shared_ptr<PrimitiveComponent>& primitiveComponent)
{
    
}

void from_json(const nlohmann::json& json, std::shared_ptr<PrimitiveComponent>& primitiveComponent)
{
    
}

// SpotLightComponent
void to_json(nlohmann::json& json, const std::shared_ptr<SpotLightComponent>& spotLightComponent)
{
    
}

void from_json(const nlohmann::json& json, std::shared_ptr<SpotLightComponent>& spotLightComponent)
{
    
}

// TransformComponent
void to_json(nlohmann::json& json, const std::shared_ptr<TransformComponent>& transformComponent)
{
    
}

void from_json(const nlohmann::json& json, std::shared_ptr<TransformComponent>& transformComponent)
{
    
}

// Object
void to_json(nlohmann::json& json, const std::shared_ptr<Object>& object)
{
    json = nlohmann::json({
        {"m_name", object->m_name}
    });
}

void from_json(const nlohmann::json& json, std::shared_ptr<Object>& object)
{
    object = std::make_shared<Object>();
    
    json.at("m_name").get_to(object->m_name);
}

// SceneNode
void to_json(nlohmann::json& json, const std::shared_ptr<SceneNode>& sceneNode)
{
    json = nlohmann::json({
        {"m_name", sceneNode->m_name},
        {"m_object", sceneNode->m_object},
        {"m_childSceneNodes", sceneNode->m_childSceneNodes}
    });
}

void from_json(const nlohmann::json& json, std::shared_ptr<SceneNode>& sceneNode)
{
    sceneNode = std::make_shared<SceneNode>();
    
    json.at("m_name").get_to(sceneNode->m_name);
    json.at("m_object").get_to(sceneNode->m_object);
    json.at("m_childSceneNodes").get_to(sceneNode->m_childSceneNodes);
}

// SceneHierarchy
void to_json(nlohmann::json& json, const std::shared_ptr<SceneHierarchy>& sceneHierarchy)
{
    json = nlohmann::json({{"m_rootSceneNode", sceneHierarchy->m_rootSceneNode}});
}

void from_json(const nlohmann::json& json, std::shared_ptr<SceneHierarchy>& sceneHierarchy)
{
    sceneHierarchy = std::make_shared<SceneHierarchy>();
    
    json.at("m_rootSceneNode").get_to(sceneHierarchy->m_rootSceneNode);
}