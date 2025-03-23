#pragma once

#include "../nlohmann/json.hpp"
#include "../Scene/Object.h"
#include "../Scene/SceneHierarchy.h"
#include "../Scene/Components/ComponentIncludes.h"
#include "../Scene/SceneNode/SceneNode.h"

#include "ComponentSerializer.h"
#include "DataBindingSerializer.h"
#include "GLMSerializers.h"
#include "SharedPointerSerializer.h"

#include <memory>

// DirectionalLightComponent
inline void to_json(nlohmann::json& json, const DirectionalLightComponent& directionalLightComponent)
{
	json = {
		{"m_lightColor", directionalLightComponent.m_lightColor}
	};
}

inline void from_json(const nlohmann::json& json, DirectionalLightComponent& directionalLightComponent)
{
	json.at("m_lightColor").get_to(directionalLightComponent.m_lightColor);
}

// MaterialComponent
inline void to_json(nlohmann::json& json, const MaterialComponent& materialComponent)
{
	json = {
		{"m_materialColor", materialComponent.m_materialColor},
		{"m_metallic", materialComponent.m_metallic},
		{"m_roughness", materialComponent.m_roughness}
	};
}

inline void from_json(const nlohmann::json& json, MaterialComponent& materialComponent)
{
	json.at("m_materialColor").get_to(materialComponent.m_materialColor);
	json.at("m_metallic").get_to(materialComponent.m_metallic);
	json.at("m_roughness").get_to(materialComponent.m_roughness);
}

// OpenGLSettingsComponent
inline void to_json(nlohmann::json& json, const OpenGLSettingsComponent& openGLSettingsComponent)
{
	json = {
		{"m_clearColor", openGLSettingsComponent.m_clearColor},
		{"m_ambientLight", openGLSettingsComponent.m_ambientLight}
	};
}

inline void from_json(const nlohmann::json& json, OpenGLSettingsComponent& openGLSettingsComponent)
{
	json.at("m_clearColor").get_to(openGLSettingsComponent.m_clearColor);
	json.at("m_ambientLight").get_to(openGLSettingsComponent.m_ambientLight);
}

// PointLightComponent
inline void to_json(nlohmann::json& json, const PointLightComponent& pointLightComponent)
{
	json = {
		{"m_lightColor", pointLightComponent.m_lightColor},
		{"m_falloff", pointLightComponent.m_falloff}
	};
}

inline void from_json(const nlohmann::json& json, PointLightComponent& pointLightComponent)
{
	json.at("m_lightColor").get_to(pointLightComponent.m_lightColor);
	json.at("m_falloff").get_to(pointLightComponent.m_falloff);
}

// PrimitiveComponent
inline void to_json(nlohmann::json& json, const PrimitiveComponent& primitiveComponent)
{
	json = {
		{"m_primitiveName", primitiveComponent.m_primitiveName},
		{"m_currentItem", primitiveComponent.m_currentItem}
	};
}

inline void from_json(const nlohmann::json& json, PrimitiveComponent& primitiveComponent)
{
	json.at("m_primitiveName").get_to(primitiveComponent.m_primitiveName);
	json.at("m_currentItem").get_to(primitiveComponent.m_currentItem);
}

// SpotLightComponent
inline void to_json(nlohmann::json& json, const SpotLightComponent& spotLightComponent)
{
	json = {
		{"m_lightColor", spotLightComponent.m_lightColor},
		{"m_falloffAngles", spotLightComponent.m_falloffAngles}
	};
}

inline void from_json(const nlohmann::json& json, SpotLightComponent& spotLightComponent)
{
	spotLightComponent.m_lightColor = json.at("m_lightColor");
	spotLightComponent.m_falloffAngles = json.at("m_falloffAngles");
}

// TransformComponent
inline void to_json(nlohmann::json& json, const TransformComponent& transformComponent)
{
	json = {
		{"m_position", transformComponent.m_position},
		{"m_rotation", transformComponent.m_rotation},
		{"m_scale", transformComponent.m_scale}
	};
}

inline void from_json(const nlohmann::json& json, TransformComponent& transformComponent)
{
	json.at("m_position").get_to(transformComponent.m_position);
	json.at("m_rotation").get_to(transformComponent.m_rotation);
	json.at("m_scale").get_to(transformComponent.m_scale);
	transformComponent.UpdateModelMatrix();
	transformComponent.UpdateLocalXUnitVector();
}

// Object
inline void to_json(nlohmann::json& json, const Object& object)
{
	json = {
		{"m_name", object.m_name},
		{"m_components", object.m_components}
	};
}

inline void from_json(const nlohmann::json& json, Object& object)
{
	json.at("m_name").get_to(object.m_name);
	json.at("m_components").get_to(object.m_components);
}

// SceneNode
inline void to_json(nlohmann::json& json, const SceneNode& sceneNode)
{
	json = {
		{"m_name", sceneNode.m_name},
		{"m_object", sceneNode.m_object},
		{"m_childSceneNodes", sceneNode.m_childSceneNodes}
	};
}

inline void from_json(const nlohmann::json& json, SceneNode& sceneNode)
{
	json.at("m_name").get_to(sceneNode.m_name);
	json.at("m_object").get_to(sceneNode.m_object);

	// Need to do this manually to set up parent-child relationships
	std::vector<std::shared_ptr<SceneNode>> childSceneNodes;
	json.at("m_childSceneNodes").get_to(childSceneNodes);
	for (const std::shared_ptr<SceneNode>& childSceneNode : childSceneNodes)
	{
		sceneNode.AddChild(childSceneNode);
	}
}

// SceneHierarchy
inline void to_json(nlohmann::json& json, const SceneHierarchy& sceneHierarchy)
{
	json = {
		{"m_rootSceneNode", sceneHierarchy.m_rootSceneNode}
	};
}

inline void from_json(const nlohmann::json& json, SceneHierarchy& sceneHierarchy)
{
	json.at("m_rootSceneNode").get_to(sceneHierarchy.m_rootSceneNode);
}
