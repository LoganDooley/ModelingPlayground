#pragma once

class Component
{
public:
    virtual ~Component() = default;
    
    virtual void RenderInspector() = 0;
};
