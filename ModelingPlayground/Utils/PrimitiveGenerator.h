#pragma once
#include <utility>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "../OpenGLHelpers/OpenGLPrimitive.h"

/// Generator for discretized unit shapes (shapes fitting within the unit square centered at (0, 0, 0))
class PrimitiveGenerator
{
public:
    static std::pair<std::vector<OpenGLVertex>, std::vector<int>> GenerateSphere(int latitudinalResolution, int longitudinalResolution);
    static std::vector<OpenGLVertex> GenerateCube();

private:
    // Converts phi, theta (both in radians), and r to cartesian xyz coordinates
    static glm::vec3 SphericalToCartesian(float phi, float theta, float r);
    static int GetSphereVertexIndex(int latitudeIndex, int longitudeIndex, int latitudinalResolution);
    static void GenerateCubeFace(std::vector<OpenGLVertex>& vertices, glm::vec3 faceNormal);
};
