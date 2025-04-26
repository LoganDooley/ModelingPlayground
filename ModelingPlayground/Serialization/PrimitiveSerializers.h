#pragma once

#include "../nlohmann/json.hpp"
#include "../Application/Rendering/Primitives/PrimitiveManager.h"
#include "../Application/Rendering/Primitives/Primitive.h"
#include "../Serialization/SharedPointerSerializer.h" // Don't remove this!

inline void to_json(nlohmann::json& json, const Primitive& primitive)
{
    json = {
        {"m_vertexAttributeLayout", primitive.m_vertexAttributeLayout},
        {"m_vertices", primitive.m_vertices},
        {"m_indices", primitive.m_indices}
    };
}

inline void from_json(const nlohmann::json& json, Primitive& primitive)
{
    json.at("m_vertexAttributeLayout").get_to(primitive.m_vertexAttributeLayout);
    json.at("m_vertices").get_to(primitive.m_vertices);
    json.at("m_indices").get_to(primitive.m_indices);
}

inline void to_json(nlohmann::json& json, const PrimitiveManager& primitiveManager)
{
    json = {
        {"m_primitives", primitiveManager.m_primitives},
    };
}

inline void from_json(const nlohmann::json& json, PrimitiveManager& primitiveManager)
{
    json.at("m_primitives").get_to(primitiveManager.m_primitives);
}
