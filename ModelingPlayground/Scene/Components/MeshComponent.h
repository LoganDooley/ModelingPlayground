#pragma once
#include "Component.h"

enum class MeshType
{
    Triangle,
};

class MeshComponent : public Component
{
public:
    MeshComponent();
    
    void RenderInspector() override;

private:
    MeshType m_meshType;
};
