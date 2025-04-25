#pragma once

#include <glm/vec3.hpp>

#include "glad/glad.h"

#include "../nlohmann/json_fwd.hpp"

enum class VertexAttribute : int
{
    Position = 0,
    Normal = 1,
    UV = 2
};

inline int GetSize(VertexAttribute vertexAttribute)
{
    switch (vertexAttribute)
    {
    case VertexAttribute::Position:
    case VertexAttribute::Normal:
        return 3;
    case VertexAttribute::UV:
        return 2;
    }

    return 0;
}

// Wrapper class used to store object geometry
class OpenGLPrimitive
{
public:
    OpenGLPrimitive();
    OpenGLPrimitive(const std::vector<float>& vertices, const std::vector<int>& indices,
                    const std::vector<VertexAttribute>& vertexAttributeLayout);
    ~OpenGLPrimitive();

    void CreateOpenGLObjects();
    void Draw() const;

    friend void to_json(nlohmann::json& json, const OpenGLPrimitive& openGLPrimitive);
    friend void from_json(const nlohmann::json& json, OpenGLPrimitive& openGLPrimitive);

    bool Raycast(glm::vec3 p, glm::vec3 d) const;

private:
    bool IsVertexAttributeLayoutSupported() const;
    bool RaycastTriangle(glm::vec3 p, glm::vec3 d, int i0, int i1, int i2) const;

    GLuint m_vbo;
    GLuint m_vao;
    GLuint m_ebo;

    std::vector<VertexAttribute> m_vertexAttributeLayout;
    std::vector<float> m_vertices;
    std::vector<int> m_indices;
};
