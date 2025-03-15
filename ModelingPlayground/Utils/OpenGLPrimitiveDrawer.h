#pragma once

#include "EnumLabels/PrimitiveTypeEnumLabel.h"
#include "glad/glad.h"

class OpenGLPrimitiveDrawer
{
public:
    OpenGLPrimitiveDrawer(int sphereLatitudinalResolution, int sphereLongitudinalResolution);

    ~OpenGLPrimitiveDrawer() = default;

    void DrawPrimitive(PrimitiveType primitiveType);

private:
    GLuint m_triangleVBO;
    GLuint m_triangleVAO;

    GLuint m_sphereVBO;
    GLuint m_sphereVAO;
    GLuint m_sphereEBO;
    GLuint m_sphereIndexCount;

    GLuint m_cubeVBO;
    GLuint m_cubeVAO;
    GLuint m_cubeEBO;
    GLuint m_cubeIndexCount;
};
