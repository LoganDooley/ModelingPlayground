#include "Primitive.h"

#include <iostream>
#include <glm/detail/func_geometric.inl>

Primitive::Primitive()
{
}

Primitive::Primitive(const std::vector<float>& vertices, const std::vector<int>& indices,
                     const std::vector<VertexAttribute>& vertexAttributeLayout):
    m_vertexAttributeLayout(vertexAttributeLayout),
    m_vertices(vertices),
    m_indices(indices)
{
    if (vertices.empty())
    {
        std::cout << "Primitive: Created with no vertices!\n";
        return;
    }

    if (indices.empty())
    {
        std::cout << "Primitive: Created with no indices!\n";
        return;
    }

    if (!IsVertexAttributeLayoutSupported())
    {
        std::cout << "Primitive: Vertex layout is not supported!\n";
        return;
    }
}

Primitive::~Primitive()
{
}

float Primitive::Raycast(glm::vec3 p, glm::vec3 d) const
{
    float t = std::numeric_limits<float>::max();
    for (int i = 0; i < m_indices.size(); i += 3)
    {
        float triangleT = RaycastTriangle(p, d, i, i + 1, i + 2);
        if (triangleT > 0 && triangleT < t)
        {
            t = triangleT;
        }
    }
    return t;
}

bool Primitive::IsVertexAttributeLayoutSupported() const
{
    if (m_vertexAttributeLayout.size() >= 2)
    {
        if (m_vertexAttributeLayout[0] != VertexAttribute::PositionF3)
        {
            return false;
        }
        if (m_vertexAttributeLayout[1] != VertexAttribute::NormalF3)
        {
            return false;
        }
        if (m_vertexAttributeLayout.size() >= 3 && m_vertexAttributeLayout[2] != VertexAttribute::UVF2)
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

float Primitive::RaycastTriangle(glm::vec3 p, glm::vec3 d, int i0, int i1, int i2) const
{
    int vertexSize = 0;
    for (auto vertexAttribute : m_vertexAttributeLayout)
    {
        vertexSize += GetSize(vertexAttribute);
    }

    auto v0 = glm::vec3(m_vertices[vertexSize * i0], m_vertices[vertexSize * i0 + 1], m_vertices[vertexSize * i0 + 2]);
    auto v1 = glm::vec3(m_vertices[vertexSize * i1], m_vertices[vertexSize * i1 + 1], m_vertices[vertexSize * i1 + 2]);
    auto v2 = glm::vec3(m_vertices[vertexSize * i2], m_vertices[vertexSize * i2 + 1], m_vertices[vertexSize * i2 + 2]);

    glm::vec3 e1 = v1 - v0;
    glm::vec3 e2 = v2 - v0;

    glm::vec3 pVec = glm::cross(d, e2);
    float det = glm::dot(e1, pVec);

    if (det > 0.0001f && det < 0.0001f)
    {
        return -1;
    }

    float invDet = 1.0f / det;
    glm::vec3 tVec = p - v0;
    float u = glm::dot(tVec, pVec) * invDet;
    if (u < 0.0f || u > 1.0f)
    {
        return -1;
    }

    glm::vec3 qVec = glm::cross(tVec, e1);
    float v = glm::dot(d, qVec) * invDet;
    if (v < 0.0f || u + v > 1.0f)
    {
        return -1;
    }

    return glm::dot(e2, qVec) * invDet;
}
