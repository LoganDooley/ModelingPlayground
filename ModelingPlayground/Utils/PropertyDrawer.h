#pragma once
#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "DataBinding.h"
#include "EnumToString.h"
#include "imgui.h"

class OpenGLTextureCache;

class PropertyDrawer
{
public:
    // Checkbox
    static bool DrawCheckbox(const std::string& propertyName, DataBinding<bool>& value);

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
    static bool DrawCombo(const char* propertyName, std::vector<std::string> options,
                          DataBinding<std::string>& currentValue);

    template <typename E>
    static bool DrawEnumCombo(const char* propertyName, E& enumValue)
    {
        int newEnumIndex = static_cast<int>(enumValue);
        if (ImGui::Combo(propertyName, &newEnumIndex, EnumToStringMap[std::type_index(typeid(E))].data(),
                         EnumToStringMap[std::type_index(typeid(E))].size()))
        {
            enumValue = static_cast<E>(newEnumIndex);
            return true;
        }
        return false;
    }

    static bool DrawTextureCacheCombo(const char* propertyName,
                                      const std::shared_ptr<OpenGLTextureCache>& openGLTextureCache,
                                      DataBinding<std::string>& currentTextureKey);
};
