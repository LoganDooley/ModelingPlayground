#pragma once
#include "../OpenGLHelpers/OpenGLRenderer.h"
#include "../Scene/SceneHierarchy.h"

class SceneLoader
{
public:
	static bool LoadScene(const std::shared_ptr<SceneHierarchy>& sceneHierarchy,
	                      const std::shared_ptr<OpenGLRenderer>& openGLRenderer, const char* sceneFilePath = nullptr);

	static bool SaveScene(const std::shared_ptr<SceneHierarchy>& sceneHierarchy,
	                      const std::shared_ptr<OpenGLRenderer>& openGLRenderer, const char* sceneFilePath);
};
