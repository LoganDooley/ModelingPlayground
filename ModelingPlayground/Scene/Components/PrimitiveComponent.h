#pragma once
#include "Component.h"

enum class PrimitiveType
{
    Triangle,
};

class PrimitiveComponent : public Component
{
public:
    PrimitiveComponent();
    
    void RenderInspector() override;

private:
    PrimitiveType m_meshType;
};
