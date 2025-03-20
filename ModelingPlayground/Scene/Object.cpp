#include "Object.h"

#include "Components/Component.h"

Object::Object(const std::string& name):
    m_name(name)
{
    
}

void Object::RemoveComponent(const std::shared_ptr<Component>& component)
{
    std::erase(m_components, component);
}

void Object::RenderInspector() const
{
    for (const auto& component : m_components)
    {
        component->RenderInspector();
    }
}