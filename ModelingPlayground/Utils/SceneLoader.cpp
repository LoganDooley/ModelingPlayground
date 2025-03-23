#include "SceneLoader.h"

#include <fstream>
#include <iostream>

#include "../Serialization/Serializers.h"

bool SceneLoader::LoadScene(const std::shared_ptr<SceneHierarchy>& sceneHierarchy,
                            const std::shared_ptr<OpenGLRenderer>& openGLRenderer, const char* sceneFilePath)
{
	if (sceneFilePath == nullptr)
	{
		// Create new scene
		*sceneHierarchy = SceneHierarchy();
		auto rootSceneNode = std::make_shared<SceneNode>("World");
		rootSceneNode->GetObject().AddComponent<OpenGLSettingsComponent>();
		sceneHierarchy->SetRootSceneNode(rootSceneNode);
		openGLRenderer->SetSceneHierarchy(sceneHierarchy);
		return true;
	}

	// Load scene from path
	std::ifstream file(sceneFilePath);
	if (file.fail())
	{
		std::cerr << "Error opening file " << sceneFilePath << "\n";
		return false;
	}

	nlohmann::json sceneJson = nlohmann::json::parse(file);
	SceneHierarchy newSceneHierarchy = sceneJson;
	*sceneHierarchy = std::move(newSceneHierarchy);
	openGLRenderer->SetSceneHierarchy(sceneHierarchy);
	sceneHierarchy->BreadthFirstProcessAllSceneNodes([openGLRenderer](std::shared_ptr<SceneNode> node)
	{
		if (std::shared_ptr<TransformComponent> transformComponent = node->GetObject().GetFirstComponentOfType<
			TransformComponent>())
		{
			transformComponent->SetOpenGLRenderer(openGLRenderer);
		}
	});
	sceneHierarchy->SetFilePath(sceneFilePath);
	return true;
}

bool SceneLoader::SaveScene(const std::shared_ptr<SceneHierarchy>& sceneHierarchy, const char* sceneFilePath)
{
	if (sceneFilePath == nullptr)
	{
		std::cout << "SceneLoader|SaveScene: No file path specified.\n";
		return false;
	}

	nlohmann::json sceneJson = sceneHierarchy;
	std::ofstream sceneFile(sceneFilePath);
	if (sceneFile.fail())
	{
		std::cerr << "Error opening file " << sceneFilePath << "\n";
	}
	sceneFile << sceneJson;
	sceneHierarchy->SetFilePath(sceneFilePath);
	return true;
}
