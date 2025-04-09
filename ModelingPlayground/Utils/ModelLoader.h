#pragma once

#include <memory>
#include <string>
#include <assimp/scene.h>
#include <glm/vec3.hpp>

class OpenGLRenderer;
class SceneNodeGenerator;
class SceneNode;

class ModelLoader
{
public:
	static void LoadModel(const std::shared_ptr<SceneNode>& parentSceneNode, const std::string& filePath,
	                      const std::shared_ptr<OpenGLRenderer>& openGLRenderer,
	                      const std::shared_ptr<SceneNodeGenerator>& sceneNodeGenerator);

private:
	static void ProcessNode(aiNode* node, const aiScene* scene, const std::shared_ptr<SceneNode>& parentSceneNode,
	                        const std::string& primitiveNameBase,
	                        const std::shared_ptr<OpenGLRenderer>& openGLRenderer,
	                        const std::shared_ptr<SceneNodeGenerator>& sceneNodeGenerator);
	static void AddPrimitiveNodes(aiNode* node, const aiScene* scene, glm::vec3 position, glm::vec3 rotation,
	                              glm::vec3 scale,
	                              const std::shared_ptr<SceneNode>& parentSceneNode,
	                              const std::string& primitiveNameBase,
	                              const std::shared_ptr<OpenGLRenderer>& openGLRenderer,
	                              const std::shared_ptr<SceneNodeGenerator>& sceneNodeGenerator);
	static void ProcessMesh(aiMesh* mesh, std::vector<float>& vertices, std::vector<int>& indices, bool& hasTexCoords);
	static void ProcessLights(const aiScene* scene, const std::shared_ptr<SceneNode>& parentSceneNode,
	                          const std::shared_ptr<SceneNodeGenerator>& sceneNodeGenerator);
};
