#include "OpenGLPrimitiveManager.h"

#include <utility>
#include <vector>
#include <glm/vec3.hpp>

#include "../../Utils/PrimitiveGenerator.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <iostream>
#include <ranges>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "../Libraries/tinyobjloader/tiny_obj_loader.h"
#include "../Utils/SceneLoader.h"

OpenGLPrimitiveManager::OpenGLPrimitiveManager():
    m_primitives({})
{
}

void OpenGLPrimitiveManager::GeneratePrimitives(int sphereLatitudinalResolution, int sphereLongitudinalResolution)
{
    // Initialize Triangle
    std::vector<float> triangleVertices = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0, 0,
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1, 0,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5, 1
    };

    std::vector<int> triangleIndices = {
        0, 1, 2
    };

    m_primitives["Triangle"] = std::make_shared<OpenGLPrimitive>(triangleVertices, triangleIndices,
                                                                 std::vector{
                                                                     VertexAttribute::Position, VertexAttribute::Normal,
                                                                     VertexAttribute::UV
                                                                 });


    std::pair<std::vector<float>, std::vector<int>> sphereData = PrimitiveGenerator::GenerateSphere(
        sphereLatitudinalResolution, sphereLongitudinalResolution);
    m_primitives["Sphere"] = std::make_shared<OpenGLPrimitive>(sphereData.first, sphereData.second, std::vector{
                                                                   VertexAttribute::Position, VertexAttribute::Normal,
                                                                   VertexAttribute::UV
                                                               });

    std::pair<std::vector<float>, std::vector<int>> cubeData = PrimitiveGenerator::GenerateCube();
    m_primitives["Cube"] = std::make_shared<OpenGLPrimitive>(cubeData.first, cubeData.second, std::vector{
                                                                 VertexAttribute::Position, VertexAttribute::Normal,
                                                                 VertexAttribute::UV
                                                             });
}

void OpenGLPrimitiveManager::AddPrimitive(const std::string& primitiveName, std::shared_ptr<OpenGLPrimitive> primitive)
{
    if (m_primitives.contains(primitiveName))
    {
        std::cout << "OpenGLPrimitiveManager|AddPrimitive: " << primitiveName << " already exists!\n";
        return;
    }
    m_primitives[primitiveName] = primitive;
}

void OpenGLPrimitiveManager::AddPrimitive(const std::string& filePath)
{
    if (filePath.empty())
    {
        std::cout << "OpenGLPrimitiveManager|AddPrimitive: No file path specified.\n";
        return;
    }

    if (m_primitives.contains(filePath))
    {
        std::cout << "OpenGLPrimitiveManager|AddPrimitive: " << filePath << " already exists!\n";
        return;
    }

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        filePath, aiProcess_OptimizeMeshes | aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "OpenGLPrimitiveManager|AddPrimitive: ERROR::ASSIMP::" << importer.GetErrorString() << "\n";
        return;
    }

    if (!scene->HasMeshes())
    {
        std::cout << "OpenGLPrimitiveManager|AddPrimitive: Selected file has no meshes!\n";
        return;
    }

    aiMesh* mesh = scene->mMeshes[0];
    std::vector<float> vertices;
    std::vector<int> indices;
    bool hasTexCoords;
    SceneLoader::ProcessMesh(mesh, vertices, indices, hasTexCoords);

    std::vector<VertexAttribute> vertexAttributes = {VertexAttribute::Position, VertexAttribute::Normal};
    if (hasTexCoords)
    {
        vertexAttributes.push_back(VertexAttribute::UV);
    }

    m_primitives[filePath] = std::make_shared<OpenGLPrimitive>(vertices, indices, vertexAttributes);
}

const std::shared_ptr<OpenGLPrimitive>& OpenGLPrimitiveManager::GetPrimitive(const std::string& primitiveName) const
{
    return m_primitives.at(primitiveName);
}

void OpenGLPrimitiveManager::DrawPrimitive(const std::string& primitiveName)
{
    if (!m_primitives.contains(primitiveName))
    {
        std::cout << "OpenGLPrimitiveManager|DrawPrimitive| Primitive for " << primitiveName << " not found\n";
        return;
    }
    m_primitives[primitiveName]->Draw();
}

std::vector<std::string> OpenGLPrimitiveManager::GetPrimitiveNames() const
{
    std::vector<std::string> names;
    for (const auto& name : m_primitives | std::views::keys)
    {
        names.push_back(name);
    }
    return names;
}
