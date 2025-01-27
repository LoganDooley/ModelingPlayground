#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Components/Component.h"

class Object
{
public:
    Object(std::string name);

    template <class T> 
    void AddComponent()
    {
        m_components.push_back(std::make_shared<T>());
    }

    void RemoveComponent(const std::shared_ptr<Component>& component);

    template <class T> 
    std::vector<std::shared_ptr<T>> GetComponents() const
    {
        std::vector<std::shared_ptr<T>> componentsOfType;
        for (const auto& component : m_components)
        {
            if (std::shared_ptr<T> componentAsType = std::dynamic_pointer_cast<T>(component))
            {
                componentsOfType.push_back(componentAsType);
            }
        }
        return componentsOfType;
    }

    void RenderInspector() const;

private:
    std::string m_name;
    std::vector<std::shared_ptr<Component>> m_components;
};
