#pragma once
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "DataBinding.h"
#include "imgui.h"
#include "EnumLabels/EnumLabel.h"

class OpenGLTextureCache;

class PropertyDrawer
{
public:
	// Drag inputs
	static bool DrawFloatDrag(const std::string& propertyName, float& floatValue, float vSpeed = 1, float vMin = 0,
	                          float vMax = 0, const char* format = "%.3f");
	static bool DrawFloatDrag(const std::string& propertyName, DataBinding<float>& floatValue, float vSpeed = 1,
	                          float vMin = 0,
	                          float vMax = 0, const char* format = "%.3f");
	static bool DrawVec2fDrag(const std::string& propertyName, glm::vec2& vec2f, float vSpeed = 1, float vMin = 0,
	                          float vMax = 0, const char* format = "%.3f");
	static bool DrawVec2fDrag(const std::string& propertyName, DataBinding<glm::vec2>& vec2f, float vSpeed = 1,
	                          float vMin = 0,
	                          float vMax = 0, const char* format = "%.3f");
	static bool DrawVec3fDrag(const std::string& propertyName, glm::vec3& vec3f, float vSpeed = 1, float vMin = 0,
	                          float vMax = 0, const char* format = "%.3f");
	static bool DrawVec3fDrag(const std::string& propertyName, DataBinding<glm::vec3>& vec3f, float vSpeed = 1,
	                          float vMin = 0,
	                          float vMax = 0, const char* format = "%.3f");
	static bool DrawVec4fDrag(const std::string& propertyName, glm::vec4& vec4f, float vSpeed = 1, float vMin = 0,
	                          float vMax = 0, const char* format = "%.3f");
	static bool DrawVec4fDrag(const std::string& propertyName, DataBinding<glm::vec4>& vec4f, float vSpeed = 1,
	                          float vMin = 0,
	                          float vMax = 0, const char* format = "%.3f");

	// Color inputs
	static bool DrawVec3fColor(const std::string& propertyName, glm::vec3& vec3f);
	static bool DrawVec3fColor(const std::string& propertyName, DataBinding<glm::vec3>& vec3f);
	static bool DrawVec4fColor(const std::string& propertyName, glm::vec4& vec4f);
	static bool DrawVec4fColor(const std::string& propertyName, DataBinding<glm::vec4>& vec4f);

	// Combo
	static bool DrawCombo(const char* propertyName, std::vector<std::string> options, std::string& currentValue);

	template <typename E>
	static bool DrawEnumLabelCombo(const char* propertyName, EnumLabel<E> enumLabel, E& enumValue)
	{
		int newEnumIndex = enumLabel.GetEnumIndex(enumValue);
		if (ImGui::Combo(propertyName, &newEnumIndex, enumLabel.GetCStrings().data(),
		                 static_cast<int>(enumLabel.Count())))
		{
			enumValue = enumLabel.GetEnums()[newEnumIndex];
			return true;
		}
		return false;
	}

	template <typename E>
	static bool DrawEnumLabelCombo(const char* propertyName, EnumLabel<E> enumLabel, DataBinding<E>& enumValue)
	{
		int newEnumIndex = enumLabel.GetEnumIndex(enumValue.GetData());
		if (ImGui::Combo(propertyName, &newEnumIndex, enumLabel.GetCStrings().data(), enumLabel.Count()))
		{
			return enumValue.SetAndNotify(enumLabel.GetEnums()[newEnumIndex]);
		}
		return false;
	}

	static bool DrawTextureCacheCombo(const char* propertyName,
	                                  const std::unique_ptr<OpenGLTextureCache>& openGLTextureCache,
	                                  std::string& currentValue);
};
