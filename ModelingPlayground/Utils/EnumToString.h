#pragma once

#include <typeindex>
#include <unordered_map>

#include "../Application/Rendering/RenderingManager.h"

static std::unordered_map<std::type_index, std::vector<const char*>> GetEnumToStringMap()
{
    std::unordered_map<std::type_index, std::vector<const char*>> enumToStringMap;
    enumToStringMap[std::type_index(typeid(RenderingMode))] = {"Raster", "Pathtraced"};
    return enumToStringMap;
}

static auto EnumToStringMap = GetEnumToStringMap();
