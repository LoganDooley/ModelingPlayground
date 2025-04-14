#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "OpenGLPrimitive.h"
#include "../nlohmann/json_fwd.hpp"

class OpenGLPrimitiveManager
{
public:
	OpenGLPrimitiveManager();
	~OpenGLPrimitiveManager() = default;

	void GeneratePrimitives(int sphereLatitudinalResolution, int sphereLongitudinalResolution);
	void AddPrimitive(const std::string& primitiveName, std::shared_ptr<OpenGLPrimitive> primitive);
	void AddPrimitive(const std::string& filePath);

	void DrawPrimitive(const std::string& primitiveName);
	std::vector<std::string> GetPrimitiveNames() const;

	friend void to_json(nlohmann::json& json, const OpenGLPrimitiveManager& openGLPrimitiveManager);
	friend void from_json(const nlohmann::json& json, OpenGLPrimitiveManager& openGLPrimitiveManager);

private:
	std::unordered_map<std::string, std::shared_ptr<OpenGLPrimitive>> m_primitives;
};
