﻿#pragma once

#include <glm/vec3.hpp>

#include "../Scene/SceneHierarchy.h"

class RenderingManager;

struct aiMesh;
struct aiScene;
struct aiNode;

class SceneLoader
{
public:
    static bool LoadScene(const std::shared_ptr<SceneHierarchy>& sceneHierarchy,
                          const std::shared_ptr<RenderingManager>& renderingManager,
                          const char* sceneFilePath = nullptr);

    static bool LoadExternalScene(const std::shared_ptr<SceneHierarchy>& sceneHierarchy,
                                  const std::shared_ptr<RenderingManager>& renderingManager,
                                  const char* sceneFilePath = nullptr);

    static bool SaveScene(const std::shared_ptr<SceneHierarchy>& sceneHierarchy,
                          const std::shared_ptr<RenderingManager>& renderingManager, const char* sceneFilePath);

    static void ProcessMesh(aiMesh* mesh, std::vector<float>& vertices, std::vector<unsigned int>& indices,
                            bool& hasTexCoords);

private:
    static void ProcessNodeForPrimitives(aiNode* node, const aiScene* scene,
                                         const std::shared_ptr<SceneNode>& parentSceneNode,
                                         const std::string& primitiveNameBase,
                                         const std::shared_ptr<RenderingManager>& renderingManager,
                                         const std::shared_ptr<SceneHierarchy>& sceneHierarchy,
                                         const std::string& sceneFileDirectory);
    static void AddPrimitiveNodes(aiNode* node, const aiScene* scene, glm::vec3 position, glm::vec3 rotation,
                                  glm::vec3 scale,
                                  const std::shared_ptr<SceneNode>& parentSceneNode,
                                  const std::string& primitiveNameBase,
                                  const std::shared_ptr<RenderingManager>& renderingManager,
                                  const std::shared_ptr<SceneHierarchy>& sceneHierarchy,
                                  const std::string& sceneFileDirectory);
    static void ProcessLights(const aiScene* scene,
                              const std::shared_ptr<RenderingManager>& renderingManager,
                              const std::shared_ptr<SceneHierarchy>& sceneHierarchy);
    static void ProcessMaterials(const aiScene* scene, const std::shared_ptr<RenderingManager>& renderingManager,
                                 const std::string& sceneFileDirectory);
};
