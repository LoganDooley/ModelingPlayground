#pragma once

#include <iostream>

#include "../nlohmann/json.hpp"
#include "../Scene/Components/ComponentIncludes.h"

namespace ComponentSerializer_Impl
{
	struct Serializer
	{
		void (*to_json)(nlohmann::json& json, const std::shared_ptr<Component>& base);
		void (*from_json)(const nlohmann::json& json, std::shared_ptr<Component>& base);
	};

	template <class Derived>
	Serializer SerializerFor()
	{
		return {
			[](nlohmann::json& json, const std::shared_ptr<Component>& base)
			{
				std::shared_ptr<Derived> derived = static_pointer_cast<Derived>(base);
				to_json(json, *derived);
			},
			[](const nlohmann::json& json, std::shared_ptr<Component>& base)
			{
				base = std::make_shared<Derived>();
				std::shared_ptr<Derived> asDerived = static_pointer_cast<Derived>(base);
				from_json(json, *asDerived);
			}
		};
	}
}

NLOHMANN_JSON_NAMESPACE_BEGIN
	// Component Shared Pointers
	template <>
	struct adl_serializer<std::shared_ptr<Component>>
	{
		static inline std::unordered_map<std::string, ComponentSerializer_Impl::Serializer> m_serializers = {
			{
				typeid(DirectionalLightComponent).name(),
				ComponentSerializer_Impl::SerializerFor<DirectionalLightComponent>()
			},
			{typeid(MaterialComponent).name(), ComponentSerializer_Impl::SerializerFor<MaterialComponent>()},
			{
				typeid(OpenGLSettingsComponent).name(),
				ComponentSerializer_Impl::SerializerFor<OpenGLSettingsComponent>()
			},
			{typeid(PointLightComponent).name(), ComponentSerializer_Impl::SerializerFor<PointLightComponent>()},
			{typeid(PrimitiveComponent).name(), ComponentSerializer_Impl::SerializerFor<PrimitiveComponent>()},
			{typeid(SpotLightComponent).name(), ComponentSerializer_Impl::SerializerFor<SpotLightComponent>()},
			{typeid(TransformComponent).name(), ComponentSerializer_Impl::SerializerFor<TransformComponent>()},
		};

		static void to_json(json& json, const std::shared_ptr<Component>& component)
		{
			if (component)
			{
				std::string typeName = typeid(*component).name();
				if (!m_serializers.contains(typeName))
				{
					std::cout << "adl_serializer<std::shared_ptr<Component>>|to_json: " << typeName <<
						" not registered!\n";
					return;
				}
				m_serializers.at(typeName).to_json(json, component);
				json["type"] = typeName;
			}
			else
			{
				json = nullptr;
			}
		}

		static void from_json(const json& json, std::shared_ptr<Component>& component)
		{
			if (json.is_null())
			{
				component = nullptr;
			}
			else
			{
				nlohmann::json typeJson = json.at("type");
				auto typeName = typeJson.get<std::string>();

				if (!m_serializers.contains(typeName))
				{
					std::cout << "adl_serializer<std::shared_ptr<Component>>|to_json: " << typeName <<
						" not registered!\n";
					return;
				}
				m_serializers.at(typeName).from_json(json, component);
			}
		}
	};

NLOHMANN_JSON_NAMESPACE_END
