#pragma once
#include <string>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "DataBinding.h"
#include "imgui.h"
#include "EnumLabels/EnumLabel.h"

class PropertyDrawer
{
public:
    // Drag inputs
    static bool DrawFloatDrag(const std::string& propertyName, float& floatValue, float vSpeed = 1, float vMin = 0,
        float vMax = 0, const char* format = "%.3f");
    static bool DrawFloatDrag(const std::string& propertyName, DataBinding<float>& floatValue, float vSpeed = 1, float vMin = 0,
        float vMax = 0, const char* format = "%.3f");
    static bool DrawVec3fDrag(const std::string& propertyName, glm::vec3& vec3f, float vSpeed = 1, float vMin = 0,
        float vMax = 0, const char* format = "%.3f");
    static bool DrawVec3fDrag(const std::string& propertyName, DataBinding<glm::vec3>& vec3f, float vSpeed = 1, float vMin = 0,
        float vMax = 0, const char* format = "%.3f");
    static bool DrawVec4fDrag(const std::string& propertyName, glm::vec4& vec4f, float vSpeed = 1, float vMin = 0,
        float vMax = 0, const char* format = "%.3f");
    static bool DrawVec4fDrag(const std::string& propertyName, DataBinding<glm::vec4>& vec4f, float vSpeed = 1, float vMin = 0,
        float vMax = 0, const char* format = "%.3f");

    // Color inputs
    static bool DrawVec3fColor(const std::string& propertyName, glm::vec3& vec3f);
    static bool DrawVec3fColor(const std::string& propertyName, DataBinding<glm::vec3>& vec3f);
    static bool DrawVec4fColor(const std::string& propertyName, glm::vec4& vec4f);
    static bool DrawVec4fColor(const std::string& propertyName, DataBinding<glm::vec4>& vec4f);

    // Combo
    template <typename E>
    static bool DrawEnumLabelCombo(const char* propertyName, EnumLabel<E> enumLabel, E& enumValue)
    {
        int newEnumIndex = enumLabel.GetEnumIndex(enumValue);
        ImGui::Combo(propertyName, &newEnumIndex, enumLabel.GetCStrings().data(), enumLabel.Count());
        if (enumLabel.GetEnums()[newEnumIndex] == enumValue)
        {
            return false;
        }
        enumValue = enumLabel.GetEnums()[newEnumIndex];
        return true;
    }

    template <typename E>
    static bool DrawEnumLabelCombo(const char* propertyName, EnumLabel<E> enumLabel, DataBinding<E>& enumValue)
    {
        int newEnumIndex = enumLabel.GetEnumIndex(enumValue.GetData());
        ImGui::Combo(propertyName, &newEnumIndex, enumLabel.GetCStrings().data(), enumLabel.Count());
        return enumValue.SetAndNotify(enumLabel.GetEnums()[newEnumIndex]);
    }
};
