#pragma once
#include <memory>
#include <string>
#include <vector>

#include "../nlohmann/json.hpp"

class Primitive;

class PrimitiveManager
{
public:
    PrimitiveManager();
    ~PrimitiveManager() = default;
    
    void AddPrimitive(const std::string& primitiveName, std::shared_ptr<Primitive> primitive);
    void AddPrimitive(const std::string& filePath);
    const std::shared_ptr<Primitive>& GetPrimitive(const std::string& primitiveName) const;

    std::vector<std::string> GetPrimitiveNames() const;

    friend void to_json(nlohmann::json& json, const PrimitiveManager& openGLPrimitiveManager);
    friend void from_json(const nlohmann::json& json, PrimitiveManager& openGLPrimitiveManager);

private:
    void GeneratePrimitives(int sphereLatitudinalResolution, int sphereLongitudinalResolution);
    
    std::unordered_map<std::string, std::shared_ptr<Primitive>> m_primitives;
};
