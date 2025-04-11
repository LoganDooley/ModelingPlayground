#pragma once

#include "../nlohmann/json.hpp"
#include "../OpenGLHelpers/OpenGLPrimitiveManager.h"
#include "../Serialization/SharedPointerSerializer.h" // Don't remove this!

inline void to_json(nlohmann::json& json, const OpenGLPrimitive& openGLPrimitive)
{
	json = {
		{"m_vertexAttributeLayout", openGLPrimitive.m_vertexAttributeLayout},
		{"m_vertices", openGLPrimitive.m_vertices},
		{"m_indices", openGLPrimitive.m_indices}
	};
}

inline void from_json(const nlohmann::json& json, OpenGLPrimitive& openGLPrimitive)
{
	json.at("m_vertexAttributeLayout").get_to(openGLPrimitive.m_vertexAttributeLayout);
	json.at("m_vertices").get_to(openGLPrimitive.m_vertices);
	json.at("m_indices").get_to(openGLPrimitive.m_indices);
}

inline void to_json(nlohmann::json& json, const OpenGLPrimitiveManager& openGLPrimitiveManager)
{
	json = {
		{"m_primitives", openGLPrimitiveManager.m_primitives},
	};
}

inline void from_json(const nlohmann::json& json, OpenGLPrimitiveManager& openGLPrimitiveManager)
{
	json.at("m_primitives").get_to(openGLPrimitiveManager.m_primitives);
}
