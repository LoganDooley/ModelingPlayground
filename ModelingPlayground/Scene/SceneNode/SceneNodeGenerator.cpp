﻿#include "SceneNodeGenerator.h"

#include <iostream>
#include <unordered_set>

#include "../Object.h"
#include "../../Application/Rendering/Renderers/OpenGLRenderer.h"
#include "../Components/DirectionalLightComponent.h"
#include "../Components/MaterialComponent.h"
#include "../Components/PointLightComponent.h"
#include "../Components/PrimitiveComponent.h"
#include "../Components/SpotLightComponent.h"
#include "../Components/TransformComponent.h"
#include "../SceneNode/SceneNode.h"

std::shared_ptr<SceneNode> SceneNodeGenerator::CreateSceneNodeAndAddAsChild(
	SceneNodeType sceneNodeType, const std::shared_ptr<SceneNode>& parent,
	const std::shared_ptr<RenderingManager>& renderingManager,
	const std::shared_ptr<SceneHierarchy>& sceneHierarchy,
	std::string sceneNodeName)
{
	auto sceneNode = std::make_shared<SceneNode>(GetSceneNodeName(sceneNodeType, parent));
	switch (sceneNodeType)
	{
	case SceneNodeType::Primitive:
		InitializePrimitiveObject(sceneNode->GetObject(), renderingManager);
		break;
	case SceneNodeType::DirectionalLight:
		InitializeDirectionalLightObject(sceneNode->GetObject());
		break;
	case SceneNodeType::PointLight:
		InitializePointLightObject(sceneNode->GetObject());
		break;
	case SceneNodeType::SpotLight:
		InitializeSpotLightObject(sceneNode->GetObject());
		break;
	case SceneNodeType::Transform:
		InitializeTransformObject(sceneNode->GetObject());
		break;
	default:
		std::cout << "SceneNodeGenerator|CreateSceneNodeAndAddAsChild: Invalid Scene Node Type!\n";
	}

	// Set parent child relationship
	parent->AddChild(sceneNode);
	sceneNode->SetParent(parent);

	sceneNode->RegisterTransformModelMatrix();

	sceneHierarchy->OnSceneNodeAdded(sceneNode);

	return sceneNode;
}

std::vector<SceneNodeType> SceneNodeGenerator::GetSceneNodeTypes()
{
	return {
		SceneNodeType::Primitive,
		SceneNodeType::DirectionalLight,
		SceneNodeType::PointLight,
		SceneNodeType::SpotLight,
		SceneNodeType::Transform,
	};
}

std::string SceneNodeGenerator::GetSceneNodeTypeName(SceneNodeType sceneNodeType)
{
	switch (sceneNodeType)
	{
	case SceneNodeType::Primitive:
		return "Primitive";
	case SceneNodeType::DirectionalLight:
		return "Directional Light";
	case SceneNodeType::PointLight:
		return "Point Light";
	case SceneNodeType::SpotLight:
		return "Spot Light";
	case SceneNodeType::Transform:
		return "Transform";
	}
	return "Unknown Type";
}

void SceneNodeGenerator::InitializePrimitiveObject(Object& object,
                                                   const std::shared_ptr<RenderingManager>& renderingManager)
{
	object.AddComponent<TransformComponent>();
	object.AddComponent<PrimitiveComponent>(renderingManager);
	object.AddComponent<MaterialComponent>(renderingManager);
}

void SceneNodeGenerator::InitializeDirectionalLightObject(Object& object)
{
	object.AddComponent<TransformComponent>();
	object.AddComponent<DirectionalLightComponent>();
}

void SceneNodeGenerator::InitializePointLightObject(Object& object)
{
	object.AddComponent<TransformComponent>();
	object.AddComponent<PointLightComponent>();
}

void SceneNodeGenerator::InitializeSpotLightObject(Object& object)
{
	object.AddComponent<TransformComponent>();
	object.AddComponent<SpotLightComponent>();
}

void SceneNodeGenerator::InitializeTransformObject(Object& object)
{
	object.AddComponent<TransformComponent>();
}

std::string SceneNodeGenerator::GetSceneNodeName(SceneNodeType sceneNodeType,
                                                 const std::shared_ptr<SceneNode>& parent,
                                                 std::string desiredSceneNodeName)
{
	std::string baseName = desiredSceneNodeName;
	if (desiredSceneNodeName.empty())
	{
		switch (sceneNodeType)
		{
		case SceneNodeType::Primitive:
			baseName = "Primitive";
			break;
		case SceneNodeType::DirectionalLight:
			baseName = "DirectionalLight";
			break;
		case SceneNodeType::PointLight:
			baseName = "PointLight";
			break;
		case SceneNodeType::SpotLight:
			baseName = "SpotLight";
			break;
		case SceneNodeType::Transform:
			baseName = "Transform";
			break;
		}
	}

	std::unordered_set<std::string> takenNames;
	for (const std::shared_ptr<SceneNode>& child : parent->GetChildren())
	{
		takenNames.insert(child->GetName());
	}
	if (!takenNames.contains(baseName))
	{
		return baseName;
	}
	for (int i = 0; i < takenNames.size(); i++)
	{
		std::string enumeratedName = baseName + "_" + std::to_string(i + 1);
		if (!takenNames.contains(enumeratedName))
		{
			return enumeratedName;
		}
	}

	// Should never get here
	return baseName;
}
