#include "TransformComponent.h"

#include <glm/ext/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

#include "imgui.h"
#include "../../Utils/PropertyDrawer.h"
#include "../../OpenGLHelpers/OpenGLRenderer.h"

TransformComponent::TransformComponent():
    m_position(glm::vec3(0)),
    m_rotation(glm::vec3(0)),
    m_scale(glm::vec3(1)),
    m_localXUnitVector(glm::vec3(1, 0, 0)),
    m_modelMatrix(glm::mat4(1))
{
}

TransformComponent::TransformComponent(std::shared_ptr<OpenGLRenderer> openGLRenderer):
    m_openGLRenderer(openGLRenderer),
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
        if (PropertyDrawer::DrawVec3fDrag("Position", m_position, 0.01f))
        {
            UpdateModelMatrix();
        }

        // Rotation
        if (PropertyDrawer::DrawVec3fDrag("Rotation", m_rotation, 1.0f))
        {
            glm::vec3 rotationModulus = m_rotation.GetData();
            rotationModulus.x = std::fmodf(rotationModulus.x, 360.0f);
            rotationModulus.y = std::fmodf(rotationModulus.y, 360.0f);
            rotationModulus.z = std::fmodf(rotationModulus.z, 360.0f);
            m_rotation.SetAndNotify(rotationModulus);
            
            UpdateModelMatrix();
            UpdateLocalXUnitVector();
        }

        // Scale
        if (PropertyDrawer::DrawVec3fDrag("Scale", m_scale, 0.01f))
        {
            UpdateModelMatrix();
        }
    }
}

void TransformComponent::SetOpenGLRenderer(std::shared_ptr<OpenGLRenderer> openGLRenderer)
{
    m_openGLRenderer = openGLRenderer;
}

const glm::mat4& TransformComponent::GetModelMatrix() const
{
    return m_modelMatrix;
}

const glm::vec3& TransformComponent::GetPosition() const
{
    return m_position.GetData();
}

DataBinding<glm::vec3>& TransformComponent::GetPositionDataBinding()
{
    return m_position;
}

const glm::vec3& TransformComponent::GetRotation() const
{
    return m_rotation.GetData();
}

const glm::vec3& TransformComponent::GetScale() const
{
    return m_scale.GetData();
}

const glm::vec3& TransformComponent::GetLocalXUnitVector() const
{
    return m_localXUnitVector.GetData();
}

DataBinding<glm::vec3>& TransformComponent::GetLocalXUnitVectorDataBinding()
{
    return m_localXUnitVector;
}

void TransformComponent::UpdateModelMatrix()
{
    // scale
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1), m_scale.GetData());

    // rotation
    glm::mat4 rotationMatrix = glm::eulerAngleXYZ(glm::radians(m_rotation.GetData().x), glm::radians(m_rotation.GetData().y), glm::radians(m_rotation.GetData().z));

    // translation
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1), m_position.GetData());

    // model matrix
    m_modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

    if (m_openGLRenderer != nullptr)
    {
        m_openGLRenderer->ResetAllLightTransforms();
    }
}

void TransformComponent::UpdateLocalXUnitVector()
{
    glm::mat4 rotationMatrix = glm::eulerAngleXYZ(glm::radians(m_rotation.GetData().x), glm::radians(m_rotation.GetData().y), glm::radians(m_rotation.GetData().z));
    m_localXUnitVector.SetAndNotify(rotationMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
}
