#pragma once

#include "OpenGLPrimitive.h"
#include "../../Utils/EnumLabels/PrimitiveTypeEnumLabel.h"

class OpenGLPrimitiveManager
{
public:
    OpenGLPrimitiveManager();
    ~OpenGLPrimitiveManager() = default;

    void GeneratePrimitives(int sphereLatitudinalResolution, int sphereLongitudinalResolution);
    std::string LoadPrimitive(std::string filePath);

    void DrawPrimitive(PrimitiveType primitiveType);
    void DrawPrimitive(std::string fileName);

private:
    std::unordered_map<PrimitiveType, std::shared_ptr<OpenGLPrimitive>> m_primitives;
    std::unordered_map<std::string, std::shared_ptr<OpenGLPrimitive>> m_customPrimitives;
};
