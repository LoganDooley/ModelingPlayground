#pragma once

#include "Component.h"
#include "../../nlohmann/json_fwd.hpp"

class RenderingManager;

class PrimitiveComponent : public Component
{
public:
	PrimitiveComponent();
	PrimitiveComponent(std::shared_ptr<RenderingManager> renderingManager);

	void SetRenderingManager(std::shared_ptr<RenderingManager> renderingManager);

	void RenderInspector() override;
	void SetPrimitiveName(const std::string& primitiveName);
	std::string GetPrimitiveName() const;

	friend void to_json(nlohmann::json& json, const PrimitiveComponent& primitiveComponent);
	friend void from_json(const nlohmann::json& json, PrimitiveComponent& primitiveComponent);

private:
	std::shared_ptr<RenderingManager> m_renderingManager;
	std::string m_primitiveName;
	int m_currentItem;
};
