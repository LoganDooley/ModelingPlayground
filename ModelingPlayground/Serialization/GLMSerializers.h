#pragma once

namespace glm
{
    inline void to_json(nlohmann::json& json, const glm::vec2& vector)
    {
        json = {
            {"x", vector.x},
            {"y", vector.y}
        };
    }

    inline void from_json(const nlohmann::json& json, glm::vec2& vector)
    {
        vector.x = json.at("x").get<float>();
        vector.y = json.at("y").get<float>();
    }

    inline void to_json(nlohmann::json& json, const glm::vec3& vector)
    {
        json = {
            {"x", vector.x},
            {"y", vector.y},
            {"z", vector.z}
        };
    }

    inline void from_json(const nlohmann::json& json, glm::vec3& vector)
    {
        vector.x = json.at("x").get<float>();
        vector.y = json.at("y").get<float>();
        vector.z = json.at("z").get<float>();
    }

    inline void to_json(nlohmann::json& json, const glm::vec4& vector)
    {
        json = {
            {"x", vector.x},
            {"y", vector.y},
            {"z", vector.z},
            {"w", vector.w}
        };
    }

    inline void from_json(const nlohmann::json& json, glm::vec4& vector)
    {
        vector.x = json.at("x").get<float>();
        vector.y = json.at("y").get<float>();
        vector.z = json.at("z").get<float>();
        vector.w = json.at("w").get<float>();
    }
}