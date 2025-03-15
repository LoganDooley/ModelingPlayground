#include "PropertyDrawer.h"

bool PropertyDrawer::DrawVec3fDrag(const std::string& propertyName, glm::vec3& vec3f, float vSpeed, float vMin,
        float vMax, const char* format)
{
    float newVec3f[3] = { vec3f.x, vec3f.y, vec3f.z };
    ImGui::DragFloat3(propertyName.c_str(), newVec3f, vSpeed, vMin, vMax, format);
    if (vec3f == glm::vec3(newVec3f[0], newVec3f[1], newVec3f[2]))
    {
        return false;
    }
    vec3f = glm::vec3(newVec3f[0], newVec3f[1], newVec3f[2]);
    return true;
}

bool PropertyDrawer::DrawVec4fDrag(const std::string& propertyName, glm::vec4& vec4f, float vSpeed, float vMin,
        float vMax, const char* format)
{
    float newVec4f[4] = { vec4f.x, vec4f.y, vec4f.z, vec4f.w };
    ImGui::DragFloat4(propertyName.c_str(), newVec4f, vSpeed, vMin, vMax, format);
    if (vec4f == glm::vec4(newVec4f[0], newVec4f[1], newVec4f[2], newVec4f[3]))
    {
        return false;
    }
    vec4f = glm::vec4(newVec4f[0], newVec4f[1], newVec4f[2], newVec4f[3]);
    return true;
}

bool PropertyDrawer::DrawVec3fColor(const std::string& propertyName, glm::vec3& vec3f)
{
    float newVec3f[3] = { vec3f.x, vec3f.y, vec3f.z };
    ImGui::ColorEdit3(propertyName.c_str(), newVec3f);
    if (vec3f == glm::vec3(newVec3f[0], newVec3f[1], newVec3f[2]))
    {
        return false;
    }
    vec3f = glm::vec3(newVec3f[0], newVec3f[1], newVec3f[2]);
    return true;
}

bool PropertyDrawer::DrawVec4fColor(const std::string& propertyName, glm::vec4& vec4f)
{
    float newVec4f[4] = { vec4f.x, vec4f.y, vec4f.z, vec4f.w };
    ImGui::ColorEdit4(propertyName.c_str(), newVec4f);
    if (vec4f == glm::vec4(newVec4f[0], newVec4f[1], newVec4f[2], newVec4f[3]))
    {
        return false;
    }
    vec4f = glm::vec4(newVec4f[0], newVec4f[1], newVec4f[2], newVec4f[3]);
    return true;
}
