#pragma once
#include <utility>
#include <vector>
#include <glm/vec3.hpp>

/// Generator for discretized unit shapes (shapes fitting within the unit square centered at (0, 0, 0))
class PrimitiveGenerator
{
public:
    static std::pair<std::vector<glm::vec3>, std::vector<int>> GenerateSphere(int latitudinalResolution, int longitudinalResolution);
    static std::pair<std::vector<glm::vec3>, std::vector<int>> GenerateCube();

private:
    // Converts phi, theta (both in radians), and r to cartesian xyz coordinates
    static glm::vec3 SphericalToCartesian(float phi, float theta, float r);
    static int GetSphereVertexIndex(int latitudeIndex, int longitudeIndex, int latitudinalResolution, int longitudinalResolution);
};
