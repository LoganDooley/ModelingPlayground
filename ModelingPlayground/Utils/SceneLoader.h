#pragma once
#include "../OpenGLHelpers/OpenGLRenderer.h"
#include "../Scene/SceneHierarchy.h"

struct aiMesh;
struct aiScene;
struct aiNode;

class SceneLoader
{
public:
	static bool LoadScene(const std::shared_ptr<SceneHierarchy>& sceneHierarchy,
	                      const std::shared_ptr<OpenGLRenderer>& openGLRenderer, const char* sceneFilePath = nullptr);

	static bool LoadExternalScene(const std::shared_ptr<SceneHierarchy>& sceneHierarchy,
	                              const std::shared_ptr<OpenGLRenderer>& openGLRenderer,
	                              const char* sceneFilePath = nullptr);

	static bool SaveScene(const std::shared_ptr<SceneHierarchy>& sceneHierarchy,
	                      const std::shared_ptr<OpenGLRenderer>& openGLRenderer, const char* sceneFilePath);

private:
	static void ProcessNodeForPrimitives(aiNode* node, const aiScene* scene,
	                                     const std::shared_ptr<SceneNode>& parentSceneNode,
	                                     const std::string& primitiveNameBase,
	                                     const std::shared_ptr<OpenGLRenderer>& openGLRenderer,
	                                     const std::shared_ptr<SceneHierarchy>& sceneHierarchy);
	static void AddPrimitiveNodes(aiNode* node, const aiScene* scene, glm::vec3 position, glm::vec3 rotation,
	                              glm::vec3 scale,
	                              const std::shared_ptr<SceneNode>& parentSceneNode,
	                              const std::string& primitiveNameBase,
	                              const std::shared_ptr<OpenGLRenderer>& openGLRenderer,
	                              const std::shared_ptr<SceneHierarchy>& sceneHierarchy);
	static void ProcessMesh(aiMesh* mesh, std::vector<float>& vertices, std::vector<int>& indices, bool& hasTexCoords);
	static void ProcessLights(const aiScene* scene,
	                          const std::shared_ptr<OpenGLRenderer>& openGLRenderer,
	                          const std::shared_ptr<SceneHierarchy>& sceneHierarchy);
};
