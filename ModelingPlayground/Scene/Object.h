#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Components/Component.h"
#include "../../nlohmann/json_fwd.hpp"

class Object
{
public:
    Object() = default;
    Object(const std::string& name);

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

    template <class T>
    std::shared_ptr<T> GetFirstComponentOfType() const
    {
        for (const auto& component : m_components)
        {
            if (std::shared_ptr<T> componentAsType = std::dynamic_pointer_cast<T>(component))
            {
                return componentAsType;
            }
        }
        return nullptr;
    }

    std::string GetName() const
    {
        return m_name;
    }

    void RenderInspector() const;

    friend void to_json(nlohmann::json& json, const Object& object);
    friend void from_json(const nlohmann::json& json, Object& object);

private:
    std::string m_name;
    std::vector<std::shared_ptr<Component>> m_components;
};
