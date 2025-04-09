#pragma once

#include <memory>
#include <string>

class OpenGLRenderer;
class SceneNodeGenerator;
class SceneNode;

class ModelLoader
{
public:
	static void LoadModel(const std::shared_ptr<SceneNode>& parentSceneNode, const std::string& filePath,
	                      const std::shared_ptr<OpenGLRenderer>& openGLRenderer,
	                      std::shared_ptr<SceneNodeGenerator> sceneNodeGenerator);
};
