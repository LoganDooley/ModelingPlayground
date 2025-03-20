#include "SceneLoader.h"

#include <fstream>

#include "../Serialization/Serializers.h"

bool SceneLoader::LoadScene(std::shared_ptr<SceneHierarchy>& sceneHierarchy,
    const std::shared_ptr<OpenGLRenderer>& openGLRenderer, const char* sceneFilePath)
{
    if (sceneFilePath == "")
    {
        // Create new scene
        sceneHierarchy = std::make_shared<SceneHierarchy>();
        std::shared_ptr<SceneNode> rootSceneNode = std::make_shared<SceneNode>("World");
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
    nlohmann::json sceneJson;
    std::shared_ptr<SceneHierarchy> newSceneHierarchy;
    try
    {
        sceneJson = nlohmann::json::parse(file);
        newSceneHierarchy = sceneJson;
    }
    catch (const std::exception& e)
    {
        std::cerr<<e.what()<<std::endl;
        return false;
    }
    sceneHierarchy = std::move(newSceneHierarchy);
    openGLRenderer->SetSceneHierarchy(sceneHierarchy);
    return true;
}

bool SceneLoader::SaveScene(const std::shared_ptr<SceneHierarchy>& sceneHierarchy, const char* sceneFilePath)
{
    nlohmann::json sceneJson;
    try
    {
        sceneJson = sceneHierarchy;
    }
    catch (const std::exception& e)
    {
        std::cerr<<e.what()<<std::endl;
        return false;
    }
    std::ofstream sceneFile(sceneFilePath);
    if (sceneFile.fail())
    {
        std::cerr << "Error opening file " << sceneFilePath << std::endl;
    }
    sceneFile << sceneJson;
    return true;
}
