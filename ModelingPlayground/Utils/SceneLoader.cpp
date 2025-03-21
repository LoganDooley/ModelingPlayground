#include "SceneLoader.h"

#include <fstream>

#include "../Serialization/Serializers.h"

void SceneLoader::Initialize()
{
    PolymorphicSerializer<Component>::RegisterTypes<DirectionalLightComponent,
    MaterialComponent,
    OpenGLSettingsComponent,
    PointLightComponent,
    PrimitiveComponent,
    SpotLightComponent,
    TransformComponent
    >();
}

bool SceneLoader::LoadScene(const std::shared_ptr<SceneHierarchy>& sceneHierarchy,
                            const std::shared_ptr<OpenGLRenderer>& openGLRenderer, const char* sceneFilePath)
{
    Initialize();
    
    if (sceneFilePath == "")
    {
        // Create new scene
        *sceneHierarchy = SceneHierarchy();
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

    nlohmann::json sceneJson = nlohmann::json::parse(file);
    SceneHierarchy newSceneHierarchy = sceneJson;
    *sceneHierarchy = std::move(newSceneHierarchy);
    openGLRenderer->SetSceneHierarchy(sceneHierarchy);
    return true;
}

bool SceneLoader::SaveScene(const std::shared_ptr<SceneHierarchy>& sceneHierarchy, const char* sceneFilePath)
{
    Initialize();
    
    nlohmann::json sceneJson = sceneHierarchy;
    std::ofstream sceneFile(sceneFilePath);
    if (sceneFile.fail())
    {
        std::cerr << "Error opening file " << sceneFilePath <<"\n";
    }
    sceneFile << sceneJson;
    return true;
}
