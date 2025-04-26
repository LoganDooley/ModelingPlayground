#pragma once
#include <utility>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

/// Generator for discretized unit shapes (shapes fitting within the unit square centered at (0, 0, 0))
class PrimitiveGenerator
{
public:
    static std::pair<std::vector<float>, std::vector<int>> GenerateSphere(
        int latitudinalResolution, int longitudinalResolution);
    static std::pair<std::vector<float>, std::vector<int>> GenerateCube();

private:
    // Converts phi, theta (both in radians), and r to cartesian xyz coordinates
    static glm::vec3 SphericalToCartesian(float phi, float theta, float r);
    static int GetSphereVertexIndex(int latitudeIndex, int longitudeIndex, int latitudinalResolution);
    static void GenerateCubeFace(std::vector<float>& vertices, std::vector<int>& indices, glm::vec3 faceNormal);

    static void EmplaceVertex(std::vector<float>& vertices, glm::vec3 position, glm::vec3 normal, glm::vec2 uv);
};
