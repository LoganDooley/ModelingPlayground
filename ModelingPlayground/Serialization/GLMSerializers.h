#pragma once

namespace glm
{
	inline void to_json(nlohmann::json& json, const vec2& vector)
	{
		json = {
			{"x", vector.x},
			{"y", vector.y}
		};
	}

	inline void from_json(const nlohmann::json& json, vec2& vector)
	{
		vector.x = json.at("x").get<float>();
		vector.y = json.at("y").get<float>();
	}

	inline void to_json(nlohmann::json& json, const vec3& vector)
	{
		json = {
			{"x", vector.x},
			{"y", vector.y},
			{"z", vector.z}
		};
	}

	inline void from_json(const nlohmann::json& json, vec3& vector)
	{
		vector.x = json.at("x").get<float>();
		vector.y = json.at("y").get<float>();
		vector.z = json.at("z").get<float>();
	}

	inline void to_json(nlohmann::json& json, const vec4& vector)
	{
		json = {
			{"x", vector.x},
			{"y", vector.y},
			{"z", vector.z},
			{"w", vector.w}
		};
	}

	inline void from_json(const nlohmann::json& json, vec4& vector)
	{
		vector.x = json.at("x").get<float>();
		vector.y = json.at("y").get<float>();
		vector.z = json.at("z").get<float>();
		vector.w = json.at("w").get<float>();
	}
}
