#pragma once
#include "EnumLabel.h"

enum class PrimitiveType : uint8_t
{
    Triangle, Sphere, Cube,
};

static EnumLabel<PrimitiveType> PrimitiveTypeEnumLabel = EnumLabel<PrimitiveType>({
            {PrimitiveType::Triangle, "Triangle"},
            {PrimitiveType::Sphere, "Sphere"},
            {PrimitiveType::Cube, "Cube"}
        });
