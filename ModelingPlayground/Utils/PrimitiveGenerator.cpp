#include "PrimitiveGenerator.h"

#include <iostream>
#include <glm/ext/scalar_constants.hpp>

std::pair<std::vector<glm::vec3>, std::vector<int>> PrimitiveGenerator::GenerateSphere(int latitudinalResolution,
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
    std::vector<int> indices;

    // First generate vertices
    for (int latitudeIndex = 0; latitudeIndex <= latitudinalResolution; latitudeIndex++)
    {
        if (latitudeIndex == 0)
        {
            // Add the bottom vertex
            vertices.emplace_back(0, -0.5, 0);
        }
        else if (latitudeIndex == latitudinalResolution)
        {
            // Add the top vertex
            vertices.emplace_back(0, 0.5, 0);
        }
        else
        {
            // Generate a ring of points at this latitude
            float phi = glm::pi<float>() * (1 - static_cast<float>(latitudeIndex)/static_cast<float>(latitudinalResolution));
            
            for (int longitudeIndex = 0; longitudeIndex < longitudinalResolution; longitudeIndex++)
            {
                float theta = 2 * glm::pi<float>() * (static_cast<float>(longitudeIndex)/static_cast<float>(longitudinalResolution));
                vertices.emplace_back(SphericalToCartesian(phi, theta, 0.5f));
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
                indices.emplace_back(GetSphereVertexIndex(latitudeIndex+1, longitudeIndex+1,
                    latitudinalResolution, longitudinalResolution));
                indices.emplace_back(GetSphereVertexIndex(latitudeIndex, longitudeIndex+1,
                    latitudinalResolution, longitudinalResolution));
            }
        }
    }

    return {vertices, indices};
}

std::pair<std::vector<glm::vec3>, std::vector<int>> PrimitiveGenerator::GenerateCube()
{
    std::vector<glm::vec3> vertices = {
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.5f, 0.5f, -0.5f),
        glm::vec3(0.5f, -0.5f, 0.5f),
        glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(-0.5f, 0.5f, 0.5f),
        glm::vec3(-0.5f, 0.5f, -0.5f),
        glm::vec3(-0.5f, -0.5f, 0.5f),
        glm::vec3(-0.5f, -0.5f, -0.5f),
    };

    std::vector<int> indices = {
        5, 7, 6,
        5, 6, 4,
        0, 2, 3,
        0, 3, 1,
        6, 3, 2,
        6, 7, 3,
        1, 4, 0,
        1, 5, 4,
        4, 6, 2,
        6, 2, 0,
        1, 7, 5,
        1, 3, 7,
    };

    return {vertices, indices};
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
