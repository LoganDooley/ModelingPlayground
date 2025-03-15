#include "TransformComponent.h"

#include <glm/ext/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

#include "imgui.h"
#include "../../Utils/PropertyDrawer.h"

TransformComponent::TransformComponent():
    m_position(glm::vec3(0)),
    m_rotation(glm::vec3(0)),
    m_scale(glm::vec3(1)),
    m_localXUnitVector(glm::vec3(1, 0, 0)),
    m_modelMatrix(glm::mat4(1))
{
}

void TransformComponent::RenderInspector()
{
    ImGuiTreeNodeFlags transformHeaderFlags = ImGuiTreeNodeFlags_DefaultOpen;
    if (ImGui::CollapsingHeader("Transform", transformHeaderFlags))
    {
        // Position
        glm::vec3 position = m_position;
        if (PropertyDrawer::DrawVec3fDrag("Position", position, 0.01f))
        {
            SetPosition(position);
        }

        // Rotation
        glm::vec3 rotation = m_rotation;
        if (PropertyDrawer::DrawVec3fDrag("Rotation", rotation, 1.0f))
        {
            SetRotation(rotation);
        }

        // Scale
        glm::vec3 scale = m_scale;
        if (PropertyDrawer::DrawVec3fDrag("Scale", scale, 0.01f))
        {
            SetScale(scale);
        }
    }
}

const glm::mat4& TransformComponent::GetModelMatrix() const
{
    return m_modelMatrix;
}

const glm::vec3& TransformComponent::GetPosition() const
{
    return m_position;
}

const glm::vec3& TransformComponent::GetRotation() const
{
    return m_rotation;
}

const glm::vec3& TransformComponent::GetScale() const
{
    return m_scale;
}

const glm::vec3& TransformComponent::GetLocalXUnitVector() const
{
    return m_localXUnitVector;
}

void TransformComponent::SetPosition(glm::vec3 newPosition)
{
    if (m_position != newPosition)
    {
        m_position = newPosition;
        UpdateModelMatrix();
    }
}

void TransformComponent::SetRotation(glm::vec3 newRotation)
{
    if (m_rotation != newRotation)
    {
        m_rotation = newRotation;
        m_rotation.x = std::fmod(m_rotation.x, 360.f);
        m_rotation.y = std::fmod(m_rotation.y, 360.f);
        m_rotation.z = std::fmod(m_rotation.z, 360.f);
        
        UpdateModelMatrix();
        UpdateLocalXUnitVector();
    }
}

void TransformComponent::SetScale(glm::vec3 newScale)
{
    if (m_scale != newScale)
    {
        m_scale = newScale;
        UpdateModelMatrix();
    }
}

void TransformComponent::UpdateModelMatrix()
{
    // scale
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1), m_scale);

    // rotation
    glm::mat4 rotationMatrix = glm::eulerAngleXYZ(glm::radians(m_rotation.x), glm::radians(m_rotation.y), glm::radians(m_rotation.z));

    // translation
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1), m_position);

    // model matrix
    m_modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}

void TransformComponent::UpdateLocalXUnitVector()
{
    glm::mat4 rotationMatrix = glm::eulerAngleXYZ(glm::radians(m_rotation.x), glm::radians(m_rotation.y), glm::radians(m_rotation.z));
    m_localXUnitVector = rotationMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
}
