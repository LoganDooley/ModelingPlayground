#pragma once

#include "../nlohmann/json.hpp"

NLOHMANN_JSON_NAMESPACE_BEGIN
	// Shared Pointers
	template <typename T>
	struct adl_serializer<std::shared_ptr<T>>
	{
		static void to_json(json& json, const std::shared_ptr<T>& pointer)
		{
			if (pointer)
			{
				json = *pointer;
			}
			else
			{
				json = nullptr;
			}
		}

		static void from_json(const json& json, std::shared_ptr<T>& pointer)
		{
			if (json.is_null())
			{
				pointer = nullptr;
			}
			else
			{
				pointer.reset(new T(json.get<T>()));
			}
		}
	};

NLOHMANN_JSON_NAMESPACE_END
