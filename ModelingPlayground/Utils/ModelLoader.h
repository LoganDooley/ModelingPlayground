#pragma once

#include <memory>
#include <string>

class SceneNodeGenerator;
class SceneNode;

class ModelLoader
{
public:
	static void LoadModel(const std::shared_ptr<SceneNode>& parentSceneNode, const std::string& filePath,
	                      std::shared_ptr<SceneNodeGenerator> sceneNodeGenerator);
};
