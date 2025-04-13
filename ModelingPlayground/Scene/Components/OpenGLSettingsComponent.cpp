#include "OpenGLSettingsComponent.h"

#include <glm/vec3.hpp>

#include "imgui.h"
#include "../../Utils/PropertyDrawer.h"

OpenGLSettingsComponent::OpenGLSettingsComponent():
	m_clearColor(glm::vec4(glm::vec3(0.0f), 1.0f)),
	m_ambientLight(glm::vec3(0.03f))
{
}

void OpenGLSettingsComponent::RenderInspector()
{
	ImGuiTreeNodeFlags ambientLightHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("OpenGL Settings", ambientLightHeaderFlags))
	{
		// Clear color
		PropertyDrawer::DrawVec4fColor("Clear Color", m_clearColor);

		// Ambient light
		PropertyDrawer::DrawVec3fColor("Ambient Light", m_ambientLight);
	}
}

glm::vec4 OpenGLSettingsComponent::GetClearColor() const
{
	return m_clearColor;
}

void OpenGLSettingsComponent::SetAmbientLight(glm::vec3 ambientLight)
{
	m_ambientLight = ambientLight;
}

glm::vec3 OpenGLSettingsComponent::GetAmbientLight() const
{
	return m_ambientLight;
}
