#include "PrimitiveManager.h"

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Primitive.h"
#include "../../Utils/PrimitiveGenerator.h"
#include "../../Utils/SceneLoader.h"

PrimitiveManager::PrimitiveManager()
{
    GeneratePrimitives(10, 10);
}

void PrimitiveManager::AddPrimitive(const std::string& primitiveName, std::shared_ptr<Primitive> primitive)
{
    if (m_primitives.contains(primitiveName))
    {
        std::cout << "PrimitiveManager|AddPrimitive: " << primitiveName << " already exists!\n";
        return;
    }
    m_primitives[primitiveName] = primitive;
}

void PrimitiveManager::AddPrimitive(const std::string& filePath)
{
    if (filePath.empty())
    {
        std::cout << "PrimitiveManager|AddPrimitive: No file path specified.\n";
        return;
    }

    if (m_primitives.contains(filePath))
    {
        std::cout << "PrimitiveManager|AddPrimitive: " << filePath << " already exists!\n";
        return;
    }

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        filePath, aiProcess_OptimizeMeshes | aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "PrimitiveManager|AddPrimitive: ERROR::ASSIMP::" << importer.GetErrorString() << "\n";
        return;
    }

    if (!scene->HasMeshes())
    {
        std::cout << "PrimitiveManager|AddPrimitive: Selected file has no meshes!\n";
        return;
    }

    aiMesh* mesh = scene->mMeshes[0];
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    bool hasTexCoords;
    SceneLoader::ProcessMesh(mesh, vertices, indices, hasTexCoords);

    std::vector<VertexAttribute> vertexAttributes = {VertexAttribute::PositionF3, VertexAttribute::NormalF3};
    if (hasTexCoords)
    {
        vertexAttributes.push_back(VertexAttribute::UVF2);
    }

    m_primitives[filePath] = std::make_shared<Primitive>(vertices, indices, vertexAttributes);
}

const std::shared_ptr<Primitive>& PrimitiveManager::GetPrimitive(const std::string& primitiveName) const
{
    return m_primitives.at(primitiveName);
}

const std::unordered_map<std::string, std::shared_ptr<Primitive>>& PrimitiveManager::GetAllPrimitives() const
{
    return m_primitives;
}

void PrimitiveManager::SubscribeToOnPrimitiveAdded(std::function<void()> callback)
{
    m_onPrimitiveAddedCallbacks.push_back(callback);
}

std::vector<std::string> PrimitiveManager::GetPrimitiveNames() const
{
    std::vector<std::string> names;
    for (const auto& name : m_primitives | std::views::keys)
    {
        names.push_back(name);
    }
    return names;
}

void PrimitiveManager::GeneratePrimitives(int sphereLatitudinalResolution, int sphereLongitudinalResolution)
{
    // Initialize Triangle
    std::vector<float> triangleVertices = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0, 0,
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1, 0,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5, 1
    };

    std::vector<unsigned int> triangleIndices = {
        0, 1, 2
    };

    m_primitives["Triangle"] = std::make_shared<Primitive>(triangleVertices, triangleIndices,
                                                           std::vector{
                                                               VertexAttribute::PositionF3,
                                                               VertexAttribute::NormalF3,
                                                               VertexAttribute::UVF2
                                                           });


    std::pair<std::vector<float>, std::vector<unsigned int>> sphereData = PrimitiveGenerator::GenerateSphere(
        sphereLatitudinalResolution, sphereLongitudinalResolution);
    m_primitives["Sphere"] = std::make_shared<Primitive>(sphereData.first, sphereData.second, std::vector{
                                                             VertexAttribute::PositionF3,
                                                             VertexAttribute::NormalF3,
                                                             VertexAttribute::UVF2
                                                         });

    std::pair<std::vector<float>, std::vector<unsigned int>> cubeData = PrimitiveGenerator::GenerateCube();
    m_primitives["Cube"] = std::make_shared<Primitive>(cubeData.first, cubeData.second, std::vector{
                                                           VertexAttribute::PositionF3, VertexAttribute::NormalF3,
                                                           VertexAttribute::UVF2
                                                       });
}
