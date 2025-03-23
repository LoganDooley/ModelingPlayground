#pragma once

#include <memory>
#include <string>

#include "OpenGLPrimitive.h"

class OpenGLPrimitiveManager
{
public:
    OpenGLPrimitiveManager();
    ~OpenGLPrimitiveManager() = default;

    void GeneratePrimitives(int sphereLatitudinalResolution, int sphereLongitudinalResolution);
    std::string LoadPrimitive(std::string filePath);
    
    void DrawPrimitive(std::string primitiveName);
    std::vector<std::string> GetPrimitiveNames() const;

private:
    std::unordered_map<std::string, std::shared_ptr<OpenGLPrimitive>> m_primitives;
};
