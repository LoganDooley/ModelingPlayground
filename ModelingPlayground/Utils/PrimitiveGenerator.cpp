#include "PrimitiveGenerator.h"

#include <iostream>
#include <glm/geometric.hpp>
#include <glm/ext/scalar_constants.hpp>

std::tuple<std::vector<glm::vec3>, std::vector<glm::vec3>, std::vector<int>> PrimitiveGenerator::GenerateSphere(int latitudinalResolution,
                                                                                       int longitudinalResolution)
{
    if (latitudinalResolution < 3)
    {
        std::cout<<"PrimitiveGenerator|GenerateSphere: Warning! latitudinalResolution must be at least 3, generating a "
                   "sphere with latitudinalResolution = 3\n";
        latitudinalResolution = 3;
    }

    if (longitudinalResolution < 2)
    {
        std::cout<<"PrimitiveGenerator|GenerateSphere: Warning! longitudinalResolution must be at least 2, generating a"
                   "sphere with longitudinalResolution = 2\n";
        longitudinalResolution = 2;
    }
    
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<int> indices;

    // First generate vertices
    for (int latitudeIndex = 0; latitudeIndex <= latitudinalResolution; latitudeIndex++)
    {
        if (latitudeIndex == 0)
        {
            // Add the bottom vertex
            vertices.emplace_back(0, -0.5, 0);
            normals.emplace_back(0, -1, 0);
        }
        else if (latitudeIndex == latitudinalResolution)
        {
            // Add the top vertex
            vertices.emplace_back(0, 0.5, 0);
            normals.emplace_back(0, 1, 0);
        }
        else
        {
            // Generate a ring of points at this latitude
            float phi = glm::pi<float>() * (1 - static_cast<float>(latitudeIndex)/static_cast<float>(latitudinalResolution));
            
            for (int longitudeIndex = 0; longitudeIndex < longitudinalResolution; longitudeIndex++)
            {
                float theta = 2 * glm::pi<float>() * (static_cast<float>(longitudeIndex)/static_cast<float>(longitudinalResolution));
                vertices.emplace_back(SphericalToCartesian(phi, theta, 0.5f));
                normals.emplace_back(glm::normalize(vertices[vertices.size() - 1]));
            }
        }
    }

    for (int latitudeIndex = 0; latitudeIndex < latitudinalResolution; latitudeIndex++)
    {
        for (int longitudeIndex = 0; longitudeIndex < longitudinalResolution; longitudeIndex++)
        {
            if (latitudeIndex == 0)
            {
                // Generate one triangle
                indices.emplace_back(GetSphereVertexIndex(0, longitudeIndex,
                    latitudinalResolution, longitudinalResolution));
                indices.emplace_back(GetSphereVertexIndex(1, longitudeIndex + 1,
                    latitudinalResolution, longitudinalResolution));
                indices.emplace_back(GetSphereVertexIndex(1, longitudeIndex,
                    latitudinalResolution, longitudinalResolution));
            }
            else if (latitudeIndex == latitudinalResolution - 1)
            {
                // Generate one triangle
                indices.emplace_back(GetSphereVertexIndex(latitudeIndex, longitudeIndex,
                    latitudinalResolution, longitudinalResolution));
                indices.emplace_back(GetSphereVertexIndex(latitudeIndex, longitudeIndex+1,
                    latitudinalResolution, longitudinalResolution));
                indices.emplace_back(GetSphereVertexIndex(latitudeIndex+1, longitudeIndex,
                    latitudinalResolution, longitudinalResolution));
            }
            else
            {
                // Generate two triangles
                indices.emplace_back(GetSphereVertexIndex(latitudeIndex, longitudeIndex,
                    latitudinalResolution, longitudinalResolution));
                indices.emplace_back(GetSphereVertexIndex(latitudeIndex, longitudeIndex+1,
                    latitudinalResolution, longitudinalResolution));
                indices.emplace_back(GetSphereVertexIndex(latitudeIndex+1, longitudeIndex,
                    latitudinalResolution, longitudinalResolution));

                indices.emplace_back(GetSphereVertexIndex(latitudeIndex+1, longitudeIndex,
                    latitudinalResolution, longitudinalResolution));
                indices.emplace_back(GetSphereVertexIndex(latitudeIndex, longitudeIndex+1,
                    latitudinalResolution, longitudinalResolution));
                indices.emplace_back(GetSphereVertexIndex(latitudeIndex+1, longitudeIndex+1,
                    latitudinalResolution, longitudinalResolution));
            }
        }
    }

    return {vertices, normals, indices};
}

std::tuple<std::vector<glm::vec3>, std::vector<glm::vec3>> PrimitiveGenerator::GenerateCube()
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;

    GenerateCubeFace(vertices, normals, glm::vec3(1, 0, 0));
    GenerateCubeFace(vertices, normals, glm::vec3(-1, 0, 0));
    GenerateCubeFace(vertices, normals, glm::vec3(0, 1, 0));
    GenerateCubeFace(vertices, normals, glm::vec3(0, -1, 0));
    GenerateCubeFace(vertices, normals, glm::vec3(0, 0, 1));
    GenerateCubeFace(vertices, normals, glm::vec3(0, 0, -1));

    return {vertices, normals};
}

glm::vec3 PrimitiveGenerator::SphericalToCartesian(float phi, float theta, float r)
{
    // Using y up coordinates
    float z = r * std::sin(phi) * std::cos(theta);
    float x = r * std::sin(phi) * std::sin(theta);
    float y = r * std::cos(phi);
    
    return glm::vec3(x, y, z);
}

int PrimitiveGenerator::GetSphereVertexIndex(int latitudeIndex, int longitudeIndex, int latitudinalResolution,
    int longitudinalResolution)
{
    // Wrap around the sphere
    longitudeIndex = longitudeIndex % longitudinalResolution;
    
    if (latitudeIndex == 0)
    {
        return 0;
    }
    if (latitudeIndex == latitudinalResolution)
    {
        return 1 + longitudinalResolution * (latitudinalResolution - 1);
    }
    return 1 + (latitudeIndex - 1) * longitudinalResolution + longitudeIndex;
}

void PrimitiveGenerator::GenerateCubeFace(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals,
    glm::vec3 faceNormal)
{
    for (int i = 0; i<6; i++)
    {
        normals.emplace_back(faceNormal);
    }

    int indexA;
    int indexB;
    
    if (faceNormal.x < 0)
    {
        indexA = 2;
        indexB = 1;
    }
    else if (faceNormal.x > 0)
    {
        indexA = 1;
        indexB = 2;
    }
    else if (faceNormal.y < 0)
    {
        indexA = 0;
        indexB = 2;
    }
    else if (faceNormal.y > 0)
    {
        indexA = 2;
        indexB = 0;
    }
    else if (faceNormal.z < 0)
    {
        indexA = 1;
        indexB = 0;
    }
    else
    {
        indexA = 0;
        indexB = 1;
    }

    glm::vec3 v0 = faceNormal/2.f;
    v0[indexA] = 0.5;
    v0[indexB] = 0.5;

    glm::vec3 v1 = faceNormal/2.f;
    v1[indexA] = 0.5;
    v1[indexB] = -0.5;

    glm::vec3 v2 = faceNormal/2.f;
    v2[indexA] = -0.5;
    v2[indexB] = -0.5;

    glm::vec3 v3 = faceNormal/2.f;
    v3[indexA] = -0.5;
    v3[indexB] = 0.5;

    vertices.emplace_back(v0);
    vertices.emplace_back(v3);
    vertices.emplace_back(v1);

    vertices.emplace_back(v1);
    vertices.emplace_back(v3);
    vertices.emplace_back(v2);
}
