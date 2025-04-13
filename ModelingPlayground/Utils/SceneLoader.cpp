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

		// Update opengl helpers
		auto openGLPrimitiveManager = new OpenGLPrimitiveManager();
		openGLPrimitiveManager->GeneratePrimitives(10, 10);
		openGLRenderer->ResetOpenGLPrimitiveManager(openGLPrimitiveManager);
		openGLRenderer->ResetOpenGLTextureCache(new OpenGLTextureCache());
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
	SceneHierarchy newSceneHierarchy;
	OpenGLPrimitiveManager* newOpenGLPrimitiveManager = new OpenGLPrimitiveManager();
	OpenGLTextureCache* newOpenGLTextureCache = new OpenGLTextureCache();
	sceneJson.at("SceneHierarchy").get_to(newSceneHierarchy);
	sceneJson.at("OpenGLPrimitiveManager").get_to(*newOpenGLPrimitiveManager);
	sceneJson.at("OpenGLTextureCache").get_to(*newOpenGLTextureCache);
	*sceneHierarchy = std::move(newSceneHierarchy);

	// Update opengl helpers
	openGLRenderer->ResetOpenGLPrimitiveManager(newOpenGLPrimitiveManager);
	openGLRenderer->ResetOpenGLTextureCache(newOpenGLTextureCache);
	openGLRenderer->SetSceneHierarchy(sceneHierarchy);

	sceneHierarchy->BreadthFirstProcessAllSceneNodes([openGLRenderer](std::shared_ptr<SceneNode> node)
	{
		if (std::shared_ptr<TransformComponent> transformComponent = node->GetObject().GetFirstComponentOfType<
			TransformComponent>())
		{
			node->RegisterTransformModelMatrix();
		}
		if (std::shared_ptr<PrimitiveComponent> primitiveComponent = node->GetObject().GetFirstComponentOfType<
			PrimitiveComponent>())
		{
			primitiveComponent->SetOpenGLRenderer(openGLRenderer);
		}
		if (std::shared_ptr<MaterialComponent> materialComponent = node->GetObject().GetFirstComponentOfType<
			MaterialComponent>())
		{
			materialComponent->SetOpenGLRenderer(openGLRenderer);
		}
	});
	sceneHierarchy->SetFilePath(sceneFilePath);
	return true;
}

bool SceneLoader::SaveScene(const std::shared_ptr<SceneHierarchy>& sceneHierarchy,
                            const std::shared_ptr<OpenGLRenderer>& openGLRenderer, const char* sceneFilePath)
{
	if (sceneFilePath == nullptr)
	{
		std::cout << "SceneLoader|SaveScene: No file path specified.\n";
		return false;
	}

	nlohmann::json sceneJson = {
		{"SceneHierarchy", sceneHierarchy},
		{"OpenGLPrimitiveManager", openGLRenderer->GetOpenGLPrimitiveManager()},
		{"OpenGLTextureCache", openGLRenderer->GetOpenGLTextureCache()}
	};

	std::ofstream sceneFile(sceneFilePath);
	if (sceneFile.fail())
	{
		std::cerr << "Error opening file " << sceneFilePath << "\n";
	}
	sceneFile << sceneJson;
	sceneHierarchy->SetFilePath(sceneFilePath);
	return true;
}
