#pragma once

#include "Component.h"
#include "../../nlohmann/json_fwd.hpp"

class OpenGLRenderer;

class PrimitiveComponent : public Component
{
public:
	PrimitiveComponent();
	PrimitiveComponent(std::shared_ptr<OpenGLRenderer> openGLRenderer);

	void SetOpenGLRenderer(std::shared_ptr<OpenGLRenderer> openGLRenderer);

	void RenderInspector() override;
	void SetPrimitiveName(const std::string& primitiveName);
	std::string GetPrimitiveName() const;

	friend void to_json(nlohmann::json& json, const PrimitiveComponent& primitiveComponent);
	friend void from_json(const nlohmann::json& json, PrimitiveComponent& primitiveComponent);

private:
	std::shared_ptr<OpenGLRenderer> m_openGLRenderer;
	std::string m_primitiveName;
	int m_currentItem;
};
