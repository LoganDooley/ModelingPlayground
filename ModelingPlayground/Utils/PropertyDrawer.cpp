#include "PropertyDrawer.h"

bool PropertyDrawer::DrawFloatDrag(const std::string& propertyName, float& floatValue, float vSpeed, float vMin,
                                   float vMax, const char* format)
{
	return ImGui::DragFloat(propertyName.c_str(), &floatValue, vSpeed, vMin, vMax, format);
}

bool PropertyDrawer::DrawFloatDrag(const std::string& propertyName, DataBinding<float>& floatValue, float vSpeed,
                                   float vMin, float vMax, const char* format)
{
	float newFloatValue = floatValue.GetData();
	if (ImGui::DragFloat(propertyName.c_str(), &newFloatValue, vSpeed, vMin, vMax, format))
	{
		return floatValue.SetAndNotify(newFloatValue);
	}
	return false;
}

bool PropertyDrawer::DrawVec2fDrag(const std::string& propertyName, glm::vec2& vec2f, float vSpeed, float vMin,
                                   float vMax, const char* format)
{
	float newVec2f[2] = {vec2f.x, vec2f.y};
	if (ImGui::DragFloat2(propertyName.c_str(), newVec2f, vSpeed, vMin, vMax, format))
	{
		vec2f = glm::vec2(newVec2f[0], newVec2f[1]);
		return true;
	}
	return false;
}

bool PropertyDrawer::DrawVec2fDrag(const std::string& propertyName, DataBinding<glm::vec2>& vec2f, float vSpeed,
                                   float vMin, float vMax, const char* format)
{
	float newVec2f[2] = {vec2f.GetData().x, vec2f.GetData().y};
	if (ImGui::DragFloat2(propertyName.c_str(), newVec2f, vSpeed, vMin, vMax, format))
	{
		return vec2f.SetAndNotify(glm::vec2(newVec2f[0], newVec2f[1]));
	}
	return false;
}

bool PropertyDrawer::DrawVec3fDrag(const std::string& propertyName, glm::vec3& vec3f, float vSpeed, float vMin,
                                   float vMax, const char* format)
{
	float newVec3f[3] = {vec3f.x, vec3f.y, vec3f.z};
	if (ImGui::DragFloat3(propertyName.c_str(), newVec3f, vSpeed, vMin, vMax, format))
	{
		vec3f = glm::vec3(newVec3f[0], newVec3f[1], newVec3f[2]);
		return true;
	}
	return false;
}

bool PropertyDrawer::DrawVec3fDrag(const std::string& propertyName, DataBinding<glm::vec3>& vec3f, float vSpeed,
                                   float vMin, float vMax, const char* format)
{
	float newVec3f[3] = {vec3f.GetData().x, vec3f.GetData().y, vec3f.GetData().z};
	if (ImGui::DragFloat3(propertyName.c_str(), newVec3f, vSpeed, vMin, vMax, format))
	{
		return vec3f.SetAndNotify(glm::vec3(newVec3f[0], newVec3f[1], newVec3f[2]));
	}
	return false;
}

bool PropertyDrawer::DrawVec4fDrag(const std::string& propertyName, glm::vec4& vec4f, float vSpeed, float vMin,
                                   float vMax, const char* format)
{
	float newVec4f[4] = {vec4f.x, vec4f.y, vec4f.z, vec4f.w};
	if (ImGui::DragFloat4(propertyName.c_str(), newVec4f, vSpeed, vMin, vMax, format))
	{
		vec4f = glm::vec4(newVec4f[0], newVec4f[1], newVec4f[2], newVec4f[3]);
		return true;
	}
	return false;
}

bool PropertyDrawer::DrawVec4fDrag(const std::string& propertyName, DataBinding<glm::vec4>& vec4f, float vSpeed,
                                   float vMin, float vMax, const char* format)
{
	float newVec4f[4] = {vec4f.GetData().x, vec4f.GetData().y, vec4f.GetData().z, vec4f.GetData().w};
	if (ImGui::DragFloat3(propertyName.c_str(), newVec4f, vSpeed, vMin, vMax, format))
	{
		return vec4f.SetAndNotify(glm::vec4(newVec4f[0], newVec4f[1], newVec4f[2], newVec4f[3]));
	}
	return false;
}

bool PropertyDrawer::DrawVec3fColor(const std::string& propertyName, glm::vec3& vec3f)
{
	float newVec3f[3] = {vec3f.x, vec3f.y, vec3f.z};
	if (ImGui::ColorEdit3(propertyName.c_str(), newVec3f))
	{
		vec3f = glm::vec3(newVec3f[0], newVec3f[1], newVec3f[2]);
		return true;
	}
	return false;
}

bool PropertyDrawer::DrawVec3fColor(const std::string& propertyName, DataBinding<glm::vec3>& vec3f)
{
	float newVec3f[3] = {vec3f.GetData().x, vec3f.GetData().y, vec3f.GetData().z};
	if (ImGui::ColorEdit3(propertyName.c_str(), newVec3f))
	{
		return vec3f.SetAndNotify(glm::vec3(newVec3f[0], newVec3f[1], newVec3f[2]));
	}
	return false;
}

bool PropertyDrawer::DrawVec4fColor(const std::string& propertyName, glm::vec4& vec4f)
{
	float newVec4f[4] = {vec4f.x, vec4f.y, vec4f.z, vec4f.w};
	if (ImGui::ColorEdit4(propertyName.c_str(), newVec4f))
	{
		vec4f = glm::vec4(newVec4f[0], newVec4f[1], newVec4f[2], newVec4f[3]);
		return true;
	}
	return false;
}

bool PropertyDrawer::DrawVec4fColor(const std::string& propertyName, DataBinding<glm::vec4>& vec4f)
{
	float newVec4f[4] = {vec4f.GetData().x, vec4f.GetData().y, vec4f.GetData().z, vec4f.GetData().w};
	if (ImGui::ColorEdit4(propertyName.c_str(), newVec4f))
	{
		return vec4f.SetAndNotify(glm::vec4(newVec4f[0], newVec4f[1], newVec4f[2], newVec4f[3]));
	}
	return false;
}

bool PropertyDrawer::DrawCombo(const char* propertyName, std::vector<std::string> options, std::string& currentValue)
{
	std::vector<const char*> optionNames;
	for (int i = 0; i < options.size(); i++)
	{
		optionNames.push_back(options[i].c_str());
	}

	auto location = std::find(options.begin(), options.end(), currentValue);
	int newOptionIndex = 0;
	if (location != options.end())
	{
		newOptionIndex = std::distance(options.begin(), location);
	}

	if (ImGui::Combo(propertyName, &newOptionIndex, optionNames.data(), static_cast<int>(options.size())))
	{
		currentValue = options[newOptionIndex];
		return true;
	}
	return false;
}
