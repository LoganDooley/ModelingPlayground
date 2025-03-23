#pragma once

#include "../nlohmann/json.hpp"

NLOHMANN_JSON_NAMESPACE_BEGIN
// Data Bindings
template<typename T>
struct adl_serializer<DataBinding<T>>
{
    static void to_json(nlohmann::json& json, const DataBinding<T>& dataBinding)
    {
        json = dataBinding.GetData();
    }

    static void from_json(const nlohmann::json& json, DataBinding<T>& dataBinding)
    {
        dataBinding.SetWithoutNotify(json.get<T>());
    }
};
NLOHMANN_JSON_NAMESPACE_END