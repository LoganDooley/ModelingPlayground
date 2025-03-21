#pragma once

#include "../nlohmann/json_fwd.hpp"

namespace PolymorphicSerializer_Impl
{
    template <class Base>
    struct Serializer
    {
        void (*to_json)(nlohmann::json& json, const Base& base);
        void (*from_json)(const nlohmann::json& json, Base& base);
    };

    template <class Base, class Derived>
    Serializer<Base> SerializerFor()
    {
        return{
            [](nlohmann::json& json, const Base& base)
            {
                const Derived& derived = static_cast<const Derived&>(base);
                return to_json(json, derived);
            },
            [](const nlohmann::json& json, Base& base)
            {
                Derived& derived = static_cast<Derived&>(base);
                return from_json(json, derived);    
            }
        };
    }
}

template <class Base>
struct PolymorphicSerializer
{
    static inline std::unordered_map<char const*, PolymorphicSerializer_Impl::Serializer<Base>> m_serializers;

    template <class... Derived>
    static void RegisterTypes()
    {
        (m_serializers.emplace(typeid(Derived).name(), PolymorphicSerializer_Impl::SerializerFor<Base, Derived>()), ...);
    }

    static void to_json(nlohmann::json& json, const Base& base)
    {
        char const* typeName = typeid(base).name();
        m_serializers.at(typeName).to_json(json, base);
        json["type"] = typeName;
    }

    static void from_json(const nlohmann::json& json, Base& base)
    {
        m_serializers.at(json.at("type").get<std::string>().c_str()).from_json(json, base);
    }
};