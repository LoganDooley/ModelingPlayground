#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include "../nlohmann/json.hpp"

enum class VertexAttribute
{
    PositionF3,
    NormalF3,
    UVF2
};

inline int GetSize(VertexAttribute vertexAttribute)
{
    switch (vertexAttribute)
    {
    case VertexAttribute::PositionF3:
    case VertexAttribute::NormalF3:
        return 3;
    case VertexAttribute::UVF2:
        return 2;
    }

    return 0;
}


class Primitive
{
public:
    Primitive();
    Primitive(const std::vector<float>& vertices, const std::vector<int>& indices,
              const std::vector<VertexAttribute>& vertexAttributeLayout);
    ~Primitive();

    friend void to_json(nlohmann::json& json, const Primitive& openGLPrimitive);
    friend void from_json(const nlohmann::json& json, Primitive& openGLPrimitive);

    float Raycast(glm::vec3 p, glm::vec3 d) const;

private:
    bool IsVertexAttributeLayoutSupported() const;
    float RaycastTriangle(glm::vec3 p, glm::vec3 d, int i0, int i1, int i2) const;

    std::vector<VertexAttribute> m_vertexAttributeLayout;
    std::vector<float> m_vertices;
    std::vector<int> m_indices;
};
