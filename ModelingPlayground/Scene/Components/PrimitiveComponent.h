﻿#pragma once

#include "Component.h"
#include "../../Utils/EnumLabels/PrimitiveTypeEnumLabel.h"
#include "../../nlohmann/json_fwd.hpp"


class PrimitiveComponent : public Component
{
public:
    PrimitiveComponent();
    
    void RenderInspector() override;
    PrimitiveType GetPrimitiveType() const;

    friend void to_json(nlohmann::json& json, const PrimitiveComponent& primitiveComponent);
    friend void from_json(const nlohmann::json& json, PrimitiveComponent& primitiveComponent);

private:
    PrimitiveType m_primitiveType;
    int m_currentItem;
};
