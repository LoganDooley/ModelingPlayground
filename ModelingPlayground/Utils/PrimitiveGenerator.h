#pragma once
#include <utility>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

/// Generator for discretized unit shapes (shapes fitting within the unit square centered at (0, 0, 0))
class PrimitiveGenerator
{
public:
    static std::tuple<std::vector<glm::vec3>, std::vector<glm::vec3>, std::vector<glm::vec2>, std::vector<int>> GenerateSphere(int latitudinalResolution, int longitudinalResolution);
    static std::tuple<std::vector<glm::vec3>, std::vector<glm::vec3>, std::vector<glm::vec2>> GenerateCube();

private:
    // Converts phi, theta (both in radians), and r to cartesian xyz coordinates
    static glm::vec3 SphericalToCartesian(float phi, float theta, float r);
    static int GetSphereVertexIndex(int latitudeIndex, int longitudeIndex, int latitudinalResolution);
    static void GenerateCubeFace(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texCoords, glm::vec3 faceNormal);
};
