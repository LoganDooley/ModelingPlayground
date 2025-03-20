#pragma once

#include "../nlohmann/json.hpp"
#include "../Scene/Object.h"
#include "../Scene/SceneHierarchy.h"
#include "../Scene/Components/ComponentIncludes.h"

#include "PolymorphicSerializer.h"

#include <memory>

NLOHMANN_JSON_NAMESPACE_BEGIN
// Shared Pointers
template<typename T>
struct adl_serializer<std::shared_ptr<T>>
{
    static void to_json(nlohmann::json& json, const std::shared_ptr<T>& pointer)
    {
        if (pointer)
        {
            json = *pointer;
        }
        else
        {
            json = nullptr;
        }
    }

    static void from_json(const nlohmann::json& json, std::shared_ptr<T>& pointer)
    {
        if (json.is_null())
        {
            pointer = nullptr;
        }
        else
        {
            pointer.reset(new T(json.get<T>()));
        }
    }
};

// Component Polymorphism
template<>
struct adl_serializer<Component> : PolymorphicSharedPointerSerializer<Component> {};
NLOHMANN_JSON_NAMESPACE_END

// DirectionalLightComponent
inline void to_json(nlohmann::json& json, const DirectionalLightComponent& directionalLightComponent)
{
    
}

inline void from_json(const nlohmann::json& json, DirectionalLightComponent& directionalLightComponent)
{
    
}

// MaterialComponent
inline void to_json(nlohmann::json& json, const MaterialComponent& materialComponent)
{
    
}

inline void from_json(const nlohmann::json& json, MaterialComponent& materialComponent)
{
    
}

// OpenGLSettingsComponent
inline void to_json(nlohmann::json& json, const OpenGLSettingsComponent& openGLSettingsComponent)
{
    
}

inline void from_json(const nlohmann::json& json, OpenGLSettingsComponent& openGLSettingsComponent)
{
    
}

// PointLightComponent
inline void to_json(nlohmann::json& json, const PointLightComponent& pointLightComponent)
{
    
}

inline void from_json(const nlohmann::json& json, PointLightComponent& pointLightComponent)
{
    
}

// PrimitiveComponent
inline void to_json(nlohmann::json& json, const PrimitiveComponent& primitiveComponent)
{
    
}

inline void from_json(const nlohmann::json& json, PrimitiveComponent& primitiveComponent)
{
    
}

// SpotLightComponent
inline void to_json(nlohmann::json& json, const SpotLightComponent& spotLightComponent)
{
    
}

inline void from_json(const nlohmann::json& json, SpotLightComponent& spotLightComponent)
{
    
}

// TransformComponent
inline void to_json(nlohmann::json& json, const TransformComponent& transformComponent)
{
    
}

inline void from_json(const nlohmann::json& json, TransformComponent& transformComponent)
{
    
}

// Object
inline void to_json(nlohmann::json& json, const Object& object)
{
    json = nlohmann::json({
        {"m_name", object.m_name}
    });
}

inline void from_json(const nlohmann::json& json, Object& object)
{
    json.at("m_name").get_to(object.m_name);
}

// SceneNode
inline void to_json(nlohmann::json& json, const SceneNode& sceneNode)
{
    json = nlohmann::json({
        {"m_name", sceneNode.m_name},
        {"m_object", sceneNode.m_object},
        {"m_childSceneNodes", sceneNode.m_childSceneNodes}
    });
}

inline void from_json(const nlohmann::json& json, SceneNode& sceneNode)
{
    json.at("m_name").get_to(sceneNode.m_name);
    json.at("m_object").get_to(sceneNode.m_object);
    json.at("m_childSceneNodes").get_to(sceneNode.m_childSceneNodes);
}

// SceneHierarchy
inline void to_json(nlohmann::json& json, const SceneHierarchy& sceneHierarchy)
{
    json = nlohmann::json({{"m_rootSceneNode", sceneHierarchy.m_rootSceneNode}});
}

inline void from_json(const nlohmann::json& json, SceneHierarchy& sceneHierarchy)
{
    json.at("m_rootSceneNode").get_to(sceneHierarchy.m_rootSceneNode);
}