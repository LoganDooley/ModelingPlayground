#pragma once

#include "Component.h"
#include "../../Utils/EnumLabels/PrimitiveTypeEnumLabel.h"


class PrimitiveComponent : public Component
{
public:
    PrimitiveComponent();
    
    void RenderInspector() override;
    PrimitiveType GetPrimitiveType() const;

private:
    PrimitiveType m_primitiveType;
    int m_currentItem;
};
