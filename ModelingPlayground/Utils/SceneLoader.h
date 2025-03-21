#pragma once
#include "../Scene/SceneHierarchy.h"
#include "../OpenGLHelpers/OpenGLRenderer.h"

class SceneLoader
{
public:
    static bool LoadScene(const std::shared_ptr<SceneHierarchy>& sceneHierarchy, const std::shared_ptr<OpenGLRenderer>& openGLRenderer, const char* sceneFilePath = "");

    static bool SaveScene(const std::shared_ptr<SceneHierarchy>& sceneHierarchy, const char* sceneFilePath);

private:
    static void Initialize();
};
